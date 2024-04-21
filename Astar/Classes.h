/*!
 * \file Classes.h
 * \date 2021/01/9 10:20
 *
 * \author lzk
 * Contact: 1274962676@qq.com
 *
 * \brief 辅助类集合模块
 *
 * TODO: long description
 *		（1）存放各种辅助类；
 * \note
*/
#pragma once


//A*算法-栅格类
struct Point
{
	int x, y; //点坐标，这里为了方便按照C++的数组来计算，x代表横排，y代表竖列
	int F, G, H; //F=G+H
	Point* parent; //parent的坐标，这里用指针，简化代码
	Point(int _x, int _y) :x(_x), y(_y), F(0), G(0), H(0), parent(nullptr) {};//变量初始化
	Point(const Point& rh) :x(rh.x), y(rh.y), F(rh.F), G(rh.G), H(rh.H), parent(rh.parent) {};//复制构造函数
	Point operator=(const Point& rh)
	{
		this->x = rh.x;
		this->y = rh.y;
		this->F = rh.F;
		this->G = rh.G;
		this->H = rh.H;
		this->parent = rh.parent;
		return *this;
	}
	friend bool operator==(const Point& A, const Point& B);
};

//重载Point等于运算符
bool operator==(const Point& A, const Point& B);
//Point的hash函数对象类
class hash_fun_Point {
public:
	int operator()(const Point& A) const {
		return A.x * 360 + A.y;
	}
};

//导引点
struct PPoint
{
	double x = 0;//导引点x坐标
	double y = 0;//导引点y坐标
	PPoint(double _x, double _y) :x(_x), y(_y) {};
	PPoint() :x(0), y(0) {};
	PPoint(const PPoint& other) :x(other.x), y(other.y) {};
	const PPoint operator=(const PPoint& p)
	{
		x = p.x;
		y = p.y;
		return *this;
	}
	bool operator==(const PPoint& p)
	{
		return(abs(x - p.x) < 0.0001 && abs(y - p.y) < 0.0001);
	}
};
struct RoutePoint
{
	PPoint coordinate;
	double angle_z;//艏摇角
	double angle_x;//
	double angle_y;
	RoutePoint(PPoint p, double angle1, double angle2, double angle3) :
		coordinate(p), angle_z(angle1), angle_x(angle2), angle_y(angle3) {};
	RoutePoint operator=(const RoutePoint& other)
	{
		coordinate = other.coordinate;
		angle_z = other.angle_z;
		angle_x = other.angle_x;
		angle_y = other.angle_y;
	}
};
//载具信息类
//坦克、战车、两栖战车（用2、3、4表示）
//坦克为9.6 * 3.4 战车为8.2 * 3.4
struct Car_Info
{
	PPoint car_center;//中心点坐标
	double car_length = 0;//载具长
	double car_width = 0;//载具宽
	double angle = 0;//载具倾斜角

};
/*布列方案中的数据结构*/
//输入接口：车辆信息
struct cars_info
{
	int vehicle_id;
	int vehicle_type;   //2代表长9.6宽3.4    3 4 代表长8.2宽3.4
};

//输入接口：气垫船信息
struct hovercraft_info
{
	int hovercraft_id;//气垫船编号
	int vehicle_counts;//载装载的载具个数
	std::vector<cars_info> vehicles;//载具数组
};

//输入接口：障碍物
struct obstacles_info
{
	double center_coordinate_x;//中心点x坐标
	double center_coordinate_y;//中心点y坐标
	double car_length = 0;//载具长
	double car_width = 0;//载具宽
	double angle = 0;//载具倾斜角

};




//输出接口：布列方案
struct Layout_Plan_End
{
	int vehicle_id;//载具编号
	unsigned int series_number;//载具布列的序号
	double end_coordinates_x;//终点载具中心点x坐标
	double end_coordinates_y;//终点载具中心点y坐标
	double direction;//终点载具方向

};
/*以下为解析json数据而准备的类*/



//路径规划部分输入
struct Ipp_Input
{
	//坦克、战车、两栖战车（用2、3、4表示）
	//坦克为9.6 * 3.4 战车为8.2 * 3.4
	int target_vehicle_type = 0;//目标载具类型
	double start_direction;//起点方向
	double end_direction;//终点方向
	PPoint start, end;//起点、终点坐标
	double speed;//载具速度
};

//单个载具导引点信息
struct Car_Guide_Points
{
	int vehicle_id;//载具编号
	std::vector<PPoint> guide_points;
};


//路径点序列信息
struct Info_Route_Points
{
	//载具编号
	int vehicle_id = 0;
	//第一次发生碰撞时的路径点序号，值从1开始
	size_t first_collison_id = 0;
	//路径点序列
	std::vector<RoutePoint> route_points;
};

//被绞车牵引的气垫船状态变量
struct TowedShip
{
	double x = 0;     //气垫船x轴坐标(m)
	double angle = 0; //气垫船艏向角(°)
};//被牵引气垫船对象
