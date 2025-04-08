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
#define MAX_INT 32726 //Ȩ�����ֵ
typedef int status;
typedef int KeyType;
typedef enum { DG, DN, UDG, UDN } GraphKind;
typedef struct {
	KeyType  key;
	char others[20];
} VertexType; //�������Ͷ���
typedef struct ArcNode {         //�������Ͷ���
	int adjvex;              //����λ�ñ�� 
	struct ArcNode* nextarc;	   //��һ������ָ��
} ArcNode;
typedef struct VNode {				//ͷ��㼰���������Ͷ���
	VertexType data;       	//������Ϣ
	ArcNode* firstarc;      	 //ָ���һ����
} VNode, AdjList[MAX_VERTEX_NUM];
typedef  struct {  //�ڽӱ�����Ͷ���
	AdjList vertices;     	 //ͷ�������
	int vexnum, arcnum;   	  //������������
	GraphKind  kind;        //ͼ������
} ALGraph;
typedef struct{  //��������
      struct { char name[30];
     		   ALGraph G;
      } elem[10];
      int length;
      int listsize;
 }LISTS;
int flag = 0;
int f[20] = {};
int a[20] = { 0 };
VertexType V[10][30] = {};//��������
    KeyType VR[10][100][2] = {};//������
    int i = 0, j;int num;
    int first =0;

void visit(VertexType v)//���ʽڵ�
{
    printf(" %d %s", v.key, v.others);
}

int check(VertexType V[], KeyType VR[][2])
{//���ؼ����Ƿ��ظ������ظ�����0�����򷵻�1
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
    for (int i = 0;VR[i][0] != -1;i++)//��黡�Ƿ��ظ�
        for (int j = i + 1;VR[j][0] != -1;j++)
            if (VR[i][0] == VR[j][0] && VR[i][1] == VR[j][1])
                return 1;
    return 0;
}
status insert(ALGraph *G, int x, int y)
{//¼�뻡
    ArcNode* p = NULL;ArcNode* p1 = NULL;
    int i = 21, j = 21;
    for (int k = 0;k < (*G).vexnum;k++)//�ҵ����
    {
        if (x == (*G).vertices[k].data.key) 
        i = k;
        if (y == (*G).vertices[k].data.key) 
        j = k;
    }
    if (i == 21 || j == 21)//���û�ҵ�������ERROR
    return ERROR;
    if ((*G).vertices[i].firstarc == NULL)//�жϽڵ�û�е�һ��
    {
        (*G).vertices[i].firstarc = (ArcNode*)malloc(sizeof(ArcNode));//����ռ�
        if ((*G).vertices[i].firstarc != NULL) 
        {
            (*G).vertices[i].firstarc->adjvex = j;//��iָ��j
            (*G).vertices[i].firstarc->nextarc = NULL;//��ʱ����һָ��
        }
    }
    else//�е�һ����
    {
        if (!(p = (ArcNode*)malloc(sizeof(ArcNode))))
            return OVERFLOW;
        p->nextarc = (*G).vertices[i].firstarc;//�����»�
        (*G).vertices[i].firstarc = p;
        p->adjvex = j;//��iָ��j
    }
    if ((*G).vertices[j].firstarc == NULL)
    {
        (*G).vertices[j].firstarc = (ArcNode*)malloc(sizeof(ArcNode));
        if ((*G).vertices[j].firstarc != NULL) {
            (*G).vertices[j].firstarc->adjvex = i;//��jָ��i
            (*G).vertices[j].firstarc->nextarc = NULL;//��ʱ����һָ��
        }
    }
    else
    {
        if (!(p = (ArcNode*)malloc(sizeof(ArcNode))))
            return OVERFLOW;
        p->nextarc = (*G).vertices[j].firstarc;//�����»�
        (*G).vertices[j].firstarc = p;
        p->adjvex = i;
    }
    (*G).arcnum++;
    return OK;
}
status CreateCraph(ALGraph *G,VertexType V[],KeyType VR[][2])
/*����V��VR����ͼT������OK�����V��VR����ȷ������ERROR
�������ͬ�Ĺؼ��֣�����ERROR��*/
{
    flag = check(V, VR);//��鶥��ͻ��Ƿ��ظ�
    if (flag == 1) 
    return ERROR;
    else
    {
        int i = 0;
        for (i = 0;V[i].key != -1;i++)
        {
            (*G).vertices[i].data.key = V[i].key;
            strcpy((*G).vertices[i].data.others, V[i].others);//¼�붥��
            (*G).vertices[i].firstarc = NULL;//��ȫ�ҿ�
        }
        (*G).vexnum = i;
        (*G).arcnum = 0;
        for (i = 0;VR[i][0] != -1;i++)
        {    int s=insert(G, VR[i][0], VR[i][1]);//¼�뻡
            if(s==ERROR)
            return ERROR;}
        return OK;
    }
}

