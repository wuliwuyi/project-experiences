#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define MAX_VERTEX_NUM 20
#define MAX_INT 32726 //权重最大值
typedef int status;
typedef int KeyType;
typedef enum { DG, DN, UDG, UDN } GraphKind;
typedef struct {
	KeyType  key;
	char others[20];
} VertexType; //顶点类型定义
typedef struct ArcNode {         //表结点类型定义
	int adjvex;              //顶点位置编号 
	struct ArcNode* nextarc;	   //下一个表结点指针
} ArcNode;
typedef struct VNode {				//头结点及其数组类型定义
	VertexType data;       	//顶点信息
	ArcNode* firstarc;      	 //指向第一条弧
} VNode, AdjList[MAX_VERTEX_NUM];
typedef  struct {  //邻接表的类型定义
	AdjList vertices;     	 //头结点数组
	int vexnum, arcnum;   	  //顶点数、弧数
	GraphKind  kind;        //图的类型
} ALGraph;
typedef struct{  //控制链表
      struct { char name[30];
     		   ALGraph G;
      } elem[10];
      int length;
      int listsize;
 }LISTS;
int flag = 0;
int f[20] = {};
int a[20] = { 0 };
VertexType V[10][30] = {};//顶点数组
    KeyType VR[10][100][2] = {};//弧数组
    int i = 0, j;int num;
    int first =0;

void visit(VertexType v)//访问节点
{
    printf(" %d %s", v.key, v.others);
}

int check(VertexType V[], KeyType VR[][2])
{//检查关键字是否重复，若重复返回0，否则返回1
    if (V[0].key == -1) 
    return 1;
    for (int i = 0;V[i].key != -1;i++)
    {
        if (i >= 20) 
        return 1;
        for (int j = i + 1;V[j].key != -1;j++)
            if (V[i].key == V[j].key)
                return 1;
    }
    for (int i = 0;VR[i][0] != -1;i++)//检查弧是否重复
        for (int j = i + 1;VR[j][0] != -1;j++)
            if (VR[i][0] == VR[j][0] && VR[i][1] == VR[j][1])
                return 1;
    return 0;
}
status insert(ALGraph *G, int x, int y)
{//录入弧
    ArcNode* p = NULL;ArcNode* p1 = NULL;
    int i = 21, j = 21;
    for (int k = 0;k < (*G).vexnum;k++)//找到结点
    {
        if (x == (*G).vertices[k].data.key) 
        i = k;
        if (y == (*G).vertices[k].data.key) 
        j = k;
    }
    if (i == 21 || j == 21)//如果没找到，返回ERROR
    return ERROR;
    if ((*G).vertices[i].firstarc == NULL)//判断节点没有第一弧
    {
        (*G).vertices[i].firstarc = (ArcNode*)malloc(sizeof(ArcNode));//分配空间
        if ((*G).vertices[i].firstarc != NULL) 
        {
            (*G).vertices[i].firstarc->adjvex = j;//弧i指向j
            (*G).vertices[i].firstarc->nextarc = NULL;//暂时无下一指针
        }
    }
    else//有第一个弧
    {
        if (!(p = (ArcNode*)malloc(sizeof(ArcNode))))
            return OVERFLOW;
        p->nextarc = (*G).vertices[i].firstarc;//插入新弧
        (*G).vertices[i].firstarc = p;
        p->adjvex = j;//弧i指向j
    }
    if ((*G).vertices[j].firstarc == NULL)
    {
        (*G).vertices[j].firstarc = (ArcNode*)malloc(sizeof(ArcNode));
        if ((*G).vertices[j].firstarc != NULL) {
            (*G).vertices[j].firstarc->adjvex = i;//弧j指向i
            (*G).vertices[j].firstarc->nextarc = NULL;//暂时无下一指针
        }
    }
    else
    {
        if (!(p = (ArcNode*)malloc(sizeof(ArcNode))))
            return OVERFLOW;
        p->nextarc = (*G).vertices[j].firstarc;//插入新弧
        (*G).vertices[j].firstarc = p;
        p->adjvex = i;
    }
    (*G).arcnum++;
    return OK;
}
status CreateCraph(ALGraph *G,VertexType V[],KeyType VR[][2])
/*根据V和VR构造图T并返回OK，如果V和VR不正确，返回ERROR
如果有相同的关键字，返回ERROR。*/
{
    flag = check(V, VR);//检查顶点和弧是否重复
    if (flag == 1) 
    return ERROR;
    else
    {
        int i = 0;
        for (i = 0;V[i].key != -1;i++)
        {
            (*G).vertices[i].data.key = V[i].key;
            strcpy((*G).vertices[i].data.others, V[i].others);//录入顶点
            (*G).vertices[i].firstarc = NULL;//弧全挂空
        }
        (*G).vexnum = i;
        (*G).arcnum = 0;
        for (i = 0;VR[i][0] != -1;i++)
        {    int s=insert(G, VR[i][0], VR[i][1]);//录入弧
            if(s==ERROR)
            return ERROR;}
        return OK;
    }
}

