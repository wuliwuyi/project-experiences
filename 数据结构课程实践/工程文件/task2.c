#include<windows.h>
#include "def2.h"
#include <stdio.h>
#include <string.h>
status InitList(LinkList *L)
// 线性表L不存在，构造一个空的线性表，返回OK，否则返回INFEASIBLE
{
    if(!(*L))//判断线性表存在
    {
        (*L)=(LNode *)malloc(sizeof(LIST_INIT_SIZE));//分配空间
        (*L)->next=NULL;
        return OK;
    }
    return INFEASIBLE;
}

status DestroyList(LinkList *L)
// 如果线性表L存在，销毁线性表L，释放数据元素的空间，返回OK，否则返回INFEASIBLE
{
    if((*L))//判断线性表存在
    {
        LNode *p;
        while((*L))
        {
            p=(*L);
            (*L)=(*L)->next;//向后遍历
            free(p);//释放空间
        }
        return OK;
    }
    return INFEASIBLE;
}

status ClearList(LinkList *L)
// 如果线性表L存在，删除线性表L中的所有元素，返回OK，否则返回INFEASIBLE
{
    if((*L))
    {
        LNode *p;
        while((*L)->next)
        {            
            p=(*L)->next;//从第一个元素开始
            (*L)=(*L)->next;//向后遍历
            free(p);//释放空间
        }
        (*L)->next=NULL;//挂空指针
        return OK;
    }
    return INFEASIBLE;
}

status ListEmpty(LinkList L)
// 如果线性表L存在，判断线性表L是否为空，空就返回TRUE，否则返回FALSE；如果线性表L不存在，返回INFEASIBLE
{
    if(L)
    {
        if(L->next)
        return FALSE;
        return TRUE;
    }
    return INFEASIBLE;
}

int ListLength(LinkList L)
// 如果线性表L存在，返回线性表L的长度，否则返回INFEASIBLE
{
    if(L)//判断存在
    {
        int i;
        LNode *q;
        q=L;
        for(i=0;q->next!=NULL;i++)
        {
            q=q->next;            
        }
        return i;
    }
    return INFEASIBLE;
}

status GetElem(LinkList L,int i,ElemType *e)
// 如果线性表L存在，获取线性表L的第i个元素，保存在e中，返回OK；如果i不合法，返回ERROR；如果线性表L不存在，返回INFEASIBLE
{
    if(L)//判断存在
    {        
        LNode *q;
        q=L;
        int m;
        for(m=0;q->next!=NULL;m++)
        {
            q=q->next;//获取长度            
        }
        int ListLength=m;
        if(i<1||i>ListLength)//判断位置是否正确，是否超过长度或小于1
        return ERROR;
        q=L;
        for(int j=0;j<i;j++)//找到位置
        {
            q=q->next;
        }
        (*e)=q->data;//赋�?
        return OK;

    }
    return INFEASIBLE;
}

status LocateElem(LinkList L,ElemType e)
// 如果线性表L存在，查找元素e在线性表L中的位置序号；如果e不存在，返回ERROR；当线性表L不存在时，返回INFEASIBLE
{
    if(L)
    {
        int i;
        LNode *q;
        q=L;
        for(i=0;q!=NULL;i++)
        {
            if(q->data==e)
            return i;
            q=q->next;
        }
        return ERROR;
        
    }
    return INFEASIBLE;
}

status PriorElem(LinkList L,ElemType e,ElemType *pre)
// 如果线性表L存在，获取线性表L中元素e的前驱，保存在pre中，返回OK；如果没有前驱，返回ERROR；如果线性表L不存在，返回INFEASIBLE
{
    if(L)
    {
        int i;
        LNode *q,*pr;
        q=L->next;
        pr=L;
        for(i=0;pr->next!=NULL;i++)
        {
            if(q->data==e&&i!=0)//找到e的位置，且位置不为1
            {(*pre)=pr->data;//赋值
            return OK;}
            pr=q;
            q=q->next;//向后遍历
        }
        return ERROR;
    }
    return INFEASIBLE;
}

status NextElem(LinkList L,ElemType e,ElemType *next)
// 如果线性表L存在，获取线性表L元素e的后继，保存在next中，返回OK；如果没有后继，返回ERROR；如果线性表L不存在，返回INFEASIBLE
{
    if(L)
    {
        LNode *q=L;
        int i;
        for(i=0;q->next!=NULL;i++)//不会遍历到最后一个
        {
            if(q->data==e)//找到e的位置
            {(*next)=q->next->data;
            return OK;}
            q=q->next;//向后遍历
        }
        return ERROR;
    }
    return INFEASIBLE;
}

