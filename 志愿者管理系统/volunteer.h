#pragma once

/*需要的头文件*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>


/*数据定义*/
#define VOLUNTEER 10		//志愿者数量上限
#define JOB 10				//任务数量
#define JOB_NEED 1			//每个任务需要的志愿者数量

/*结构体定义*/
//
//外语掌握情况
struct Language
{
	char species[10];
	int proficiency;		//1掌握0没掌握 
};


//志愿者
struct Volunteer
{
	char name[10];
	char sex[7];
	int age;
	char phone[11];
	char number[18];		//身份证号 
	char job[10];			//单位信息
	struct Language l;
	int experience;			//是否有志愿者经验1\0
	int freetime[10];		//1\0
	char vjob[15];			//分配的志愿工作 
};
struct Volunteer v[VOLUNTEER];
int datanum;


/*函数声明*/
//
/*控制台美化函数*/
void Control();				//控制台美化

/*菜单*/
void Menu();				//主菜单
void Registration();		//小菜单——信息登记
void Taskarrange();			//小菜单——任务安排
void Printjob();			//小菜单——日程输出

/*返回主菜单*/
void Return();				//返回主菜单或者退出

/*数据输入功能*/
void Read();				//读取文件

/*数据处理功能*/	
void AppendRecord();		//添加信息
void DeleteRecord();		//删除信息
void SearchByName();		//姓名查找
void SearchByNumber();		//身份证查找
void ModifyRecord();		//修改信息

void dfs(int i);			//回溯法分配任务
void getarr();				//生成数组
void allot(int i, int j);	//任务
void AllotJob();			//自动分配任务

/*数据输出功能*/
void Write();				//文件存盘
void Print();				//显示输出