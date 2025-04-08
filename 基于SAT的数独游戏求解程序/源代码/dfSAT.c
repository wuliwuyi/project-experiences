#include"definition.h"
extern char FileName[100] ;//文件名
extern struct problem qq;
extern sud sudo;
clause* root = NULL;
int arr1[65];
float time1=0,time2=0;

/*sat问题用到的函数*/

void init()									//初始化问题
{
	clause* cla_tem = root;					//根节点
	while (cla_tem != NULL) {
		element* liter_tem = cla_tem->first;//指向第一个元素
		while (liter_tem != NULL) {
			element* rel_liter = liter_tem;	
			liter_tem = liter_tem->nextnode;//指向下一个元素
			free(rel_liter);
		}
		clause* rel_cla = cla_tem;			//release_clause
		cla_tem = cla_tem->nextcla;
		free(rel_cla);
	}
	root = NULL;
	memset(qq.ans, 0, sizeof(qq.ans));      //初始化数值
	memset(qq.element, 0, sizeof(qq.element));
	qq.clanum = 0;	qq.vexnum = 0;
}

void _ReadFile(char filename[200]) 			//读取文件
{
	init();				//初始化 
	FILE* fp = NULL;
	if (!(fp = fopen(filename, "r"))) {
		printf("无法打开文件！");
		getchar();
	}
	else {
		char c;
		char str[5000] = { '\0' };
		while ((c = fgetc(fp)) == 'c') {
			fgets(str, 5000, fp);
		}
		fgetc(fp);								//读空格
		fscanf(fp, "%s%d%d", str, &qq.vexnum, &qq.clanum);
		root = (clause*)malloc(sizeof(clause));
		root->literal_num = 0;
		clause* tem1 = root;						//指向子句 
		element* tem2_r = NULL, * tem2_l = NULL;	//指向元素，采用尾插法插入 
		int num_cur;
		int cnt = 0;								//计数 
		for (int i = 1; i <= qq.clanum; i++) {
			if (i != 1) {							//第一次的tem1->nextcla就是root指向的位置，不分配空间 
				tem1->nextcla = (clause*)malloc(sizeof(clause));
				tem1 = tem1->nextcla;
				tem1->literal_num = 0;
			}
			tem1->flag = 0;
			tem2_l = NULL;	tem2_r = NULL;

			fscanf(fp, "%d", &num_cur);
			while (num_cur != 0) {
				tem1->literal_num++;
				tem2_r = (element*)malloc(sizeof(element));//分配空间
				tem2_r->value = num_cur;
				qq.element[abs(num_cur)]++;					//元素个数
				tem2_r->flag = 0;
				if (!cnt) {
					tem1->first = tem2_r;
					cnt++;
				}
				else {
					tem2_l->nextnode = tem2_r;
				}
				tem2_l = tem2_r;
				fscanf(fp, "%d", &num_cur);
			}
			tem2_l->nextnode = NULL;			//子句的最后元素挂空
			cnt = 0;
		}
		tem1->nextcla = NULL;					//最后子句挂空 	
	}
	fclose(fp);
}

void PrintFile() {//输出文件内容（仅子句）
	if (qq.clanum == 0 && qq.vexnum == 0) {
		printf("尚未读取文件！\n");
		getchar();
		return;
	}
	printf("该文件所有子句如下：\n");
	clause* tem1 = root;
	element* tem2;
	while (tem1 != NULL) {
		tem2 = tem1->first;
		printf(" %d :",tem1->literal_num);
		while (tem2 != NULL) {
			printf("%5d", tem2->value);
			tem2 = tem2->nextnode;
		}
		printf("\n");
		tem1 = tem1->nextcla;
	}
	printf("---------------------------------------------\n");
	getchar();
}

void RecoverBegin() {//返回初始状态 
	memset(qq.element, 0, sizeof(qq.element));//初始化各元素数量记录
	qq.clanum = 0;
	for (clause* cla = root; cla; cla = cla->nextcla) {
		cla->flag = 0;							//恢复子句节点的标志域 
		cla->literal_num = 0;					//重置子句节点中记录的元素数 
		for (element* lit = cla->first; lit; lit = lit->nextnode) {
			lit->flag = 0;						//恢复各元素节点的标志域 
			qq.element[abs(lit->value)]++;		//恢复各元素出现的次数 
			cla->literal_num++;					//恢复子句节点中记录的元素数 
		}
		qq.clanum++;
	}
}

status FindSingleClause()//寻找一个单子句 
{
	int sin_cla = 0, judge = 0;
	for (clause* p1 = root; p1; p1 = p1->nextcla)
	{
		if (p1->literal_num == 1 && !p1->flag) //子句中未被删除的元素为1且该子句未被删除 
		{
			for (element* p2 = p1->first; p2; p2 = p2->nextnode) //找单子句中元素 
			{
				if (!p2->flag)					 //未被删除的是目标元素 
				{
					sin_cla = p2->value;
					judge = 1;
					break;
				}
			}
			if (judge == 1)	break;
		}
	}
	return sin_cla;												//没找到单子句时返回0 
}

status DeleteTarget(int depth, int single)//删除目标子句和元素
{
	for (clause* cla = root; cla; cla = cla->nextcla)
	{
		if (cla->flag)	continue;								//该子句已经删除，判断下一子句 
		element* lit = cla->first;
		while (lit)
		{
			if (lit->flag)			//该元素已经删除，判断下一元素
			{
				lit = lit->nextnode;
				continue;
			}
			if (lit->value == single)//找到目标
			{
				cla->flag = depth;								//删除子句
				element* lit2 = cla->first;
				while (lit2) {									//对应的数量（element数组）减一 
					if (!lit2->flag)							//该子句中在之前的操作中未被删除的才算数
					{
						qq.element[abs(lit2->value)]--;
						lit2 = lit2->nextnode;
						continue;
					}
					lit2 = lit2->nextnode;
				}
				qq.clanum--;									//总子句数减一 
				break;
			}
			else if (lit->value == -single) {						//仅删除该元素 
				lit->flag = depth;
				qq.element[abs(single)]--;
				cla->literal_num--;								//该子句所含元素数减一 
				if (!cla->literal_num)	return ERROR;			//说明删除后出现了空子句 
				lit = lit->nextnode;
				continue;
			}
			lit = lit->nextnode;
		}
	}
	return OK;
}

status FindShortestLastLiteral() {//寻找最短子句中最后一个元素
	int len = MAX_NUM;
	int maxx = 0, res = 0;
	for (clause* cla = root; cla; cla = cla->nextcla) {
		if (cla->flag)	continue;           //子句已删除
		if (cla->literal_num < len) {		//寻找最短子句 
			len = cla->literal_num;
			for (element* lit = cla->first; lit; lit = lit->nextnode)
			{
				if (!lit->flag && maxx < qq.element[abs(lit->value)])	//找到最后一个元素
				res = lit->value;
			}
		}
	}
	return res;
}

void RecoverLastLevel(int depth)				//（递归）返回上一层递归时的状态
{
	for (clause* cla = root; cla; cla = cla->nextcla) {
		if (cla->flag == depth) {					//说明该子句在本层被修改
			cla->flag = 0;						//恢复子句有效性 
			qq.clanum++;						//恢复总子句数 
			for (element* lit = cla->first; lit; lit = lit->nextnode) {//寻找子句中在该层被改动的元素 
				if (lit->flag == depth) {
					lit->flag = 0;				//恢复元素有效性 
					cla->literal_num++;			//恢复子句节点记录的元素数 
					qq.element[abs(lit->value)]++;//恢复该元素出现的次数 
					continue;
				}
				else if (!lit->flag) {
					qq.element[abs(lit->value)]++;
				}
			}
		}
		else {
			element* lit2 = cla->first;			//子句没有被删除，但是元素可能被删除 
			while (lit2) {
				if (lit2->flag == depth) {		//有被删除的元素 
					lit2->flag = 0;				//恢复元素有效性 
					cla->literal_num++;			//该子句元素数+1
					qq.element[abs(lit2->value)]++;//该元素数+1
					lit2 = lit2->nextnode;
					continue;
				}
				lit2 = lit2->nextnode;
			}
		}
	}
}