status DestroyGraph(ALGraph *G)
/*销毁无向图G,删除G的全部顶点和边*/
{
    if ((*G).vexnum == 0) //无顶点，返回错误
    return ERROR;
    (*G).arcnum = 0;
    ArcNode* p1, * p2;
    int i = 0, j;
    for (i = 0;i < (*G).vexnum;i++)//销毁弧
    {
        if ((*G).vertices[i].firstarc != NULL) //有弧
        p1 = (*G).vertices[i].firstarc;
        else 
        p1 = NULL;
        do
        {
            p2 = p1;
            if(p1!=NULL){
                if (p1->nextarc == NULL) 
                p1 = NULL;
                else 
                p1 = p1->nextarc;
                free(p2);//释放
            }
        } while (p1);
        (*G).vertices[i].firstarc = NULL;//挂空
    }
    (*G).vexnum = 0;
    return OK;
}
int LocateVex(ALGraph G,KeyType u)
//根据u在图G中查找顶点，查找成功返回位序，否则返回-1；
{
int i = 0;
    while (i < G.vexnum)
    {
        if (u == G.vertices[i].data.key) 
        return i;
        i++;
    }
    return -1;
}

status PutVex(ALGraph *G,KeyType u,VertexType value)
//根据u在图G中查找顶点，查找成功将该顶点值修改成value，返回OK；
//如果查找失败或关键字不唯一，返回ERROR
{
int i = 0, num = -1;
    while (i < (*G).vexnum)
    {
        if (u == (*G).vertices[i].data.key) 
        num = i;
        if (value.key == (*G).vertices[i].data.key && value.key != u)//检查关键字
        return ERROR;
        i++;
    }
    if (num == -1) 
    return ERROR;
    (*G).vertices[num].data = value;//修改
    return OK;
}

int FirstAdjVex(ALGraph G,KeyType u)
//根据u在图G中查找顶点，查找成功返回顶点u的第一邻接顶点位序，否则返回-1；
{
int i = 0, num = -1;
    while (i < G.vexnum)
    {
        if (u == G.vertices[i].data.key) //找到邻接点
        {
            num = i;
            break;
        }
        i++;
    }
    if (num == -1 || G.vertices[num].firstarc == NULL) 
    return -1;
    else 
    return G.vertices[num].firstarc->adjvex;
}
int NextAdjVex(ALGraph G,KeyType v,KeyType w)
//v对应G的一个顶点,w对应v的邻接顶点；操作结果是返回v的（相对于w）下一个邻接顶点的位序；如果w是最后一个邻接顶点，或v、w对应顶点不存在，则返回-1。
{
int i = 0, num1 = -1, num2 = -1;
    while (i < G.vexnum)
    {
        if (v == G.vertices[i].data.key) num1 = i;//找到邻接点
        if (w == G.vertices[i].data.key) num2 = i;//下一个邻接点
        i++;
    }
    if (num1 == -1 || num2 == -1) 
    return -1;
    if (num1 == num2 || G.vertices[num1].firstarc == NULL) //不存在
    return -1;
    ArcNode* p1;
    p1 = G.vertices[num1].firstarc;
    while (p1->adjvex != num2) {
        p1 = p1->nextarc;
        if (p1 == NULL)
         return -1;
    }
    if (p1->nextarc == NULL)
     return -1;
    else 
    return p1->nextarc->adjvex;
}