status DestroyGraph(ALGraph *G)
/*��������ͼG,ɾ��G��ȫ������ͱ�*/
{
    if ((*G).vexnum == 0) //�޶��㣬���ش���
    return ERROR;
    (*G).arcnum = 0;
    ArcNode* p1, * p2;
    int i = 0, j;
    for (i = 0;i < (*G).vexnum;i++)//���ٻ�
    {
        if ((*G).vertices[i].firstarc != NULL) //�л�
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
                free(p2);//�ͷ�
            }
        } while (p1);
        (*G).vertices[i].firstarc = NULL;//�ҿ�
    }
    (*G).vexnum = 0;
    return OK;
}
int LocateVex(ALGraph G,KeyType u)
//����u��ͼG�в��Ҷ��㣬���ҳɹ�����λ�򣬷��򷵻�-1��
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
//����u��ͼG�в��Ҷ��㣬���ҳɹ����ö���ֵ�޸ĳ�value������OK��
//�������ʧ�ܻ�ؼ��ֲ�Ψһ������ERROR
{
int i = 0, num = -1;
    while (i < (*G).vexnum)
    {
        if (u == (*G).vertices[i].data.key) 
        num = i;
        if (value.key == (*G).vertices[i].data.key && value.key != u)//���ؼ���
        return ERROR;
        i++;
    }
    if (num == -1) 
    return ERROR;
    (*G).vertices[num].data = value;//�޸�
    return OK;
}

