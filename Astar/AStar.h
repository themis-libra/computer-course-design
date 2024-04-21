/*!
 * \file AStar.h
 * \date 2021/01/9 10:20
 *
 * \author lzk
 * Contact: 1274962676@qq.com
 *
 * \brief A*�㷨������
 *
 * TODO: long description
 *		��1��Astar�༯����A*�㷨�����й��̣��ܹ���������դ���ͼ���ҳ�����㵽�յ��դ��·�������У���
 *		��2��Astar����������·�����������еı�������֤�ҵ���·���ܹ��������Ϊ���դ����ؾ�ͨ����
 *		��3��Astar���ڲ�ʹ��˫��A*����ʹ�ø����ṹ����̬��Ȩ�����A*���ٶȣ�
  *		��4��Astar���ҵ���դ��·�����������ţ��̣�·����
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


//A*�㷨��
class Astar final
{
public:
	//ɾ�����Ĭ�Ϲ��캯�������ƹ���͸�ֵ�����
	Astar() = delete;
	Astar(const Astar&) = delete;
	Astar& operator=(const Astar&) = delete;
	//ʹ���ⲿդ���ͼmaze����
	explicit Astar(std::vector<std::vector<int>>& _maze);
	//ʹ���ⲿ�ϰ�����Ϣ����
	explicit Astar(int rows, int cols, const std::vector<Car_Info>& obstacle_info);
	//ʹ��Ĭ����������
	~Astar() = default;
	//��������դ����복�ĸ��߽�ĸ�������
	void SetCarInfo(int head, int heel, int left, int right);
	//������ײ��ȫ����
	void SetCollisionRadius(int len);
	std::list<Point> GetPath(Point& startPoint, Point& endPoint, bool isIgnoreCorner);
	void printGraphToCSV() const;
	void printPathToGraphInCSV(const std::list<Point>& path, const Point& start, const Point& end);
private:
	std::pair<Point*, Point*> findPath(Point& startPoint, Point& endPoint, bool isIgnoreCorner);
	std::vector<Point*> getSurroundPoints(const Point* point, bool isIgnoreCorner, bool direction) const;
	bool isCanreach(const Point* point, const Point* target, bool isIgnoreCorner, bool direction) const; //�ж�ĳ���Ƿ����������һ���ж�,direction�б��������A*
	Point* GetPointOfList(const std::list<Point*>& list, const Point* point) const; //�ӿ���/�ر��б��л�ȡĳ��
	Point* getLeastFpoint_Forward() const; //���������б��з���Fֵ��С�Ľڵ�
	Point* getLeastFpoint_Reverse() const; //�ӷ������б��з���Fֵ��С�Ľڵ�
	bool isInOpenList_Forward(const Point* point) const; //�����ж�������/�ر��б����Ƿ����ĳ��
	bool isInCloseList_Forward(const Point point) const; //�����ж�������/�ر��б����Ƿ����ĳ��
	bool isInOpenList_Reverse(const Point* point) const; //�����жϷ�����/�ر��б����Ƿ����ĳ��
	bool isInCloseList_Reverse(const Point point) const; //�����жϷ�����/�ر��б����Ƿ����ĳ��
	//����FGHֵ
	int calcG(Point* temp_start, Point* point);
	int calcH(Point* point, Point* end);
	int calcF(Point* point);
	//դ���ͼ
	std::vector<std::vector<int>> maze;
	std::list<Point*> openList;  //�������б�
	std::list<Point*> closeList; //����ر��б�
	std::list<Point*> reverse_openList;  //�������б�
	std::list<Point*> reverse_closeList; //����ر��б�

	//�����ڿ����б�͹ر��б��е��������� �Ҳ�������Ľṹ
	//�����������򡢷������б�͹ر��б��е�դ��
	std::unordered_set<Point, hash_fun_Point> openListOfHash, closeListOfHash;
	std::unordered_set<Point, hash_fun_Point> reverse_openListOfHash, reverse_closeListOfHash;
	bool CollisionCheck(int x, int y, int direction) const;
	enum {
		kCost1 = 10,//ֱ��һ������10
		kCost2 = 14, //б��һ������14
	};
	//����դ������ؾ�ǰ��β������ұ߽�ľ���
	int head_distance = 23, heel_distance = 24, left_distace = 8, right_distance = 8;
	int CollisionRadius = 2;//��ײ��ȫ���룬Ĭ��Ϊ2��դ��
};