status FindShortestFirstLiteral() {//寻找最短子句中的第一个元素
	int len = MAX_NUM;
	for (clause* cla = root; cla; cla = cla->nextcla) {
		if (cla->flag)	continue;
		if (cla->literal_num < len) {
			len = cla->literal_num;
			for (element* lit = cla->first; lit; lit = lit->nextnode) {
				if (!lit->flag)	return lit->value;
			}
		}

	}
	return 0;
}

status FindShortestMaxLiteral() {//寻找最短子句中出现最多的元素 
	int len = MAX_NUM;
	int maxx = 0, res = 0;
	for (clause* cla = root; cla; cla = cla->nextcla) {
		if (cla->flag)	continue;			//子句已删除
		if (cla->literal_num <= len) {		//寻找最短子句 
			len = cla->literal_num;
			for (element* lit = cla->first; lit; lit = lit->nextnode) 
			{
				if (!lit->flag && maxx < qq.element[abs(lit->value)]) {//找频率最高的元素
					maxx = qq.element[abs(lit->value)];
					res = lit->value;
				}
			}
		}

	}
	return res;
}

status FindFirstLiteral() {//寻找所有（未删除）元素中的首个元素 
	clause* cla = root;
	while (cla) {
		if (cla->flag) {		//子句已删除
			cla = cla->nextcla;
			continue;
		}
		else {
			element* lit = cla->first;
			while (lit) {
				if (lit->flag) {//元素已删除
					lit = lit->nextnode;
					continue;
				}
				return lit->value;
			}
		}
	}
	return 0;//没有元素，已经找完
}

status func1(int depth, int tar)				//递归
{
	if (qq.clanum == 0)	return OK;				//求解完成，剩余子句数为0 
	int single = 0;								
	if (!tar)									//表示
		single = FindSingleClause();			//寻找一个单子句（这里得到的single可正可负） 
	else
		single = tar;							

	while (single) {								//如果能找到单子句 
		qq.ans[abs(single)] = single;			//存储结果 
		int res = DeleteTarget(depth, single);	//删除目标子句与元素
		if (res == ERROR)	return ERROR;		//删除的时候出现了空子句，不满足要求
		single = FindSingleClause();
	}
	int max_lit = FindShortestLastLiteral();	//寻找最短子句中最后一个元素 
	if (!max_lit)	return OK;					//如果没找到，说明所有元素出现次数都已经变成0，此时说明已经正确求解 
	if (func1(depth + 1, max_lit) == OK) {			//当已经没有单子句的时候才进行递归 //赋值为真
		return OK;								//如果递归返回了OK，说明找到了解 
	}
	//上一个if没有return说明没找到解
	RecoverLastLevel(depth + 1);					//把在下一层做的更改复原 
	if (func1(depth + 1, -max_lit) == OK) {//赋值为假
		return OK;
	}
	RecoverLastLevel(depth + 1);
	return ERROR;								//不能求解 
}

status func2(int depth, int tar) {				//递归
	if (qq.clanum == 0)	return OK;				//求解完成，剩余子句数为0 
	int single = 0;
	if (!tar)	single = FindSingleClause();	//寻找一个单子句（single可正可负） 
	else	single = tar;

	while (single) {								//如果能找到单子句 
		qq.ans[abs(single)] = single;			//存储结果 
		int res = DeleteTarget(depth, single);	//删除目标子句与元素
		if (res == ERROR)	return ERROR;		//删除的时候出现了空子句，不满足要求
		single = FindSingleClause();
	}
	int max_lit = FindShortestMaxLiteral();		//寻找最短子句中出现频率最多的元素 
	if (!max_lit)	return OK;					//如果没找到，说明所有元素出现次数都已经变成0，此时说明已经正确求解 
	if (func2(depth + 1, max_lit) == OK) {			//当已经没有单子句的时候才进行递归 
		return OK;								//如果递归返回了OK，说明找到了解 
	}
	//上一个if没有return说明没找到解
	RecoverLastLevel(depth + 1);					//把在下一层做的更改复原 
	if (func2(depth + 1, -max_lit) == OK) {
		return OK;
	}
	RecoverLastLevel(depth + 1);
	return ERROR;								//不能求解
}

status func3(int depth, int tar) {				//递归
	if (qq.clanum == 0)	return OK;				//求解完成，剩余子句数为0 
	int single = 0;
	if (!tar)	single = FindSingleClause();	//寻找一个单子句 
	else	single = tar;

	while (single) {								//如果能找到单子句 
		qq.ans[abs(single)] = single;			//存储结果 
		int res = DeleteTarget(depth, single);	//删除目标子句与元素
		if (res == ERROR)	return ERROR;		//删除的时候出现了空子句，不满足要求
		single = FindSingleClause();
	}
	int max_lit = FindFirstLiteral();			//寻找（未被删除的元素中）首个元素 
	if (!max_lit)	return OK;					//如果没找到，说明所有元素出现次数都已经变成0，此时说明已经正确求解 
	if (func3(depth + 1, max_lit) == OK) {			//当已经没有单子句的时候才进行递归 
		return OK;								//如果递归返回了OK，说明找到了解 
	}
	//上一个if没有return说明没找到解
	RecoverLastLevel(depth + 1);					//把在下一层做的更改复原 
	if (func3(depth + 1, -max_lit) == OK) {
		return OK;
	}
	RecoverLastLevel(depth + 1);
	return ERROR;								//不能求解
}

void FormAnsFile(int res, int time, char filename[200]) {//将答案保存在res文件中 
	FILE* fp;
	int len = strlen(filename);
	filename[len - 3] = 'r';					//文件后缀名 
	filename[len - 2] = 'e';
	filename[len - 1] = 's';
	fp = fopen(filename, "w+");
	fprintf(fp, "s %d", res);
	fprintf(fp, "\nv");
	if (res != 1);								//不能求解 
	else {
		for (int i = 1; i <= qq.vexnum; i++) {		//写入答案 
			fprintf(fp, " %d", qq.ans[i]);
		}
	}
	fprintf(fp, "\nt %d", time);				//写入求解时间 
	fclose(fp);
}

void improve(int opt)//求优化率
{
	int res;
	time_t t1, t2;
	t1 = clock();
	RecoverBegin(); 		//将存储数据的链表恢复到初始状态，以便再进行一次求解
	res = func3(1, 0);
	t2 = clock();
	time2=t2-t1;
	printf("func5用时：%d",t2-t1);
	if(time1<time2)
	printf("优化率(func%d相比于func3)：%.4f%%\n",opt,((time2-time1)/time2)*100);
	else if(time1>time2)
	printf("优化率(func3相比于func%d)：%.4f%%\n",opt,((time1-time2)/time1)*100);
	else
	printf("优化率：0.0000%%\n");
}

