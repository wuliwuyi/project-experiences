#include "def.h"
#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

typedef int status;
typedef int ElemType; //数据元素类型定义

#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
typedef int ElemType;
typedef struct{  //顺序表（顺序结构）的定义
	      ElemType * elem;
	      int length;
	      int listsize;
         }SqList;

typedef struct{  //线��表的管理表定义
     struct { char name[30];
     		  SqList L;	
      } elem[10];
      int length;
      int listsize;
 }LISTS;
status InitList(SqList *K)// 线��表L不存�?，构造一�?空的线��表，返回OK，否则返回INFEASIBLE�?
{ 
    if((*K).elem==NULL)//不存�?
    {
        (*K).elem=(ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));//分配空间
        if(!(*K).elem)
        exit(OVERFLOW);//�?成功，����?
        (*K).length=0;
        (*K).listsize=LIST_INIT_SIZE;
        return OK;
    }
    if((*K).elem!=NULL)//存在
    return INFEASIBLE;
}

status DestroyList(SqList *L)
// 如果线��表L存在，销毁线性表L，释放数�?元素的空间，返回OK，否则返回INFEASIBLE�?
{
    if((*L).elem)//存在
    {
        free((*L).elem);//释放空间
        (*L).elem=NULL;
        (*L).length=0;
        (*L).listsize=0;
        return OK;
    }
    return INFEASIBLE;
}

status ClearList(SqList *L)
// 如果线��表L存在，删除线性表L�?的所有元素，返回OK，否则返回INFEASIBLE�?
{
    if((*L).elem)//判断存在
    {
        for(int i=0;i<=(*L).length;i++)//每个元素都等�?0
        {
            (*L).elem[i]=0;
        }
        (*L).length=0;
        return OK;
    }
    else
    return INFEASIBLE;
}

status ListEmpty(SqList L)
// 如果线��表L存在，判�?线��表L�?否为空，空就返回TRUE，否则返回FALSE；�?�果线��表L不存�?，返回INFEASIBLE�?
{
    if(L.elem)
    {
        if(L.length==0)
        return OK;
        else
        return ERROR;
    }
    else
    return INFEASIBLE;
}

status ListLength(SqList L)
// 如果线��表L存在，返回线性表L的长度，否则返回INFEASIBLE�?
{
    if(L.elem)
    {
        return L.length;
    }
    else
    return INFEASIBLE;
}

status GetElem(SqList L,int i,ElemType *e)
// 如果线��表L存在，获取线性表L的�?�i�?元素，保存在e�?，返回OK；�?�果i不合法，返回ERROR；�?�果线��表L不存�?，返回INFEASIBLE�?
{
    if(L.elem)
    {
        if(i<1||i>L.length)//当i不�?�合要求时，错�??
        return 0;
        (*e)=L.elem[i-1];//赋��给e
        return OK;
    }
    else
    return INFEASIBLE;
}

int LocateElem(SqList L,ElemType e)
// 如果线��表L存在，查找元素e在线性表L�?的位�?序号并返回�?�序号；如果e不存�?，返�?0；当线��表L不存在时，返回INFEASIBLE（即-1）��?
{
    if(L.elem)
    {
        ElemType *p;
        int i=0;
        p=L.elem;
        for(;i<=L.length;i++)
        {
            if(*(p+i)==e)//�?p+i指向元素=e，返回位�?
            return i+1;
        }
        return ERROR;
    }
    return INFEASIBLE;
}

status PriorElem(SqList L,ElemType e,ElemType *pre)
// 如果线��表L存在，获取线性表L�?元素e的前驱，保存在pre�?，返回OK；�?�果没有前驱，返回ERROR；�?�果线��表L不存�?，返回INFEASIBLE�?
{
  if(L.elem)
    {
        int i=0;
        ElemType *p;
        p=L.elem;
        for(;i<L.length;i++)
        {
            if(*(p+i)==e) //p+i指向元素=e
            {if(i==0)//�?�?丢��?元素，无前驱
            return ERROR;
            else
            (*pre)=*(p+i-1);
            return OK;
            }
        }
        return ERROR;
    }
    return INFEASIBLE;
}

