#include "pch.h"
#include <math.h>
#include<cmath>
#include "AStar.h"
#include "Calculate.h"
using namespace std;
Astar::Astar(std::vector<std::vector<int>>& _maze) :maze(_maze) {}
//使用外部障碍物信息构造
Astar::Astar(int rows, int cols, const std::vector<Car_Info>& obstacle_info)
{
	//生成地图：行 列
	maze.reserve(rows);
	vector<int> row(cols, 0);
	for (int i = 0; i < rows; i++)
		maze.push_back(row);
	//遍历障碍物信息，将对应的栅格置为1
	for (auto& car_info : obstacle_info)
	{
		Caculate_SetGridesOfCar(car_info.car_length, car_info.car_width, car_info.car_center, car_info.angle, 0, maze);
	}
}
//************************************
// Method:    SetCarInfo
// FullName:  Astar::SetCarInfo
// Access:    public 
// Returns:   void
// Qualifier:设置中心栅格距离车四个边界的格子字数
// Parameter: int head	中心栅格与前边界栅格的距离格子数
// Parameter: int heel	中心栅格与后边界栅格的~
// Parameter: int left	中心栅格与左边界栅格的~
// Parameter: int right	中心栅格与右边界栅格的~
//************************************
void Astar::SetCarInfo(int head, int heel, int left, int right)
{
	head_distance = head;
	heel_distance = heel;
	left_distace = left;
	right_distance = right;
}

//************************************
// Method:    SetCollisionRadius
// FullName:  Astar::SetCollisionRadius
// Access:    public 
// Returns:   void
// Qualifier:设置碰撞安全距离
// Parameter: int len 安全距离
//************************************
void Astar::SetCollisionRadius(int len)
{
	CollisionRadius = len;
}

int Astar::calcG(Point* temp_start, Point* point)
{
	int extraG = (abs(point->x - temp_start->x) + abs(point->y - temp_start->y)) == 1 ? kCost1 : kCost2;
	int parentG = point->parent == NULL ? 0 : point->parent->G; //如果是初始节点，则其父节点是空
	return parentG + extraG;
}

int Astar::calcH(Point* point, Point* end)
{
	//用简单的欧几里得距离计算H，这个H的计算是关键，还有很多算法，没深入研究^_^
	return static_cast<int>(sqrt((double)(end->x - point->x) * (double)(end->x - point->x) + (double)(end->y - point->y) * (double)(end->y - point->y)) * kCost1);;
}

int Astar::calcF(Point* point)
{
	//动态加权
	/*在搜索开始时，快速到达目的地所在区域更重要；
	在搜索结束时，得到到达目标的最佳路径更重要。*/
	//取当前栅格到终点栅格的距离
	int w = static_cast<int>(sqrt(point->H));
	//分情况取w值
	if (w > 70)
		w = 5;
	else if (w > 50)
		w = 4;
	else if (w > 30)
		w = 3;
	else if (w > 10)
		w = 2;
	else
		w = 1;

	return point->G + w * point->H;
}

Point* Astar::getLeastFpoint_Forward() const
{
	if (!openList.empty())
	{
		auto resPoint = openList.front();
		for (auto& point : openList)
			if (point->F < resPoint->F)
				resPoint = point;
		return resPoint;

	}
	return NULL;
}
Point* Astar::getLeastFpoint_Reverse() const
{
	if (!reverse_openList.empty())
	{
		auto resPoint = reverse_openList.front();
		for (auto& point : reverse_openList)
			if (point->F < resPoint->F)
				resPoint = point;
		return resPoint;

	}
	return NULL;
}