int FirstAdjVex(ALGraph G,KeyType u)
//����u��ͼG�в��Ҷ��㣬���ҳɹ����ض���u�ĵ�һ�ڽӶ���λ�򣬷��򷵻�-1��
{
int i = 0, num = -1;
    while (i < G.vexnum)
    {
        if (u == G.vertices[i].data.key) //�ҵ��ڽӵ�
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
//v��ӦG��һ������,w��Ӧv���ڽӶ��㣻��������Ƿ���v�ģ������w����һ���ڽӶ����λ�����w�����һ���ڽӶ��㣬��v��w��Ӧ���㲻���ڣ��򷵻�-1��
{
int i = 0, num1 = -1, num2 = -1;
    while (i < G.vexnum)
    {
        if (v == G.vertices[i].data.key) num1 = i;//�ҵ��ڽӵ�
        if (w == G.vertices[i].data.key) num2 = i;//��һ���ڽӵ�
        i++;
    }
    if (num1 == -1 || num2 == -1) 
    return -1;
    if (num1 == num2 || G.vertices[num1].firstarc == NULL) //������
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
//��ͼG�в��붥��v���ɹ�����OK,���򷵻�ERROR
{
int i = 0;
    while (i < (*G).vexnum) {
        if (v.key == (*G).vertices[i].data.key) //���ؼ����Ƿ��ظ�
        return ERROR;
        i++;
        if (i >= 20) 
        return ERROR;
    }
    (*G).vertices[i].data = v;//���붥��
    (*G).vertices[i].firstarc = NULL;
    (*G).vexnum++;
    return OK;
}
status DeleteVex(ALGraph *G,KeyType v)
//��ͼG��ɾ���ؼ���v��Ӧ�Ķ����Լ���صĻ����ɹ�����OK,���򷵻�ERROR
{
int i = 0, num = -1;
    while (i < (*G).vexnum) {//�ҵ�����
        if (v == (*G).vertices[i].data.key) 
        num = i;
        i++;
    }
    if (num == -1) //δ�ҵ�����
    return ERROR;
    if (num != -1 && (*G).vexnum == 1) 
    return ERROR;
    ArcNode* p1, * p2;
    for (i = 0;i < (*G).vexnum;i++)
    {
        p1 = (*G).vertices[i].firstarc;
        p2 = p1;
        if (p1 == NULL) //û�л�
        continue;
        if (i == num)//�ѱ��������һ������
        {
            do{
                p2 = p1;
                p1 = p1->nextarc;
                free(p2);//�ͷ�
            } while (p1);
            (*G).vertices[i].firstarc = NULL;
            continue;
        }
        else
        {
            if (p1->adjvex == num)//�Ļ��붥��v����
            {
                (*G).vertices[i].firstarc = p1->nextarc;
                free(p1);
                (*G).arcnum--;
            }
            else
            {
                p1 = p1->nextarc;//�����ڻ�����
                while (p1)
                {
                    if (p1->adjvex == num) //�Ļ��붥��v����
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
    for (i = num;i < (*G).vexnum;i++)//ɾ������
        (*G).vertices[i] = (*G).vertices[i + 1];
    (*G).vexnum--;
    for (i = 0;i < (*G).vexnum;i++)
    {
        p1 = (*G).vertices[i].firstarc;
        while (p1)
        {
            if (p1->adjvex > num) p1->adjvex--;//�޸Ļ��м�¼�Ķ������
            p1 = p1->nextarc;
        }
    }
    return OK;
}
status InsertArc(ALGraph *G,KeyType v,KeyType w)
//��ͼG�����ӻ�<v,w>���ɹ�����OK,���򷵻�ERROR
{
int i = 0, num1 = -1, num2 = -1;
    while (i < (*G).vexnum)
    {//�ҵ���������
        if (v == (*G).vertices[i].data.key) 
        num1 = i;
        if (w == (*G).vertices[i].data.key) 
        num2 = i;
        i++;
    }
    if (num1 == -1 || num2 == -1 || num1 == num2) 
    return ERROR;//δ�ҵ����㣬�򶥵�һ��

    ArcNode* p1, * p2;

    p1 = (*G).vertices[num1].firstarc;
    while (p1)//���뻡
    {
        if (p1->adjvex == num2) 
        return ERROR;
        p1 = p1->nextarc;
    }

    p1 = (*G).vertices[num1].firstarc;
    p2 = (ArcNode*)malloc(sizeof(ArcNode));
    (*G).vertices[num1].firstarc = p2;//��vָ��w
    p2->nextarc = p1;
    p2->adjvex = num2;

    p1 = (*G).vertices[num2].firstarc;
    p2 = (ArcNode*)malloc(sizeof(ArcNode));
    (*G).vertices[num2].firstarc = p2;//��wָ��v
    p2->nextarc = p1;
    p2->adjvex = num1;

    (*G).arcnum++;
    return OK;
}
status DeleteArc(ALGraph *G,KeyType v,KeyType w)
//��ͼG��ɾ����<v,w>���ɹ�����OK,���򷵻�ERROR
{
    int pos1,pos2,a=0;
    ArcNode *p,*q;
    pos1=LocateVex(*G,v);
    pos2=LocateVex(*G,w);
    if(pos1==-1||pos2==-1) return ERROR;//�ؼ��ֲ�����
	//ɾһ������Ļ� 
    p=(*G).vertices[pos1].firstarc;
    q=p;
    if(p==NULL) return ERROR;//��һ������Ϊ�գ����������� 
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
	//ɾ��һ������Ļ� 
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
	if(a==0) return ERROR;//û�иû� 
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
        num = p->adjvex;//�Ƚ��õ��������ڵĻ�������
        DFS(G, num, visit);
        p = p->nextarc;
    }
    return;
}
status DFSTraverse(ALGraph *G,void (*visit)(VertexType))
//��ͼG������������������������ζ�ͼ�е�ÿһ������ʹ�ú���visit����һ�Σ��ҽ�����һ��
{
int i;
    for (i = 0;i < (*G).vexnum;i++) {
        DFS(G, i, visit);
    }
    return OK;
}
status BFSTraverse(ALGraph *G,void (*visit)(VertexType))
//��ͼG���й�������������������ζ�ͼ�е�ÿһ������ʹ�ú���visit����һ�Σ��ҽ�����һ��
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
            p1 = p1->nextarc;//������ָ��ĵ�
        }
        i++;
    }
    return OK;
}

status SaveGraph(ALGraph G, char FileName[])
//��ͼ������д�뵽�ļ�FileName��
{
FILE* fp;
    if ((fp = fopen(FileName, "w")) == NULL) return ERROR;
    fprintf(fp, "%d ", G.vexnum);
    fprintf(fp, "%d ", G.arcnum);//���涥�����뻡��
    for (int i = 0; i < G.vexnum; i++) {//������
        fprintf(fp, "%d ", G.vertices[i].data.key);
        fprintf(fp, "%s ", G.vertices[i].data.others);
    }
    int keyflag = -1;
    char othersflag[10] = "nil";
    fprintf(fp, "%d ", keyflag);//�������
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
    fprintf(fp, "%d ", keyflag);//������
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
//�����ļ�FileName��ͼ���ݣ�����ͼ���ڽӱ�
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

int ShortestPathLength(ALGraph G,int v,int w)//���·��
{
    if(G.vexnum == 0 )  //ͼ������
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
            flag = i;//��¼����1
        }
        if(G.vertices[i].data.key == w)
        {
            flagw = i;//��¼����2
        }
    }
    if(flag == -1  || flagw == -1)//���㲻����
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
                arr[tail][1] = arr[head][1]+1;//·�����ȼ�һ
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
{//��ͨ��֧��
    memset(flag16,0,sizeof(flag16));  //ÿ��ʹ��֮ǰҪ���
    int i  ;
    if(G.vexnum == 0)
    {
        printf("Ϊ��ʼ������Ϊ��\n");
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
//��ʼ������ͼG���ڣ���������Ƿ����붥��v����С��k�Ķ��㼯�ϣ�
{
	int a=1,flag[500],i,j,q[500],z=0,temp[500],s,m[200]={0};
	int pos=LocateVex(G,v);
	if(pos==-1||k<=1) return ERROR;
	for(i=0;i<500;i++) {flag[i]=-1;q[i]=-1;temp[i]=-1;}
	flag[0]=pos;
	while(a<k)//�ȴӾ���Ϊ1�����𣬾��������� 
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
		for(s=0;s<j;s++) flag[s]=temp[s];//�Ѷ�����Ŵ浽flag���0λ�ÿ�ʼ�� 
		flag[s]=-1;
		a++;
	}
	if(z==0) printf("�ö�������������\n");
	for(a=0;a<z;a++){
		if(q[a]!=pos&&m[q[a]]==0){//ԭ������Ѿ�������Ķ��㲻��� 
			m[q[a]]=1;
			printf("%d %s ",G.vertices[q[a]].data.key,G.vertices[q[a]].data.others);
		}
	}
	return OK;
}


status AddList(LISTS *Lists,char ListName[])
// ���ͼ
{   if(first==1) num+=1;
    strcpy((*Lists).elem[(*Lists).length].name,ListName);
    printf("�����붥�����У���-1 nil������");
            do{
                scanf("%d%s", &V[num][i].key, V[num][i].others);//���붥��ؼ���
            }while (V[num][i++].key != -1);
            i = 0;
            printf("�������ϵ�����У���-1 -1������");
            do{
                scanf("%d%d", &VR[num][i][0], &VR[num][i][1]);//���뻡
            }while (VR[num][i++][0] != -1);
            flag=CreateCraph(&(*Lists).elem[(*Lists).length].G, V[num], VR[num]) ;
    (*Lists).length+=1;
    if(flag==OK)
    return OK;
    if(flag==ERROR)
    return ERROR;
}
status change(ALGraph **G,int i,LISTS *Lists)//ѡ����Ҫ���Ƶ�ͼ18

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
    printf("     1. CreateGraph����ͼ         2. DestroyGraph����ͼ\n");
    printf("     3. LocateVex���Ҷ���         4. PutVex���㸳ֵ\n");
    printf("     5. FirstAdjVex��һ�ڽӵ�     6. NextAdjVex��һ�ڽӵ�\n");
    printf("     7. InsertVex���붥��         8. DeleteVexɾ������\n");
    printf("     9. InsertArc���뻡           10.DeleteArcɾ����\n");
    printf("     11.DFSTraverse�������       12.BFSTraverse�������\n");
    printf("     13.SaveGraph�����ļ�         14.LoadGraphд���ļ�\n");
    printf("     15.ShortestPathLength���·�� 16.ConnectedComponentsNums��ͨ��֧��\n");
    printf("     17.VerticesSetLessThanK����С��k 18.AddGraph ���ͼ\n");
    printf("     19.Change�л�\n");
    printf("     0. Exit\n");
    printf("------------------------------------------------\n");
    printf("\n��ѡ����Ĳ���[0~19]:\n");
    while (op) {
        scanf("%d", &op);
        switch (op) {
        case 1:
            {i = 0;
            printf("�����붥�����У���-1 nil������");
            do{
                scanf("%d%s", &V[num][i].key, V[num][i].others);//���붥��ؼ���
            }while (V[num][i++].key != -1);
            i = 0;
            printf("�������ϵ�����У���-1 -1������");
            do{
                scanf("%d%d", &VR[num][i][0], &VR[num][i][1]);//���뻡
            }while (VR[num][i++][0] != -1);
            if (CreateCraph(G, V[num], VR[num]) == OK)
                printf("ͼ�����ɹ���\n");
            else
                printf("ͼ����ʧ�ܣ�\n");
            break;}
        case 2:
            {if (DestroyGraph(G) == OK)
                printf("�ɹ�����ͼ���ͷ�����Ԫ�صĿռ䡣\n");
            else
                printf("���ܶԲ����ڵ�ͼ�������ٲ�����\n");
            break;}
        case 3:
            {printf("��������Ҫ��ͼ�в��ҵĶ���Ĺؼ��֣�");
            scanf("%d", &e);
            i = LocateVex(*G, e);
            if (i != -1) printf("ͼ�йؼ���Ϊ%d�Ķ����λ��Ϊ%d\n", e, i);
            else
                printf("ͼ�в����ڹؼ���Ϊ%d�Ķ��㣡\n", e);
            break;}
        case 4:
            {printf("��������Ҫ�޸ĵĶ���Ĺؼ��֣�");
            scanf("%d", &e);
            printf("���䶥��ֵ�޸�Ϊ��");
            scanf("%d %s", &value.key, value.others);
            flag1 = PutVex(G, e, value);
            if (flag1 == ERROR)
                printf("��ֵ����ʧ�ܣ�\n");
            else if (flag1 == OK)
                printf("�ѽ��ؼ���Ϊ%d�Ķ���ֵ�޸�Ϊ%d,%s\n", e, value.key, value.others);
            break;}
        case 5:
            {printf("��������Ҫ�������һ�ڽӵ�Ķ��㣺");
            scanf("%d", &e);
            i = FirstAdjVex(*G, e);
            if (i != -1)
                printf("����%d�ĵ�һ�ڽӵ��λ��Ϊ%d\n", e, i);
            else
                printf("����%dû�е�һ�ڽӵ㣡\n", e);
            break;}
        case 6:
            {printf("��������������Ĺؼ��֣�");
            scanf("%d %d", &e, &j);
            i = NextAdjVex(*G, e, j);
            if (i != -1)
                printf("����%d����ڶ���%d����һ���ڽӶ���Ϊ%d %s\n", e, j, (*G).vertices[i].data.key, (*G).vertices[i].data.others);
            else printf("����һ�ڽӶ��㣡\n");
            break;}
        case 7:
            {printf("��������Ҫ����Ķ���ֵ��");
            scanf("%d %s", &value.key, value.others);
            flag1 = InsertVex(G, value);
            if (flag1 == OK)
                printf("���� %d %s �ѳɹ�����ͼ��\n", value.key, value.others);
            else if (flag == ERROR)
                printf("����ʧ�ܣ�\n");
            break;}
        case 8:
            {printf("��������Ҫɾ���Ķ���Ĺؼ��֣�");
            scanf("%d", &e);
            flag1 = DeleteVex(G, e);
            if (flag1 == OK)
                printf("�ؼ���Ϊ%d�Ķ����Ѵ�ͼ��ɾ��\n", e);
            else if (flag1 == ERROR)
                printf("ɾ��ʧ�ܣ�\n");
            break;}
        case 9:
            {printf("��������Ҫ����Ļ���");
            scanf("%d %d", &e, &j);
            flag1 = InsertArc(G, e, j);
            if (flag1 == OK)
                printf("����ɹ���\n");
            else if (flag1 == ERROR)
                printf("����ʧ�ܣ�\n");
            break;}
        case 10:
            printf("��������Ҫɾ���Ļ���");
            scanf("%d %d", &e, &j);
            flag1 = DeleteArc(G, e, j);
            if (flag1 == OK)
                printf("ɾ���ɹ���\n");
            else if (flag1 == ERROR)
                printf("ɾ��ʧ�ܣ�\n");
            break;
        case 11:
            for (int m = 0;m < 20;m++)
                f[m] = 0;
            printf("�����������������\n");
            DFSTraverse(G, visit);
            printf("\n");
            break;
        case 12:
            for (int m = 0;m < 20;m++)
                a[m] = 0;
            printf("�����������������\n");
            BFSTraverse(G, visit);
            printf("\n");
            break;
        case 13:
            {printf("�������ļ����ƣ�");
            scanf("%s", FileName);
            flag1 = SaveGraph(*G, FileName);
            if (flag1 == ERROR)
                printf("�ļ���ʧ�ܣ�\n");
            else if (flag1 == OK)
                printf("ͼ�������Ѿ����Ƶ�����Ϊ %s ���ļ��С�\n", FileName);
            break;}
        case 14:
            {printf("�������ļ����ƣ�");
            scanf("%s", FileName);
            flag1 = LoadGraph(G, FileName);
            if (flag1 == ERROR)
                printf("�ļ���ʧ�ܣ�\n");
            else if (flag1 == OK)
                printf("����Ϊ %s ���ļ��е������Ѹ��Ƶ�ͼ�С�\n", FileName);
            else if (flag1 == INFEASIBLE)
                printf("���ܶ��Ѵ��ڵ�ͼ���н��ж��ļ�������\n");
            else if (flag1 == OVERFLOW)
                printf("�����\n");
            break;}
        case 15:
                printf("�����붥��v�Ͷ���w�Ĺؼ���\n");
                int v15,w15;
                scanf("%d %d",&v15,&w15);
                int feedback = ShortestPathLength(*G,v15,w15);
                if(feedback==-1)
                {
                    printf("��ͼ�򶥵㲻���ڻ�δ��ʼ��\n");
                    break;
                }
                if(feedback != -1)
                {
                    printf("���·��Ϊ %d \n",feedback);

                } else{
                    printf("����ʧ��\n");
                }
                break;
        case 16:
        {
            flag1=ConnectedComponentsNums( *G);
            if(flag1!=0)
             printf("ͼ����ͨ����Ϊ��%d\n",flag1);
             break;
        }
        case 17:
        {
            printf("��������Ҫ���ҵĶ���ͳ��ȣ�\n");
            int s,k;
            scanf("%d %d",&s,&k);
            printf("�붥��%d����С��%d�Ķ����У�",s,k);
            VerticesSetLessThanK( *G, s, k);
            printf("\n");
            break;
        }
        case 19:{
            printf("��������Ҫ������ͼ�����\n");
            scanf("%d",&i);
            flag1=change(&G,i,&Lists);
            if(flag1==OK)
            printf("�ѳɹ��л�\n");
            else
            printf("δ�ɹ��л�.\n");
            break;
        }
        case 18:{
            char name[30];
            printf("������ͼ�����֣�");
            scanf("%s",name);
            
            j=AddList(&Lists,name);
            if (j==OK) 
            {
                printf("�ɹ����ͼ\n");
            }
            else printf("ʧ��\n");
            break;
        }
        case 0:
            break;
        }//end of switch
        printf("\n");
    }//end of while
    printf("��ӭ�´���ʹ�ñ�ϵͳ��\n");
}//end of main()