status InsertVex(ALGraph *G,VertexType v)
//在图G中插入顶点v，成功返回OK,否则返回ERROR
{
int i = 0;
    while (i < (*G).vexnum) {
        if (v.key == (*G).vertices[i].data.key) //检查关键字是否重复
        return ERROR;
        i++;
        if (i >= 20) 
        return ERROR;
    }
    (*G).vertices[i].data = v;//输入顶点
    (*G).vertices[i].firstarc = NULL;
    (*G).vexnum++;
    return OK;
}
status DeleteVex(ALGraph *G,KeyType v)
//在图G中删除关键字v对应的顶点以及相关的弧，成功返回OK,否则返回ERROR
{
int i = 0, num = -1;
    while (i < (*G).vexnum) {//找到顶点
        if (v == (*G).vertices[i].data.key) 
        num = i;
        i++;
    }
    if (num == -1) //未找到顶点
    return ERROR;
    if (num != -1 && (*G).vexnum == 1) 
    return ERROR;
    ArcNode* p1, * p2;
    for (i = 0;i < (*G).vexnum;i++)
    {
        p1 = (*G).vertices[i].firstarc;
        p2 = p1;
        if (p1 == NULL) //没有弧
        continue;
        if (i == num)//已遍历到最后一个顶点
        {
            do{
                p2 = p1;
                p1 = p1->nextarc;
                free(p2);//释放
            } while (p1);
            (*G).vertices[i].firstarc = NULL;
            continue;
        }
        else
        {
            if (p1->adjvex == num)//改弧与顶点v关联
            {
                (*G).vertices[i].firstarc = p1->nextarc;
                free(p1);
                (*G).arcnum--;
            }
            else
            {
                p1 = p1->nextarc;//向相邻弧遍历
                while (p1)
                {
                    if (p1->adjvex == num) //改弧与顶点v关联
                    { p2->nextarc = p1->nextarc;
                    free(p1);
                    (*G).arcnum--;
                    break; 
                    }
                    else 
                    { p1 = p1->nextarc;
                    p2 = p2->nextarc; }
                }
            }
        }
    }
    for (i = num;i < (*G).vexnum;i++)//删除顶点
        (*G).vertices[i] = (*G).vertices[i + 1];
    (*G).vexnum--;
    for (i = 0;i < (*G).vexnum;i++)
    {
        p1 = (*G).vertices[i].firstarc;
        while (p1)
        {
            if (p1->adjvex > num) p1->adjvex--;//修改弧中记录的顶点序号
            p1 = p1->nextarc;
        }
    }
    return OK;
}
status InsertArc(ALGraph *G,KeyType v,KeyType w)
//在图G中增加弧<v,w>，成功返回OK,否则返回ERROR
{
int i = 0, num1 = -1, num2 = -1;
    while (i < (*G).vexnum)
    {//找到两个顶点
        if (v == (*G).vertices[i].data.key) 
        num1 = i;
        if (w == (*G).vertices[i].data.key) 
        num2 = i;
        i++;
    }
    if (num1 == -1 || num2 == -1 || num1 == num2) 
    return ERROR;//未找到顶点，或顶点一样

    ArcNode* p1, * p2;

    p1 = (*G).vertices[num1].firstarc;
    while (p1)//插入弧
    {
        if (p1->adjvex == num2) 
        return ERROR;
        p1 = p1->nextarc;
    }

    p1 = (*G).vertices[num1].firstarc;
    p2 = (ArcNode*)malloc(sizeof(ArcNode));
    (*G).vertices[num1].firstarc = p2;//弧v指向w
    p2->nextarc = p1;
    p2->adjvex = num2;

    p1 = (*G).vertices[num2].firstarc;
    p2 = (ArcNode*)malloc(sizeof(ArcNode));
    (*G).vertices[num2].firstarc = p2;//弧w指向v
    p2->nextarc = p1;
    p2->adjvex = num1;

    (*G).arcnum++;
    return OK;
}
status DeleteArc(ALGraph *G,KeyType v,KeyType w)
//在图G中删除弧<v,w>，成功返回OK,否则返回ERROR
{
    int pos1,pos2,a=0;
    ArcNode *p,*q;
    pos1=LocateVex(*G,v);
    pos2=LocateVex(*G,w);
    if(pos1==-1||pos2==-1) return ERROR;//关键字不存在
	//删一个顶点的弧 
    p=(*G).vertices[pos1].firstarc;
    q=p;
    if(p==NULL) return ERROR;//有一个顶点为空，即弧不存在 
	if(p->adjvex==pos2){
		(*G).vertices[pos1].firstarc=p->nextarc;
		a=1; 
        free(p);
	}
	else{
		p=p->nextarc;
		while(p){
			if(p->adjvex==pos2)
			{
				q->nextarc=p->nextarc;
				a=1;
                free(p);
                break;
			}
			q=p;
			p=p->nextarc;
		}
	}
	//删另一个顶点的弧 
	p=(*G).vertices[pos2].firstarc;
    q=p;
    if(p==NULL) return ERROR;
	if(p->adjvex==pos1){
		(*G).vertices[pos2].firstarc=p->nextarc;
        free(p);
	}
	else{
		p=p->nextarc;
		while(p){
			if(p->adjvex==pos1)
			{
				q->nextarc=p->nextarc;
                free(p);
                break;
			}
			q=p;
			p=p->nextarc;
		}
	}
	if(a==0) return ERROR;//没有该弧 
	(*G).arcnum--;
    return OK;
}