std::pair<Point*, Point*> Astar::findPath(Point& startPoint, Point& endPoint, bool isIgnoreCorner)
{
	openList.push_back(new Point(startPoint.x, startPoint.y)); //置入起点
	reverse_openList.push_back(new Point(endPoint.x, endPoint.y)); //置入终点
	//开启列表辅助结构添加栅格
	openListOfHash.insert(Point(startPoint.x, startPoint.y));
	reverse_openListOfHash.insert(Point(endPoint.x, endPoint.y));
	//0、初始化两个当前点cur_grid1,cur_grid2
	Point* cur_grid1 = openList.front();
	Point* cur_grid2 = reverse_openList.front();
	while (!openList.empty() && !reverse_openList.empty())
	{
		//正向A*
		//正向1、
		Point* curPoint = cur_grid1; //将cur_grid1设为当前格. 
		openList.remove(curPoint); //从正向开启列表中删除
		//正向列表辅助结构删除栅格
		openListOfHash.erase(*curPoint);
		closeList.push_back(curPoint); //放到正向关闭列表
		//正向关闭列表辅助结构增加栅格
		closeListOfHash.insert(*curPoint);
		//正向2、找到正向情况下当前周围八个格中可以通过的格子
		std::vector<Point*>  surroundPoints = getSurroundPoints(curPoint, isIgnoreCorner, true);
		for (auto& target : surroundPoints)
		{
			//正向3、对某一个格子，如果它不在正向开启列表中，把它添加进 "正向开启列表", 把当前格作为这一格的父节点, 并根据cur_grid2，计算这一格的 FGH 
			if (!isInOpenList_Forward(target))
			{
				target->parent = curPoint;

				target->G = calcG(curPoint, target);
				target->H = calcH(target, cur_grid2);
				target->F = calcF(target);
				openList.push_back(target);
				//正向开启列表辅助结构添加栅格
				openListOfHash.insert(*target);
			}
			//正向4、对某一个格子，它在正向开启列表中，计算G值, 如果比原来的大, 就什么都不做, 否则设置它的父节点为当前点,并更新G和F
			else
			{
				//获取正向列表中target对应的point
				auto getPoint = GetPointOfList(openList, target);
				int tempG = calcG(curPoint, getPoint);
				if (tempG < getPoint->G)
				{
					getPoint->parent = curPoint;

					getPoint->G = tempG;
					getPoint->F = calcF(target);
				}
				//delete target
				delete target;
			}
		}
		//正向5、if(cur_grid2已经在 "正向开启列表" ) 找到路径; return;
		if (isInOpenList_Forward(cur_grid2))
		{
			//Point *resPoint = GetPointOfList(openList, cur_grid2);
			//从正向开启列表中找到cur_grid2所对应的grid1，使得从grid1回溯即可得到正向A*部分的路径
			//直接从cur_grid2回溯，可得到反向A*部分的路径
			auto gtePoint = GetPointOfList(openList, cur_grid2);
			if (gtePoint != nullptr)
				cur_grid1 = gtePoint;
			//返回cur_grid1和cur_grid2
			return make_pair(cur_grid1, cur_grid2);
		}
		//正向6、将cur_grid1重置为正向开启列表的F值最小的栅格
		cur_grid1 = getLeastFpoint_Forward();


		//反向A*
		//反向1、
		//curPoint = cur_grid2; //将cur_grid2设为当前格. 
		//reverse_openList.remove(curPoint); //从反向开启列表中删除
		////反向列表辅助结构删除栅格
		//reverse_openListOfHash.erase(*curPoint);
		//reverse_closeList.push_back(curPoint); //放到正向关闭列表
		////反向关闭列表辅助结构增加栅格
		//reverse_closeListOfHash.insert(*curPoint);
		////反向2、找到反向情况下当前周围八个格中可以通过的格子
		//surroundPoints.clear();
		//surroundPoints = getSurroundPoints(curPoint, isIgnoreCorner, false);
		//for (auto &target : surroundPoints)
		//{
		//	//反向3、对某一个格子，如果它不在反向开启列表中，把它添加进 "反向开启列表", 把当前格作为这一格的父节点, 并根据cur_grid1，计算这一格的 FGH 
		//	if (!isInOpenList_Reverse(target))
		//	{
		//		target->parent = curPoint;

		//		target->G = calcG(curPoint, target);
		//		target->H = calcH(target, cur_grid1);
		//		target->F = calcF(target);
		//		reverse_openList.push_back(target);
		//		//反向开启列表辅助结构添加栅格
		//		reverse_openListOfHash.insert(*target);
		//	}
		//	//反向4、对某一个格子，它在反向开启列表中，计算G值, 如果比原来的大, 就什么都不做, 否则设置它的父节点为当前点,并更新G和F
		//	else
		//	{
		//		//获取反向列表中target对应的point
		//		auto getPoint = GetPointOfList(reverse_openList, target);
		//		int tempG = calcG(curPoint, getPoint);
		//		if (tempG < getPoint->G)
		//		{
		//			getPoint->parent = curPoint;

		//			getPoint->G = tempG;
		//			getPoint->F = calcF(target);
		//		}
		//		delete target;
		//		/*	int tempG = calcG(curPoint, target);
		//			if (tempG < target->G)
		//			{
		//				target->parent = curPoint;

		//				target->G = tempG;
		//				target->F = calcF(target);
		//			}*/
		//	}

		//}
		//// 反向5、if(cur_grid1已经在 "反向开启列表" ) 找到路径; return;
		//if (isInOpenList_Reverse(cur_grid1))
		//{
		//	//从反向开启列表中找到cur_grid1所对应的grid2，使得从grid2回溯即可得到反向A*部分的路径
		//	//直接从cur_grid1回溯，可得到正向A*部分的路径
		//	auto gtePoint = GetPointOfList(reverse_openList, cur_grid1);
		//	if (gtePoint != nullptr)
		//		cur_grid2 = gtePoint;
		//	//返回cur_grid1和cur_grid2
		//	return make_pair(cur_grid1, cur_grid2);
		//}
		////反向6、将cur_grid2重置为反向开启列表的F值最小的栅格
		//cur_grid2 = getLeastFpoint_Reverse();

	}

	return make_pair(nullptr, nullptr);
}