status NextElem(SqList L,ElemType e,ElemType *next)
// 如果线��表L存在，获取线性表L元素e的后继，保存在next�?，返回OK；�?�果没有后继，返回ERROR；�?�果线��表L不存�?，返回INFEASIBLE�?
{
    if(L.elem)
    {
        int i;
        ElemType *p;
        p=L.elem;
        for(i=0;i<L.length-1;i++)//不会遍历到最后一�?
        {
            if(*(p+i)==e)//p+i指向元素=e
            {
                (*next)=*(p+i+1);
                return OK;
            }
        }
        return ERROR;
    }
    return INFEASIBLE;
}

status ListInsert(SqList *L,int i,ElemType e)
// 如果线��表L存在，将元素e插入到线性表L的�?�i�?元素之前，返回OK；当插入位置不�?�确时，返回ERROR；�?�果线��表L不存�?，返回INFEASIBLE�?
{
    if((*L).elem)
    {
        if(i<1||i>(*L).length+1)//插入位置不�?�确
        return 0;
        if((*L).length>=(*L).listsize)
        {
            ElemType *newbase=(ElemType*)malloc(LIST_INIT_SIZE *sizeof(ElemType));//分配空间
            if(!newbase)
            exit -2;//空间分配失败，错�?
            (*L).listsize+=LISTINCREMENT;
        }
        ElemType *q=&((*L).elem[i-1]);//第i�?元素位置赋��给q
        for(int *p=&((*L).elem[(*L).length-1]);p>=q;--p)
        {  
            *(p+1)=*p;//每个徢�后移丢��?
        }
        *q=e;//加入新��到第i�?元素
        ++(*L).length;//长度+1
        return OK;
    }
    return INFEASIBLE;
}

status ListDelete(SqList *L,int i,ElemType *e)
// 如果线��表L存在，删除线性表L的�?�i�?元素，并保存在e�?，返回OK；当删除位置不�?�确时，返回ERROR；�?�果线��表L不存�?，返回INFEASIBLE�?
{
    if((*L).elem)
    {
        if(i<1||i>(*L).length)//位置不�?�确
        return 0;
        ElemType *p;
        p=(*L).elem;
        (*e)=*(p+i-1);//赋��给e
        for(int j=i-1;j<(*L).length-1;j++)
        {
            *(p+j)=*(p+j+1);//徢�前移
        }
        (*L).length-=1;
        return OK;
    }
    return INFEASIBLE;
}

status ListTraverse(SqList L)
// 如果线��表L存在，依次显示线性表�?的元素，每个元素间空丢�格，返回OK；�?�果线��表L不存�?，返回INFEASIBLE�?
{
    if(L.elem)
    {int *p;
        for(p=L.elem;p<=&L.elem[L.length-1];p++)
        {
            printf("%d ",*p);
        }
        return OK;
    }
    return INFEASIBLE;
}

status  SaveList(SqList L,char FileName[])
// 如果线��表L存在，将线��表L的的元素写到FileName文件�?，返回OK，否则返回INFEASIBLE�?
{
    if(L.elem)
    {
       FILE *fp;
       if((fp=fopen(FileName,"w"))==NULL)
       {
           return 0;
       } 
        for(int i=0;i<L.length;i++)
        {
            fprintf(fp,"%d ",L.elem[i]);
        }
        fclose(fp);
        return OK;
    }
    return INFEASIBLE;
}

status  LoadList(SqList *L,char FileName[])
// 如果线��表L不存�?，将FileName文件�?的数�?读入到线性表L�?，返回OK，否则返回INFEASIBLE�?
{
    if((*L).elem==NULL)
    {

        FILE *fp;

        if((fp=fopen(FileName,"r"))==NULL)
        return 0;
        (*L).elem=(ElemType*)malloc((*L).listsize*sizeof(ElemType));
        (*L).length=1;
        (*L).listsize=100;
        for(int i=0;i<(*L).length;i++,(*L).length++)
		{
            fscanf(fp,"%d",&(*L).elem[i]);//读入
            if((*L).elem[i]==0)
            break;
        }
        (*L).length-=1;
        fclose(fp);
        return OK;
    }
    return INFEASIBLE;
}

