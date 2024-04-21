/*!
 * \file Classes.h
 * \date 2021/01/9 10:20
 *
 * \author lzk
 * Contact: 1274962676@qq.com
 *
 * \brief �����༯��ģ��
 *
 * TODO: long description
 *		��1����Ÿ��ָ����ࣻ
 * \note
*/
#pragma once


//A*�㷨-դ����
struct Point
{
	int x, y; //�����꣬����Ϊ�˷��㰴��C++�����������㣬x������ţ�y��������
	int F, G, H; //F=G+H
	Point* parent; //parent�����꣬������ָ�룬�򻯴���
	Point(int _x, int _y) :x(_x), y(_y), F(0), G(0), H(0), parent(nullptr) {};//������ʼ��
	Point(const Point& rh) :x(rh.x), y(rh.y), F(rh.F), G(rh.G), H(rh.H), parent(rh.parent) {};//���ƹ��캯��
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

//����Point���������
bool operator==(const Point& A, const Point& B);
//Point��hash����������
class hash_fun_Point {
public:
	int operator()(const Point& A) const {
		return A.x * 360 + A.y;
	}
};

//������
struct PPoint
{
	double x = 0;//������x����
	double y = 0;//������y����
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
	double angle_z;//��ҡ��
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
//�ؾ���Ϣ��
//̹�ˡ�ս��������ս������2��3��4��ʾ��
//̹��Ϊ9.6 * 3.4 ս��Ϊ8.2 * 3.4
struct Car_Info
{
	PPoint car_center;//���ĵ�����
	double car_length = 0;//�ؾ߳�
	double car_width = 0;//�ؾ߿�
	double angle = 0;//�ؾ���б��

};
/*���з����е����ݽṹ*/
//����ӿڣ�������Ϣ
struct cars_info
{
	int vehicle_id;
	int vehicle_type;   //2����9.6��3.4    3 4 ����8.2��3.4
};

//����ӿڣ����洬��Ϣ
struct hovercraft_info
{
	int hovercraft_id;//���洬���
	int vehicle_counts;//��װ�ص��ؾ߸���
	std::vector<cars_info> vehicles;//�ؾ�����
};

//����ӿڣ��ϰ���
struct obstacles_info
{
	double center_coordinate_x;//���ĵ�x����
	double center_coordinate_y;//���ĵ�y����
	double car_length = 0;//�ؾ߳�
	double car_width = 0;//�ؾ߿�
	double angle = 0;//�ؾ���б��

};




//����ӿڣ����з���
struct Layout_Plan_End
{
	int vehicle_id;//�ؾ߱��
	unsigned int series_number;//�ؾ߲��е����
	double end_coordinates_x;//�յ��ؾ����ĵ�x����
	double end_coordinates_y;//�յ��ؾ����ĵ�y����
	double direction;//�յ��ؾ߷���

};
/*����Ϊ����json���ݶ�׼������*/



//·���滮��������
struct Ipp_Input
{
	//̹�ˡ�ս��������ս������2��3��4��ʾ��
	//̹��Ϊ9.6 * 3.4 ս��Ϊ8.2 * 3.4
	int target_vehicle_type = 0;//Ŀ���ؾ�����
	double start_direction;//��㷽��
	double end_direction;//�յ㷽��
	PPoint start, end;//��㡢�յ�����
	double speed;//�ؾ��ٶ�
};

//�����ؾߵ�������Ϣ
struct Car_Guide_Points
{
	int vehicle_id;//�ؾ߱��
	std::vector<PPoint> guide_points;
};


//·����������Ϣ
struct Info_Route_Points
{
	//�ؾ߱��
	int vehicle_id = 0;
	//��һ�η�����ײʱ��·������ţ�ֵ��1��ʼ
	size_t first_collison_id = 0;
	//·��������
	std::vector<RoutePoint> route_points;
};

//���ʳ�ǣ�������洬״̬����
struct TowedShip
{
	double x = 0;     //���洬x������(m)
	double angle = 0; //���洬�����(��)
};//��ǣ�����洬����
