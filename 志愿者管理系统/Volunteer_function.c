#include"volunteer.h"


/*���������������ݶ���*/
int x[VOLUNTEER+1];			//��ʱ��
int cost = 0;				//��ʱ��ĳɱ�
int bestx[VOLUNTEER+1];		//���Ž�
int maxcost = 0;			//���Ž�ĳɱ�
bool worker[VOLUNTEER+1];		//worker[j]��ʾ����j�Ƿ��ѷ�����Ա
int allotjob[VOLUNTEER][VOLUNTEER];


/*����̨��������*/
void Control()
{
	system("color F0");						//���ÿ���̨���汳����ɫ��ǰ����ɫ
	system("mode con cols=80 lines=40");	//��ʼ����������С
	system("title ־Ը�߹���ϵͳ");			//�޸Ŀ���̨����
}


/*���˵�����*/
void Menu()
{
	printf("\n\n\n\n");
	printf("--------------------------�������˻�־Ը����Ϣ����ϵͳ--------------------------\n\n");
	printf("----------------------------------ϵͳ���ܲ˵�----------------------------------\n");
	printf("|                                                                              |\n");
	printf("|                               1.־Ը����Ϣ����                               |\n");
	printf("|                                                                              |\n");
	printf("|                               2.־Ը��������                               |\n");
	printf("|                                                                              |\n");
	printf("|                                3.־Ը����˵��                                |\n");
	printf("|                                                                              |\n");
	printf("|                                  4.�˳�ϵͳ                                  |\n");
	printf("--------------------------------------------------------------------------------\n\n");
	printf("--------------------------------------------------------------------------------\n\n\n\n");

	printf("������˵���ţ�");

	int num;
	scanf("%d", &num);
	switch (num)
	{
	case 1:system("cls"); Registration(); break;
	case 2:system("cls"); Taskarrange(); break;
	case 3:system("cls"); Printjob(); break;
	case 4:return; break;
	default:printf("����1-4֮��ѡ��\n");
	}
}


/*С�˵�������Ϣ�ǼǺ���*/
void Registration()
{
	printf("\n\n\n\n");
	printf("-------------------------------־Ը����Ϣ�������-------------------------------\n\n");
	printf("----------------------------------ϵͳ���ܲ˵�----------------------------------\n");
	printf("|                                                                              |\n");
	printf("|                                  1.¼������                                  |\n");
	printf("|                                                                              |\n");
	printf("|                                  2.�޸���Ϣ                                  |\n");
	printf("|                                                                              |\n");
	printf("|                                  3.�����Ϣ                                  |\n");
	printf("|                                                                              |\n");
	printf("|                                  4.ɾ����Ϣ                                  |\n");
	printf("|                                                                              |\n");
	printf("|                                 5.����������                                 |\n");
	printf("|                                                                              |\n");
	printf("|                                6.�����֤����                                |\n");
	printf("|                                                                              |\n");
	printf("|                                 7.�������˵�                                 |\n");
	printf("|                                                                              |\n");
	printf("|                                  8.�˳�ϵͳ                                  |\n");
	printf("--------------------------------------------------------------------------------\n\n");
	printf("--------------------------------------------------------------------------------\n\n\n\n");

	printf("������˵���ţ�");

	int num;
	scanf("%d", &num);
	switch (num)
	{
	case 1:system("cls"); Read(); break;
	case 2:system("cls"); ModifyRecord(); break;
	case 3:system("cls"); AppendRecord(); break;
	case 4:system("cls"); DeleteRecord(); break;
	case 5:system("cls"); SearchByName(); break;
	case 6:system("cls"); SearchByNumber(); break;
	case 7:system("cls"); Menu(); break;
	case 8:return; break;
	default:printf("����1-8֮��ѡ��\n");
	}
}