void DFS(ALGraph *G, int num, void (*visit)(VertexType))
{
    if (f[num]) return;
    ArcNode* p = NULL;
    f[num] = 1;
    visit((*G).vertices[num].data);
    p = (*G).vertices[num].firstarc;
    while (p) {
        num = p->adjvex;//先将该点后面的相邻的弧遍历完
        DFS(G, num, visit);
        p = p->nextarc;
    }
    return;
}
status DFSTraverse(ALGraph *G,void (*visit)(VertexType))
//对图G进行深度优先搜索遍历，依次对图中的每一个顶点使用函数visit访问一次，且仅访问一次
{
int i;
    for (i = 0;i < (*G).vexnum;i++) {
        DFS(G, i, visit);
    }
    return OK;
}
status BFSTraverse(ALGraph *G,void (*visit)(VertexType))
//对图G进行广度优先搜索遍历，依次对图中的每一个顶点使用函数visit访问一次，且仅访问一次
{
int i = 0;
    ArcNode* p1;
    while (i < (*G).vexnum)
    {
        if (a[i] == 0) { visit((*G).vertices[i].data);a[i] = 1; }
        p1 = (*G).vertices[i].firstarc;
        while (p1)
        {
            if (a[p1->adjvex] == 0) 
            { visit((*G).vertices[p1->adjvex].data);
            a[p1->adjvex] = 1; }
            p1 = p1->nextarc;//换到弧指向的点
        }
        i++;
    }
    return OK;
}

