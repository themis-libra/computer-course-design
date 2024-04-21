#include"volunteer.h"


/*分配任务所需数据定义*/
int x[VOLUNTEER+1];			//临时解
int cost = 0;				//临时解的成本
int bestx[VOLUNTEER+1];		//最优解
int maxcost = 0;			//最优解的成本
bool worker[VOLUNTEER+1];		//worker[j]表示任务j是否已分配人员
int allotjob[VOLUNTEER][VOLUNTEER];


/*控制台美化函数*/
void Control()
{
	system("color F0");						//设置控制台界面背景颜色和前景颜色
	system("mode con cols=80 lines=40");	//初始化缓冲区大小
	system("title 志愿者管理系统");			//修改控制台标题
}


/*主菜单函数*/
void Menu()
{
	printf("\n\n\n\n");
	printf("--------------------------杭州亚运会志愿者信息管理系统--------------------------\n\n");
	printf("----------------------------------系统功能菜单----------------------------------\n");
	printf("|                                                                              |\n");
	printf("|                               1.志愿者信息管理                               |\n");
	printf("|                                                                              |\n");
	printf("|                               2.志愿者任务安排                               |\n");
	printf("|                                                                              |\n");
	printf("|                                3.志愿任务说明                                |\n");
	printf("|                                                                              |\n");
	printf("|                                  4.退出系统                                  |\n");
	printf("--------------------------------------------------------------------------------\n\n");
	printf("--------------------------------------------------------------------------------\n\n\n\n");

	printf("请输入菜单编号：");

	int num;
	scanf("%d", &num);
	switch (num)
	{
	case 1:system("cls"); Registration(); break;
	case 2:system("cls"); Taskarrange(); break;
	case 3:system("cls"); Printjob(); break;
	case 4:return; break;
	default:printf("请在1-4之间选择\n");
	}
}


/*小菜单——信息登记函数*/
void Registration()
{
	printf("\n\n\n\n");
	printf("-------------------------------志愿者信息管理界面-------------------------------\n\n");
	printf("----------------------------------系统功能菜单----------------------------------\n");
	printf("|                                                                              |\n");
	printf("|                                  1.录入数据                                  |\n");
	printf("|                                                                              |\n");
	printf("|                                  2.修改信息                                  |\n");
	printf("|                                                                              |\n");
	printf("|                                  3.添加信息                                  |\n");
	printf("|                                                                              |\n");
	printf("|                                  4.删除信息                                  |\n");
	printf("|                                                                              |\n");
	printf("|                                 5.按姓名查找                                 |\n");
	printf("|                                                                              |\n");
	printf("|                                6.按身份证查找                                |\n");
	printf("|                                                                              |\n");
	printf("|                                 7.返回主菜单                                 |\n");
	printf("|                                                                              |\n");
	printf("|                                  8.退出系统                                  |\n");
	printf("--------------------------------------------------------------------------------\n\n");
	printf("--------------------------------------------------------------------------------\n\n\n\n");

	printf("请输入菜单编号：");

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
	default:printf("请在1-8之间选择\n");
	}
}


/*小菜单——任务安排函数*/
void Taskarrange()
{
	printf("\n\n\n\n");
	printf("-------------------------------志愿者任务安排界面-------------------------------\n\n");
	printf("----------------------------------系统功能菜单----------------------------------\n");
	printf("|                                                                              |\n");
	printf("|                                1.自动分配任务                                |\n");
	printf("|                                                                              |\n");
	printf("|                                2.查看分配结果                                |\n");
	printf("|                                                                              |\n");
	printf("|                              3.将信息存储到文件                              |\n");
	printf("|                                                                              |\n");
	printf("|                                 4.返回主菜单                                 |\n");
	printf("|                                                                              |\n");
	printf("|                                  5.退出系统                                  |\n");
	printf("--------------------------------------------------------------------------------\n\n");
	printf("--------------------------------------------------------------------------------\n\n");

	printf("请输入菜单编号：");

	int num;
	scanf("%d", &num);
	switch (num)
	{
	case 1:system("cls"); AllotJob(); break;
	case 2:system("cls"); Print(); break;
	case 3:system("cls"); Write(); break;
	case 4:system("cls"); Menu(); break;
	case 5:return; break;
	default:printf("请在1-5之间选择\n");
	}
}