status ListInsert(LinkList *L,int i,ElemType e)
// 如果线性表L存在，将元素e插入到线性表L的第i个元素之前，返回OK；当插入位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE
{
    if(L)
    {
        LNode *q,*p;
        q=(*L);
        int m;
        for(m=1;q->next!=NULL;m++)//m为线性表长度
        {
            q=q->next;            
        }
        if(i<1||i>m)//判断位置是否正确
        return ERROR;
        int n;
        q=(*L);
        for(n=0;n<i-1;n++)//遍历到插入的位置
        {
            q=q->next;
        }
        p=(LNode *)malloc(sizeof(LISTINCREMENT));//分配空间
        p->data=e;
        p->next=q->next;//新插入p的next连到插入位置前一个原指向的next
        q->next=p;//插入位置前的next指向q
        return OK;
    }
    return INFEASIBLE;
}

status ListDelete(LinkList *L,int i,ElemType *e)
// 如果线性表L存在，删除线性表L的第i个元素，并保存在e中，返回OK；当删除位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE
{
        if((*L))
    {
        LNode *q,*p;
        q=(*L);
        int m;
        for(m=0;q->next!=NULL;m++)//m为长度
        {
            q=q->next;            
        }
        if(i<1||i>m)//判断位置是否正确
        return ERROR;
        int n;
        q=(*L);
        for(n=0;n<i-1;n++)
        {
            q=q->next;//找到删除的位置
        }
        p=q->next;
        (*e)=p->data;//赋值
        q->next=q->next->next;//直接跳过删除的元素，连到下一个next
        free(p);//释放空间
        return OK;
    }
    return INFEASIBLE;
}

status ListTraverse(LinkList L)
// 如果线性表L存在，依次显示线性表中的元素，每个元素间空一格，返回OK；如果线性表L不存在，返回INFEASIBLE
{
    if(L)//判断存在
    {
        LNode *q;
        q=L->next;
        if(L->next==NULL)//没有元素
        {
        return OK;
        }
        else{
        for(;q->next!=NULL;q=q->next)//只遍历到倒数第二位
        printf("%d ",q->data);
        printf("%d",q->data);//最后一个后面不加空格
        return OK;}
    }
    return INFEASIBLE;
}

status SaveList(LinkList L,char FileName[])
// 如果线性表L存在，将线性表L的的元素写到FileName文件中，返回OK，否则返回INFEASIBLE
{
       if(L)
    {
       FILE *fp;
       if((fp=fopen(FileName,"w"))==NULL)
       {
           return ERROR;
       } 
       LNode *q=L->next;
        for(;q;q=q->next)
        {
            fprintf(fp,"%d ",q->data);
        }
        fclose(fp);       
        return OK;
    }
    return INFEASIBLE;
}

status LoadList(LinkList *L,char FileName[])
// 如果线性表L不存在，将FileName文件中的数据读入到线性表L中，返回OK，否则返回INFEASIBLE
{
     if(!(*L))
    {
        FILE *fp;
        if((fp=fopen(FileName,"r"))==NULL)
        return 0;
        (*L)=(LNode *)malloc(sizeof(LIST_INIT_SIZE));
        LNode *q,*s;
        q=(LNode *)malloc(sizeof(LISTINCREMENT));
        s=(LNode *)malloc(sizeof(LISTINCREMENT));
        (*L)->next=q;
        
        while((fscanf(fp,"%d",&q->data))!=EOF)
		{
            s=q;
            q=(LNode *)malloc(sizeof(LISTINCREMENT));
            s->next=q;      
        }
        s->next=NULL;
        fclose(fp);
        return OK;
    }
    return INFEASIBLE;
}

status reverseList(LinkList L)//翻转链表，成功返回OK，线性表不存在返回INFEASIBLE
{
    if(L){//判断存在
    int length=(ListLength(L));//长度
    int a[100], i=0;
    LNode *q;
        q=L->next;
    for(;i<length;i++)
    {
        a[i]=q->data;//先读一遍，存入数组a中
        q=q->next;
    }
    q=L->next;
    for(i=i-1;i>=0;i--)
    {
        q->data=a[i];//再将a倒着存入
        q=q->next;
    }
    return OK;
    }
    else
    return INFEASIBLE;
}

status RemoveNthFromEnd(LinkList *L,int i)
{
         if((*L))
    {
        LNode *q,*p;
        q=(*L);
        int m=(ListLength((*L)));
        if(i<1||i>m)//判断位置是否正确
        return ERROR;
        int n=m-i;
        q=(*L);
        for(;n>0;n--)
        {
            q=q->next;//找到删除的位�?,此时q指向需删除数的前一个数
        }
        p=q->next;
      
        q->next=q->next->next;//直接跳过删除的元素，连到下一个next
        free(p);//释放空间
        return OK;
    }
    return INFEASIBLE;
}