status SaveGraph(ALGraph G, char FileName[])
//将图的数据写入到文件FileName中
{
FILE* fp;
    if ((fp = fopen(FileName, "w")) == NULL) return ERROR;
    fprintf(fp, "%d ", G.vexnum);
    fprintf(fp, "%d ", G.arcnum);//保存顶点数与弧数
    for (int i = 0; i < G.vexnum; i++) {//保存结点
        fprintf(fp, "%d ", G.vertices[i].data.key);
        fprintf(fp, "%s ", G.vertices[i].data.others);
    }
    int keyflag = -1;
    char othersflag[10] = "nil";
    fprintf(fp, "%d ", keyflag);//顶点结束
    fprintf(fp, "%s ", othersflag);
    int stack[50][2];
    int top = 0;
    int key1 = 0, key2 = 0;
    ArcNode* p = NULL;
    for (int i = 0; i < G.vexnum; i++) {
        p = G.vertices[i].firstarc;
        while (p) {
            stack[top][0] = G.vertices[i].data.key;
            stack[top][1] = G.vertices[p->adjvex].data.key;
            top++;
            p = p->nextarc;
        }
        while (top != 0) {
            key1 = stack[top - 1][0];
            key2 = stack[top - 1][1];
            top--;
            fprintf(fp, "%d ", key1);
            fprintf(fp, "%d ", key2);
        }
    }
    fprintf(fp, "%d ", keyflag);//弧结束
    fprintf(fp, "%d ", keyflag);
    fclose(fp);
    return OK;
}
status createCraph(ALGraph *G, VertexType V[], KeyType VR[][2])
{
    int i = 0, j = 0;
    ArcNode* p;
    VertexType v;
    (*G).vexnum = 0;
    (*G).arcnum = 0;
    int arrkey[50] = { 0 };
    v.key = V[0].key;
    strcpy(v.others, V[0].others);
    while (v.key != -1) {
        for (int m = 0; m < (*G).vexnum; m++) 
        {
            if (v.key == arrkey[m]) 
            return ERROR;
        }
        (*G).vertices[(*G).vexnum].data = v;
        (*G).vertices[(*G).vexnum].firstarc = NULL;
        arrkey[(*G).vexnum] = v.key;
        (*G).vexnum++;
        i++;
        v.key = V[i].key;
        strcpy(v.others, V[i].others);
    }
    if ((*G).vexnum > 20 || (*G).vexnum < 1)  
    return ERROR;
    int a, b;
    int k = 0;
    a = VR[0][0];
    b = VR[0][1];
    while (a != -1 && b != -1) {
        (*G).arcnum++;
        int loca = LocateVex(*G, a);
        int locb = LocateVex(*G, b);
        if (loca == -1 || locb == -1) return ERROR;

        p = (ArcNode*)malloc(sizeof(ArcNode));
        p->nextarc = (*G).vertices[loca].firstarc;
        p->adjvex = locb;
        (*G).vertices[loca].firstarc = p;

        k++;
        a = VR[k][0];
        b = VR[k][1];
    }
    return OK;
}
status LoadGraph(ALGraph *G, char FileName[])
//读入文件FileName的图数据，创建图的邻接表
{
if ((*G).vexnum != 0) return INFEASIBLE;
    FILE* fp;
    fp = fopen(FileName, "r");
    int vexnum = -1, arcnum = -1, k = 0;
    VertexType V[25];
    KeyType VR[200][2];
    int vis[25][25] = { 0 };
    int key1 = -1, key2 = -1;
    fscanf(fp, "%d ", &vexnum);
    fscanf(fp, "%d ", &arcnum);
    for (int i = 0; i <= vexnum; i++) {
        fscanf(fp, "%d ", &V[i].key);
        fscanf(fp, "%s ", V[i].others);
    }
    fscanf(fp, "%d ", &key1);
    fscanf(fp, "%d ", &key2);
    while (key1 != -1 && key2 != -1) {
        VR[k][0] = key1;
        VR[k][1] = key2;
        k++;
        fscanf(fp, "%d ", &key1);
        fscanf(fp, "%d ", &key2);
    }
    createCraph(G, V, VR);
    return OK;
}