/*小菜单——日程输出函数*/
void Printjob()
{
	printf("第一天：篮球（basketball）\n\n第二天：游泳（swimming）\n\n第三天：网球（tennis）\n\n第四天：足球（football）\n\n第五天：乒乓球（table tennis）\n\n");
	printf("第六天：羽毛球（badminton）\n\n第七天：马术（equestrian）\n\n第八天：排球（volleyball）\n\n第九天：电竞（e-sports）\n\n第十天：体操（gymnastics）\n\n\n\n");

	Return();
	
}


/*返回主菜单*/
void Return()
{
	printf("\n\n请选择下一步操作：\n\n");
	printf("1.返回主菜单\n2.退出系统\n\n");
	printf("您的选择是：");

	int num;
	scanf("%d", &num);
	switch (num)
	{
	case 1:system("cls"); Menu(); break;
	case 2:return; break;
	default:printf("请在1-2之间选择\n");
	}
}

/*读取文件函数*/
void Read()
{
	char fname[256];
	char name[20];

	printf("--------------------------------------------------------------------------------\n\n");
	printf("提示，您的文件须是这种格式：\n");
	printf("姓名 性别 年龄 手机号 身份证号 职业 掌握的外语 \n");
	printf("接下来的数据都用1/0表示是/否： \n");
	printf("外语掌握情况\(熟练1/不熟练0\)\n是否有志愿经验\(是1/否0\)\n工作十天中是否空闲\(是1/否0,需要输入十个数字，每个数字间需要空格\)\n\n");
	printf("--------------------------------------------------------------------------------\n\n\n");

	printf("请输入存放数据的文件名：\n");
	printf("--------------------------------------------------------------------------------\n\n");
	printf("读取文件使用方法：\n");
	printf("（1）复制文件地址；\n（2）在地址后加上\"文件名.文件类型\"\(注意所有标点要使用英文标点\)；\n\n");
	printf("--------------------------------------------------------------------------------\n\n");

	scanf("%s", fname);
	printf("\n志愿者数量：\n");
	scanf("%d", &datanum);
	FILE* fp = fopen(fname, "rb");
	if (fp == NULL) {
		printf("不能打开文件\n");
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


/*显示输出函数*/
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


/*添加信息函数*/
void AppendRecord()
{
	int i, j;
	int num;
	printf("请输入要增加几条志愿者信息：");
	scanf("%d", &num);
	while (datanum + num > VOLUNTEER)
	{
		printf("总志愿者数量超过上限，请重新输入要增加的人数：\n");
		scanf("%d", &num);
	}
	for (i = datanum; i < num + datanum; i++)
	{
		printf("请输入第%d个志愿者的信息：\n", i + 1);
		scanf("%s%s%d%s%s%s%s%d%d", v[i].name, v[i].sex, &v[i].age, v[i].phone, v[i].number, v[i].job, v[i].l.species,
			&v[i].l.proficiency, &v[i].experience);
		for (j = 0; j < 10; j++)
		{
			scanf("%d ", &v[i].freetime[j]);
		}
	}
	printf("完成读取\n");
	datanum = datanum + num;

	Return();
}


/*删除信息函数*/
void DeleteRecord()
{
	int i, j;
	char num[18];
	char ch;
	printf("请输入你要删除的志愿者信息对应的身份证号码：");
	scanf("%s", num);
	for (i = 0; i < datanum; i++)
	{
		if (strcmp(v[i].number, num) == 0)
		{
			printf("找到了该生的记录\n");
			printf("请确实是否要删除这条记录？（Y/y or N/n）:");
			getchar();
			scanf("%c", &ch);
			if (ch == 'Y' || ch == 'y')
			{
				for (j = i; j < datanum; j++)
				{
					v[j] = v[j + 1];
				}
				datanum--;
				printf("删除完毕\n");
			}
			else if (ch == 'N' || ch == 'n')
			{
				printf("已放弃删除\n");
			}
			else
			{
				printf("输入错误\n");
			}
		}
	}
	printf("未找到对应记录\n");

	Return();
}


/*姓名查找函数*/
void SearchByName()
{
	int i, j;
	int flag = 1;
	char num[10];
	printf("输入要查找的志愿者姓名：\n");
	scanf("%s", num);
	for (i = 0; i < datanum; i++)
	{
		if (strcmp(v[i].name, num) == 0)
		{
			printf("找到该志愿者，志愿者信息如下：\n");
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
		printf("没有找到对应的志愿者\n");

	Return();
}


/*身份证查找函数*/
void SearchByNumber()
{
	int i, j;
	int flag = 1;
	char num[18];
	printf("输入要查找的志愿者身份证:\n");
	scanf("%s", num);
	for (i = 0; i < datanum; i++)
	{
		if (strcmp(v[i].number, num) == 0)
		{
			printf("找到该志愿者，志愿者信息如下：\n");
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
		printf("没有找到对应的志愿者信息\n");

	Return();
}


/*修改信息函数*/
void ModifyRecord()
{
	int i, j;
	char num[18];
	char ch;
	int flag = 1;
	printf("请输入要修改的志愿者信息对应的身份证号码：");
	scanf("%s", num);
	for (i = 0; i < datanum; i++)
	{
		if (strcmp(v[i].number, num) == 0)
		{
			flag = 0;
			printf("找到了该志愿者的信息\n");
			printf("请确认是否要修改？（Y/y or N/n）:");
			getchar();
			scanf("%c", &ch);
			if (ch == 'Y' || ch == 'y')
			{
				printf("请输入要修改的信息：\n");
				scanf("%s%s%d%s%s%s%s%d%d", v[i].name, v[i].sex, &v[i].age, v[i].phone, v[i].number, v[i].job, v[i].l.species,
					&v[i].l.proficiency, &v[i].experience);
				for (j = 0; j < 10; j++)
				{
					scanf("%d", &v[i].freetime[j]);
				}
				printf("完成修改\n");
			}
			else if (ch == 'N' || ch == 'n')
			{
				printf("已放弃修改\n");
			}
			else
			{
				printf("输入错误\n");
			}
		}
	}
	if (flag)
	{
		printf("没找到志愿者信息\n");
	}
	Return();
}


/*自动分配工作函数*/

//回溯法分配任务
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
		for (j = 1; j <= VOLUNTEER; j++)	//为人员i试探任务j，从1到n
		{							
			if (!worker[j])					//任务j还未分配	
			{ 
				worker[j] = true;
				allot(i - 1, j - 1);
				cost += allotjob[i - 1][j - 1];
				x[i] = j;
				dfs(i + 1);					//回溯
				worker[j] = false;
				cost -= allotjob[i - 1][j - 1];
				x[i] = 0;
			}
		}
	}
}

//生成数组
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

//任务
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

//总分配函数
void AllotJob()				
{
	getarr();
	memset(worker, false, sizeof(worker));//任务还未分配
	dfs(1);
	printf("已经完成任务分配\n");
	int k;
	for (k = 1; k <= VOLUNTEER; k++) 
	{
		printf("第%d个人安排任务\n", bestx[k]);
	}
	printf("总共安排数量:%d\n", maxcost);

	Return();
}   


/*文件存盘函数*/
void Write()
{
	int i, j;
	FILE* fp;
	char fname[256];
	printf("请输入要存储的地址:");
	scanf("%s", fname);
	if ((fp = fopen(fname, "w")) == NULL)
	{
		printf("文件无法正常打开！\n");
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
	printf("存盘完成！\n");

	Return();
}