void DPLL() {//DPLL模块 
	if (qq.clanum == 0 && qq.vexnum == 0)
	{
		printf("尚未读取文件！\n");
		getchar();
		return;
	}
	RecoverBegin(); 		//将存储数据的链表恢复到初始状态，以便再进行一次求解
	printf("1.func1(寻找最短子句的最后一个未被删除的元素)\n2.func2(寻找最短子句中在子句集中出现次数最多的元素)\n3.func3(寻找最短子句的第一个元素)\n");
	printf("请选择：");
	int opt, res;			//res记录求解结果（有解还是无解） 
	scanf("%d", &opt);
	time_t t1, t2;
	
	if (opt == 1)
	{
		t1 = clock();
		res = func1(1, 0);
	}
	
	else if (opt == 2)
	{
		t1 = clock();
		res = func2(1, 0);
	}
	
	else if (opt == 3)	
	{
		t1 = clock();
		res = func3(1, 0);
	}
	else {
		printf("请输入1~3中的数字！\n");
		getchar();
	}
	t2 = clock();
	time1=t2-t1;
	printf("此方法用时：%dms\n", t2 - t1);
	
	if (res == OK)	
	{
		printf("已成功求解！\n");
		printf("是否求优化率？（较难不建议求，时间会过长）是输入1，否输入0\n");
		int x;
		scanf("%d",&x);
		if(x)
		improve(opt);
	}	
	else	printf("此题无解！\n");
	FormAnsFile(res, t2 - t1, FileName);			//将答案保存在res文件中
	getchar(); getchar();
}

void PrintAnswer() {//打印答案 
	printf("答案如下：");
	for (int i = 1; i <= qq.vexnum; i++) {
		if (!qq.ans[i])	printf("%5d", i);		//说明是无关变量，取正值即可 
		else	printf("%5d", qq.ans[i]);
		if (i % 10 == 0)	;
	}
	getchar();
}

void CheckAnswer() {//检查答案 
	int res = 0, flag = 0;
	for (clause* cla = root; cla; cla = cla->nextcla) {
		flag = 0;
		for (element* lit = cla->first; lit; lit = lit->nextnode) {
			if (lit->value == qq.ans[abs(lit->value)]) {
				//一个子句中存在元素是真的，则子句为真
				flag = 1;
				break;
			}
		}
		if (!flag)	break; 	 //子句中没有一个元素与求得的解相同，错误
		
	}

	if (flag)	printf("所求解正确！\n");
	else printf("所求解错误！\n");
	getchar();
}

void SATQuestion() {
	int op = 1;
	while (op) {
		//fflush(stdin);
		//system("cls");	
		printf("------------------------------------------------------------------------\n");
		printf("                  1.读入cnf文件			2.输出文件内容                  \n");
		printf("                  3.DPLL求解			4.输出求解结果                  \n");	//选项3同时输出时间性能和保存文件 
		printf("                  5.检查答案		                                    \n");
		printf("                  0.退出  		                                        \n");
		printf("------------------------------------------------------------------------\n");
		scanf("%d",&op);
		if (op == 1) {
			fflush(stdin);
			printf("请输入文件名：");
			scanf("%s", FileName);
			_ReadFile(FileName);
			printf("该文件有%d个变量，%d个子句\n", qq.vexnum, qq.clanum);
			printf("已成功读取文件！\n");
			getchar(); 
		}
		else if (op == 2) {
			fflush(stdin);
			PrintFile();
			getchar();
		}
		else if (op == 3) {
			fflush(stdin);
			DPLL();
			getchar(); 
		}
		else if (op == 4) {
			fflush(stdin);
			PrintAnswer();
			getchar(); 
		}
		else if (op == 5) {
			fflush(stdin);
			CheckAnswer();
			getchar(); 
		}
		else if (op != 0) {
			fflush(stdin);
			printf("输入错误！请重新输入！");
			getchar(); 
		}
	}
}

/*********************************************************************************************************/

void initsudo() {//数独初始化 
	sudo.num = 0;
	for (int i = 1; i <= 9; i++) {
		for (int j = 1; j <= 9; j++) {
			if ((i < 5 && j >(4 + i % 5)) || (i > 5 && j > (9 - i % 5)))
			{
				sudo.finalans[i][j] = -1;
				sudo.original[i][j] = -1;
				sudo.flag[i][j] = -1; //蜂窝数独范围外标注-1
				continue;
			}
			sudo.finalans[i][j] = 0;
			sudo.original[i][j] = 0;
			sudo.flag[i][j] = 0;//数独内初始化为0
		}
	}
}

status VarTrans(int i, int j, int k)
{//将数独的1-9数字转化成自然顺序编码 
	return i * 100 + j * 10 + k;
}