int ShortestPathLength(ALGraph G,int v,int w)//最短路径
{
    if(G.vexnum == 0 )  //图不存在
    {
        return INFEASIBLE;
    }
    int head = 0 ,tail =0;
    int record[100]={0};
    int arr[100][2];
    memset(arr,0,sizeof(arr));
    int i = 0; int flag = -1;
    int flagw = -1;
    for( ; i<G.vexnum ;i++)
    {
        if(G.vertices[i].data.key == v)
        {
            flag = i;//记录顶点1
        }
        if(G.vertices[i].data.key == w)
        {
            flagw = i;//记录顶点2
        }
    }
    if(flag == -1  || flagw == -1)//顶点不存在
    {
        return INFEASIBLE;
    }
    arr[head][0] = flag;

    while (head <= tail)
    {
        ArcNode *p = G.vertices[arr[head][0]].firstarc;
        if(G.vertices[arr[head][0]].data.key == w)
        {
            return arr[head][1];
        }
        while (p)
        {
            if(record[p->adjvex] == 0)
            {
                tail++;
                arr[tail][0] = p->adjvex;
                arr[tail][1] = arr[head][1]+1;//路径长度加一
                record[arr[head][0]]++;
            }
            p = p->nextarc;
        }
        head++;
    }
    return -1;
}

int flag16[100] ={0};
void dfs(ALGraph G,int nownode)
{
    flag16[nownode]=1;
    ArcNode *p =G.vertices[nownode].firstarc;
    while (p)
    {
        if(flag16[p->adjvex] == 0)
        {
            dfs(G,p->adjvex);
        }
        p = p->nextarc ;
    }
}

int ConnectedComponentsNums(ALGraph G)
{//连通分支数
    memset(flag16,0,sizeof(flag16));  //每次使用之前要清空
    int i  ;
    if(G.vexnum == 0)
    {
        printf("为初始化或者为空\n");
        return 0;
    }
    int count = 0;
    for(i = 0; i<G.vexnum ;i++)
    {
        if(flag16[i] == 0)
        {
            count++;
            dfs(G,i);
        }
    }
    return count;
}

status VerticesSetLessThanK(ALGraph G,KeyType v,int k)
//初始条件是图G存在；操作结果是返回与顶点v距离小于k的顶点集合；
{
	int a=1,flag[500],i,j,q[500],z=0,temp[500],s,m[200]={0};
	int pos=LocateVex(G,v);
	if(pos==-1||k<=1) return ERROR;
	for(i=0;i<500;i++) {flag[i]=-1;q[i]=-1;temp[i]=-1;}
	flag[0]=pos;
	while(a<k)//先从距离为1的找起，距离逐渐增大 
	{
		i=0;j=0;
		while(flag[i]!=-1){ 
			ArcNode *p=G.vertices[flag[i++]].firstarc;
			while(p){
				q[z++]=p->adjvex;
				temp[j++]=p->adjvex;
				p=p->nextarc;
			}
		}
		for(s=0;s<j;s++) flag[s]=temp[s];//把顶点序号存到flag里，从0位置开始存 
		flag[s]=-1;
		a++;
	}
	if(z==0) printf("该顶点无相连顶点\n");
	for(a=0;a<z;a++){
		if(q[a]!=pos&&m[q[a]]==0){//原顶点和已经输出过的顶点不输出 
			m[q[a]]=1;
			printf("%d %s ",G.vertices[q[a]].data.key,G.vertices[q[a]].data.others);
		}
	}
	return OK;
}


status AddList(LISTS *Lists,char ListName[])
// 添加图
{   if(first==1) num+=1;
    strcpy((*Lists).elem[(*Lists).length].name,ListName);
    printf("请输入顶点序列，以-1 nil结束：");
            do{
                scanf("%d%s", &V[num][i].key, V[num][i].others);//存入顶点关键字
            }while (V[num][i++].key != -1);
            i = 0;
            printf("请输入关系对序列，以-1 -1结束：");
            do{
                scanf("%d%d", &VR[num][i][0], &VR[num][i][1]);//存入弧
            }while (VR[num][i++][0] != -1);
            flag=CreateCraph(&(*Lists).elem[(*Lists).length].G, V[num], VR[num]) ;
    (*Lists).length+=1;
    if(flag==OK)
    return OK;
    if(flag==ERROR)
    return ERROR;
}
status change(ALGraph **G,int i,LISTS *Lists)//选择需要控制的图18