/*С�˵����������ź���*/
void Taskarrange()
{
	printf("\n\n\n\n");
	printf("-------------------------------־Ը�������Ž���-------------------------------\n\n");
	printf("----------------------------------ϵͳ���ܲ˵�----------------------------------\n");
	printf("|                                                                              |\n");
	printf("|                                1.�Զ���������                                |\n");
	printf("|                                                                              |\n");
	printf("|                                2.�鿴������                                |\n");
	printf("|                                                                              |\n");
	printf("|                              3.����Ϣ�洢���ļ�                              |\n");
	printf("|                                                                              |\n");
	printf("|                                 4.�������˵�                                 |\n");
	printf("|                                                                              |\n");
	printf("|                                  5.�˳�ϵͳ                                  |\n");
	printf("--------------------------------------------------------------------------------\n\n");
	printf("--------------------------------------------------------------------------------\n\n");

	printf("������˵���ţ�");

	int num;
	scanf("%d", &num);
	switch (num)
	{
	case 1:system("cls"); AllotJob(); break;
	case 2:system("cls"); Print(); break;
	case 3:system("cls"); Write(); break;
	case 4:system("cls"); Menu(); break;
	case 5:return; break;
	default:printf("����1-5֮��ѡ��\n");
	}
}


/*С�˵������ճ��������*/
void Printjob()
{
	printf("��һ�죺����basketball��\n\n�ڶ��죺��Ӿ��swimming��\n\n�����죺����tennis��\n\n�����죺����football��\n\n�����죺ƹ����table tennis��\n\n");
	printf("�����죺��ë��badminton��\n\n�����죺������equestrian��\n\n�ڰ��죺����volleyball��\n\n�ھ��죺�羺��e-sports��\n\n��ʮ�죺��٣�gymnastics��\n\n\n\n");

	Return();
	
}


/*�������˵�*/
void Return()
{
	printf("\n\n��ѡ����һ��������\n\n");
	printf("1.�������˵�\n2.�˳�ϵͳ\n\n");
	printf("����ѡ���ǣ�");

	int num;
	scanf("%d", &num);
	switch (num)
	{
	case 1:system("cls"); Menu(); break;
	case 2:return; break;
	default:printf("����1-2֮��ѡ��\n");
	}
}

/*��ȡ�ļ�����*/
void Read()
{
	char fname[256];
	char name[20];

	printf("--------------------------------------------------------------------------------\n\n");
	printf("��ʾ�������ļ��������ָ�ʽ��\n");
	printf("���� �Ա� ���� �ֻ��� ���֤�� ְҵ ���յ����� \n");
	printf("�����������ݶ���1/0��ʾ��/�� \n");
	printf("�����������\(����1/������0\)\n�Ƿ���־Ը����\(��1/��0\)\n����ʮ�����Ƿ����\(��1/��0,��Ҫ����ʮ�����֣�ÿ�����ּ���Ҫ�ո�\)\n\n");
	printf("--------------------------------------------------------------------------------\n\n\n");

	printf("�����������ݵ��ļ�����\n");
	printf("--------------------------------------------------------------------------------\n\n");
	printf("��ȡ�ļ�ʹ�÷�����\n");
	printf("��1�������ļ���ַ��\n��2���ڵ�ַ�����\"�ļ���.�ļ�����\"\(ע�����б��Ҫʹ��Ӣ�ı��\)��\n\n");
	printf("--------------------------------------------------------------------------------\n\n");

	scanf("%s", fname);
	printf("\n־Ը��������\n");
	scanf("%d", &datanum);
	FILE* fp = fopen(fname, "rb");
	if (fp == NULL) {
		printf("���ܴ��ļ�\n");
	}
	int i, j;
	for (i = 0; i < datanum; i++)
	{
		fscanf(fp, "%s%s%d%s%s%s%s%d%d", v[i].name, v[i].sex, &v[i].age, v[i].phone, v[i].number, v[i].job, v[i].l.species,
			&v[i].l.proficiency, &v[i].experience);
		for (j = 0; j < 10; j++)
		{
			fscanf(fp, "%d", &v[i].freetime[j]);
		}
	}
	fclose(fp);

	Return();
}


/*��ʾ�������*/
void Print()
{
	int i, j;
	for (i = 0; i < datanum; i++)
	{

		printf("%s,%s,%d,%s,%s,%s,%s,%d,%d ", v[i].name, v[i].sex, v[i].age, v[i].phone, v[i].number, v[i].job, v[i].l.species,
			v[i].l.proficiency, v[i].experience);
		printf("{");
		for (j = 0; j < 10; j++)
		{
			printf("%d ", v[i].freetime[j]);
		}
		printf("} %s\n", v[i].vjob);
	}

	Return();
}


