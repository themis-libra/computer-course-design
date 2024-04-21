#include "pch.h"
#include <math.h>
#include<cmath>
#include "AStar.h"
#include "Calculate.h"
using namespace std;
Astar::Astar(std::vector<std::vector<int>>& _maze) :maze(_maze) {}
//ʹ���ⲿ�ϰ�����Ϣ����
Astar::Astar(int rows, int cols, const std::vector<Car_Info>& obstacle_info)
{
	//���ɵ�ͼ���� ��
	maze.reserve(rows);
	vector<int> row(cols, 0);
	for (int i = 0; i < rows; i++)
		maze.push_back(row);
	//�����ϰ�����Ϣ������Ӧ��դ����Ϊ1
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
// Qualifier:��������դ����복�ĸ��߽�ĸ�������
// Parameter: int head	����դ����ǰ�߽�դ��ľ��������
// Parameter: int heel	����դ�����߽�դ���~
// Parameter: int left	����դ������߽�դ���~
// Parameter: int right	����դ�����ұ߽�դ���~
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
// Qualifier:������ײ��ȫ����
// Parameter: int len ��ȫ����
//************************************
void Astar::SetCollisionRadius(int len)
{
	CollisionRadius = len;
}

int Astar::calcG(Point* temp_start, Point* point)
{
	int extraG = (abs(point->x - temp_start->x) + abs(point->y - temp_start->y)) == 1 ? kCost1 : kCost2;
	int parentG = point->parent == NULL ? 0 : point->parent->G; //����ǳ�ʼ�ڵ㣬���丸�ڵ��ǿ�
	return parentG + extraG;
}

int Astar::calcH(Point* point, Point* end)
{
	//�ü򵥵�ŷ����þ������H�����H�ļ����ǹؼ������кܶ��㷨��û�����о�^_^
	return static_cast<int>(sqrt((double)(end->x - point->x) * (double)(end->x - point->x) + (double)(end->y - point->y) * (double)(end->y - point->y)) * kCost1);;
}

int Astar::calcF(Point* point)
{
	//��̬��Ȩ
	/*��������ʼʱ�����ٵ���Ŀ�ĵ������������Ҫ��
	����������ʱ���õ�����Ŀ������·������Ҫ��*/
	//ȡ��ǰդ���յ�դ��ľ���
	int w = static_cast<int>(sqrt(point->H));
	//�����ȡwֵ
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
	openList.push_back(new Point(startPoint.x, startPoint.y)); //�������
	reverse_openList.push_back(new Point(endPoint.x, endPoint.y)); //�����յ�
	//�����б����ṹ���դ��
	openListOfHash.insert(Point(startPoint.x, startPoint.y));
	reverse_openListOfHash.insert(Point(endPoint.x, endPoint.y));
	//0����ʼ��������ǰ��cur_grid1,cur_grid2
	Point* cur_grid1 = openList.front();
	Point* cur_grid2 = reverse_openList.front();
	while (!openList.empty() && !reverse_openList.empty())
	{
		//����A*
		//����1��
		Point* curPoint = cur_grid1; //��cur_grid1��Ϊ��ǰ��. 
		openList.remove(curPoint); //���������б���ɾ��
		//�����б����ṹɾ��դ��
		openListOfHash.erase(*curPoint);
		closeList.push_back(curPoint); //�ŵ�����ر��б�
		//����ر��б����ṹ����դ��
		closeListOfHash.insert(*curPoint);
		//����2���ҵ���������µ�ǰ��Χ�˸����п���ͨ���ĸ���
		std::vector<Point*>  surroundPoints = getSurroundPoints(curPoint, isIgnoreCorner, true);
		for (auto& target : surroundPoints)
		{
			//����3����ĳһ�����ӣ�����������������б��У�������ӽ� "�������б�", �ѵ�ǰ����Ϊ��һ��ĸ��ڵ�, ������cur_grid2��������һ��� FGH 
			if (!isInOpenList_Forward(target))
			{
				target->parent = curPoint;

				target->G = calcG(curPoint, target);
				target->H = calcH(target, cur_grid2);
				target->F = calcF(target);
				openList.push_back(target);
				//�������б����ṹ���դ��
				openListOfHash.insert(*target);
			}
			//����4����ĳһ�����ӣ������������б��У�����Gֵ, �����ԭ���Ĵ�, ��ʲô������, �����������ĸ��ڵ�Ϊ��ǰ��,������G��F
			else
			{
				//��ȡ�����б���target��Ӧ��point
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
		//����5��if(cur_grid2�Ѿ��� "�������б�" ) �ҵ�·��; return;
		if (isInOpenList_Forward(cur_grid2))
		{
			//Point *resPoint = GetPointOfList(openList, cur_grid2);
			//���������б����ҵ�cur_grid2����Ӧ��grid1��ʹ�ô�grid1���ݼ��ɵõ�����A*���ֵ�·��
			//ֱ�Ӵ�cur_grid2���ݣ��ɵõ�����A*���ֵ�·��
			auto gtePoint = GetPointOfList(openList, cur_grid2);
			if (gtePoint != nullptr)
				cur_grid1 = gtePoint;
			//����cur_grid1��cur_grid2
			return make_pair(cur_grid1, cur_grid2);
		}
		//����6����cur_grid1����Ϊ�������б��Fֵ��С��դ��
		cur_grid1 = getLeastFpoint_Forward();


		//����A*
		//����1��
		//curPoint = cur_grid2; //��cur_grid2��Ϊ��ǰ��. 
		//reverse_openList.remove(curPoint); //�ӷ������б���ɾ��
		////�����б����ṹɾ��դ��
		//reverse_openListOfHash.erase(*curPoint);
		//reverse_closeList.push_back(curPoint); //�ŵ�����ر��б�
		////����ر��б����ṹ����դ��
		//reverse_closeListOfHash.insert(*curPoint);
		////����2���ҵ���������µ�ǰ��Χ�˸����п���ͨ���ĸ���
		//surroundPoints.clear();
		//surroundPoints = getSurroundPoints(curPoint, isIgnoreCorner, false);
		//for (auto &target : surroundPoints)
		//{
		//	//����3����ĳһ�����ӣ���������ڷ������б��У�������ӽ� "�������б�", �ѵ�ǰ����Ϊ��һ��ĸ��ڵ�, ������cur_grid1��������һ��� FGH 
		//	if (!isInOpenList_Reverse(target))
		//	{
		//		target->parent = curPoint;

		//		target->G = calcG(curPoint, target);
		//		target->H = calcH(target, cur_grid1);
		//		target->F = calcF(target);
		//		reverse_openList.push_back(target);
		//		//�������б����ṹ���դ��
		//		reverse_openListOfHash.insert(*target);
		//	}
		//	//����4����ĳһ�����ӣ����ڷ������б��У�����Gֵ, �����ԭ���Ĵ�, ��ʲô������, �����������ĸ��ڵ�Ϊ��ǰ��,������G��F
		//	else
		//	{
		//		//��ȡ�����б���target��Ӧ��point
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
		//// ����5��if(cur_grid1�Ѿ��� "�������б�" ) �ҵ�·��; return;
		//if (isInOpenList_Reverse(cur_grid1))
		//{
		//	//�ӷ������б����ҵ�cur_grid1����Ӧ��grid2��ʹ�ô�grid2���ݼ��ɵõ�����A*���ֵ�·��
		//	//ֱ�Ӵ�cur_grid1���ݣ��ɵõ�����A*���ֵ�·��
		//	auto gtePoint = GetPointOfList(reverse_openList, cur_grid1);
		//	if (gtePoint != nullptr)
		//		cur_grid2 = gtePoint;
		//	//����cur_grid1��cur_grid2
		//	return make_pair(cur_grid1, cur_grid2);
		//}
		////����6����cur_grid2����Ϊ�������б��Fֵ��С��դ��
		//cur_grid2 = getLeastFpoint_Reverse();

	}

	return make_pair(nullptr, nullptr);
}

std::list<Point> Astar::GetPath(Point& startPoint, Point& endPoint, bool isIgnoreCorner)
{
	std::list<Point> path;
	//���û�ж�դ���ͼmaze��ʼ����ֱ�ӷ��ؿձ�
	if (maze.size() == 0)
		return path;
	auto result = findPath(startPoint, endPoint, isIgnoreCorner);
	//����·�������û�ҵ�·�������ؿձ�
	if (result.first && result.second)
	{
		auto p1 = result.first;
		auto p2 = result.second;
		while (p1)
		{
			path.push_front(*p1);
			p1 = p1->parent;
		}
		//p1,p2�ظ���դ��Ҫpush���Σ�
		p2 = p2->parent;
		while (p2)
		{
			path.push_back(*p2);
			p2 = p2->parent;
		}

	}

	// �����ʱ�����б����Ӧ�ĸ����б�
	//delete����ָ��
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
	//��path�е�դ�����Чparentָ����Ϊnull
	for (auto& point : path)
		point.parent = nullptr;
	return path;
}

Point* Astar::GetPointOfList(const std::list<Point*>& list, const Point* point) const
{
	//��ȡ���б�����ĳ���ڵ���ͬ�Ľڵ㣬���ﲻ�ܱȽ�ָ�룬��Ϊÿ�μ����б����¿��ٵĽڵ㣬ֻ�ܱȽ�����

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
	bool isInCloseList;//�ж��Ƿ��ڹر��б��У������򡢷���A*֮��
	if (direction)
		isInCloseList = isInCloseList_Forward(*target);
	else
		isInCloseList = isInCloseList_Reverse(*target);

	if (target->x<0 || target->x>maze.size() - 1
		|| target->y<0 || target->y>maze[0].size() - 1
		|| maze[target->x][target->y] == 1
		|| target->x == point->x && target->y == point->y
		|| isInCloseList) //������뵱ǰ�ڵ��غϡ�������ͼ�����ϰ�������ڹر��б��У�����false
		return false;
	else
	{
		if (abs(point->x - target->x) + abs(point->y - target->y) == 1) //��б�ǿ���
			return true;
		else
		{
			//б�Խ�Ҫ�ж��Ƿ��ס
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
// Returns:   std::vector<Point *> �ڽ�դ������
// Qualifier: const ��ȡ��ǰդ����ڽ�դ��
// Parameter: const Point * point	��ǰդ��
// Parameter: bool isIgnoreCorner	�Ƿ���Ա߽�
// Parameter: bool direction		������A*
//************************************
std::vector<Point*> Astar::getSurroundPoints(const Point* point, bool isIgnoreCorner, bool direction) const
{
	std::vector<Point*> surroundPoints;
	//�޸Ĵ�
	//�������Ϸ�Ϊ0��������˳�����ӣ�0 1 2 3 4 5 6 7 ��ʾ ���� �� ����  �� �� ���� �� ����
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
// Qualifier: const ���ݵ�ǰդ�񡢳����ͺͷ����ж��Ƿ�����ײ��������򷵻�true�����򷵻�false
// Parameter: int x դ��x����
// Parameter: int y դ��y����
// Parameter: int direction �������Ϸ�Ϊ0��������˳�����ӣ�0 1 2 3 4 5 6 7 ��ʾ ���� �� ����  �� �� ���� �� ����
//************************************
bool Astar::CollisionCheck(int x, int y, int direction) const
{
	//�޸ģ�������Ϊһ�������Σ�ֻ����ͷ���߽粻��ײ����⣬��ͷ���߽�������ĵ�Ϊ������˲����ֳ������ͣ�
	bool collision_check = false;
	//�������Ϸ�Ϊ0��������˳�����ӣ�0 1 2 3 4 5 6 7 ��ʾ ���� �� ����  �� �� ���� �� ����
	//���ݷ������ж��Ƿ�����ײ
	switch (direction)
	{
	case 0://����
	{
		//ȷ�����ı߽��ĸ���
		pair<double, double> p1, p2, p3, p4;
		p1.first = x - head_distance / sqrt(2) + left_distace / sqrt(2);
		p1.second = y - head_distance / sqrt(2) - left_distace / sqrt(2) - CollisionRadius;
		p2.first = x + heel_distance / sqrt(2) + left_distace / sqrt(2) + CollisionRadius;
		p2.second = y + heel_distance / sqrt(2) - left_distace / sqrt(2);
		p3.first = x + heel_distance / sqrt(2) - right_distance / sqrt(2);
		p3.second = y + heel_distance / sqrt(2) + right_distance / sqrt(2) + CollisionRadius;
		p4.first = x - head_distance / sqrt(2) - right_distance / sqrt(2) - CollisionRadius;
		p4.second = y - head_distance / sqrt(2) + right_distance / sqrt(2);
		//ʹ��forѭ��������Χ�߽磬������Ҫ���߽���ĸ�������ȡ��
		//�ж������߽�դ���Ƿ����ϰ������ֱ�ӷ���true�����Խ�磬����
		//�߽�1
		for (int i = GetIntBig(p1.first), j = GetIntBig(p1.second); i >= GetIntBig(p4.first); --i, ++j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//�߽�2
		for (int i = GetIntBig(p1.first), j = GetIntBig(p1.second); i <= GetIntBig(p2.first); ++i, ++j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//�߽�3
		for (int i = GetIntBig(p2.first), j = GetIntBig(p2.second); i >= GetIntBig(p3.first); --i, ++j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//�߽�4
		for (int i = GetIntBig(p4.first), j = GetIntBig(p4.second); i <= GetIntBig(p3.first); ++i, ++j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
	}break;
	case 1://��
	{
		//ȷ�����ı߽�
		int boundary_1, boundary_2, boundary_3, boundary_4;
		//boundary_1 = x - (Car1_length / 2 - 1) - CollisionRadius;
		boundary_1 = (x - head_distance - CollisionRadius);
		boundary_2 = y - left_distace - CollisionRadius;
		boundary_3 = x + heel_distance + CollisionRadius;
		boundary_4 = y + right_distance + CollisionRadius;
		//�߽���ֵת��Ϊ������
		boundary_1 = boundary_1 < 0 ? 0 : boundary_1;
		boundary_2 = boundary_2 < 0 ? 0 : boundary_2;
		boundary_3 = boundary_3 < 0 ? 0 : boundary_3;
		boundary_4 = boundary_4 < 0 ? 0 : boundary_4;
		//�ж������߽�դ���Ƿ����ϰ������ֱ�ӷ���true�����Խ�磬����
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
	case 2://����
	{
		//ȷ�����ı߽��ĸ���
		pair<double, double> p1, p2, p3, p4;
		p1.first = x - head_distance / sqrt(2) - left_distace / sqrt(2) - CollisionRadius;
		p1.second = y + head_distance / sqrt(2) - left_distace / sqrt(2);
		p2.first = x + heel_distance / sqrt(2) - left_distace / sqrt(2);
		p2.second = y - heel_distance / sqrt(2) - left_distace / sqrt(2) - CollisionRadius;
		p3.first = x + heel_distance / sqrt(2) + right_distance / sqrt(2) + CollisionRadius;
		p3.second = y - heel_distance / sqrt(2) + right_distance / sqrt(2);
		p4.first = x - head_distance / sqrt(2) + right_distance / sqrt(2);
		p4.second = y + head_distance / sqrt(2) + right_distance / sqrt(2) + CollisionRadius;
		//ʹ��forѭ��������Χ�߽磬������Ҫ���߽���ĸ�������ȡ��
		//�ж������߽�դ���Ƿ����ϰ������ֱ�ӷ���true�����Խ�磬����
		//�߽�1
		for (int i = GetIntBig(p1.first), j = GetIntBig(p1.second); i <= GetIntBig(p4.first); ++i, ++j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//�߽�2
		for (int i = GetIntBig(p1.first), j = GetIntBig(p1.second); i <= GetIntBig(p2.first); ++i, --j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//�߽�3
		for (int i = GetIntBig(p2.first), j = GetIntBig(p2.second); i <= GetIntBig(p3.first); ++i, ++j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//�߽�4
		for (int i = GetIntBig(p4.first), j = GetIntBig(p4.second); i <= GetIntBig(p3.first); ++i, --j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;

	}break;

	case 3://��
	{
		//ȷ�����ı߽�,�߽�1-4Ϊ��ʱ��˳��
		int boundary_1, boundary_2, boundary_3, boundary_4;
		boundary_1 = y - CollisionRadius - head_distance;
		boundary_2 = x + left_distace + CollisionRadius;
		boundary_3 = y + heel_distance + CollisionRadius;
		boundary_4 = x - right_distance - CollisionRadius;
		//�߽���ֵת��Ϊ������
		boundary_1 = boundary_1 < 0 ? 0 : boundary_1;
		boundary_2 = boundary_2 < 0 ? 0 : boundary_2;
		boundary_3 = boundary_3 < 0 ? 0 : boundary_3;
		boundary_4 = boundary_4 < 0 ? 0 : boundary_4;
		//�ж������߽�դ���Ƿ����ϰ������ֱ�ӷ���true�����Խ�磬����
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
	case 4://��
	{
		//ȷ�����ı߽�,�߽�1-4Ϊ��ʱ��˳��
		int boundary_1, boundary_2, boundary_3, boundary_4;
		boundary_1 = y + CollisionRadius + head_distance;
		boundary_2 = x - left_distace - CollisionRadius;
		boundary_3 = y - heel_distance - CollisionRadius;
		boundary_4 = x + right_distance + CollisionRadius;
		//�߽���ֵת��Ϊ������
		boundary_1 = boundary_1 < 0 ? 0 : boundary_1;
		boundary_2 = boundary_2 < 0 ? 0 : boundary_2;
		boundary_3 = boundary_3 < 0 ? 0 : boundary_3;
		boundary_4 = boundary_4 < 0 ? 0 : boundary_4;
		//�ж������߽�դ���Ƿ����ϰ������ֱ�ӷ���true�����Խ�磬����
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
	case 5://����
	{
		//ȷ�����ı߽��ĸ���
		pair<double, double> p1, p2, p3, p4;
		p1.first = x + head_distance / sqrt(2) + left_distace / sqrt(2) + CollisionRadius;
		p1.second = y - head_distance / sqrt(2) + left_distace / sqrt(2);
		p2.first = x - heel_distance / sqrt(2) + left_distace / sqrt(2);
		p2.second = y + heel_distance / sqrt(2) + left_distace / sqrt(2) + CollisionRadius;
		p3.first = x - heel_distance / sqrt(2) - right_distance / sqrt(2) - CollisionRadius;
		p3.second = y + heel_distance / sqrt(2) - right_distance / sqrt(2);
		p4.first = x + head_distance / sqrt(2) - right_distance / sqrt(2);
		p4.second = y - head_distance / sqrt(2) - right_distance / sqrt(2) - CollisionRadius;
		//ʹ��forѭ��������Χ�߽磬������Ҫ���߽���ĸ�������ȡ��
		//�ж������߽�դ���Ƿ����ϰ������ֱ�ӷ���true�����Խ�磬����
		//�߽�1
		for (int i = GetIntBig(p1.first), j = GetIntBig(p1.second); i >= GetIntBig(p4.first); --i, --j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//�߽�2
		for (int i = GetIntBig(p1.first), j = GetIntBig(p1.second); i >= GetIntBig(p2.first); --i, ++j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//�߽�3
		for (int i = GetIntBig(p2.first), j = GetIntBig(p2.second); i >= GetIntBig(p3.first); --i, --j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//�߽�4
		for (int i = GetIntBig(p4.first), j = GetIntBig(p4.second); i >= GetIntBig(p3.first); --i, ++j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;

	}break;
	case 6://��
	{
		//ȷ�����ı߽�,�߽�1-4Ϊ��ʱ��˳��
		int boundary_1, boundary_2, boundary_3, boundary_4;
		boundary_1 = x + CollisionRadius + head_distance;
		boundary_2 = y + left_distace + CollisionRadius;
		boundary_3 = x - heel_distance - CollisionRadius;
		boundary_4 = y - right_distance - CollisionRadius;
		//�߽���ֵת��Ϊ������
		boundary_1 = boundary_1 < 0 ? 0 : boundary_1;
		boundary_2 = boundary_2 < 0 ? 0 : boundary_2;
		boundary_3 = boundary_3 < 0 ? 0 : boundary_3;
		boundary_4 = boundary_4 < 0 ? 0 : boundary_4;
		//�ж������߽�դ���Ƿ����ϰ������ֱ�ӷ���true
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
	case 7://����
	{
		//ȷ�����ı߽��ĸ���
		pair<double, double> p1, p2, p3, p4;
		p1.first = x + head_distance / sqrt(2) - left_distace / sqrt(2);
		p1.second = y + head_distance / sqrt(2) + left_distace / sqrt(2) + CollisionRadius;
		p2.first = x - heel_distance / sqrt(2) - left_distace / sqrt(2) - CollisionRadius;
		p2.second = y - heel_distance / sqrt(2) + left_distace / sqrt(2);
		p3.first = x - heel_distance / sqrt(2) + right_distance / sqrt(2);
		p3.second = y - heel_distance / sqrt(2) - right_distance / sqrt(2) - CollisionRadius;
		p4.first = x + head_distance / sqrt(2) + right_distance / sqrt(2) + CollisionRadius;
		p4.second = y + head_distance / sqrt(2) - right_distance / sqrt(2);
		//ʹ��forѭ��������Χ�߽磬������Ҫ���߽���ĸ�������ȡ��
		//�ж������߽�դ���Ƿ����ϰ������ֱ�ӷ���false
		//�߽�1
		for (int i = GetIntBig(p1.first), j = GetIntBig(p1.second); i <= GetIntBig(p4.first); ++i, --j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//�߽�2
		for (int i = GetIntBig(p1.first), j = GetIntBig(p1.second); i >= GetIntBig(p2.first); --i, --j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//�߽�3
		for (int i = GetIntBig(p2.first), j = GetIntBig(p2.second); i <= GetIntBig(p3.first); ++i, --j)
			if (i < 0 || i >= maze.size() || j < 0 || j >= maze.front().size())
				;
			else if (maze[i][j] == 1)
				return true;
		//�߽�4
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
		//�洢ͼ��Ϣ
		for (auto row : maze)
		{
			//դ��0 1 ��ת Ϊ��ʹ��matlab��ʾ
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
	//����·��
	for (const auto& p : path)
	{
		maze[p.x][p.y] = 1;
	}
	//����������յ�
	maze[start.x][start.y] = 1;
	maze[end.x][end.y] = 1;

	FILE* fd = NULL;
	if ((fd = fopen("Graph_Astar.csv", "wt+")) != NULL)
	{
		//�洢ͼ��Ϣ
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