{
    first=1;
    if(i<=(*Lists).length)
    {(*G)=&((*Lists).elem[i-1].G);
    num=i-1;
    return OK;}
    else
    return ERROR;
}
int main(void)
{
    LISTS Lists;
    Lists.length=0;
    ALGraph *G;
    
    VertexType value;
    
    
    int op = 1, length, e, pre, next, flag1 = 0;
    char FileName[30] = { '\0' };
    char ListName[30] = { '\0' };
    printf("\n\n");
    printf("      Menu for Linear Table On Sequence Structure \n");
    printf("------------------------------------------------\n");
    printf("     1. CreateGraph创建图         2. DestroyGraph销毁图\n");
    printf("     3. LocateVex查找顶点         4. PutVex顶点赋值\n");
    printf("     5. FirstAdjVex第一邻接点     6. NextAdjVex下一邻接点\n");
    printf("     7. InsertVex插入顶点         8. DeleteVex删除顶点\n");
    printf("     9. InsertArc插入弧           10.DeleteArc删除弧\n");
    printf("     11.DFSTraverse深度优先       12.BFSTraverse广度优先\n");
    printf("     13.SaveGraph存入文件         14.LoadGraph写出文件\n");
    printf("     15.ShortestPathLength最短路径 16.ConnectedComponentsNums连通分支数\n");
    printf("     17.VerticesSetLessThanK距离小于k 18.AddGraph 添加图\n");
    printf("     19.Change切换\n");
    printf("     0. Exit\n");
    printf("------------------------------------------------\n");
    printf("\n请选择你的操作[0~19]:\n");
    while (op) {
        scanf("%d", &op);
        switch (op) {
        case 1:
            {i = 0;
            printf("请输入顶点序列，以-1 nil结束：");
            do{
                scanf("%d%s", &V[num][i].key, V[num][i].others);//存入顶点关键字
            }while (V[num][i++].key != -1);
            i = 0;
            printf("请输入关系对序列，以-1 -1结束：");
            do{
                scanf("%d%d", &VR[num][i][0], &VR[num][i][1]);//存入弧
            }while (VR[num][i++][0] != -1);
            if (CreateCraph(G, V[num], VR[num]) == OK)
                printf("图创建成功！\n");
            else
                printf("图创建失败！\n");
            break;}
        case 2:
            {if (DestroyGraph(G) == OK)
                printf("成功销毁图并释放数据元素的空间。\n");
            else
                printf("不能对不存在的图进行销毁操作！\n");
            break;}
        case 3:
            {printf("请输入想要在图中查找的顶点的关键字：");
            scanf("%d", &e);
            i = LocateVex(*G, e);
            if (i != -1) printf("图中关键字为%d的顶点的位序为%d\n", e, i);
            else
                printf("图中不存在关键字为%d的顶点！\n", e);
            break;}
        case 4:
            {printf("请输入想要修改的顶点的关键字：");
            scanf("%d", &e);
            printf("将其顶点值修改为：");
            scanf("%d %s", &value.key, value.others);
            flag1 = PutVex(G, e, value);
            if (flag1 == ERROR)
                printf("赋值操作失败！\n");
            else if (flag1 == OK)
                printf("已将关键字为%d的顶点值修改为%d,%s\n", e, value.key, value.others);
            break;}
        case 5:
            {printf("请输入想要查找其第一邻接点的顶点：");
            scanf("%d", &e);
            i = FirstAdjVex(*G, e);
            if (i != -1)
                printf("顶点%d的第一邻接点的位序为%d\n", e, i);
            else
                printf("顶点%d没有第一邻接点！\n", e);
            break;}
        case 6:
            {printf("请输入两个顶点的关键字：");
            scanf("%d %d", &e, &j);
            i = NextAdjVex(*G, e, j);
            if (i != -1)
                printf("顶点%d相对于顶点%d的下一个邻接顶点为%d %s\n", e, j, (*G).vertices[i].data.key, (*G).vertices[i].data.others);
            else printf("无下一邻接顶点！\n");
            break;}
        case 7:
            {printf("请输入想要插入的顶点值：");
            scanf("%d %s", &value.key, value.others);
            flag1 = InsertVex(G, value);
            if (flag1 == OK)
                printf("顶点 %d %s 已成功插入图中\n", value.key, value.others);
            else if (flag == ERROR)
                printf("插入失败！\n");
            break;}
        case 8:
            {printf("请输入想要删除的顶点的关键字：");
            scanf("%d", &e);
            flag1 = DeleteVex(G, e);
            if (flag1 == OK)
                printf("关键字为%d的顶点已从图中删除\n", e);
            else if (flag1 == ERROR)
                printf("删除失败！\n");
            break;}
        case 9:
            {printf("请输入想要插入的弧：");
            scanf("%d %d", &e, &j);
            flag1 = InsertArc(G, e, j);
            if (flag1 == OK)
                printf("插入成功！\n");
            else if (flag1 == ERROR)
                printf("插入失败！\n");
            break;}
        case 10:
            printf("请输入想要删除的弧：");
            scanf("%d %d", &e, &j);
            flag1 = DeleteArc(G, e, j);
            if (flag1 == OK)
                printf("删除成功！\n");
            else if (flag1 == ERROR)
                printf("删除失败！\n");
            break;
        case 11:
            for (int m = 0;m < 20;m++)
                f[m] = 0;
            printf("深度优先搜索遍历：\n");
            DFSTraverse(G, visit);
            printf("\n");
            break;
        case 12:
            for (int m = 0;m < 20;m++)
                a[m] = 0;
            printf("广度优先搜索遍历：\n");
            BFSTraverse(G, visit);
            printf("\n");
            break;
        case 13:
            {printf("请输入文件名称：");
            scanf("%s", FileName);
            flag1 = SaveGraph(*G, FileName);
            if (flag1 == ERROR)
                printf("文件打开失败！\n");
            else if (flag1 == OK)
                printf("图的内容已经复制到名称为 %s 的文件中。\n", FileName);
            break;}
        case 14:
            {printf("请输入文件名称：");
            scanf("%s", FileName);
            flag1 = LoadGraph(G, FileName);
            if (flag1 == ERROR)
                printf("文件打开失败！\n");
            else if (flag1 == OK)
                printf("名称为 %s 的文件中的内容已复制到图中。\n", FileName);
            else if (flag1 == INFEASIBLE)
                printf("不能对已存在的图进行进行读文件操作。\n");
            else if (flag1 == OVERFLOW)
                printf("溢出！\n");
            break;}
        case 15:
                printf("请输入顶点v和顶点w的关键字\n");
                int v15,w15;
                scanf("%d %d",&v15,&w15);
                int feedback = ShortestPathLength(*G,v15,w15);
                if(feedback==-1)
                {
                    printf("该图或顶点不存在或未初始化\n");
                    break;
                }
                if(feedback != -1)
                {
                    printf("最短路径为 %d \n",feedback);

                } else{
                    printf("操作失败\n");
                }
                break;
        case 16:
        {
            flag1=ConnectedComponentsNums( *G);
            if(flag1!=0)
             printf("图的连通分量为：%d\n",flag1);
             break;
        }
        case 17:
        {
            printf("请输入想要查找的顶点和长度：\n");
            int s,k;
            scanf("%d %d",&s,&k);
            printf("与顶点%d距离小于%d的顶点有：",s,k);
            VerticesSetLessThanK( *G, s, k);
            printf("\n");
            break;
        }
        case 19:{
            printf("请输入需要操作的图的序号\n");
            scanf("%d",&i);
            flag1=change(&G,i,&Lists);
            if(flag1==OK)
            printf("已成功切换\n");
            else
            printf("未成功切换.\n");
            break;
        }
        case 18:{
            char name[30];
            printf("请输入图的名字：");
            scanf("%s",name);
            
            j=AddList(&Lists,name);
            if (j==OK) 
            {
                printf("成功添加图\n");
            }
            else printf("失败\n");
            break;
        }
        case 0:
            break;
        }//end of switch
        printf("\n");
    }//end of while
    printf("欢迎下次再使用本系统！\n");
}//end of main()