/*�����Ϣ����*/
void AppendRecord()
{
	int i, j;
	int num;
	printf("������Ҫ���Ӽ���־Ը����Ϣ��");
	scanf("%d", &num);
	while (datanum + num > VOLUNTEER)
	{
		printf("��־Ը�������������ޣ�����������Ҫ���ӵ�������\n");
		scanf("%d", &num);
	}
	for (i = datanum; i < num + datanum; i++)
	{
		printf("�������%d��־Ը�ߵ���Ϣ��\n", i + 1);
		scanf("%s%s%d%s%s%s%s%d%d", v[i].name, v[i].sex, &v[i].age, v[i].phone, v[i].number, v[i].job, v[i].l.species,
			&v[i].l.proficiency, &v[i].experience);
		for (j = 0; j < 10; j++)
		{
			scanf("%d ", &v[i].freetime[j]);
		}
	}
	printf("��ɶ�ȡ\n");
	datanum = datanum + num;

	Return();
}


/*ɾ����Ϣ����*/
void DeleteRecord()
{
	int i, j;
	char num[18];
	char ch;
	printf("��������Ҫɾ����־Ը����Ϣ��Ӧ�����֤���룺");
	scanf("%s", num);
	for (i = 0; i < datanum; i++)
	{
		if (strcmp(v[i].number, num) == 0)
		{
			printf("�ҵ��˸����ļ�¼\n");
			printf("��ȷʵ�Ƿ�Ҫɾ��������¼����Y/y or N/n��:");
			getchar();
			scanf("%c", &ch);
			if (ch == 'Y' || ch == 'y')
			{
				for (j = i; j < datanum; j++)
				{
					v[j] = v[j + 1];
				}
				datanum--;
				printf("ɾ�����\n");
			}
			else if (ch == 'N' || ch == 'n')
			{
				printf("�ѷ���ɾ��\n");
			}
			else
			{
				printf("�������\n");
			}
		}
	}
	printf("δ�ҵ���Ӧ��¼\n");

	Return();
}


/*�������Һ���*/
void SearchByName()
{
	int i, j;
	int flag = 1;
	char num[10];
	printf("����Ҫ���ҵ�־Ը��������\n");
	scanf("%s", num);
	for (i = 0; i < datanum; i++)
	{
		if (strcmp(v[i].name, num) == 0)
		{
			printf("�ҵ���־Ը�ߣ�־Ը����Ϣ���£�\n");
			printf("%s,%s,%d,%s,%s,%s,%s,%d,%d", v[i].name, v[i].sex, v[i].age, v[i].phone, v[i].number, v[i].job, v[i].l.species,
				v[i].l.proficiency, v[i].experience);
			printf("{");
			for (j = 0; j < 10; j++)
			{
				printf("%d ", v[i].freetime[j]);
			}
			printf("} %s\n", v[i].vjob);
			flag = 0;
			break;
		}
	}
	if (flag)
		printf("û���ҵ���Ӧ��־Ը��\n");

	Return();
}


/*���֤���Һ���*/
void SearchByNumber()
{
	int i, j;
	int flag = 1;
	char num[18];
	printf("����Ҫ���ҵ�־Ը�����֤:\n");
	scanf("%s", num);
	for (i = 0; i < datanum; i++)
	{
		if (strcmp(v[i].number, num) == 0)
		{
			printf("�ҵ���־Ը�ߣ�־Ը����Ϣ���£�\n");
			printf("%s,%s,%d,%s,%s,%s,%s,%d,%d", v[i].name, v[i].sex, v[i].age, v[i].phone, v[i].number, v[i].job, v[i].l.species,
				v[i].l.proficiency, v[i].experience);
			printf("{");
			for (j = 0; j < 10; j++)
			{
				printf("%d ", v[i].freetime[j]);
			}
			printf("} %s\n", v[i].vjob);
			flag = 0;
			break;
		}
	}
	if (flag)
		printf("û���ҵ���Ӧ��־Ը����Ϣ\n");

	Return();
}