std::list<Point> Astar::GetPath(Point& startPoint, Point& endPoint, bool isIgnoreCorner)
{
	std::list<Point> path;
	//如果没有对栅格地图maze初始化，直接返回空表
	if (maze.size() == 0)
		return path;
	auto result = findPath(startPoint, endPoint, isIgnoreCorner);
	//返回路径，如果没找到路径，返回空表
	if (result.first && result.second)
	{
		auto p1 = result.first;
		auto p2 = result.second;
		while (p1)
		{
			path.push_front(*p1);
			p1 = p1->parent;
		}
		//p1,p2重复的栅格不要push两次！
		p2 = p2->parent;
		while (p2)
		{
			path.push_back(*p2);
			p2 = p2->parent;
		}

	}

	// 清空临时开闭列表与对应的辅助列表，
	//delete所有指针
	for (auto& p : openList)
		delete p;
	for (auto& p : closeList)
		delete p;
	for (auto& p : reverse_openList)
		delete p;
	for (auto& p : reverse_closeList)
		delete p;
	openList.clear();
	closeList.clear();
	openListOfHash.clear();
	closeListOfHash.clear();
	reverse_openList.clear();
	reverse_closeList.clear();
	reverse_openListOfHash.clear();
	reverse_closeListOfHash.clear();
	//将path中的栅格的无效parent指针置为null
	for (auto& point : path)
		point.parent = nullptr;
	return path;
}

Point* Astar::GetPointOfList(const std::list<Point*>& list, const Point* point) const
{
	//获取在列表中与某个节点相同的节点，这里不能比较指针，因为每次加入列表是新开辟的节点，只能比较坐标

	for (auto ite = list.begin(); ite != list.end(); ++ite)
	{
		auto p = *ite;
		if (p->x == point->x && p->y == point->y)
			return p;
	}
	return nullptr;
}

bool Astar::isCanreach(const Point* point, const Point* target, bool isIgnoreCorner, bool direction) const
{
	bool isInCloseList;//判断是否在关闭列表中，有正向、反向A*之分
	if (direction)
		isInCloseList = isInCloseList_Forward(*target);
	else
		isInCloseList = isInCloseList_Reverse(*target);

	if (target->x<0 || target->x>maze.size() - 1
		|| target->y<0 || target->y>maze[0].size() - 1
		|| maze[target->x][target->y] == 1
		|| target->x == point->x && target->y == point->y
		|| isInCloseList) //如果点与当前节点重合、超出地图、是障碍物、或者在关闭列表中，返回false
		return false;
	else
	{
		if (abs(point->x - target->x) + abs(point->y - target->y) == 1) //非斜角可以
			return true;
		else
		{
			//斜对角要判断是否绊住
			if (maze[point->x][target->y] == 0 && maze[target->x][point->y] == 0)
				return true;
			else
				return isIgnoreCorner;
		}
	}
}