void FormCnfFile(int flag) {//将数独数字转换为自然语言编码并存入cnf文件 
	//flag仅仅是用于判断此函数被哪个函数调用了，以确定生成的文件名是什么样的
	char cnfname[200] = { '\0' };
	if (flag == 1)	strcpy(cnfname, "prepare.cnf");
	else if (flag == 2)	strcpy(cnfname, "dighole.cnf");
	FILE* fp = NULL;
	int dis[9][2] = { {0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2} };
	if (!(fp = fopen(cnfname, "w"))) {
		printf("无法打开文件\n");
		getchar();
	}
	else
	{
		fprintf(fp, "p cnf %d %d\n", 959, 7468 + sudo.num);
		int k = 1, i = 1, j = 1, count, n, q = 1;
		{//将特有规则写入文件（总共sudo.num条语句）
			for (i = 1; i <= 9; i++)
			{
				for (j = 1; j <= 9; j++) {
					if (sudo.flag[i][j] == 1)	fprintf(fp, "%d 0\n", i * 100 + j * 10 + sudo.original[i][j]);
				}
			}
		}
		//将基础规则写入文件
		//只有行规则 分三次写入
		//1.
		for (i = 1; i <= 9; i++)
		{
			if (i == 5)//第五行有数字1-9
			{
				//必填数字
				for (k = 1; k <= 9; k++)
				{
					for (j = 1; j <= 9; j++)
					{
						fprintf(fp, "%d ", VarTrans(i, j, k));
					}
					fprintf(fp, "0\n");
				}
				//格约束--两格不能同时填一个数字
				for (j = 1; j <= 9; j++)
				{
					for (k = 1; k <= 9; k++)
					{
						for (q = k + 1; q <= 9; q++)
						{
							fprintf(fp, "%d %d 0\n", -VarTrans(i, j, k), -VarTrans(i, j, q));
						}
					}
				}
				for (j = 1; j <= 9; j++)
				{
					for (k = 1; k <= 9; k++)
					{
						fprintf(fp, "%d ", VarTrans(i, j, k));
					}
					fprintf(fp, "0\n");
				}
				//无重复数字--一行中数字不重复
				for (k = 1; k <= 9; k++)
				{
					for (q = 1; q <= 9; q++)
					{
						for (j = q + 1; j <= 9; j++)
						{
							fprintf(fp, "%d %d 0\n", -VarTrans(i, j, k), -VarTrans(i, q, k));
						}
					}
				}
			}
			if (i == 4 || i == 6)//第四行和第六号
			{
				//必填数字未2-8
				for (k = 2; k <= 8; k++)
				{
					for (j = 1; j <= 8; j++)
					{
						fprintf(fp, "%d ", VarTrans(i, j, k));
					}
					fprintf(fp, "0\n");
				}
				//约束选填--1或9
				for (j = 1; j <= 8; j++)
				{
					fprintf(fp, "%d ", VarTrans(i, j, 1));
				}
				for (j = 1; j <= 8; j++)
				{
					fprintf(fp, "%d ", VarTrans(i, j, 9));
				}
				fprintf(fp, "0\n");
				//格约束--两格不能同时填一个数字
				for (j = 1; j <= 8; j++)
				{
					for (k = 1; k <= 9; k++)
					{
						for (q = k + 1; q <= 9; q++)
						{
							fprintf(fp, "%d %d 0\n", -VarTrans(i, j, k), -VarTrans(i, j, q));
						}
					}
				}
				for (j = 1; j <= 8; j++)
				{
					for (k = 1; k <= 9; k++)
					{
						fprintf(fp, "%d ", VarTrans(i, j, k));
					}
					fprintf(fp, "0\n");
				}
				//无重复数字
				for (k = 1; k <= 9; k++)
				{
					for (q = 1; q <= 8; q++)
					{
						for (j = q + 1; j <= 8; j++)
						{
							fprintf(fp, "%d %d 0\n", -VarTrans(i, j, k), -VarTrans(i, q, k));
						}
					}
				}
			}
			if (i == 3 || i == 7)///第三行和第七行
			{
				//必填数字3-7
				for (k = 3; k <= 7; k++)
				{
					for (j = 1; j <= 7; j++)
					{
						fprintf(fp, "%d ", i * 100 + j * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//选填约束12或28或89
				for (k = 1; k <= 2; k++)
				{
					for (q = 8; q <= 7 + k; q++)
					{
						for (j = 1; j <= 7; j++)
						{
							fprintf(fp, "%d ", i * 100 + j * 10 + q);
						}
						for (j = 1; j <= 7; j++)
						{
							fprintf(fp, "%d ", i * 100 + j * 10 + k);
						}
						fprintf(fp, "0\n");
					}
				}
				//格约束--两格不能同时填一个数字
				for (j = 1; j <= 7; j++)
				{
					for (k = 1; k <= 9; k++)
					{
						for (q = k + 1; q <= 9; q++)
						{
							fprintf(fp, "%d %d 0\n", -(i * 100 + j * 10 + k), -(i * 100 + j * 10 + q));
						}
					}
				}
				for (j = 1; j <= 7; j++)
				{
					for (k = 1; k <= 9; k++)
					{
						fprintf(fp, "%d ", i * 100 + j * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//无重复数字
				for (k = 1; k <= 9; k++)
				{
					for (q = 1; q <= 7; q++)
					{
						for (j = q + 1; j <= 7; j++)
						{
							fprintf(fp, "%d %d 0\n", -(i * 100 + j * 10 + k), -(i * 100 + q * 10 + k));
						}
					}
				}
			}
			if (i == 2 || i == 8)//第二行和第八行
			{
				//必填数字4-6
				for (k = 4; k <= 6; k++)
				{
					for (j = 1; j <= 6; j++)
					{
						fprintf(fp, "%d ", i * 100 + j * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//选填约束123或237或378或789
				for (k = 1; k <= 3; k++)
				{
					for (q = 7; q <= 6 + k; q++)
					{
						for (j = 1; j <= 6; j++)
						{
							fprintf(fp, "%d ", i * 100 + j * 10 + q);
						}
						for (j = 1; j <= 6; j++)
						{
							fprintf(fp, "%d ", i * 100 + j * 10 + k);
						}
						fprintf(fp, "0\n");
					}
				}
				//格约束--两格不能同时填一个数字
				for (j = 1; j <= 6; j++)
				{
					for (k = 1; k <= 9; k++)
					{
						for (q = k + 1; q <= 9; q++)
						{
							fprintf(fp, "%d %d 0\n", -(i * 100 + j * 10 + k), -(i * 100 + j * 10 + q));
						}
					}
				}
				for (j = 1; j <= 6; j++)
				{
					for (k = 1; k <= 9; k++)
					{
						fprintf(fp, "%d ", i * 100 + j * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//无重复数字
				for (k = 1; k <= 9; k++)
				{
					for (q = 1; q <= 6; q++)
					{
						for (j = q + 1; j <= 6; j++)
						{
							fprintf(fp, "%d %d 0\n", -(i * 100 + j * 10 + k), -(i * 100 + q * 10 + k));
						}
					}
				}
			}
			if (i == 1 || i == 9)//第一行和第九行
			{
				//必填数字5
				for (j = 1; j <= 6; j++)
				{
					fprintf(fp, "%d ", i * 100 + j * 10 + 5);
				}
				fprintf(fp, "0\n");
				//选填约束1234或2346或3467或4678或6789
				for (k = 1; k <= 4; k++)
				{
					for (q = 6; q <= 5 + k; q++)
					{
						for (j = 1; j <= 5; j++)
						{
							fprintf(fp, "%d ", i * 100 + j * 10 + q);
						}
						for (j = 1; j <= 5; j++)
						{
							fprintf(fp, "%d ", i * 100 + j * 10 + k);
						}
						fprintf(fp, "0\n");
					}
				}
				//格约束--两格不能同时填一个数字
				for (j = 1; j <= 5; j++)
				{
					for (k = 1; k <= 9; k++)
					{
						for (q = k + 1; q <= 9; q++)
						{
							fprintf(fp, "%d %d 0\n", -(i * 100 + j * 10 + k), -(i * 100 + j * 10 + q));
						}
					}
				}
				for (j = 1; j <= 5; j++)
				{
					for (k = 1; k <= 9; k++)
					{
						fprintf(fp, "%d ", i * 100 + j * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//无重复数字
				for (k = 1; k <= 9; k++)
				{
					for (q = 1; q <= 5; q++)
					{
						for (j = q + 1; j <= 5; j++)
						{
							fprintf(fp, "%d %d 0\n", -(i * 100 + j * 10 + k), -(i * 100 + q * 10 + k));
						}
					}
				}
			}
		}
		//2.//更换每个位置的代号数字，相当于斜着看数独，这样后面的代码几乎完全一样
		int arr[10][10];
		memset(arr, 0, sizeof(arr));
		arr[1][1] = 15; arr[1][2] = 26; arr[1][3] = 37; arr[1][4] = 48; arr[1][5] = 59;
		arr[2][1] = 14; arr[2][2] = 25; arr[2][3] = 36; arr[2][4] = 47; arr[2][5] = 58; arr[2][6] = 68;
		arr[3][1] = 13; arr[3][2] = 24; arr[3][3] = 35; arr[3][4] = 46; arr[3][5] = 57; arr[3][6] = 67; arr[3][7] = 77;
		arr[4][1] = 12; arr[4][2] = 23; arr[4][3] = 34; arr[4][4] = 45; arr[4][5] = 56; arr[4][6] = 66; arr[4][7] = 76; arr[4][8] = 86;
		arr[5][1] = 11; arr[5][2] = 22; arr[5][3] = 33; arr[5][4] = 44; arr[5][5] = 55; arr[5][6] = 65; arr[5][7] = 75; arr[5][8] = 85; arr[5][9] = 95;
		arr[6][1] = 21; arr[6][2] = 32; arr[6][3] = 43; arr[6][4] = 54; arr[6][5] = 64; arr[6][6] = 74; arr[6][7] = 84; arr[6][8] = 94;
		arr[7][1] = 31; arr[7][2] = 42; arr[7][3] = 53; arr[7][4] = 63; arr[7][5] = 73; arr[7][6] = 83; arr[7][7] = 93;
		arr[8][1] = 41; arr[8][2] = 52; arr[8][3] = 62; arr[8][4] = 72; arr[8][5] = 82; arr[8][6] = 92;
		arr[9][1] = 51; arr[9][2] = 61; arr[9][3] = 71; arr[9][4] = 81; arr[9][5] = 91;
		for (i = 1; i <= 9; i++)
		{
			if (i == 1 || i == 9)
			{
				//必填数字
				for (j = 1; j <= 5; j++)
				{
					fprintf(fp, "%d ", arr[i][j] * 10 + 5);
				}
				fprintf(fp, "0\n");
				//选填约束
				for (k = 1; k <= 4; k++)
				{
					for (q = 6; q <= 5 + k; q++)
					{
						for (j = 1; j <= 5; j++)
						{
							fprintf(fp, "%d ", arr[i][j] * 10 + q);
						}
						for (j = 1; j <= 5; j++)
						{
							fprintf(fp, "%d ", arr[i][j] * 10 + k);
						}
						fprintf(fp, "0\n");
					}
				}
				//无重复数字
				for (k = 1; k <= 9; k++)
				{
					for (q = 1; q <= 5; q++)
					{
						for (j = q + 1; j <= 5; j++)
						{
							fprintf(fp, "%d %d 0\n", -(arr[i][j] * 10 + k), -(arr[i][q] * 10 + k));
						}
					}
				}
			}
			if (i == 2 || i == 8)
			{
				//必填数字
				for (k = 4; k <= 6; k++)
				{
					for (j = 1; j <= 6; j++)
					{
						fprintf(fp, "%d ", arr[i][j] * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//选填约束
				for (k = 1; k <= 3; k++)
				{
					for (q = 7; q <= 6 + k; q++)
					{
						for (j = 1; j <= 6; j++)
						{
							fprintf(fp, "%d ", arr[i][j] * 10 + q);
						}
						for (j = 1; j <= 6; j++)
						{
							fprintf(fp, "%d ", arr[i][j] * 10 + k);
						}
						fprintf(fp, "0\n");
					}
				}
				//无重复数字
				for (k = 1; k <= 9; k++)
				{
					for (q = 1; q <= 6; q++)
					{
						for (j = q + 1; j <= 6; j++)
						{
							fprintf(fp, "%d %d 0\n", -(arr[i][j] * 10 + k), -(arr[i][q] * 10 + k));
						}
					}
				}
			}
			if (i == 3 || i == 7)
			{
				//必填数字
				for (k = 3; k <= 7; k++)
				{
					for (j = 1; j <= 7; j++)
					{
						fprintf(fp, "%d ", arr[i][j] * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//选填约束
				for (k = 1; k <= 2; k++)
				{
					for (q = 8; q <= 7 + k; q++)
					{
						for (j = 1; j <= 7; j++)
						{
							fprintf(fp, "%d ", arr[i][j] * 10 + q);
						}
						for (j = 1; j <= 7; j++)
						{
							fprintf(fp, "%d ", arr[i][j] * 10 + k);
						}
						fprintf(fp, "0\n");
					}
				}
				//无重复数字
				for (k = 1; k <= 9; k++)
				{
					for (q = 1; q <= 7; q++)
					{
						for (j = q + 1; j <= 7; j++)
						{
							fprintf(fp, "%d %d 0\n", -(arr[i][j] * 10 + k), -(arr[i][q] * 10 + k));
						}
					}
				}
			}
			if (i == 4 || i == 6)
			{
				//必填数字
				for (k = 2; k <= 8; k++)
				{
					for (j = 1; j <= 8; j++)
					{
						fprintf(fp, "%d ", arr[i][j] * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//约束选填
				for (j = 1; j <= 8; j++)
				{
					fprintf(fp, "%d ", arr[i][j] * 10 + 1);
				}
				for (j = 1; j <= 8; j++)
				{
					fprintf(fp, "%d ", arr[i][j] * 10 + 9);
				}
				fprintf(fp, "0\n");
				//无重复数字
				for (k = 1; k <= 9; k++)
				{
					for (q = 1; q <= 8; q++)
					{
						for (j = q + 1; j <= 8; j++)
						{
							fprintf(fp, "%d %d 0\n", -(arr[i][j] * 10 + k), -(arr[i][q] * 10 + k));
						}
					}
				}
			}
			if (i == 5)
			{
				//必填数字
				for (k = 1; k <= 9; k++)
				{
					for (j = 1; j <= 9; j++)
					{
						fprintf(fp, "%d ", arr[i][j] * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//无重复数字
				for (k = 1; k <= 9; k++)
				{
					for (q = 1; q <= 9; q++)
					{
						for (j = q + 1; j <= 9; j++)
						{
							fprintf(fp, "%d %d 0\n", -(arr[i][j] * 10 + k), -(arr[i][q] * 10 + k));
						}
					}
				}
			}
		}
		
		//3.//更换每个位置的代号数字，相当于斜着看数独，这样后面的代码几乎完全一样
		memset(arr, 0, sizeof(arr));
		arr[1][1] = 59; arr[1][2] = 68; arr[1][3] = 77; arr[1][4] = 86; arr[1][5] = 95;
		arr[2][1] = 48; arr[2][2] = 58; arr[2][3] = 67; arr[2][4] = 76; arr[2][5] = 85; arr[2][6] = 94;
		arr[3][1] = 37; arr[3][2] = 47; arr[3][3] = 57; arr[3][4] = 66; arr[3][5] = 75; arr[3][6] = 84; arr[3][7] = 93;
		arr[4][1] = 26; arr[4][2] = 36; arr[4][3] = 46; arr[4][4] = 56; arr[4][5] = 65; arr[4][6] = 74; arr[4][7] = 83; arr[4][8] = 92;
		arr[5][1] = 15; arr[5][2] = 25; arr[5][3] = 35; arr[5][4] = 45; arr[5][5] = 55; arr[5][6] = 64; arr[5][7] = 73; arr[5][8] = 82; arr[5][9] = 91;
		arr[6][1] = 14; arr[6][2] = 24; arr[6][3] = 34; arr[6][4] = 44; arr[6][5] = 54; arr[6][6] = 63; arr[6][7] = 72; arr[6][8] = 81;
		arr[7][1] = 13; arr[7][2] = 23; arr[7][3] = 33; arr[7][4] = 43; arr[7][5] = 53; arr[7][6] = 62; arr[7][7] = 71;
		arr[8][1] = 12; arr[8][2] = 22; arr[8][3] = 32; arr[8][4] = 42; arr[8][5] = 52; arr[8][6] = 61;
		arr[9][1] = 11; arr[9][2] = 21; arr[9][3] = 31; arr[9][4] = 41; arr[9][5] = 51;
		for (i = 1; i <= 9; i++)
		{
			if (i == 1 || i == 9)
			{
				//必填数字
				for (j = 1; j <= 5; j++)
				{
					fprintf(fp, "%d ", arr[i][j] * 10 + 5);
				}
				fprintf(fp, "0\n");
				//选填约束
				for (k = 1; k <= 4; k++)
				{
					for (q = 6; q <= 5 + k; q++)
					{
						for (j = 1; j <= 5; j++)
						{
							fprintf(fp, "%d ", arr[i][j] * 10 + q);
						}
						for (j = 1; j <= 5; j++)
						{
							fprintf(fp, "%d ", arr[i][j] * 10 + k);
						}
						fprintf(fp, "0\n");
					}
				}
				//无重复数字
				for (k = 1; k <= 9; k++)
				{
					for (q = 1; q <= 5; q++)
					{
						for (j = q + 1; j <= 5; j++)
						{
							fprintf(fp, "%d %d 0\n", -(arr[i][j] * 10 + k), -(arr[i][q] * 10 + k));
						}
					}
				}
			}
			if (i == 2 || i == 8)
			{
				//必填数字
				for (k = 4; k <= 6; k++)
				{
					for (j = 1; j <= 6; j++)
					{
						fprintf(fp, "%d ", arr[i][j] * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//选填约束
				for (k = 1; k <= 3; k++)
				{
					for (q = 7; q <= 6 + k; q++)
					{
						for (j = 1; j <= 6; j++)
						{
							fprintf(fp, "%d ", arr[i][j] * 10 + q);
						}
						for (j = 1; j <= 6; j++)
						{
							fprintf(fp, "%d ", arr[i][j] * 10 + k);
						}
						fprintf(fp, "0\n");
					}
				}
				//无重复数字
				for (k = 1; k <= 9; k++)
				{
					for (q = 1; q <= 6; q++)
					{
						for (j = q + 1; j <= 6; j++)
						{
							fprintf(fp, "%d %d 0\n", -(arr[i][j] * 10 + k), -(arr[i][q] * 10 + k));
						}
					}
				}
			}
			if (i == 3 || i == 7)
			{
				//必填数字
				for (k = 3; k <= 7; k++)
				{
					for (j = 1; j <= 7; j++)
					{
						fprintf(fp, "%d ", arr[i][j] * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//选填约束
				for (k = 1; k <= 2; k++)
				{
					for (q = 8; q <= 7 + k; q++)
					{
						for (j = 1; j <= 7; j++)
						{
							fprintf(fp, "%d ", arr[i][j] * 10 + q);
						}
						for (j = 1; j <= 7; j++)
						{
							fprintf(fp, "%d ", arr[i][j] * 10 + k);
						}
						fprintf(fp, "0\n");
					}
				}
				//无重复数字
				for (k = 1; k <= 9; k++)
				{
					for (q = 1; q <= 7; q++)
					{
						for (j = q + 1; j <= 7; j++)
						{
							fprintf(fp, "%d %d 0\n", -(arr[i][j] * 10 + k), -(arr[i][q] * 10 + k));
						}
					}
				}
			}
			if (i == 4 || i == 6)
			{
				//必填数字
				for (k = 2; k <= 8; k++)
				{
					for (j = 1; j <= 8; j++)
					{
						fprintf(fp, "%d ", arr[i][j] * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//约束选填
				for (j = 1; j <= 8; j++)
				{
					fprintf(fp, "%d ", arr[i][j] * 10 + 1);
				}
				for (j = 1; j <= 8; j++)
				{
					fprintf(fp, "%d ", arr[i][j] * 10 + 9);
				}
				fprintf(fp, "0\n");
				//无重复数字
				for (k = 1; k <= 9; k++)
				{
					for (q = 1; q <= 8; q++)
					{
						for (j = q + 1; j <= 8; j++)
						{
							fprintf(fp, "%d %d 0\n", -(arr[i][j] * 10 + k), -(arr[i][q] * 10 + k));
						}
					}
				}
			}
			if (i == 5)
			{
				//必填数字
				for (k = 1; k <= 9; k++)
				{
					for (j = 1; j <= 9; j++)
					{
						fprintf(fp, "%d ", arr[i][j] * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//无重复数字
				for (k = 1; k <= 9; k++)
				{
					for (q = 1; q <= 9; q++)
					{
						for (j = q + 1; j <= 9; j++)
						{
							fprintf(fp, "%d %d 0\n", -(arr[i][j] * 10 + k), -(arr[i][q] * 10 + k));
						}
					}
				}
			}
		}
	}
	fclose(fp);
	return;
}

void Printorigin()//打印现在的题面
{
	printf("         / \\ / \\ / \\ / \\ / \\ \n");
	printf("        | %d | %d | %d | %d | %d | \n",sudo.original[1][1],sudo.original[1][2],sudo.original[1][3],sudo.original[1][4],sudo.original[1][5]);
	printf("       / \\ / \\ / \\ / \\ / \\ / \\ \n");
	printf("      | %d | %d | %d | %d | %d | %d | \n",sudo.original[2][1],sudo.original[2][2],sudo.original[2][3],sudo.original[2][4],sudo.original[2][5],sudo.original[2][6]);
	printf("     / \\ / \\ / \\ / \\ / \\ / \\ / \\\n");
	printf("    | %d | %d | %d | %d | %d | %d | %d | \n",sudo.original[3][1],sudo.original[3][2],sudo.original[3][3],sudo.original[3][4],sudo.original[3][5],sudo.original[3][6],sudo.original[3][7]);
	printf("   / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\\n");
	printf("  | %d | %d | %d | %d | %d | %d | %d | %d | \n",sudo.original[4][1],sudo.original[4][2],sudo.original[4][3],sudo.original[4][4],sudo.original[4][5],sudo.original[4][6],sudo.original[4][7],sudo.original[4][8]);
	printf(" / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ \n");
	printf("| %d | %d | %d | %d | %d | %d | %d | %d | %d |\n",sudo.original[5][1],sudo.original[5][2],sudo.original[5][3],sudo.original[5][4],sudo.original[5][5],sudo.original[5][6],sudo.original[5][7],sudo.original[5][8],sudo.original[5][9]);
	printf(" \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ /\n");
	printf("  | %d | %d | %d | %d | %d | %d | %d | %d | \n",sudo.original[6][1],sudo.original[6][2],sudo.original[6][3],sudo.original[6][4],sudo.original[6][5],sudo.original[6][6],sudo.original[6][7],sudo.original[6][8]);
	printf("   \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \n");
	printf("    | %d | %d | %d | %d | %d | %d | %d | \n",sudo.original[7][1],sudo.original[7][2],sudo.original[7][3],sudo.original[7][4],sudo.original[7][5],sudo.original[7][6],sudo.original[7][7]);
	printf("     \\ / \\ / \\ / \\ / \\ / \\ / \\ / \n");
	printf("      | %d | %d | %d | %d | %d | %d | \n",sudo.original[8][1],sudo.original[8][2],sudo.original[8][3],sudo.original[8][4],sudo.original[8][5],sudo.original[8][6]);
	printf("       \\ / \\ / \\ / \\ / \\ / \\ / \n");
	printf("        | %d | %d | %d | %d | %d | \n",sudo.original[9][1],sudo.original[9][2],sudo.original[9][3],sudo.original[9][4],sudo.original[9][5]);
	printf("         \\ / \\ / \\ / \\ / \\ / \n");
		putchar('\n');

}

void Printfinal()//打印答案题面
{
	printf("         / \\ / \\ / \\ / \\ / \\ \n");
	printf("        | %d | %d | %d | %d | %d | \n",sudo.finalans[1][1],sudo.finalans[1][2],sudo.finalans[1][3],sudo.finalans[1][4],sudo.finalans[1][5]);
	printf("       / \\ / \\ / \\ / \\ / \\ / \\ \n");
	printf("      | %d | %d | %d | %d | %d | %d | \n",sudo.finalans[2][1],sudo.finalans[2][2],sudo.finalans[2][3],sudo.finalans[2][4],sudo.finalans[2][5],sudo.finalans[2][6]);
	printf("     / \\ / \\ / \\ / \\ / \\ / \\ / \\\n");
	printf("    | %d | %d | %d | %d | %d | %d | %d | \n",sudo.finalans[3][1],sudo.finalans[3][2],sudo.finalans[3][3],sudo.finalans[3][4],sudo.finalans[3][5],sudo.finalans[3][6],sudo.finalans[3][7]);
	printf("   / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\\n");
	printf("  | %d | %d | %d | %d | %d | %d | %d | %d | \n",sudo.finalans[4][1],sudo.finalans[4][2],sudo.finalans[4][3],sudo.finalans[4][4],sudo.finalans[4][5],sudo.finalans[4][6],sudo.finalans[4][7],sudo.finalans[4][8]);
	printf(" / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ \n");
	printf("| %d | %d | %d | %d | %d | %d | %d | %d | %d |\n",sudo.finalans[5][1],sudo.finalans[5][2],sudo.finalans[5][3],sudo.finalans[5][4],sudo.finalans[5][5],sudo.finalans[5][6],sudo.finalans[5][7],sudo.finalans[5][8],sudo.finalans[5][9]);
	printf(" \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ /\n");
	printf("  | %d | %d | %d | %d | %d | %d | %d | %d | \n",sudo.finalans[6][1],sudo.finalans[6][2],sudo.finalans[6][3],sudo.finalans[6][4],sudo.finalans[6][5],sudo.finalans[6][6],sudo.finalans[6][7],sudo.finalans[6][8]);
	printf("   \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \n");
	printf("    | %d | %d | %d | %d | %d | %d | %d | \n",sudo.finalans[7][1],sudo.finalans[7][2],sudo.finalans[7][3],sudo.finalans[7][4],sudo.finalans[7][5],sudo.finalans[7][6],sudo.finalans[7][7]);
	printf("     \\ / \\ / \\ / \\ / \\ / \\ / \\ / \n");
	printf("      | %d | %d | %d | %d | %d | %d | \n",sudo.finalans[8][1],sudo.finalans[8][2],sudo.finalans[8][3],sudo.finalans[8][4],sudo.finalans[8][5],sudo.finalans[8][6]);
	printf("       \\ / \\ / \\ / \\ / \\ / \\ / \n");
	printf("        | %d | %d | %d | %d | %d | \n",sudo.finalans[9][1],sudo.finalans[9][2],sudo.finalans[9][3],sudo.finalans[9][4],sudo.finalans[9][5]);
	printf("         \\ / \\ / \\ / \\ / \\ / \n");
		putchar('\n');
}

void AnsTransSudo()//将ans数组中保存的答案，转移到sudo.finalans数组中
{			
	int k, i, j, n;
	for (i = 1; i <= 9; i++) {
		if (i < 5)
		{
			for (j = 1; j <= (4 + i % 5); j++)
			{
				for (k = 1; k <= 9; k++)
				{
					if (qq.ans[i * 100 + j * 10 + k] > 0)
					{
						sudo.finalans[i][j] = k;
						
					}
				}
			}
		}
		if (i == 5)
		{
			for (j = 1; j <= 9; j++)
			{
				for (k = 1; k <= 9; k++)
				{
					if (qq.ans[i * 100 + j * 10 + k] > 0)
					{
						sudo.finalans[i][j] = k;
					}
				}
			}
		}
		if (i > 5)
		{
			for (j = 1; j <= (9 - i % 5); j++)
			{
				for (k = 1; k <= 9; k++)
				{
					if (qq.ans[i * 100 + j * 10 + k] > 0)
					{
						sudo.finalans[i][j] = k;
					}
				}
			}
		}
	}
}

void _CreateSudoku() //生成数独用
{
	srand((unsigned)time(NULL));
	int x = rand() % 6 ;	
	int i;
	int a[7][62]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,4,0,4,0,0,0,0,5,1,7,3,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0},
				  {5,3,0,4,0,0,0,7,0,3,5,7,0,9,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,8,0,0,0,0,0,0,0},
				  {0,2,0,6,4,6,0,0,7,0,0,0,7,9,0,0,0,0,3,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0},
				  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,5,2,0,0,6,0,0,0,0,0,0,0,0,0,4,7,6,0,0,0,0,0,5,0,0,0,2,0,8,0,0,0,0,0,0,7,0,0,0,0,0},
				  {0,0,0,4,0,0,0,7,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,2,5,0,4,2,0,0,7,0,5,3,0},
				  {0,0,0,0,0,0,1,5,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,3,0,6,4,3,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
				  {0,0,7,0,4,5,0,0,0,0,0,0,0,0,0,2,4,3,4,0,0,0,0,0,2,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,6,0,0,0,0,0,0}};
	sudo.num = 61;
	for (i = 1; i <= 61;i++ )
	{
			sudo.flag[arr1[i] / 10][arr1[i] % 10] = 1;
			sudo.original[arr1[i] / 10][arr1[i] % 10] = a[x][i-1];

	}
	FormCnfFile(1);				//将初始数独盘导入cnf文件，便于后续求解已得到完整数独题面 
	char cnfname[200] = "prepare.cnf";
	_ReadFile(cnfname);	//读取刚刚生成的cnf文件，准备DPLL求解以生成数独题面
	func1(1, 0);
	AnsTransSudo();	//将ans数组中保存的答案，转移到sudo.finalans数组中		
	sudo.num = 61;	
	return;
}

void CreateSudoku() {//求解数独用
	int i = 1;
	char temp;
	sudo.num = 0;
	printf("输入:");
	for (i = 1; i <= 61; )
	{
		temp = getchar();
		if (temp >= '0' && temp <= '9')
		{
			if (temp != '0')
			{
				sudo.num++;
				sudo.flag[arr1[i] / 10][arr1[i] % 10] = 1;
				sudo.original[arr1[i] / 10][arr1[i] % 10] = temp - '0';
			}
			i++;
		}
	}
	printf("初始数独:\n");
	Printorigin();
	getchar(); 
	FormCnfFile(1);				//将初始数独盘导入cnf文件，便于后续求解已得到完整数独题面 
	char cnfname[200] = "prepare.cnf";
	_ReadFile(cnfname);	//读取刚刚生成的cnf文件，准备DPLL求解以生成数独题面
	time_t t1, t2;
	t1 = clock();
	func1(1, 0);
	t2 = clock();
	printf("用时: %d ms\n", t2 - t1);
	AnsTransSudo();	//将ans数组中保存的答案，转移到sudo.finalans数组中		
	Printfinal();
	sudo.num = 61;		
	return;
}

status IfCanDig(int x, int y)
{//判断当前位置可否挖洞 
	if(sudo.original[x][y] == 0 || sudo.finalans[x][y] == -1)	
	return ERROR;
	else{
		for(int i = 1; i <= 9; i++)
		{
			if(i == sudo.original[x][y])	
			continue;
			else
			{
				sudo.original[x][y] = i;
				FormCnfFile(2);
				char cnfname[200] = "dighole.cnf";
				_ReadFile(cnfname);
				int res = func1(1,0);
				if(res == OK){		//说明解不唯一，则该点不能删除 
					sudo.original[x][y] = sudo.finalans[x][y];	//恢复 
					return ERROR;
				}
				else{
					sudo.original[x][y] = 0;	//可以挖洞
					sudo.num--; 
					return OK;
				}
				
			}
		}
	}
}

void DigHole(int diff)
{	//挖洞法创造数独题面 
	int count_del = 0;	//记录已经删除（挖洞）的个数 
	int target = 0;		//目标挖洞数 
	if(diff == 1)	target = 30;
	else if(diff == 2)	target = 40;
	else if(diff == 3)	target = 50;
	while(1){
		if(count_del >= target)	break; 
		int x = rand() % 9 + 1;
		int y = rand() % 9 + 1;
		if((x <= 5 && y >x+4) || (x > 5 && y >14-x ))
		continue;
		if(IfCanDig(x,y) == OK){
			sudo.flag[x][y] = 1;			//在输入求解时用到，flag为1说明可以输入数字，flag为0说明是题面初始数字，不能更改 
			count_del++;
		}
	}
} 

void InputToSolve(){//输入求解模块 
	printf("请依次输入行、列以及填充的数字：\n");
	int x, y, v;
	scanf("%d %d %d",&x,&y,&v);
	if((x < 1 || x > 9) ||(x<=5&&y>x+4 )||(x>5&&y>14-x)|| (v < 1 || v > 9)){
		printf("请输入合法的位置以及填充的数字！\n");
		getchar();
		return;
	}
	else if(sudo.flag[x][y] == 0){
		printf("您输入的位置是题目所给数字的位置，此位置数字不可更改！\n");
		getchar();
		return; 
	}
	sudo.original[x][y] = v;
	sudo.num++;
	
}

void hint(){//提示
	while(1){
		int x = rand() % 9 + 1;
		int y = rand() % 9 + 1;
		if (!sudo.original[x][y])//未填入的位置
		{	if((x <= 5 && y >x+4) || (x > 5 && y >14-x ))
			continue;
			sudo.original[x][y] = sudo.finalans[x][y];
			sudo.num++;
			break; 
		}
	} 
}

status CheckCurrentSolve(){//检查当前输入的解是否有误 
	for(int i = 1; i <= 9; i++){
		for(int j = 1; j <= 9; j++){
			if(sudo.finalans[i][j] == -1)	continue;
			if(sudo.original[i][j] != sudo.finalans[i][j] && sudo.original[i][j] != 0)	return ERROR;
		}
	}
	return OK;
}
 

void Sudoku() {//数独模块 
	int op = 1;
	while (op) {
		//fflush(stdin);
		//system("cls");	
		printf("------------------------------------------------------------------------\n");
		printf(" 1.求解数独 sudoku solve  2.生成数独 sudoku game 0.退出 back		              	\n");
		printf("------------------------------------------------------------------------\n");
		scanf("%d",&op);
		if (op == 1) 
		{
			//fflush(stdin);
			initsudo();//初始化棋盘 
			//system("cls");
			memset(arr1, 0, sizeof(arr1));//初始化每个位置的序号，后续只用更换序号就可以将斜行转化
			arr1[1] = 11; arr1[2] = 12; arr1[3] = 13; arr1[4] = 14; arr1[5] = 15;
			arr1[6] = 21; arr1[7] = 22; arr1[8] = 23; arr1[9] = 24; arr1[10] = 25; arr1[11] = 26;
			arr1[12] = 31; arr1[13] = 32; arr1[14] = 33; arr1[15] = 34; arr1[16] = 35; arr1[17] = 36; arr1[18] = 37;
			arr1[19] = 41; arr1[20] = 42; arr1[21] = 43; arr1[22] = 44; arr1[23] = 45; arr1[24] = 46; arr1[25] = 47; arr1[26] = 48;
			arr1[27] = 51; arr1[28] = 52; arr1[29] = 53; arr1[30] = 54; arr1[31] = 55; arr1[32] = 56; arr1[33] = 57; arr1[34] = 58; arr1[35] = 59;
			arr1[36] = 61; arr1[37] = 62; arr1[38] = 63; arr1[39] = 64; arr1[40] = 65; arr1[41] = 66; arr1[42] = 67; arr1[43] = 68;
			arr1[44] = 71; arr1[45] = 72; arr1[46] = 73; arr1[47] = 74; arr1[48] = 75; arr1[49] = 76; arr1[50] = 77;
			arr1[51] = 81; arr1[52] = 82; arr1[53] = 83; arr1[54] = 84; arr1[55] = 85; arr1[56] = 86;
			arr1[57] = 91; arr1[58] = 92; arr1[59] = 93; arr1[60] = 94; arr1[61] = 95;
			CreateSudoku();//创造数独 
			getchar(); getchar();
		}
		else if(op==2)
		{
			initsudo();//初始化棋盘 
			//system("cls");
			memset(arr1, 0, sizeof(arr1));
			arr1[1] = 11; arr1[2] = 12; arr1[3] = 13; arr1[4] = 14; arr1[5] = 15;
			arr1[6] = 21; arr1[7] = 22; arr1[8] = 23; arr1[9] = 24; arr1[10] = 25; arr1[11] = 26;
			arr1[12] = 31; arr1[13] = 32; arr1[14] = 33; arr1[15] = 34; arr1[16] = 35; arr1[17] = 36; arr1[18] = 37;
			arr1[19] = 41; arr1[20] = 42; arr1[21] = 43; arr1[22] = 44; arr1[23] = 45; arr1[24] = 46; arr1[25] = 47; arr1[26] = 48;
			arr1[27] = 51; arr1[28] = 52; arr1[29] = 53; arr1[30] = 54; arr1[31] = 55; arr1[32] = 56; arr1[33] = 57; arr1[34] = 58; arr1[35] = 59;
			arr1[36] = 61; arr1[37] = 62; arr1[38] = 63; arr1[39] = 64; arr1[40] = 65; arr1[41] = 66; arr1[42] = 67; arr1[43] = 68;
			arr1[44] = 71; arr1[45] = 72; arr1[46] = 73; arr1[47] = 74; arr1[48] = 75; arr1[49] = 76; arr1[50] = 77;
			arr1[51] = 81; arr1[52] = 82; arr1[53] = 83; arr1[54] = 84; arr1[55] = 85; arr1[56] = 86;
			arr1[57] = 91; arr1[58] = 92; arr1[59] = 93; arr1[60] = 94; arr1[61] = 95;
			printf("正在生成数独，请稍后...\n");
			_CreateSudoku();//创造数独 
			for (int i = 1; i <= 9; i++) {
			for (int j = 1; j <= 9; j++) {
				if ((i < 5 && j >(4 + i % 5)) || (i > 5 && j > (9 - i % 5)))
				{
				continue;
				}
				sudo.original[i][j] = 	sudo.finalans[i][j] ;
			}
		}	
			printf("请选择难度：\n");
			printf("1.简单		2.中等		3.困难\n");
			int diff;
			scanf("%d",&diff);
			printf("正在给数独挖空，请稍后...\n"); 
			time_t t3, t4;
			t3 = clock();
			DigHole(diff);//（根据不同难度）挖洞法生成题面 
			t4 = clock();
			printf("已生成题面，用时 %dms\n",t4-t3);//挖洞用时 
			printf("按回车查看题面\n");
			getchar();getchar();
			int opp = 1;
			while(opp){
				//system("cls");
				Printorigin();		//打印当前题面			
				if(sudo.num == 61){//双数独共153个数字 
					if(CheckCurrentSolve())	
					{
						printf("当前求解全部正确！\n");
						printf("你已完成该数独的求解！\n");
						break;
					}
					else
					{  printf("当前解答有误！\n");
						continue;}
					
					getchar(); 
					
				}
				printf("1.输入求解		2.提示		3.显示答案		4.检查		0.退出\n");
				printf("请输入：");
				scanf("%d",&opp);
				if(opp == 1){
					fflush(stdin);
					InputToSolve();		//输入求解 
				}	
				else if(opp == 2){
					fflush(stdin);
					hint();				//提示 
					getchar();
				} 
				else if(opp == 3){
					//fflush(stdin);
					//system("cls");
					Printfinal();	//打印最终答案 
					printf("按任意键退出");
					getchar();
					break; 
				}
				else if(opp == 4){		//检查当前解答是否有误 
					fflush(stdin);
					if(CheckCurrentSolve())	printf("当前求解全部正确！\n");
					else printf("当前解答有误！\n");
					getchar();
				}
				else if(opp != 0){
					fflush(stdin);
					printf("输入错误！请重新输入！");
					getchar();
				}
			}

		}
	}
}