status AddList(LISTS *Lists,char ListName[])
// �?霢�要在Lists�?增加丢��?名称为ListName的空线��表，线性表数据又后台测试程序插入��?
{
    strcpy((*Lists).elem[(*Lists).length].name,ListName);
    (*Lists).elem[(*Lists).length].L.elem=(ElemType*)malloc(LIST_INIT_SIZE*sizeof(ElemType));
    (*Lists).elem[(*Lists).length].L.length=0;
    (*Lists).elem[(*Lists).length].L.listsize=LIST_INIT_SIZE;
    (*Lists).length+=1;
    return OK;
}

status RemoveList(LISTS *Lists,char ListName[])
// Lists�?删除丢��?名称为ListName的线性表
{
    int i,*q,m;
    m=(*Lists).length;
    for(i=0;i<(*Lists).length;i++)
    {
        for(i=0;i<(*Lists).length;i++)
    {
        for(m=0;ListName[m]!='\0';m++)//对照名称
        {
            if((*Lists).elem[i].name[m]==ListName[m])
        {
            continue;
        }
        else
        break;
        }
        if(ListName[m]=='\0')//若名称完全一样，此时对应字�?�空
        {
            for(int j=i;j<((*Lists).length-1);j++)
            (*Lists).elem[j]=(*Lists).elem[j+1];
            (*Lists).length--;
            return OK;
        }
    }
    return ERROR;
    }
}

int LocateList(LISTS Lists,char ListName[])
// 在Lists�?查找丢��?名称为ListName的线性表，成功返回��辑序号，否则返�?0
{
    int i,m;
    for(i=0;i<Lists.length;i++)
    {
        for(m=0;ListName[m]!='\0';m++)
        {
            if(Lists.elem[i].name[m]==ListName[m])//对照名称
        {
            continue;
        }
        else
        break;
        }
        if(ListName[m]=='\0')
        return i+1;
    }
    return 0;
}

int maxSubArray(SqList *L)
{
    int numsSize=ListLength(*L);
    int i = 0, sum = 0;
    int max = (*L).elem[0];
    for(i = 0; i < numsSize; i++) {
        sum += (*L).elem[i];
        if(max < sum)//如果加下丢��?数后和变�?
            max = sum;
        if(sum < 0)//�?丢��?就是负数
            sum = 0;
    }
    return max;
}

int subarraySum(SqList *L, int e)
{
    int numsSize=ListLength(*L);
    int sum=0;
    int key=0;
    for(int i=0;i<numsSize;i++)
    {
        sum=(*L).elem[i];
        if(sum==e)//丢��?数��本�?等于给定�?
        {
            key++;
        }
        for(int j=i+1;j<numsSize;j++)
        {
            sum+=(*L).elem[j];
            if(sum==e)
            {
                key++;
            }
        }
    }
    return key;
}

status SortList(SqList *L)  //升序排列
{
    if(!(*L).length)//无数�?
    {
        return ERROR;
    }
    int i, j, k;
    int t;
    for(i = 0; i <(*L).length-1; ++i)
    {
        k = i;//k随i徢�后找
        for(j = i+1; j < (*L).length; ++j)
            if((*L).elem[j] < (*L).elem[k])//i和i后面的比�?
                k = j;//较小的位�?给k
        if(k != i)//交换
        {
            t = (*L).elem[i];
            (*L).elem[i] = (*L).elem[k];
            (*L).elem[k] = t;
        }
    }
    for(i = 0; i < (*L).length; ++i)
       return (*L).elem[i] ;
}

status change(SqList **K,int i,LISTS *Lists)
{
    if(i<=(*Lists).length)
    {(*K)=&((*Lists).elem[i-1].L);
    return OK;}
    else
    return ERROR;
}