status sortList(LinkList *L)
{    if((*L)!=NULL)
	{int i , num;//count记录链表结点的个数，num进行内层循环数
	LinkList p, q,tail;
	p = *L;
    int count=ListLength(*L);
	for(i = 0; i < count - 1; i++)//外层循环
	{
		num = count - i - 1;//记录内层循环需要的次数
		q = (*L)->next;//令q指向第一个结点
		p = q->next;//令p指向后一个结点
		tail = (*L);//让tail始终指向q前一个结点，方便交换
		while(num--)//内层循环 
		{
			if(q->data > p->data)//如果该结点的值大于后一个结点，则交换
			{
				q->next = p->next;
				p->next = q;
				tail->next = p;
			}
			tail = tail->next;
			q = tail->next;
			p = q->next;
		 } 
	} return OK;
    }
    return INFEASIBLE;
}

status change(LinkList **L,int i,LISTS *Lists)//将链表指针指向需要操作的链表
{
    if(i<=(*Lists).length)
    {(*L)=&((*Lists).elem[i-1].L);
    return OK;}
    else
    return ERROR;
}

status AddList(LISTS *Lists,char ListName[])
// 在Lists中增加一个名称为ListName的空线性表
{
    strcpy((*Lists).elem[(*Lists).length].name,ListName);
    (*Lists).elem[(*Lists).length].L=(LNode *)malloc(sizeof(LIST_INIT_SIZE));//分配空间
    (*Lists).elem[(*Lists).length].L->next=NULL;
    (*Lists).length+=1;
    return OK;
    
}

status RemoveList(LISTS *Lists,char ListName[])
// Lists中删除一个名称为ListName的线性表
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
        if(ListName[m]=='\0')//若名称完全一样，此时对应字符为空
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

