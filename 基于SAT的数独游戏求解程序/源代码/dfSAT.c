#include"definition.h"
extern char FileName[100] ;//�ļ���
extern struct problem qq;
extern sud sudo;
clause* root = NULL;
int arr1[65];
float time1=0,time2=0;

/*sat�����õ��ĺ���*/

void init()									//��ʼ������
{
	clause* cla_tem = root;					//���ڵ�
	while (cla_tem != NULL) {
		element* liter_tem = cla_tem->first;//ָ���һ��Ԫ��
		while (liter_tem != NULL) {
			element* rel_liter = liter_tem;	
			liter_tem = liter_tem->nextnode;//ָ����һ��Ԫ��
			free(rel_liter);
		}
		clause* rel_cla = cla_tem;			//release_clause
		cla_tem = cla_tem->nextcla;
		free(rel_cla);
	}
	root = NULL;
	memset(qq.ans, 0, sizeof(qq.ans));      //��ʼ����ֵ
	memset(qq.element, 0, sizeof(qq.element));
	qq.clanum = 0;	qq.vexnum = 0;
}

void _ReadFile(char filename[200]) 			//��ȡ�ļ�
{
	init();				//��ʼ�� 
	FILE* fp = NULL;
	if (!(fp = fopen(filename, "r"))) {
		printf("�޷����ļ���");
		getchar();
	}
	else {
		char c;
		char str[5000] = { '\0' };
		while ((c = fgetc(fp)) == 'c') {
			fgets(str, 5000, fp);
		}
		fgetc(fp);								//���ո�
		fscanf(fp, "%s%d%d", str, &qq.vexnum, &qq.clanum);
		root = (clause*)malloc(sizeof(clause));
		root->literal_num = 0;
		clause* tem1 = root;						//ָ���Ӿ� 
		element* tem2_r = NULL, * tem2_l = NULL;	//ָ��Ԫ�أ�����β�巨���� 
		int num_cur;
		int cnt = 0;								//���� 
		for (int i = 1; i <= qq.clanum; i++) {
			if (i != 1) {							//��һ�ε�tem1->nextcla����rootָ���λ�ã�������ռ� 
				tem1->nextcla = (clause*)malloc(sizeof(clause));
				tem1 = tem1->nextcla;
				tem1->literal_num = 0;
			}
			tem1->flag = 0;
			tem2_l = NULL;	tem2_r = NULL;

			fscanf(fp, "%d", &num_cur);
			while (num_cur != 0) {
				tem1->literal_num++;
				tem2_r = (element*)malloc(sizeof(element));//����ռ�
				tem2_r->value = num_cur;
				qq.element[abs(num_cur)]++;					//Ԫ�ظ���
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
			tem2_l->nextnode = NULL;			//�Ӿ�����Ԫ�عҿ�
			cnt = 0;
		}
		tem1->nextcla = NULL;					//����Ӿ�ҿ� 	
	}
	fclose(fp);
}

void PrintFile() {//����ļ����ݣ����Ӿ䣩
	if (qq.clanum == 0 && qq.vexnum == 0) {
		printf("��δ��ȡ�ļ���\n");
		getchar();
		return;
	}
	printf("���ļ������Ӿ����£�\n");
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

void RecoverBegin() {//���س�ʼ״̬ 
	memset(qq.element, 0, sizeof(qq.element));//��ʼ����Ԫ��������¼
	qq.clanum = 0;
	for (clause* cla = root; cla; cla = cla->nextcla) {
		cla->flag = 0;							//�ָ��Ӿ�ڵ�ı�־�� 
		cla->literal_num = 0;					//�����Ӿ�ڵ��м�¼��Ԫ���� 
		for (element* lit = cla->first; lit; lit = lit->nextnode) {
			lit->flag = 0;						//�ָ���Ԫ�ؽڵ�ı�־�� 
			qq.element[abs(lit->value)]++;		//�ָ���Ԫ�س��ֵĴ��� 
			cla->literal_num++;					//�ָ��Ӿ�ڵ��м�¼��Ԫ���� 
		}
		qq.clanum++;
	}
}

status FindSingleClause()//Ѱ��һ�����Ӿ� 
{
	int sin_cla = 0, judge = 0;
	for (clause* p1 = root; p1; p1 = p1->nextcla)
	{
		if (p1->literal_num == 1 && !p1->flag) //�Ӿ���δ��ɾ����Ԫ��Ϊ1�Ҹ��Ӿ�δ��ɾ�� 
		{
			for (element* p2 = p1->first; p2; p2 = p2->nextnode) //�ҵ��Ӿ���Ԫ�� 
			{
				if (!p2->flag)					 //δ��ɾ������Ŀ��Ԫ�� 
				{
					sin_cla = p2->value;
					judge = 1;
					break;
				}
			}
			if (judge == 1)	break;
		}
	}
	return sin_cla;												//û�ҵ����Ӿ�ʱ����0 
}

status DeleteTarget(int depth, int single)//ɾ��Ŀ���Ӿ��Ԫ��
{
	for (clause* cla = root; cla; cla = cla->nextcla)
	{
		if (cla->flag)	continue;								//���Ӿ��Ѿ�ɾ�����ж���һ�Ӿ� 
		element* lit = cla->first;
		while (lit)
		{
			if (lit->flag)			//��Ԫ���Ѿ�ɾ�����ж���һԪ��
			{
				lit = lit->nextnode;
				continue;
			}
			if (lit->value == single)//�ҵ�Ŀ��
			{
				cla->flag = depth;								//ɾ���Ӿ�
				element* lit2 = cla->first;
				while (lit2) {									//��Ӧ��������element���飩��һ 
					if (!lit2->flag)							//���Ӿ�����֮ǰ�Ĳ�����δ��ɾ���Ĳ�����
					{
						qq.element[abs(lit2->value)]--;
						lit2 = lit2->nextnode;
						continue;
					}
					lit2 = lit2->nextnode;
				}
				qq.clanum--;									//���Ӿ�����һ 
				break;
			}
			else if (lit->value == -single) {						//��ɾ����Ԫ�� 
				lit->flag = depth;
				qq.element[abs(single)]--;
				cla->literal_num--;								//���Ӿ�����Ԫ������һ 
				if (!cla->literal_num)	return ERROR;			//˵��ɾ��������˿��Ӿ� 
				lit = lit->nextnode;
				continue;
			}
			lit = lit->nextnode;
		}
	}
	return OK;
}

status FindShortestLastLiteral() {//Ѱ������Ӿ������һ��Ԫ��
	int len = MAX_NUM;
	int maxx = 0, res = 0;
	for (clause* cla = root; cla; cla = cla->nextcla) {
		if (cla->flag)	continue;           //�Ӿ���ɾ��
		if (cla->literal_num < len) {		//Ѱ������Ӿ� 
			len = cla->literal_num;
			for (element* lit = cla->first; lit; lit = lit->nextnode)
			{
				if (!lit->flag && maxx < qq.element[abs(lit->value)])	//�ҵ����һ��Ԫ��
				res = lit->value;
			}
		}
	}
	return res;
}

void RecoverLastLevel(int depth)				//���ݹ飩������һ��ݹ�ʱ��״̬
{
	for (clause* cla = root; cla; cla = cla->nextcla) {
		if (cla->flag == depth) {					//˵�����Ӿ��ڱ��㱻�޸�
			cla->flag = 0;						//�ָ��Ӿ���Ч�� 
			qq.clanum++;						//�ָ����Ӿ��� 
			for (element* lit = cla->first; lit; lit = lit->nextnode) {//Ѱ���Ӿ����ڸò㱻�Ķ���Ԫ�� 
				if (lit->flag == depth) {
					lit->flag = 0;				//�ָ�Ԫ����Ч�� 
					cla->literal_num++;			//�ָ��Ӿ�ڵ��¼��Ԫ���� 
					qq.element[abs(lit->value)]++;//�ָ���Ԫ�س��ֵĴ��� 
					continue;
				}
				else if (!lit->flag) {
					qq.element[abs(lit->value)]++;
				}
			}
		}
		else {
			element* lit2 = cla->first;			//�Ӿ�û�б�ɾ��������Ԫ�ؿ��ܱ�ɾ�� 
			while (lit2) {
				if (lit2->flag == depth) {		//�б�ɾ����Ԫ�� 
					lit2->flag = 0;				//�ָ�Ԫ����Ч�� 
					cla->literal_num++;			//���Ӿ�Ԫ����+1
					qq.element[abs(lit2->value)]++;//��Ԫ����+1
					lit2 = lit2->nextnode;
					continue;
				}
				lit2 = lit2->nextnode;
			}
		}
	}
}

status FindShortestFirstLiteral() {//Ѱ������Ӿ��еĵ�һ��Ԫ��
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

status FindShortestMaxLiteral() {//Ѱ������Ӿ��г�������Ԫ�� 
	int len = MAX_NUM;
	int maxx = 0, res = 0;
	for (clause* cla = root; cla; cla = cla->nextcla) {
		if (cla->flag)	continue;			//�Ӿ���ɾ��
		if (cla->literal_num <= len) {		//Ѱ������Ӿ� 
			len = cla->literal_num;
			for (element* lit = cla->first; lit; lit = lit->nextnode) 
			{
				if (!lit->flag && maxx < qq.element[abs(lit->value)]) {//��Ƶ����ߵ�Ԫ��
					maxx = qq.element[abs(lit->value)];
					res = lit->value;
				}
			}
		}

	}
	return res;
}

status FindFirstLiteral() {//Ѱ�����У�δɾ����Ԫ���е��׸�Ԫ�� 
	clause* cla = root;
	while (cla) {
		if (cla->flag) {		//�Ӿ���ɾ��
			cla = cla->nextcla;
			continue;
		}
		else {
			element* lit = cla->first;
			while (lit) {
				if (lit->flag) {//Ԫ����ɾ��
					lit = lit->nextnode;
					continue;
				}
				return lit->value;
			}
		}
	}
	return 0;//û��Ԫ�أ��Ѿ�����
}

status func1(int depth, int tar)				//�ݹ�
{
	if (qq.clanum == 0)	return OK;				//�����ɣ�ʣ���Ӿ���Ϊ0 
	int single = 0;								
	if (!tar)									//��ʾ
		single = FindSingleClause();			//Ѱ��һ�����Ӿ䣨����õ���single�����ɸ��� 
	else
		single = tar;							

	while (single) {								//������ҵ����Ӿ� 
		qq.ans[abs(single)] = single;			//�洢��� 
		int res = DeleteTarget(depth, single);	//ɾ��Ŀ���Ӿ���Ԫ��
		if (res == ERROR)	return ERROR;		//ɾ����ʱ������˿��Ӿ䣬������Ҫ��
		single = FindSingleClause();
	}
	int max_lit = FindShortestLastLiteral();	//Ѱ������Ӿ������һ��Ԫ�� 
	if (!max_lit)	return OK;					//���û�ҵ���˵������Ԫ�س��ִ������Ѿ����0����ʱ˵���Ѿ���ȷ��� 
	if (func1(depth + 1, max_lit) == OK) {			//���Ѿ�û�е��Ӿ��ʱ��Ž��еݹ� //��ֵΪ��
		return OK;								//����ݹ鷵����OK��˵���ҵ��˽� 
	}
	//��һ��ifû��return˵��û�ҵ���
	RecoverLastLevel(depth + 1);					//������һ�����ĸ��ĸ�ԭ 
	if (func1(depth + 1, -max_lit) == OK) {//��ֵΪ��
		return OK;
	}
	RecoverLastLevel(depth + 1);
	return ERROR;								//������� 
}

status func2(int depth, int tar) {				//�ݹ�
	if (qq.clanum == 0)	return OK;				//�����ɣ�ʣ���Ӿ���Ϊ0 
	int single = 0;
	if (!tar)	single = FindSingleClause();	//Ѱ��һ�����Ӿ䣨single�����ɸ��� 
	else	single = tar;

	while (single) {								//������ҵ����Ӿ� 
		qq.ans[abs(single)] = single;			//�洢��� 
		int res = DeleteTarget(depth, single);	//ɾ��Ŀ���Ӿ���Ԫ��
		if (res == ERROR)	return ERROR;		//ɾ����ʱ������˿��Ӿ䣬������Ҫ��
		single = FindSingleClause();
	}
	int max_lit = FindShortestMaxLiteral();		//Ѱ������Ӿ��г���Ƶ������Ԫ�� 
	if (!max_lit)	return OK;					//���û�ҵ���˵������Ԫ�س��ִ������Ѿ����0����ʱ˵���Ѿ���ȷ��� 
	if (func2(depth + 1, max_lit) == OK) {			//���Ѿ�û�е��Ӿ��ʱ��Ž��еݹ� 
		return OK;								//����ݹ鷵����OK��˵���ҵ��˽� 
	}
	//��һ��ifû��return˵��û�ҵ���
	RecoverLastLevel(depth + 1);					//������һ�����ĸ��ĸ�ԭ 
	if (func2(depth + 1, -max_lit) == OK) {
		return OK;
	}
	RecoverLastLevel(depth + 1);
	return ERROR;								//�������
}

status func3(int depth, int tar) {				//�ݹ�
	if (qq.clanum == 0)	return OK;				//�����ɣ�ʣ���Ӿ���Ϊ0 
	int single = 0;
	if (!tar)	single = FindSingleClause();	//Ѱ��һ�����Ӿ� 
	else	single = tar;

	while (single) {								//������ҵ����Ӿ� 
		qq.ans[abs(single)] = single;			//�洢��� 
		int res = DeleteTarget(depth, single);	//ɾ��Ŀ���Ӿ���Ԫ��
		if (res == ERROR)	return ERROR;		//ɾ����ʱ������˿��Ӿ䣬������Ҫ��
		single = FindSingleClause();
	}
	int max_lit = FindFirstLiteral();			//Ѱ�ң�δ��ɾ����Ԫ���У��׸�Ԫ�� 
	if (!max_lit)	return OK;					//���û�ҵ���˵������Ԫ�س��ִ������Ѿ����0����ʱ˵���Ѿ���ȷ��� 
	if (func3(depth + 1, max_lit) == OK) {			//���Ѿ�û�е��Ӿ��ʱ��Ž��еݹ� 
		return OK;								//����ݹ鷵����OK��˵���ҵ��˽� 
	}
	//��һ��ifû��return˵��û�ҵ���
	RecoverLastLevel(depth + 1);					//������һ�����ĸ��ĸ�ԭ 
	if (func3(depth + 1, -max_lit) == OK) {
		return OK;
	}
	RecoverLastLevel(depth + 1);
	return ERROR;								//�������
}

void FormAnsFile(int res, int time, char filename[200]) {//���𰸱�����res�ļ��� 
	FILE* fp;
	int len = strlen(filename);
	filename[len - 3] = 'r';					//�ļ���׺�� 
	filename[len - 2] = 'e';
	filename[len - 1] = 's';
	fp = fopen(filename, "w+");
	fprintf(fp, "s %d", res);
	fprintf(fp, "\nv");
	if (res != 1);								//������� 
	else {
		for (int i = 1; i <= qq.vexnum; i++) {		//д��� 
			fprintf(fp, " %d", qq.ans[i]);
		}
	}
	fprintf(fp, "\nt %d", time);				//д�����ʱ�� 
	fclose(fp);
}

void improve(int opt)//���Ż���
{
	int res;
	time_t t1, t2;
	t1 = clock();
	RecoverBegin(); 		//���洢���ݵ�����ָ�����ʼ״̬���Ա��ٽ���һ�����
	res = func3(1, 0);
	t2 = clock();
	time2=t2-t1;
	printf("func5��ʱ��%d",t2-t1);
	if(time1<time2)
	printf("�Ż���(func%d�����func3)��%.4f%%\n",opt,((time2-time1)/time2)*100);
	else if(time1>time2)
	printf("�Ż���(func3�����func%d)��%.4f%%\n",opt,((time1-time2)/time1)*100);
	else
	printf("�Ż��ʣ�0.0000%%\n");
}

void DPLL() {//DPLLģ�� 
	if (qq.clanum == 0 && qq.vexnum == 0)
	{
		printf("��δ��ȡ�ļ���\n");
		getchar();
		return;
	}
	RecoverBegin(); 		//���洢���ݵ�����ָ�����ʼ״̬���Ա��ٽ���һ�����
	printf("1.func1(Ѱ������Ӿ�����һ��δ��ɾ����Ԫ��)\n2.func2(Ѱ������Ӿ������Ӿ伯�г��ִ�������Ԫ��)\n3.func3(Ѱ������Ӿ�ĵ�һ��Ԫ��)\n");
	printf("��ѡ��");
	int opt, res;			//res��¼��������н⻹���޽⣩ 
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
		printf("������1~3�е����֣�\n");
		getchar();
	}
	t2 = clock();
	time1=t2-t1;
	printf("�˷�����ʱ��%dms\n", t2 - t1);
	
	if (res == OK)	
	{
		printf("�ѳɹ���⣡\n");
		printf("�Ƿ����Ż��ʣ������Ѳ�������ʱ��������������1��������0\n");
		int x;
		scanf("%d",&x);
		if(x)
		improve(opt);
	}	
	else	printf("�����޽⣡\n");
	FormAnsFile(res, t2 - t1, FileName);			//���𰸱�����res�ļ���
	getchar(); getchar();
}

void PrintAnswer() {//��ӡ�� 
	printf("�����£�");
	for (int i = 1; i <= qq.vexnum; i++) {
		if (!qq.ans[i])	printf("%5d", i);		//˵�����޹ر�����ȡ��ֵ���� 
		else	printf("%5d", qq.ans[i]);
		if (i % 10 == 0)	;
	}
	getchar();
}

void CheckAnswer() {//���� 
	int res = 0, flag = 0;
	for (clause* cla = root; cla; cla = cla->nextcla) {
		flag = 0;
		for (element* lit = cla->first; lit; lit = lit->nextnode) {
			if (lit->value == qq.ans[abs(lit->value)]) {
				//һ���Ӿ��д���Ԫ������ģ����Ӿ�Ϊ��
				flag = 1;
				break;
			}
		}
		if (!flag)	break; 	 //�Ӿ���û��һ��Ԫ������õĽ���ͬ������
		
	}

	if (flag)	printf("�������ȷ��\n");
	else printf("��������\n");
	getchar();
}

void SATQuestion() {
	int op = 1;
	while (op) {
		//fflush(stdin);
		//system("cls");	
		printf("------------------------------------------------------------------------\n");
		printf("                  1.����cnf�ļ�			2.����ļ�����                  \n");
		printf("                  3.DPLL���			4.��������                  \n");	//ѡ��3ͬʱ���ʱ�����ܺͱ����ļ� 
		printf("                  5.����		                                    \n");
		printf("                  0.�˳�  		                                        \n");
		printf("------------------------------------------------------------------------\n");
		scanf("%d",&op);
		if (op == 1) {
			fflush(stdin);
			printf("�������ļ�����");
			scanf("%s", FileName);
			_ReadFile(FileName);
			printf("���ļ���%d��������%d���Ӿ�\n", qq.vexnum, qq.clanum);
			printf("�ѳɹ���ȡ�ļ���\n");
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
			printf("����������������룡");
			getchar(); 
		}
	}
}

/*********************************************************************************************************/

void initsudo() {//������ʼ�� 
	sudo.num = 0;
	for (int i = 1; i <= 9; i++) {
		for (int j = 1; j <= 9; j++) {
			if ((i < 5 && j >(4 + i % 5)) || (i > 5 && j > (9 - i % 5)))
			{
				sudo.finalans[i][j] = -1;
				sudo.original[i][j] = -1;
				sudo.flag[i][j] = -1; //����������Χ���ע-1
				continue;
			}
			sudo.finalans[i][j] = 0;
			sudo.original[i][j] = 0;
			sudo.flag[i][j] = 0;//�����ڳ�ʼ��Ϊ0
		}
	}
}

status VarTrans(int i, int j, int k)
{//��������1-9����ת������Ȼ˳����� 
	return i * 100 + j * 10 + k;
}

void FormCnfFile(int flag) {//����������ת��Ϊ��Ȼ���Ա��벢����cnf�ļ� 
	//flag�����������жϴ˺������ĸ����������ˣ���ȷ�����ɵ��ļ�����ʲô����
	char cnfname[200] = { '\0' };
	if (flag == 1)	strcpy(cnfname, "prepare.cnf");
	else if (flag == 2)	strcpy(cnfname, "dighole.cnf");
	FILE* fp = NULL;
	int dis[9][2] = { {0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2} };
	if (!(fp = fopen(cnfname, "w"))) {
		printf("�޷����ļ�\n");
		getchar();
	}
	else
	{
		fprintf(fp, "p cnf %d %d\n", 959, 7468 + sudo.num);
		int k = 1, i = 1, j = 1, count, n, q = 1;
		{//�����й���д���ļ����ܹ�sudo.num����䣩
			for (i = 1; i <= 9; i++)
			{
				for (j = 1; j <= 9; j++) {
					if (sudo.flag[i][j] == 1)	fprintf(fp, "%d 0\n", i * 100 + j * 10 + sudo.original[i][j]);
				}
			}
		}
		//����������д���ļ�
		//ֻ���й��� ������д��
		//1.
		for (i = 1; i <= 9; i++)
		{
			if (i == 5)//������������1-9
			{
				//��������
				for (k = 1; k <= 9; k++)
				{
					for (j = 1; j <= 9; j++)
					{
						fprintf(fp, "%d ", VarTrans(i, j, k));
					}
					fprintf(fp, "0\n");
				}
				//��Լ��--������ͬʱ��һ������
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
				//���ظ�����--һ�������ֲ��ظ�
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
			if (i == 4 || i == 6)//�����к͵�����
			{
				//��������δ2-8
				for (k = 2; k <= 8; k++)
				{
					for (j = 1; j <= 8; j++)
					{
						fprintf(fp, "%d ", VarTrans(i, j, k));
					}
					fprintf(fp, "0\n");
				}
				//Լ��ѡ��--1��9
				for (j = 1; j <= 8; j++)
				{
					fprintf(fp, "%d ", VarTrans(i, j, 1));
				}
				for (j = 1; j <= 8; j++)
				{
					fprintf(fp, "%d ", VarTrans(i, j, 9));
				}
				fprintf(fp, "0\n");
				//��Լ��--������ͬʱ��һ������
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
				//���ظ�����
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
			if (i == 3 || i == 7)///�����к͵�����
			{
				//��������3-7
				for (k = 3; k <= 7; k++)
				{
					for (j = 1; j <= 7; j++)
					{
						fprintf(fp, "%d ", i * 100 + j * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//ѡ��Լ��12��28��89
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
				//��Լ��--������ͬʱ��һ������
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
				//���ظ�����
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
			if (i == 2 || i == 8)//�ڶ��к͵ڰ���
			{
				//��������4-6
				for (k = 4; k <= 6; k++)
				{
					for (j = 1; j <= 6; j++)
					{
						fprintf(fp, "%d ", i * 100 + j * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//ѡ��Լ��123��237��378��789
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
				//��Լ��--������ͬʱ��һ������
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
				//���ظ�����
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
			if (i == 1 || i == 9)//��һ�к͵ھ���
			{
				//��������5
				for (j = 1; j <= 6; j++)
				{
					fprintf(fp, "%d ", i * 100 + j * 10 + 5);
				}
				fprintf(fp, "0\n");
				//ѡ��Լ��1234��2346��3467��4678��6789
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
				//��Լ��--������ͬʱ��һ������
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
				//���ظ�����
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
		//2.//����ÿ��λ�õĴ������֣��൱��б�ſ���������������Ĵ��뼸����ȫһ��
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
				//��������
				for (j = 1; j <= 5; j++)
				{
					fprintf(fp, "%d ", arr[i][j] * 10 + 5);
				}
				fprintf(fp, "0\n");
				//ѡ��Լ��
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
				//���ظ�����
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
				//��������
				for (k = 4; k <= 6; k++)
				{
					for (j = 1; j <= 6; j++)
					{
						fprintf(fp, "%d ", arr[i][j] * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//ѡ��Լ��
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
				//���ظ�����
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
				//��������
				for (k = 3; k <= 7; k++)
				{
					for (j = 1; j <= 7; j++)
					{
						fprintf(fp, "%d ", arr[i][j] * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//ѡ��Լ��
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
				//���ظ�����
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
				//��������
				for (k = 2; k <= 8; k++)
				{
					for (j = 1; j <= 8; j++)
					{
						fprintf(fp, "%d ", arr[i][j] * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//Լ��ѡ��
				for (j = 1; j <= 8; j++)
				{
					fprintf(fp, "%d ", arr[i][j] * 10 + 1);
				}
				for (j = 1; j <= 8; j++)
				{
					fprintf(fp, "%d ", arr[i][j] * 10 + 9);
				}
				fprintf(fp, "0\n");
				//���ظ�����
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
				//��������
				for (k = 1; k <= 9; k++)
				{
					for (j = 1; j <= 9; j++)
					{
						fprintf(fp, "%d ", arr[i][j] * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//���ظ�����
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
		
		//3.//����ÿ��λ�õĴ������֣��൱��б�ſ���������������Ĵ��뼸����ȫһ��
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
				//��������
				for (j = 1; j <= 5; j++)
				{
					fprintf(fp, "%d ", arr[i][j] * 10 + 5);
				}
				fprintf(fp, "0\n");
				//ѡ��Լ��
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
				//���ظ�����
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
				//��������
				for (k = 4; k <= 6; k++)
				{
					for (j = 1; j <= 6; j++)
					{
						fprintf(fp, "%d ", arr[i][j] * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//ѡ��Լ��
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
				//���ظ�����
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
				//��������
				for (k = 3; k <= 7; k++)
				{
					for (j = 1; j <= 7; j++)
					{
						fprintf(fp, "%d ", arr[i][j] * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//ѡ��Լ��
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
				//���ظ�����
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
				//��������
				for (k = 2; k <= 8; k++)
				{
					for (j = 1; j <= 8; j++)
					{
						fprintf(fp, "%d ", arr[i][j] * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//Լ��ѡ��
				for (j = 1; j <= 8; j++)
				{
					fprintf(fp, "%d ", arr[i][j] * 10 + 1);
				}
				for (j = 1; j <= 8; j++)
				{
					fprintf(fp, "%d ", arr[i][j] * 10 + 9);
				}
				fprintf(fp, "0\n");
				//���ظ�����
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
				//��������
				for (k = 1; k <= 9; k++)
				{
					for (j = 1; j <= 9; j++)
					{
						fprintf(fp, "%d ", arr[i][j] * 10 + k);
					}
					fprintf(fp, "0\n");
				}
				//���ظ�����
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

void Printorigin()//��ӡ���ڵ�����
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

void Printfinal()//��ӡ������
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

void AnsTransSudo()//��ans�����б���Ĵ𰸣�ת�Ƶ�sudo.finalans������
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

void _CreateSudoku() //����������
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
	FormCnfFile(1);				//����ʼ�����̵���cnf�ļ������ں�������ѵõ������������� 
	char cnfname[200] = "prepare.cnf";
	_ReadFile(cnfname);	//��ȡ�ո����ɵ�cnf�ļ���׼��DPLL�����������������
	func1(1, 0);
	AnsTransSudo();	//��ans�����б���Ĵ𰸣�ת�Ƶ�sudo.finalans������		
	sudo.num = 61;	
	return;
}

void CreateSudoku() {//���������
	int i = 1;
	char temp;
	sudo.num = 0;
	printf("����:");
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
	printf("��ʼ����:\n");
	Printorigin();
	getchar(); 
	FormCnfFile(1);				//����ʼ�����̵���cnf�ļ������ں�������ѵõ������������� 
	char cnfname[200] = "prepare.cnf";
	_ReadFile(cnfname);	//��ȡ�ո����ɵ�cnf�ļ���׼��DPLL�����������������
	time_t t1, t2;
	t1 = clock();
	func1(1, 0);
	t2 = clock();
	printf("��ʱ: %d ms\n", t2 - t1);
	AnsTransSudo();	//��ans�����б���Ĵ𰸣�ת�Ƶ�sudo.finalans������		
	Printfinal();
	sudo.num = 61;		
	return;
}

status IfCanDig(int x, int y)
{//�жϵ�ǰλ�ÿɷ��ڶ� 
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
				if(res == OK){		//˵���ⲻΨһ����õ㲻��ɾ�� 
					sudo.original[x][y] = sudo.finalans[x][y];	//�ָ� 
					return ERROR;
				}
				else{
					sudo.original[x][y] = 0;	//�����ڶ�
					sudo.num--; 
					return OK;
				}
				
			}
		}
	}
}

void DigHole(int diff)
{	//�ڶ��������������� 
	int count_del = 0;	//��¼�Ѿ�ɾ�����ڶ����ĸ��� 
	int target = 0;		//Ŀ���ڶ��� 
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
			sudo.flag[x][y] = 1;			//���������ʱ�õ���flagΪ1˵�������������֣�flagΪ0˵���������ʼ���֣����ܸ��� 
			count_del++;
		}
	}
} 

void InputToSolve(){//�������ģ�� 
	printf("�����������С����Լ��������֣�\n");
	int x, y, v;
	scanf("%d %d %d",&x,&y,&v);
	if((x < 1 || x > 9) ||(x<=5&&y>x+4 )||(x>5&&y>14-x)|| (v < 1 || v > 9)){
		printf("������Ϸ���λ���Լ��������֣�\n");
		getchar();
		return;
	}
	else if(sudo.flag[x][y] == 0){
		printf("�������λ������Ŀ�������ֵ�λ�ã���λ�����ֲ��ɸ��ģ�\n");
		getchar();
		return; 
	}
	sudo.original[x][y] = v;
	sudo.num++;
	
}

void hint(){//��ʾ
	while(1){
		int x = rand() % 9 + 1;
		int y = rand() % 9 + 1;
		if (!sudo.original[x][y])//δ�����λ��
		{	if((x <= 5 && y >x+4) || (x > 5 && y >14-x ))
			continue;
			sudo.original[x][y] = sudo.finalans[x][y];
			sudo.num++;
			break; 
		}
	} 
}

status CheckCurrentSolve(){//��鵱ǰ����Ľ��Ƿ����� 
	for(int i = 1; i <= 9; i++){
		for(int j = 1; j <= 9; j++){
			if(sudo.finalans[i][j] == -1)	continue;
			if(sudo.original[i][j] != sudo.finalans[i][j] && sudo.original[i][j] != 0)	return ERROR;
		}
	}
	return OK;
}
 

void Sudoku() {//����ģ�� 
	int op = 1;
	while (op) {
		//fflush(stdin);
		//system("cls");	
		printf("------------------------------------------------------------------------\n");
		printf(" 1.������� sudoku solve  2.�������� sudoku game 0.�˳� back		              	\n");
		printf("------------------------------------------------------------------------\n");
		scanf("%d",&op);
		if (op == 1) 
		{
			//fflush(stdin);
			initsudo();//��ʼ������ 
			//system("cls");
			memset(arr1, 0, sizeof(arr1));//��ʼ��ÿ��λ�õ���ţ�����ֻ�ø�����žͿ��Խ�б��ת��
			arr1[1] = 11; arr1[2] = 12; arr1[3] = 13; arr1[4] = 14; arr1[5] = 15;
			arr1[6] = 21; arr1[7] = 22; arr1[8] = 23; arr1[9] = 24; arr1[10] = 25; arr1[11] = 26;
			arr1[12] = 31; arr1[13] = 32; arr1[14] = 33; arr1[15] = 34; arr1[16] = 35; arr1[17] = 36; arr1[18] = 37;
			arr1[19] = 41; arr1[20] = 42; arr1[21] = 43; arr1[22] = 44; arr1[23] = 45; arr1[24] = 46; arr1[25] = 47; arr1[26] = 48;
			arr1[27] = 51; arr1[28] = 52; arr1[29] = 53; arr1[30] = 54; arr1[31] = 55; arr1[32] = 56; arr1[33] = 57; arr1[34] = 58; arr1[35] = 59;
			arr1[36] = 61; arr1[37] = 62; arr1[38] = 63; arr1[39] = 64; arr1[40] = 65; arr1[41] = 66; arr1[42] = 67; arr1[43] = 68;
			arr1[44] = 71; arr1[45] = 72; arr1[46] = 73; arr1[47] = 74; arr1[48] = 75; arr1[49] = 76; arr1[50] = 77;
			arr1[51] = 81; arr1[52] = 82; arr1[53] = 83; arr1[54] = 84; arr1[55] = 85; arr1[56] = 86;
			arr1[57] = 91; arr1[58] = 92; arr1[59] = 93; arr1[60] = 94; arr1[61] = 95;
			CreateSudoku();//�������� 
			getchar(); getchar();
		}
		else if(op==2)
		{
			initsudo();//��ʼ������ 
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
			printf("�����������������Ժ�...\n");
			_CreateSudoku();//�������� 
			for (int i = 1; i <= 9; i++) {
			for (int j = 1; j <= 9; j++) {
				if ((i < 5 && j >(4 + i % 5)) || (i > 5 && j > (9 - i % 5)))
				{
				continue;
				}
				sudo.original[i][j] = 	sudo.finalans[i][j] ;
			}
		}	
			printf("��ѡ���Ѷȣ�\n");
			printf("1.��		2.�е�		3.����\n");
			int diff;
			scanf("%d",&diff);
			printf("���ڸ������ڿգ����Ժ�...\n"); 
			time_t t3, t4;
			t3 = clock();
			DigHole(diff);//�����ݲ�ͬ�Ѷȣ��ڶ����������� 
			t4 = clock();
			printf("���������棬��ʱ %dms\n",t4-t3);//�ڶ���ʱ 
			printf("���س��鿴����\n");
			getchar();getchar();
			int opp = 1;
			while(opp){
				//system("cls");
				Printorigin();		//��ӡ��ǰ����			
				if(sudo.num == 61){//˫������153������ 
					if(CheckCurrentSolve())	
					{
						printf("��ǰ���ȫ����ȷ��\n");
						printf("������ɸ���������⣡\n");
						break;
					}
					else
					{  printf("��ǰ�������\n");
						continue;}
					
					getchar(); 
					
				}
				printf("1.�������		2.��ʾ		3.��ʾ��		4.���		0.�˳�\n");
				printf("�����룺");
				scanf("%d",&opp);
				if(opp == 1){
					fflush(stdin);
					InputToSolve();		//������� 
				}	
				else if(opp == 2){
					fflush(stdin);
					hint();				//��ʾ 
					getchar();
				} 
				else if(opp == 3){
					//fflush(stdin);
					//system("cls");
					Printfinal();	//��ӡ���մ� 
					printf("��������˳�");
					getchar();
					break; 
				}
				else if(opp == 4){		//��鵱ǰ����Ƿ����� 
					fflush(stdin);
					if(CheckCurrentSolve())	printf("��ǰ���ȫ����ȷ��\n");
					else printf("��ǰ�������\n");
					getchar();
				}
				else if(opp != 0){
					fflush(stdin);
					printf("����������������룡");
					getchar();
				}
			}

		}
	}
}