/*�޸���Ϣ����*/
void ModifyRecord()
{
	int i, j;
	char num[18];
	char ch;
	int flag = 1;
	printf("������Ҫ�޸ĵ�־Ը����Ϣ��Ӧ�����֤���룺");
	scanf("%s", num);
	for (i = 0; i < datanum; i++)
	{
		if (strcmp(v[i].number, num) == 0)
		{
			flag = 0;
			printf("�ҵ��˸�־Ը�ߵ���Ϣ\n");
			printf("��ȷ���Ƿ�Ҫ�޸ģ���Y/y or N/n��:");
			getchar();
			scanf("%c", &ch);
			if (ch == 'Y' || ch == 'y')
			{
				printf("������Ҫ�޸ĵ���Ϣ��\n");
				scanf("%s%s%d%s%s%s%s%d%d", v[i].name, v[i].sex, &v[i].age, v[i].phone, v[i].number, v[i].job, v[i].l.species,
					&v[i].l.proficiency, &v[i].experience);
				for (j = 0; j < 10; j++)
				{
					scanf("%d", &v[i].freetime[j]);
				}
				printf("����޸�\n");
			}
			else if (ch == 'N' || ch == 'n')
			{
				printf("�ѷ����޸�\n");
			}
			else
			{
				printf("�������\n");
			}
		}
	}
	if (flag)
	{
		printf("û�ҵ�־Ը����Ϣ\n");
	}
	Return();
}


/*�Զ����乤������*/

//���ݷ���������
void dfs(int i) 
{
	if (i > VOLUNTEER)
	{
		if (cost > maxcost)
		{
			maxcost = cost;
			for (int k = 1; k <= VOLUNTEER; k++)
				bestx[k] = x[k];
		}
	}
	else
	{
		int j;
		for (j = 1; j <= VOLUNTEER; j++)	//Ϊ��Աi��̽����j����1��n
		{							
			if (!worker[j])					//����j��δ����	
			{ 
				worker[j] = true;
				allot(i - 1, j - 1);
				cost += allotjob[i - 1][j - 1];
				x[i] = j;
				dfs(i + 1);					//����
				worker[j] = false;
				cost -= allotjob[i - 1][j - 1];
				x[i] = 0;
			}
		}
	}
}

//��������
void getarr()
{
	int i, j;
	for (i = 0; i < datanum; i++)
	{
		for (j = 0; j < 10; j++)
		{
			allotjob[i][j] = v[i].freetime[j];
		}
	}

}

//����
void allot(int i, int j)	
{
	switch (j)
	{
	case 0:strcpy(v[i].vjob, "basketball"); break;
	case 1:strcpy(v[i].vjob, "swimming"); break;
	case 2:strcpy(v[i].vjob, "tennis"); break;
	case 3:strcpy(v[i].vjob, "football"); break;
	case 4:strcpy(v[i].vjob, "table-tennis"); break;
	case 5:strcpy(v[i].vjob, "badminton"); break;
	case 6:strcpy(v[i].vjob, "equestrian"); break;
	case 7:strcpy(v[i].vjob, "volleyball"); break;
	case 8:strcpy(v[i].vjob, "e-sports"); break;
	case 9:strcpy(v[i].vjob, "gymnastics"); break;
	}
}

//�ܷ��亯��
void AllotJob()				
{
	getarr();
	memset(worker, false, sizeof(worker));//����δ����
	dfs(1);
	printf("�Ѿ�����������\n");
	int k;
	for (k = 1; k <= VOLUNTEER; k++) 
	{
		printf("��%d���˰�������\n", bestx[k]);
	}
	printf("�ܹ���������:%d\n", maxcost);

	Return();
}   


/*�ļ����̺���*/
void Write()
{
	int i, j;
	FILE* fp;
	char fname[256];
	printf("������Ҫ�洢�ĵ�ַ:");
	scanf("%s", fname);
	if ((fp = fopen(fname, "w")) == NULL)
	{
		printf("�ļ��޷������򿪣�\n");
		exit(0);
	}
	for (i = 0; i < datanum; i++)
	{
		fprintf(fp, "%s,%s,%d,%s,%s,%s,%s,%d,%d ", v[i].name, v[i].sex, v[i].age, v[i].phone, v[i].number, v[i].job, v[i].l.species,
			v[i].l.proficiency, v[i].experience);
		for (j = 0; j < 10; j++)
		{
			fprintf(fp, "%d ", v[i].freetime[j]);
		}
		fprintf(fp, "%s\n",v[i].vjob);
	}
	fclose(fp);
	printf("������ɣ�\n");

	Return();
}