int main()
{
  
    FILE *fp;
    LISTS Lists;
    Lists.length=0;
    LinkList *L;
    
	int i,j,con=1,n,e;
  while(con)
{   if(con==1){
    printf(
       "初始化----1\n"
       "销毁------2\n"
       "清空------3\n"
       "判空------4\n"
       "长度------5\n"
       "获取元素--6\n"
       "查找元素--7\n"
       "获取前驱--8\n"
       "获取后继--9\n"
       "插入-----10\n"
       "删除-----11\n"
       "遍历-----12\n"
       "写入文件-13\n"
       "写出文件-14\n"
       "翻转链表-15\n"
       "倒数删除-16\n"
       "排序-----17\n"
       "切换-----18\n"
       "添加线性表---19\n"
       "移除线性表---20\n"


       );
    scanf("%d",&n);
    switch(n)
    {
        case 1:{
            j = InitList(L);
            if (j==INFEASIBLE) 
            {
                printf("线性表已存在\n");
                break;
            }
            if (j==OK) 
            {
                printf("已完成初始化\n");
            }
            else
            printf("未成功初始化\n");
            break;
                }
        
        case 2:{
            j=DestroyList(L);
            if (j==INFEASIBLE) 
            {
                printf("线性表不存在\n");
                break;
            }
            if (j==OK) 
            {
                printf("已成功销毁\n");
            }
            else printf("未成功销毁\n");
            break;
        }
    
        case 3:{
            j=ClearList(L);
            if (j==INFEASIBLE) 
            {
                printf("线性表不存在\n");
                break;
            }
            if (j==OK) 
            {
                printf("已清空线性表\n");
            }
            else printf("未成功清空\n");
            break;
        }
    
        case 4:{
            j=ListEmpty(*L);
            if (j==INFEASIBLE) 
            {
                printf("线性表不存在\n");
                break;
            }
            if (j==OK) 
            {
                printf("线性表为空\n");
            }
            else printf("线性表不为空\n");
            break;
        }
    
        case 5:{
            j=ListLength(*L);
            if (j==INFEASIBLE) 
            {
                printf("线性表不存在\n");
            }
            else
            {printf("长度为%d\n",j);}
            break;
        }
    
        case 6:{
            printf("请输入:位置\n");
            scanf("%d",&i);
            j=GetElem(*L,i,&e);
            if (j==INFEASIBLE) 
            {
                printf("线性表不存在\n");
                break;
            }
            if (j==OK) 
            {
                printf("位置%d元素为%d\n",i,e);
            }
            else printf("位置错误\n");
            break;
        }

        case 7:{
            printf("请输入要查找的元素\n");
            scanf("%d",&e);
            j=LocateElem(*L,e); 
            if (j==INFEASIBLE) 
            {
                printf("线性表不存在\n");
                break;
            }
            else if(j==ERROR) 
            {
                printf("该元素不存在\n");
            }
            else 
            {
                printf("元素%d在位置%d\n",e,j);
            }
            break;
        }

        case 8:{
            int pre;
            printf("请输入想要找到前驱的元素\n");
            scanf("%d",&e);
            j=PriorElem(*L,e,&pre);
            if (j==INFEASIBLE) 
            {
                printf("线性表不存在\n");
                break;
            }
            if (j==OK) 
            {
                printf("前驱元素为%d\n",pre);
            }
            else printf("没有前驱\n");
            break;
        }

        case 9:{
            int next;
            printf("请输入想要找到后继的元素\n");
            scanf("%d",&e);
            j=NextElem(*L,e,&next);
            if (j==INFEASIBLE) 
            {
                printf("线性表不存在\n");
                break;
            }
            if (j==OK) 
            {
                printf("后继元素为%d\n",next);
            }
            else printf("无后继元素\n");
            break;
        }

        case 10:{
            printf("请输入：插入位置 元素值\n");
            scanf("%d %d",&i,&e);
            j=ListInsert(L,i,e);
            printf("%s\n", j==OK? "已成功插元素" : j==ERROR? "未成功插入（位置错误）" : "线性表不存在");
            break;
        }

        case 11:{
            printf("请输入需要删除的元素位置\n");
            scanf("%d",&i);
            j=ListDelete(L,i,&e);
            if (j==INFEASIBLE) 
            {
                printf("线性表不存在\n");
                break;
            }
            if (j==OK) 
            {
                printf("已删除位置为%d的元为%d\n",i,e);
            }
            else printf("未成功删除（位置错误）\n");
            break;
        }

        case 12:{
            j=ListTraverse(*L); 
            if (j==INFEASIBLE) 
            {
                printf("线性表不存在\n");
                break;
            }
            if (j==OK) 
            {
                printf("\n遍历完成\n");
            }
            else printf("遍历错误\n");
            break;
        }
    
        case 13:{
            j=SaveList(*L,"C:\\Users\\ffsyd\\Desktop\\vscode\\ffsyd.txt");
        if (j==INFEASIBLE) 
            {
                printf("线性表不存在\n");
                break;
            }
            if (j==OK) 
            {
                printf("已成功读入文件\n");
            }
            else printf("未成功，出现错误\n");
            break;
        }
    
        case 14:{
            j=LoadList(L,"C:\\Users\\ffsyd\\Desktop\\vscode\\ffsyd.txt");
        if (j==INFEASIBLE) 
            {
                printf("线性表不存在\n");
                break;
            }
            if (j==OK) 
            {
                printf("已成功读出文件到线性表\n");
            }
            else printf("未成功，出现错误\n");
            break;
        }
        
        case 15:{
            j=reverseList(*L);
            if(j==OK)
            printf("已成功翻转\n");
            else
            printf("线性表不存在\n");
            break;
        }
    
        case 16:{
            printf("需要倒数第几个元素？\n");
            scanf("%d",&i);
            j=RemoveNthFromEnd(L,i);
            if (j==INFEASIBLE) 
            {
                printf("线性表不存在\n");
                break;
            }
            if (j==OK) 
            {
                
                printf("已删除倒数第%d个元素\n",i);
            }
            else printf("未成功删除（位置错误）\n");
            break;
        }
        case 17:{
        	if(sortList(L)!=ERROR)	
			printf("已完成排序\n");	
			else printf("排序失败\n");
			break;}
        case 18:{
            printf("请输入希望控制的线性表序号\n");
            scanf("%d",&i);
            j=change(&L,i,&Lists);
            if(j==OK)
            printf("成功切换\n");
            else
            printf("未成功切换");
            break;
        }

        case 19:{
            char name[30];
            printf("请输入新线性表的名字\n");
            scanf("%s",name);
            j=AddList(&Lists,name);
            if (j==OK) 
            {
                printf("已添加新线性表\n");
            }
            else printf("未成功添加\n");
            break;
        }

        case 20:{
            char name[30];
            printf("请输入需删除线性表名字\n");
            scanf("%s",name);
            j=RemoveList(&Lists,name);
            if (j==OK) 
            {
                printf("已成功删除\n");
            }
            else printf("未找到该线性表\n");
            break;
        }

    }
    printf("继续操作输1,否则输0\n");
    scanf("%d",&con);
}
else 
{printf("ERROR");break;}
}
    return 0;
}