//************************************
// Method:    getSurroundPoints
// FullName:  Astar::getSurroundPoints
// Access:    private 
// Returns:   std::vector<Point *> 邻接栅格数组
// Qualifier: const 获取当前栅格的邻接栅格
// Parameter: const Point * point	当前栅格
// Parameter: bool isIgnoreCorner	是否忽略边角
// Parameter: bool direction		正向反向A*
//************************************
std::vector<Point*> Astar::getSurroundPoints(const Point* point, bool isIgnoreCorner, bool direction) const
{
	std::vector<Point*> surroundPoints;
	//修改处
	//方向左上方为0，按行列顺序增加：0 1 2 3 4 5 6 7 表示 左上 上 右上  左 右 左下 下 右下
	int direction_type = 0;
	for (int x = point->x - 1; x <= point->x + 1; x++)
		for (int y = point->y - 1; y <= point->y + 1; y++)
		{
			if (x != point->x || y != point->y)
			{
				Point curPoint(x, y);
				Point* p = &curPoint;
				if (isCanreach(point, p, isIgnoreCorner, direction) && !CollisionCheck(x, y, direction_type))
					surroundPoints.push_back(new Point(x, y));
				direction_type++;
			}
		}

	return surroundPoints;
}

//************************************
// Method:    CollisionCheck
// FullName:  Astar::CollisionCheck
// Access:    private 
// Returns:   bool
// Qualifier: const 根据当前栅格、车类型和方向判断是否发生碰撞，如果是则返回true，否则返回false
// Parameter: int x 栅格x坐标
// Parameter: int y 栅格y坐标
// Parameter: int direction 方向左上方为0，按行列顺序增加：0 1 2 3 4 5 6 7 表示 左上 上 右上  左 右 左下 下 右下
//************************************
bool Astar::CollisionCheck(int x, int y, int direction) const
{
	//修改：将车作为一个正方形，只是以头部边界不碰撞来检测，且头部边界距离中心点为车宽，因此不区分车的类型；
	bool collision_check = false;
	//方向左上方为0，按行列顺序增加：0 1 2 3 4 5 6 7 表示 左上 上 右上  左 右 左下 下 右下
	//根据方向来判断是否发生碰撞
	switch (direction)
	{
	case 0://左上
	{
		//确定车的边界四个点
		pair<double, double> p1, p2, p3, p4;
		p1.first = x - head_distance / sqrt(2) + left_distace / sqrt(2);
		p1.second = y - head_distance / sqrt(2) - left_distace / sqrt(2) - CollisionRadius;
		p2.first = x + heel_distance / sqrt(2) + left_distace / sqrt(2) + CollisionRadius;
		p2.second = y + heel_distance / sqrt(2) - left_distace / sqrt(2);
		p3.first = x + heel_distance / sqrt(2) - right_distance / sqrt(2);
		p3.second = y + heel_distance / sqrt(2) + right_distance / sqrt(2) + CollisionRadius;
		p4.first = x - head_distance / sqrt(2) - right_distance / sqrt(2) - CollisionRadius;
		p4.second = y - head_distance / sqrt(2) + right_distance / sqrt(2);
		//使用for循环遍历外围边界，这里需要将边界的四个点向上取整
		//判定各个边界栅格是否有障碍物，是则直接返回true，如果越界，跳过
		//边界1
		for (int i = GetIntBig(p1.first), j = GetIntBig(p1.second); i >= GetIntBig(p4.first); --i, ++j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//边界2
		for (int i = GetIntBig(p1.first), j = GetIntBig(p1.second); i <= GetIntBig(p2.first); ++i, ++j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//边界3
		for (int i = GetIntBig(p2.first), j = GetIntBig(p2.second); i >= GetIntBig(p3.first); --i, ++j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//边界4
		for (int i = GetIntBig(p4.first), j = GetIntBig(p4.second); i <= GetIntBig(p3.first); ++i, ++j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
	}break;
	case 1://上
	{
		//确定车的边界
		int boundary_1, boundary_2, boundary_3, boundary_4;
		//boundary_1 = x - (Car1_length / 2 - 1) - CollisionRadius;
		boundary_1 = (x - head_distance - CollisionRadius);
		boundary_2 = y - left_distace - CollisionRadius;
		boundary_3 = x + heel_distance + CollisionRadius;
		boundary_4 = y + right_distance + CollisionRadius;
		//边界数值转换为正整数
		boundary_1 = boundary_1 < 0 ? 0 : boundary_1;
		boundary_2 = boundary_2 < 0 ? 0 : boundary_2;
		boundary_3 = boundary_3 < 0 ? 0 : boundary_3;
		boundary_4 = boundary_4 < 0 ? 0 : boundary_4;
		//判定各个边界栅格是否有障碍物，是则直接返回true，如果越界，跳过
		for (int j = boundary_2, i = boundary_1; j <= boundary_4; ++j)
		{
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		}
		for (int i = boundary_1, j = boundary_2; i <= boundary_3; ++i)
		{
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		}

		for (int j = boundary_2, i = boundary_3; j <= boundary_4; ++j)
		{
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		}

		for (int i = boundary_1, j = boundary_4; i <= boundary_3; ++i)
		{
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		}
	}break;
	case 2://右上
	{
		//确定车的边界四个点
		pair<double, double> p1, p2, p3, p4;
		p1.first = x - head_distance / sqrt(2) - left_distace / sqrt(2) - CollisionRadius;
		p1.second = y + head_distance / sqrt(2) - left_distace / sqrt(2);
		p2.first = x + heel_distance / sqrt(2) - left_distace / sqrt(2);
		p2.second = y - heel_distance / sqrt(2) - left_distace / sqrt(2) - CollisionRadius;
		p3.first = x + heel_distance / sqrt(2) + right_distance / sqrt(2) + CollisionRadius;
		p3.second = y - heel_distance / sqrt(2) + right_distance / sqrt(2);
		p4.first = x - head_distance / sqrt(2) + right_distance / sqrt(2);
		p4.second = y + head_distance / sqrt(2) + right_distance / sqrt(2) + CollisionRadius;
		//使用for循环遍历外围边界，这里需要将边界的四个点向上取整
		//判定各个边界栅格是否有障碍物，是则直接返回true，如果越界，跳过
		//边界1
		for (int i = GetIntBig(p1.first), j = GetIntBig(p1.second); i <= GetIntBig(p4.first); ++i, ++j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//边界2
		for (int i = GetIntBig(p1.first), j = GetIntBig(p1.second); i <= GetIntBig(p2.first); ++i, --j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//边界3
		for (int i = GetIntBig(p2.first), j = GetIntBig(p2.second); i <= GetIntBig(p3.first); ++i, ++j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//边界4
		for (int i = GetIntBig(p4.first), j = GetIntBig(p4.second); i <= GetIntBig(p3.first); ++i, --j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;

	}break;

	case 3://左
	{
		//确定车的边界,边界1-4为逆时针顺序
		int boundary_1, boundary_2, boundary_3, boundary_4;
		boundary_1 = y - CollisionRadius - head_distance;
		boundary_2 = x + left_distace + CollisionRadius;
		boundary_3 = y + heel_distance + CollisionRadius;
		boundary_4 = x - right_distance - CollisionRadius;
		//边界数值转换为正整数
		boundary_1 = boundary_1 < 0 ? 0 : boundary_1;
		boundary_2 = boundary_2 < 0 ? 0 : boundary_2;
		boundary_3 = boundary_3 < 0 ? 0 : boundary_3;
		boundary_4 = boundary_4 < 0 ? 0 : boundary_4;
		//判定各个边界栅格是否有障碍物，是则直接返回true，如果越界，跳过
		for (int i = boundary_2, j = boundary_1; i >= boundary_4; --i)
		{
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		}
		for (int j = boundary_1, i = boundary_2; j <= boundary_3; ++j)
		{
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		}

		for (int i = boundary_2, j = boundary_3; i >= boundary_4; --i)
		{
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		}

		for (int j = boundary_1, i = boundary_4; j <= boundary_3; ++j)
		{
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		}
	}break;
	case 4://右
	{
		//确定车的边界,边界1-4为逆时针顺序
		int boundary_1, boundary_2, boundary_3, boundary_4;
		boundary_1 = y + CollisionRadius + head_distance;
		boundary_2 = x - left_distace - CollisionRadius;
		boundary_3 = y - heel_distance - CollisionRadius;
		boundary_4 = x + right_distance + CollisionRadius;
		//边界数值转换为正整数
		boundary_1 = boundary_1 < 0 ? 0 : boundary_1;
		boundary_2 = boundary_2 < 0 ? 0 : boundary_2;
		boundary_3 = boundary_3 < 0 ? 0 : boundary_3;
		boundary_4 = boundary_4 < 0 ? 0 : boundary_4;
		//判定各个边界栅格是否有障碍物，是则直接返回true，如果越界，跳过
		for (int i = boundary_2, j = boundary_1; i <= boundary_4; ++i)
		{
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		}
		for (int j = boundary_1, i = boundary_2; j >= boundary_3; --j)
		{
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		}
		for (int i = boundary_2, j = boundary_3; i <= boundary_4; ++i)
		{
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		}
		for (int j = boundary_1, i = boundary_4; j >= boundary_3; --j)
		{
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		}

	}break;
	case 5://左下
	{
		//确定车的边界四个点
		pair<double, double> p1, p2, p3, p4;
		p1.first = x + head_distance / sqrt(2) + left_distace / sqrt(2) + CollisionRadius;
		p1.second = y - head_distance / sqrt(2) + left_distace / sqrt(2);
		p2.first = x - heel_distance / sqrt(2) + left_distace / sqrt(2);
		p2.second = y + heel_distance / sqrt(2) + left_distace / sqrt(2) + CollisionRadius;
		p3.first = x - heel_distance / sqrt(2) - right_distance / sqrt(2) - CollisionRadius;
		p3.second = y + heel_distance / sqrt(2) - right_distance / sqrt(2);
		p4.first = x + head_distance / sqrt(2) - right_distance / sqrt(2);
		p4.second = y - head_distance / sqrt(2) - right_distance / sqrt(2) - CollisionRadius;
		//使用for循环遍历外围边界，这里需要将边界的四个点向上取整
		//判定各个边界栅格是否有障碍物，是则直接返回true，如果越界，跳过
		//边界1
		for (int i = GetIntBig(p1.first), j = GetIntBig(p1.second); i >= GetIntBig(p4.first); --i, --j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//边界2
		for (int i = GetIntBig(p1.first), j = GetIntBig(p1.second); i >= GetIntBig(p2.first); --i, ++j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//边界3
		for (int i = GetIntBig(p2.first), j = GetIntBig(p2.second); i >= GetIntBig(p3.first); --i, --j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//边界4
		for (int i = GetIntBig(p4.first), j = GetIntBig(p4.second); i >= GetIntBig(p3.first); --i, ++j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;

	}break;
	case 6://下
	{
		//确定车的边界,边界1-4为逆时针顺序
		int boundary_1, boundary_2, boundary_3, boundary_4;
		boundary_1 = x + CollisionRadius + head_distance;
		boundary_2 = y + left_distace + CollisionRadius;
		boundary_3 = x - heel_distance - CollisionRadius;
		boundary_4 = y - right_distance - CollisionRadius;
		//边界数值转换为正整数
		boundary_1 = boundary_1 < 0 ? 0 : boundary_1;
		boundary_2 = boundary_2 < 0 ? 0 : boundary_2;
		boundary_3 = boundary_3 < 0 ? 0 : boundary_3;
		boundary_4 = boundary_4 < 0 ? 0 : boundary_4;
		//判定各个边界栅格是否有障碍物，是则直接返回true
		for (int j = boundary_2, i = boundary_1; j >= boundary_4; --j)
		{
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		}
		for (int i = boundary_1, j = boundary_2; i >= boundary_3; --i)
		{
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		}
		for (int j = boundary_2, i = boundary_3; j >= boundary_4; --j)
		{
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		}
		for (int i = boundary_1, j = boundary_4; i >= boundary_3; --i)
		{
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		}

	}break;
	case 7://右下
	{
		//确定车的边界四个点
		pair<double, double> p1, p2, p3, p4;
		p1.first = x + head_distance / sqrt(2) - left_distace / sqrt(2);
		p1.second = y + head_distance / sqrt(2) + left_distace / sqrt(2) + CollisionRadius;
		p2.first = x - heel_distance / sqrt(2) - left_distace / sqrt(2) - CollisionRadius;
		p2.second = y - heel_distance / sqrt(2) + left_distace / sqrt(2);
		p3.first = x - heel_distance / sqrt(2) + right_distance / sqrt(2);
		p3.second = y - heel_distance / sqrt(2) - right_distance / sqrt(2) - CollisionRadius;
		p4.first = x + head_distance / sqrt(2) + right_distance / sqrt(2) + CollisionRadius;
		p4.second = y + head_distance / sqrt(2) - right_distance / sqrt(2);
		//使用for循环遍历外围边界，这里需要将边界的四个点向上取整
		//判定各个边界栅格是否有障碍物，是则直接返回false
		//边界1
		for (int i = GetIntBig(p1.first), j = GetIntBig(p1.second); i <= GetIntBig(p4.first); ++i, --j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//边界2
		for (int i = GetIntBig(p1.first), j = GetIntBig(p1.second); i >= GetIntBig(p2.first); --i, --j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//边界3
		for (int i = GetIntBig(p2.first), j = GetIntBig(p2.second); i <= GetIntBig(p3.first); ++i, --j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//边界4
		for (int i = GetIntBig(p4.first), j = GetIntBig(p4.second); i >= GetIntBig(p3.first); --i, --j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;

	}break;

	}

	return collision_check;
}
bool Astar::isInOpenList_Forward(const Point* point) const
{
	auto got = openListOfHash.find(*point);
	if (got == openListOfHash.end())
		return false;
	else
		return true;
}
bool Astar::isInCloseList_Forward(const Point point) const
{
	auto got = closeListOfHash.find(point);
	if (got == closeListOfHash.end())
		return false;
	else
		return true;
}
bool Astar::isInOpenList_Reverse(const Point* point) const
{
	auto got = reverse_openListOfHash.find(*point);
	if (got == reverse_openListOfHash.end())
		return false;
	else
		return true;
}
bool Astar::isInCloseList_Reverse(const Point point) const
{
	auto got = reverse_closeListOfHash.find(point);
	if (got == reverse_closeListOfHash.end())
		return false;
	else
		return true;
}
void Astar::printGraphToCSV() const
{
	FILE* fd = NULL;
	if ((fd = fopen("Graph_Astar.csv", "wt+")) != NULL)
	{
		//存储图信息
		for (auto row : maze)
		{
			//栅格0 1 反转 为了使用matlab显示
			for (auto i : row)
				fprintf(fd, "%d,", 1 - i);
			fprintf(fd, "\n");
		}
	}
	fclose(fd);
	fd = NULL;
}
void Astar::printPathToGraphInCSV(const list<Point>& path, const  Point& start, const Point& end)
{
	//设置路径
	for (const auto& p : path)
	{
		maze[p.x][p.y] = 1;
	}
	//设置起点与终点
	maze[start.x][start.y] = 1;
	maze[end.x][end.y] = 1;

	FILE* fd = NULL;
	if ((fd = fopen("Graph_Astar.csv", "wt+")) != NULL)
	{
		//存储图信息
		for (auto row : maze)
		{
			for (auto i : row)
				fprintf(fd, "%d,", 1 - i);
			fprintf(fd, "\n");
		}
	}
	fclose(fd);
	fd = NULL;
}
bool operator==(const Point& A, const Point& B) {
	return (A.x == B.x && A.y == B.y);
};