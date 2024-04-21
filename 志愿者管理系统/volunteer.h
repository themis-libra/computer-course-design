#pragma once

/*��Ҫ��ͷ�ļ�*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>


/*���ݶ���*/
#define VOLUNTEER 10		//־Ը����������
#define JOB 10				//��������
#define JOB_NEED 1			//ÿ��������Ҫ��־Ը������

/*�ṹ�嶨��*/
//
//�����������
struct Language
{
	char species[10];
	int proficiency;		//1����0û���� 
};


//־Ը��
struct Volunteer
{
	char name[10];
	char sex[7];
	int age;
	char phone[11];
	char number[18];		//���֤�� 
	char job[10];			//��λ��Ϣ
	struct Language l;
	int experience;			//�Ƿ���־Ը�߾���1\0
	int freetime[10];		//1\0
	char vjob[15];			//�����־Ը���� 
};
struct Volunteer v[VOLUNTEER];
int datanum;


/*��������*/
//
/*����̨��������*/
void Control();				//����̨����

/*�˵�*/
void Menu();				//���˵�
void Registration();		//С�˵�������Ϣ�Ǽ�
void Taskarrange();			//С�˵�����������
void Printjob();			//С�˵������ճ����

/*�������˵�*/
void Return();				//�������˵������˳�

/*�������빦��*/
void Read();				//��ȡ�ļ�

/*���ݴ�����*/	
void AppendRecord();		//�����Ϣ
void DeleteRecord();		//ɾ����Ϣ
void SearchByName();		//��������
void SearchByNumber();		//���֤����
void ModifyRecord();		//�޸���Ϣ

void dfs(int i);			//���ݷ���������
void getarr();				//��������
void allot(int i, int j);	//����
void AllotJob();			//�Զ���������

/*�����������*/
void Write();				//�ļ�����
void Print();				//��ʾ���