int main()
{
    FILE *fp;
    LISTS Lists;
    Lists.length=0;
    SqList *K;
    
    int n, j,con=1,i,e;
  while(con)
{   if(con==1){
    printf("      Menu for Linear Table On Sequence Structure \n");
	printf("-------------------------------------------------\n");
	printf("    	  1. InitList��ʼ�����Ա�       7. LocateElem����Ԫ��\n");
	printf("    	  2. DestroyList�������Ա�      8. PriorElem��ȡǰ��Ԫ��\n");
	printf("    	  3. ClearList������Ա�        9. NextElem��ȡ���Ԫ�� \n");
	printf("    	  4. ListEmpty�п����Ա�        10.ListInsert����Ԫ��\n");
	printf("    	  5. ListLength���Ա���       11.ListDeleteɾ��Ԫ��\n");
	printf("    	  6. GetElem��ȡԪ��            12.ListTrabverse����Ԫ��\n");
	printf("    	  13.SaveListд���ļ�           14.LoadList�����ļ�\n");
	printf("    	  15.AddList�������Ա�          16.RemoveList�Ƴ����Ա�\n");
	printf("    	  17.LocateList�������Ա�       18.MaxSubArray��������������\n");
    printf("    	  19.SubArrayNum��ΪK��������   20.SortList���Ա����� \n");
    printf("    	  21.change�л�                 0. Exit\n");
	printf("-------------------------------------------------\n");
	printf("    ��ѡ����Ĳ���[0~21]:");

    scanf("%d",&n);
    switch(n)
    {
        case 1:{
            j=InitList(K);
            if (j==INFEASIBLE) 
            {
                printf("���Ա��Ѵ���\n");
                break;
            }
            if (j==OK) 
            {
                printf("����ɳ�ʼ��\n");
            }
            else
            printf("δ��ɳ�ʼ��\n");
            break;
                }
        
        case 2:{
            j=DestroyList(K);
            if (j==INFEASIBLE) 
            {
                printf("���Ա�����\n");
                break;
            }
            if (j==OK) 
            {
                printf("���Ա�������\n");
            }
            else printf("δ�ɹ�����\n");
            break;
        }
    
        case 3:{
            j=ClearList(K);
            if (j==INFEASIBLE) 
            {
                printf("���Ա�����\n");
                break;
            }
            if (j==OK) 
            {
                printf("��������Ա�\n");
            }
            else printf("δ�ɹ����\n");
            break;
        }
    
        case 4:{
            j=ListEmpty(*K);
            if (j==INFEASIBLE) 
            {
                printf("���Ա�����\n");
                break;
            }
            if (j==OK) 
            {
                printf("���Ա�Ϊ��\n");
            }
            else printf("���Ա�Ϊ��\n");
            break;
        }
    
        case 5:{
            j=ListLength(*K);
            if (j==INFEASIBLE) 
            {
                printf("���Ա�����\n");
            }
            else
            {printf("���Ա���Ϊ%d\n",j);}
            break;
        }
    
        case 6:{
            printf("�����룺λ��\n");
            scanf("%d",&i);
            j=GetElem(*K,i,&e);
            if (j==INFEASIBLE) 
            {
                printf("���Ա�����\n");
                break;
            }
            if (j==OK) 
            {
                printf("λ��%dԪ��Ϊ%d\n",i,e);
            }
            else printf("λ�ô���\n");
            break;
        }

        case 7:{
            printf("��������Ҫ���ҵ�Ԫ��\n");
            scanf("%d",&e);
            j=LocateElem(*K,e); 
            if (j==INFEASIBLE) 
            {
                printf("���Ա�����\n");
                break;
            }
            else if(j==ERROR) 
            {
                printf("��Ԫ�ز�����\n");
            }
            else 
            {
                printf("Ԫ��%d��λ��%d\n",e,j);
            }
            break;
        }

        case 8:{
            int pre;
            printf("��������Ҫ�ҵ�ǰ����Ԫ��\n");
            scanf("%d",&e);
            j=PriorElem(*K,e,&pre);
            if (j==INFEASIBLE) 
            {
                printf("���Ա�����\n");
                break;
            }
            if (j==OK) 
            {
                printf("ǰ��Ԫ��Ϊ%d\n",pre);
            }
            else printf("û��ǰ��\n");
            break;
        }

        case 9:{
            int next;
            printf("��������Ҫ�ҵ���̵�Ԫ��\n");
            scanf("%d",&e);
            j=NextElem(*K,e,&next);
            if (j==INFEASIBLE) 
            {
                printf("���Ա�����\n");
                break;
            }
            if (j==OK) 
            {
                printf("���Ԫ��Ϊ%d\n",next);
            }
            else printf("�޺��Ԫ��\n");
            break;
        }

        case 10:{
            printf("�����룺����λ�� ����Ԫ��");
            scanf("%d %d",&i,&e);
            j=ListInsert(K,i,e);
            printf("%s\n", j==OK? "�ѳɹ�����\n" : j==ERROR? "δ�ɹ����루λ�ô���\n" : "���Ա�����\n");
            break;
        }

        case 11:{
            printf("��������Ҫɾ����Ԫ��λ��\n");
            scanf("%d",&i);
            j=ListDelete(K,i,&e);
            if (j==INFEASIBLE) 
            {
                printf("���Ա�����\n");
                break;
            }
            if (j==OK) 
            {
                printf("��ɾ��λ��Ϊ%d��Ԫ��%d\n",i,e);
            }
            else printf("δ�ɹ�ɾ����λ�ô���\n");
            break;
        }

        case 12:{
            j=ListTraverse(*K); 
            if (j==INFEASIBLE) 
            {
                printf("���Ա�����\n");
                break;
            }
            if (j==OK) 
            {
                printf("\n�������\n");
            }
            else printf("δ�ɹ�����\n");
            break;
        }
    
        case 13:{
            j=SaveList(*K,"C:\\Users\\ffsyd\\Desktop\\vscode\\ffsyd.txt");
        if (j==INFEASIBLE) 
            {
                printf("���Ա�����\n");
                break;
            }
            if (j==OK) 
            {
                printf("�ѳɹ�д���ļ�\n");
            }
            else printf("δ�ɹ�д���ļ�\n");
            break;
        }
    
        case 14:{
            j=LoadList(K,"C:\\Users\\ffsyd\\Desktop\\vscode\\ffsyd.txt");
        if (j==INFEASIBLE) 
            {
                printf("���Ա��Ѵ���\n");
                break;
            }
            if (j==OK) 
            {
                printf("�ѳɹ������ļ������Ա�\n");
            }
            else printf("δ�ɹ������ļ�\n");
            break;

        }

        case 15:{
            char name[30];
            printf("�����������Ա������\n");
            scanf("%s",name);
            j=AddList(&Lists,name);
            if (j==OK) 
            {
                printf("����������Ա�\n");
            }
            else printf("δ�ɹ����\n");
            break;
        }

        case 16:{
            char name[30];
            printf("��������ɾ�����Ա������\n");
            scanf("%s",name);
            j=RemoveList(&Lists,name);
            if (j==OK) 
            {
                printf("�ѳɹ�ɾ��\n");
            }
            else printf("δ�ҵ������Ա�\n");
            break;
        }

        case 17:{
            char name[30];
            printf("����������ҵ����Ա������\n");
            scanf("%s",name);
            j=LocateList(Lists,name);
            if (j==INFEASIBLE) 
            {
                printf("���Ա�����");
                break;
            }
            else if(j==ERROR) 
            {
                printf("δ�ҵ������Ա�\n");
            }
            else 
            {
                printf("�����Ա���߼�λ��Ϊ%d\n",j);
            }
            break;
        }
    
        case 18:{
            j=maxSubArray(K);
            printf("����������Ϊ%d\n",j);
            break;
        }
    
        case 19:{
            printf("������ϣ���ĺ�ֵ��");
            scanf("%d",&e);
            j=subarraySum(K,e);
            printf("��Ϊ%d�����������Ϊ%d\nOK\n",e,j);
            break;
        }

         case 20:{
        	if(SortList(K)!=ERROR)	
			printf("���������\n");	
			else printf("δ�������\n");
			break;}

        case 21:{
            printf("������ϣ�����Ƶ����Ա����\n");
            scanf("%d",&i);
            j=change(&K,i,&Lists);
            if(j==OK)
            printf("�ɹ��л�\n");
            else
            printf("δ�ɹ��л�");
            break;
        }
        case 0:
        break;
    }
}
else 
{printf("ERROR");break;}
}
    return 0;
}