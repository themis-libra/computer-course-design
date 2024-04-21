/*!
 * \file AStar.h
 * \date 2021/01/9 10:20
 *
 * \author lzk
 * Contact: 1274962676@qq.com
 *
 * \brief A*算法对象类
 *
 * TODO: long description
 *		（1）Astar类集成了A*算法的所有过程，能够在外界给出栅格地图后，找出从起点到终点的栅格路径（序列）；
 *		（2）Astar类增加了在路径搜索过程中的避碰，保证找到的路径能够满足面积为多个栅格的载具通过；
 *		（3）Astar类内部使用双向A*、并使用辅助结构、动态加权提高了A*的速度；
  *		（4）Astar类找到的栅格路径并不是最优（短）路径；
 * \note
*/
#pragma once

#include "pch.h"
#include <vector>
#include <list>
#include <unordered_set>
#include <set>
#include "ConfigurationPlan.h"
#include "Classes.h"


//A*算法类
class Astar final
{
public:
	//删除类的默认构造函数、复制构造和赋值运算符
	Astar() = delete;
	Astar(const Astar&) = delete;
	Astar& operator=(const Astar&) = delete;
	//使用外部栅格地图maze构造
	explicit Astar(std::vector<std::vector<int>>& _maze);
	//使用外部障碍物信息构造
	explicit Astar(int rows, int cols, const std::vector<Car_Info>& obstacle_info);
	//使用默认析构函数
	~Astar() = default;
	//设置中心栅格距离车四个边界的格子字数
	void SetCarInfo(int head, int heel, int left, int right);
	//设置碰撞安全距离
	void SetCollisionRadius(int len);
	std::list<Point> GetPath(Point& startPoint, Point& endPoint, bool isIgnoreCorner);
	void printGraphToCSV() const;
	void printPathToGraphInCSV(const std::list<Point>& path, const Point& start, const Point& end);
private:
	std::pair<Point*, Point*> findPath(Point& startPoint, Point& endPoint, bool isIgnoreCorner);
	std::vector<Point*> getSurroundPoints(const Point* point, bool isIgnoreCorner, bool direction) const;
	bool isCanreach(const Point* point, const Point* target, bool isIgnoreCorner, bool direction) const; //判断某点是否可以用于下一步判断,direction判别正向或反向A*
	Point* GetPointOfList(const std::list<Point*>& list, const Point* point) const; //从开启/关闭列表中获取某点
	Point* getLeastFpoint_Forward() const; //从正向开启列表中返回F值最小的节点
	Point* getLeastFpoint_Reverse() const; //从反向开启列表中返回F值最小的节点
	bool isInOpenList_Forward(const Point* point) const; //快速判断正向开启/关闭列表中是否包含某点
	bool isInCloseList_Forward(const Point point) const; //快速判断正向开启/关闭列表中是否包含某点
	bool isInOpenList_Reverse(const Point* point) const; //快速判断反向开启/关闭列表中是否包含某点
	bool isInCloseList_Reverse(const Point point) const; //快速判断反向开启/关闭列表中是否包含某点
	//计算FGH值
	int calcG(Point* temp_start, Point* point);
	int calcH(Point* point, Point* end);
	int calcF(Point* point);
	//栅格地图
	std::vector<std::vector<int>> maze;
	std::list<Point*> openList;  //正向开启列表
	std::list<Point*> closeList; //正向关闭列表
	std::list<Point*> reverse_openList;  //反向开启列表
	std::list<Point*> reverse_closeList; //反向关闭列表

	//辅助在开启列表和关闭列表中的搜索、查 找操作引入的结构
	//用于搜索正向、反向开启列表和关闭列表中的栅格
	std::unordered_set<Point, hash_fun_Point> openListOfHash, closeListOfHash;
	std::unordered_set<Point, hash_fun_Point> reverse_openListOfHash, reverse_closeListOfHash;
	bool CollisionCheck(int x, int y, int direction) const;
	enum {
		kCost1 = 10,//直移一格消耗10
		kCost2 = 14, //斜移一格消耗14
	};
	//中心栅格距离载具前、尾、左和右边界的距离
	int head_distance = 23, heel_distance = 24, left_distace = 8, right_distance = 8;
	int CollisionRadius = 2;//碰撞安全距离，默认为2个栅格
};
