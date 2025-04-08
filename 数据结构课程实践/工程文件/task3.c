#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#pragma warning(disable:4996)
#pragma warning(disable:6031)



typedef int status;
typedef int KeyType; 
typedef struct {
    KeyType  key;
    char others[20];
} TElemType; //二叉树结点类型定义

typedef struct BiTNode{  //二叉链表结点的定义
    TElemType  data;
    struct BiTNode *lchild,*rchild;
} BiTNode, *BiTree;
typedef struct{  //控制链表
      struct { char name[30];
     		    BiTree Tr;
      } elem[10];
      int length;
      int listsize;
 }LISTS;
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

TElemType definition[50]={0};
BiTree A[100];
int gjz[20][100]={0};//记录每棵树的关键字，查重
int q=0,j=0,i,z;
int flag=0;//标记是否为空,标记是否重复

int num=0;//记录控制的树的序号
char FileName[30] = { '\0' };

void traverse(BiTree T){
    if(T){
        gjz[num][z]=T->data.key;
        z++;
        traverse(T->lchild);
        traverse(T->rchild);
    }
}
status CreateBiTree(BiTree *T,TElemType definition[])
{//根据带空枝的二叉树先根遍历序列definition构造一棵二叉树，将根节点指针赋值给T并返回OK，如果有相同的关键字，返回ERROR。
    if(definition[q].key==0){
        (*T)=NULL;
        q++;
        return OK;
    }
    else if(definition[q].key==-1)
        return OK;
    else{
        (*T)=(BiTree)malloc(sizeof(BiTNode));
        (*T)->data.key=definition[q].key;
        gjz[num][j]=definition[q].key;
        for(int m=0;m<j;m++)
        {
            if(gjz[num][m]==gjz[num][j])
            return ERROR;
        }
        j++;//j是全局变量，查找数组里的关键字
        strcpy((*T)->data.others,definition[q++].others);
        (*T)->lchild=NULL;
        (*T)->rchild=NULL;
    }
    CreateBiTree(&(*T)->lchild,definition);
    CreateBiTree(&(*T)->rchild,definition);
}

//销毁
status DestroyBTree(BiTree *T)	
	{
	        if(*T)
	        {   //若当前结点存在
	                if((*T)->lchild)
	                {   //则判断是否有左孩子，若有则先销毁左孩子
	                        DestroyBTree(&(*T)->lchild);   //递归销毁左孩子
	                }
	                if((*T)->rchild)
	                {   //无左孩子则判断是否有右孩子，若有则销毁右孩子
	                        DestroyBTree(&(*T)->rchild);   //递归销毁右孩子
	                }
	                free(*T);   //若都没有则释放当前结点
	                *T=NULL;   //将结点置空
	                /*在通过free函数释放空间后，将指针立即置空*/
                    return OK;
	        }
	        return INFEASIBLE;
    }
//判空
/*status BiTreeEmpty(BiTree T)
{ 
    if(T)
    return FALSE;
    else
    return TRUE;
}*/
status BiTreeEmpty(BiTree T)
{
	if (T == NULL)
	{
		return INFEASIBLE;
	}
	BiTreeEmpty(T->lchild);
	BiTreeEmpty(T->rchild);
	if (T->data.key != 0)
	{
		flag = 1;//flag是全局变量，进入函数前已设为0，若不为空则变成1
	}
	return OK;
}

/*status ClearBiTree(BiTree *T)  
{  
    if (!*T)  
    {  
        return INFEASIBLE;  
    }  

    ClearBiTree(&(*T)->lchild);  
    ClearBiTree(&(*T)->rchild);  
    free(*T);  
    *T = NULL;  
}  */
status ClearBiTree(BiTree *T)
//将二叉树设置成空，并删除所有结点，释放结点空间
{
	if (*T == NULL)
	{
		return INFEASIBLE;
	}
	ClearBiTree(&(*T)->lchild);
	ClearBiTree(&(*T)->rchild);
	(*T)->data.key = 0;
	(*T)->data.others[0] = 0;
	return OK;
}

int BiTreeDepth(BiTree T)
//求二叉树T的深度
{
  int ansl,ansr,ans;
    if(T){
        ansl=BiTreeDepth(T->lchild);
        ansr=BiTreeDepth(T->rchild);
        ans=ansl>ansr?ansl:ansr;//哪个大返回哪个
        return ans+1;
    }
    return 0;
}

BiTNode* LocateNode(BiTree T,KeyType e)
//查找结点
{
       if(!T)//判断不存在
        return NULL;
    if(T->data.key==e)
        return T;
    BiTNode* an;
    if(T){
        an=LocateNode(T->lchild,e);
        if(an)//如果不是NULL，返回ans
            return an;
        an=LocateNode(T->rchild,e);
        if(an)
            return an;
    }
    return an;
}

status Assign(BiTree *T,KeyType e,TElemType value)
//实现结点赋值。此题允许通过增加其它函数辅助实现本关任务
{
    BiTree target=LocateNode(*T,e);
    if(target==NULL)
        return ERROR;

    traverse(*T);
    for(int c=0;c<j;c++)
    {
        if(value.key==gjz[num][c]&&target->data.key!=gjz[num][c])
        return ERROR;
    }  
    target->data.key=value.key;
    strcpy(target->data.others,value.others);
    return OK;
}

BiTNode* GetSibling(BiTree T,KeyType e)
//实现获得兄弟结点
{
    if(!T||T->lchild==NULL&&T->rchild==NULL)
        return NULL;
  
    if(T->lchild&&T->lchild->data.key==e)
        return T->rchild;
    if(T->rchild&&T->rchild->data.key==e)//首先保证左右孩子的存在再讨论其data
        return T->lchild;
    BiTNode* an=NULL;
    if(T){
        an=GetSibling(T->lchild,e);
        if(an)
            return an;
        an=GetSibling(T->rchild,e);
        if(an)
            return an;
    }
    return an;
}

status InsertNode(BiTree *T,KeyType e,int LR,TElemType c)
//插入结点。
{
    BiTree cur=LocateNode(*T,e);
    if(cur==NULL)
        return ERROR;  
        z=0;
    traverse(*T);
    for(int m=0;m<z;m++)
    {
        if(gjz[num][m]==c.key)//检查关键字是否重复
        {flag=1;//标记有重复
        return ERROR;}
    }
    if(LR==0){//左节点
        BiTree p=(BiTree)malloc(sizeof(BiTNode));
        p->data=c;
        p->rchild=cur->lchild;
        p->lchild=NULL;
        cur->lchild=p;
    }
    else if(LR==1){//右节点
        BiTree p=(BiTree)malloc(sizeof(BiTNode));
        p->data=c;
        p->rchild=cur->rchild;
        p->lchild=NULL;
        cur->rchild=p;
    }
    else if(LR==-1){//父节点
        BiTree p=(BiTree)malloc(sizeof(BiTNode));
        p->data.key=c.key;
        strcpy(p->data.others,c.others);
        p->rchild=*T;
        p->lchild=NULL;//不用的节点放NULL
        *T=p;
    }
    return OK;
}

//删除节点
BiTNode* LocateFather(BiTree T,KeyType e)
//查找双亲结点
{
    if(!T)
        return NULL;
    if(T->lchild!=NULL&&T->lchild->data.key==e||T->rchild!=NULL&&T->rchild->data.key==e)
        return T;
    BiTNode* an;
    if(T){
        an=LocateFather(T->lchild,e);
        if(an)
            return an;
        an=LocateFather(T->rchild,e);
        if(an)
            return an;
    }
    return an;
}
status DeleteNode(BiTree *T,KeyType e)
//删除结点。此题允许通过增加其它函数辅助实现本关任务
{
        BiTree cur=LocateFather(*T,e);//找到这个节点的父节点方便进行删除操作
    BiTree root=LocateNode(*T,e);//根节点特判
    if(cur==NULL&&root==NULL)
        return ERROR;//待删除节点不存在
    BiTree l;
    if(cur==NULL){//父节点不存在（空树）
        l=root;
        if(l->lchild==NULL&&l->rchild==NULL){//左右子节点都不在
            free(l);
            *T=NULL;
        }
        else if(l->lchild==NULL&&l->rchild!=NULL){//仅有右子节点
            *T=l->rchild;
            free(l);
        }
        else if(l->rchild==NULL&&l->lchild!=NULL){//仅有左子节点
            *T=l->lchild;
            free(l);
        }
        else if(l->lchild&&l->rchild){//左右子节点都存在
            *T=l->lchild;
            cur=l->lchild;
            while(cur->rchild!=NULL){
                cur=cur->rchild;
            }
            cur->rchild=l->rchild;
            free(l);
        }
        return OK;
    }
    else{
        l=cur->lchild;//l指向左子节点
        if(l!=NULL&&l->data.key==e){
            if(l->lchild==NULL&&l->rchild==NULL){//无子结点
                free(l);
                cur->lchild=NULL;
            }
            else if(l->lchild==NULL&&l->rchild!=NULL){//仅有右子节点
                cur->lchild=l->rchild;//将右子节点放上去
                free(l);
            }
            else if(l->rchild==NULL&&l->lchild!=NULL){//仅有左子节点
                cur->lchild=l->lchild;//将左子节点及其以下向上移
                free(l);
            }
            else if(l->lchild&&l->rchild){//都有
                cur->lchild=l->lchild;//左子节点及其以下上移
                cur=cur->lchild;
                while(cur->rchild!=NULL){
                    cur=cur->rchild;//右子节点上移
                }
                cur->rchild=l->rchild;
                free(l);
            }
            return OK;
        }   
        l=cur->rchild;//与上面的情况类比，道理相同
        if(l!=NULL&&l->data.key==e){
            if(l->lchild==NULL&&l->rchild==NULL){
                free(l);
                cur->rchild=NULL;
            }
            else if(l->lchild==NULL&&l->rchild!=NULL){
                cur->rchild=l->rchild;
                free(l);
            }
            else if(l->rchild==NULL&&l->lchild!=NULL){
                cur->rchild=l->lchild;
                free(l);
            }
            else if(l->lchild!=NULL&&l->rchild!=NULL){
                cur->rchild=l->lchild;
                cur=cur->rchild;
                while(cur->rchild!=NULL){
                    cur=cur->rchild;
                }
                cur->rchild=l->rchild;
                free(l);
            }
            return OK;
        }
        return ERROR;
    }
}

//四种遍历方法
void visit(BiTree T)//访问结点
{
    printf(" %d,%s", T->data.key, T->data.others);
}
status PreOrderTraverse(BiTree T,void (*visit)(BiTree))
//先序遍历二叉树T
{
       if(!T)
        return 0;
    if(T){
        visit(T);
        PreOrderTraverse(T->lchild,visit);
        PreOrderTraverse(T->rchild,visit);
    }
    return OK;
}
status InOrderTraverse(BiTree T,void (*visit)(BiTree))
//中序遍历二叉树T
{
    if(!T)
        return 0;
    if(T){

        InOrderTraverse(T->lchild,visit);
        visit(T);
        InOrderTraverse(T->rchild,visit);
    }
    return OK;
}
status PostOrderTraverse(BiTree T,void (*visit)(BiTree))
//后序遍历二叉树T
{
if(!T)
        return 0;
    if(T){

        PostOrderTraverse(T->lchild,visit);
        //visit(T);
        PostOrderTraverse(T->rchild,visit);
        visit(T);
    }
    return OK;
}
status LevelOrderTraverse(BiTree T, void (*visit)(BiTree))
{
    A[0] = T;
    if (T == NULL) return ERROR;
    else
    {   
        visit(T);i++;
        if (T->lchild) { A[z] = T->lchild;z++; }
        if (T->rchild) { A[z] = T->rchild;z++; }
        LevelOrderTraverse(A[i], visit);
        return OK;
    }
}

int max(int l,int r)
{
    if(l>r)
    return l;
    if(l<=r)
    return r;
}

int MPS(BiTree T)
{
	if(!T) return 0;
	int l=MPS(T->lchild);
	int r=MPS(T->rchild);
	return max(l,r)+T->data.key;
}

status MaxPathSum(BiTree T)
//最大路径和
{
	int ans=MPS(T);
	return ans;
}

BiTNode* LowestCommonAncestor(BiTree T,KeyType e1,KeyType e2)
//最近公共祖先
{
    if(!T) return NULL;
	if(LocateNode(T->lchild,e1))
    {
		if(LocateNode(T->lchild,e2))  return LowestCommonAncestor(T->lchild,e1,e2);
		return T;
	}
	else
	{
		if(LocateNode(T->rchild,e2))  return LowestCommonAncestor(T->rchild,e1,e2);
		return T;
	}
}
 
status InvertTree(BiTree *T)
//翻转二叉树
{
	if(!*T)  return INFEASIBLE;
	BiTNode*t;//交换
	t=(*T)->lchild;
	(*T)->lchild=(*T)->rchild;
	(*T)->rchild=t;
	InvertTree(&(*T)->lchild);//往子节点遍历
	InvertTree(&(*T)->rchild);
	return OK;
}

status SaveBiTree(BiTree T, char FileName[])
//将二叉树的结点数据写入到文件FileName中
{if(T){
    FILE* fp;
    fp = fopen(FileName, "w");
    if (T == NULL) return ERROR;
    int key = 0;
    char ch[10];
    ch[0] = '#';
    BiTree st[50];
    int top = 0;
    st[top++] = T;
    BiTree p;
    while (top != 0) {
        p = st[--top];
        if (p != NULL) {
            fprintf(fp, "%d ", p->data.key);
            fprintf(fp, "%s ", p->data.others);
            st[top++] = p->rchild;
            st[top++] = p->lchild;
        }
        else {
            fprintf(fp, "%d ", key);
            fprintf(fp, "%s ", ch);
        }
    }
    fclose(fp);
    return OK;
}
return INFEASIBLE;
}

status LoadBiTree(BiTree *T,  char FileName[])
//读入文件FileName的结点数据，创建二叉树
{
    if (*T != NULL) return ERROR;
    FILE* fp;
    fp = fopen(FileName, "r");
    int key;
    char ch[10];
    int k = 0;
    TElemType dataa[50];
    while (1) {
        if (fscanf(fp, "%d ", &key) == EOF)
            break;
        fscanf(fp, "%s ", ch);
        dataa[k].key = key;
        strcpy(dataa[k].others, ch);
        k++;
    }
    dataa[k].key = -1;
    j=0;q=0;
    CreateBiTree(T, dataa);
    fclose(fp);
    return OK;
}

status AddList(LISTS *Lists,char ListName[])
// 添加树
{
    
    strcpy((*Lists).elem[(*Lists).length].name,ListName);
    i=0;j=0;q=0;
        printf("请输入带空子树的二叉树先序遍历序列：\n");
        do {
	        scanf("%d %s",&definition[i].key,definition[i].others);
        } while (definition[i++].key!=-1);
    flag=CreateBiTree(&(*Lists).elem[(*Lists).length].Tr,definition);
    (*Lists).length+=1;
    if(flag==OK)
    return OK;
    if(flag==ERROR)
    return ERROR;
}

status change(BiTree **T,int i,LISTS *Lists)//选择需要控制的二叉树
{
    if(i<=(*Lists).length)
    {(*T)=&((*Lists).elem[i-1].Tr);
    num=i-1;
    return OK;}
    else
    return ERROR;
}


int main(void)
{
    printf("      Menu for Linear Table On Sequence Structure \n");
    printf("------------------------------------------------\n");
    printf("     1. CreateBiTree创建二叉树      2. DestroyBTree销毁二叉树\n");
    printf("     3. ClearBiTree 清空二叉树      4. BiTreeEmpty 判空二叉树\n");
    printf("     5. BiTreeDepth 二叉树深度      6. LocateNode  查找结点\n");
    printf("     7. Assign      结点赋值        8. GetSibling  获得兄弟节点\n");
    printf("     9. InsertNode  插入结点        10. DeleteNode 删除节点\n");
    printf("     11. PreOrderTraverse前序遍历   12.InOrderTraverse中序遍历\n");
    printf("     13.PostOrderTraverse后序遍历   14.LevelOrderTraverse层序遍历\n");
    printf("     15.MaxPathSum最大路径和        16.LowestCommonAncestor最近公共祖先\n");
    printf("     17.InvertTree翻转二叉树        \n");
    printf("     18.SaveBiTree  写入文件        19.LoadBiTree写出文件\n");
    printf("     20.AddBiTree         21.RemoveBiTree\n");
    printf("     22.LocateBiTree      23.TreesTraverse\n");
    printf("     24.change选择需要控制的树\n");
    printf("     0. Exit\n");
    printf("------------------------------------------------\n");
    LISTS Lists;
    Lists.length=0;
    TElemType value;
    BiTree *T;
    int op=1;//选择功能
    int fl,LR;//fl记录函数返回值
    int e;//e用于输入查找的结点的关键字
    BiTree T1,T2;
    while (op) {
        printf("请输入需要的操作：\n");
        scanf("%d", &op);
        switch (op)
        {
        case 1: {
            for(int m=0;m<i;m++)
            {
                definition[m].key=0;
                for(int w=0;definition[m].others[w]!='\0';w++)
                definition[m].others[w]='\0';
            }//重置
            i=0;//i是全局变量，从头遍历definition存入数据
            j=0;//j是全局变量，查找数组gjz里的关键字，因此应该重置
            q=0;//q是全局变量，在函数中从头遍历definition读出数据，应该重置5
            fl=0;
            printf("请输入带空子树的二叉树先序遍历序列：\n");
            do {
	        scanf("%d %s",&definition[i].key,definition[i].others);
            } while (definition[i++].key!=-1);
            fl=CreateBiTree(T,definition);
            if (fl == OK)
                printf("二叉树创建成功！\n");
            else if (fl== ERROR)
                printf("关键字不唯一\n");
            else if (fl == OVERFLOW)
                printf("溢出！\n");
            break;
            }
        case 2 :{
            fl=DestroyBTree(T);
            if(fl==OK)
            printf("已成功销毁二叉树。\n");
            if(fl==INFEASIBLE)
            printf("不能销毁不存在的二叉树！\n");
            break;
        }

        case 3:{
            if (ClearBiTree(T) == OK)
                printf("成功清空二叉树。\n");
            else
                printf("不能对不存在的二叉树进行清空操作！\n");           
            break;
        }

        case 4:{
            flag=0;//标志，若不为空会变成1
            fl=BiTreeEmpty(*T);
            if(fl==INFEASIBLE)
            printf("二叉树不存在！\n");
            else if(flag==0)
            printf("二叉树为空。\n");
            else
            printf("二叉树不为空。\n");
            break;
        }

        case 5:{
            int depth = 0;
            depth=BiTreeDepth(*T);
            if(depth==0)
            printf("二叉树不存在！\n");
            else
            printf("二叉树的深度为%d\n", depth);
            break;
        }
        case 6:{
            T1 = NULL;
            printf("请输入想要查找的结点的关键字：");
            scanf("%d", &e);
            T1=LocateNode(*T, e);
            if (T1) printf("关键字为%d的结点%d, %s\n", e, T1->data.key, T1->data.others);
            else printf("查找失败，不存在关键字为%d的结点\n", e);
            break;
        }
        
        case 7:{
            fl = 0;T1 = NULL;
            printf("请输入想要修改的结点的关键字：");
            scanf("%d", &e);
            printf("将其结点值修改为：");
            scanf("%d %s", &value.key, value.others);
            fl = Assign(T, e, value);
            if (fl == ERROR)
                printf("赋值操作失败！\n");
            else if (fl == OK)
                printf("已将关键字为%d的结点值修改为 %d,%s\n", e, value.key, value.others);
            break;
        }
        
        case 8:{
            T1 = NULL; T2 = NULL;
            printf("请输入想要从二叉树中获得兄弟结点的关键字：");
            scanf("%d", &e);
            T2=GetSibling(*T, e);
            if (T2) printf("关键字为%d的结点的兄弟结点为%d,%s\n", e, T2->data.key, T2->data.others);
            else printf("关键字为%d的结点无兄弟结点\n", e);
            break;
        }
        
        case 9:{
            flag = 0;
            T1 = NULL;
            printf("请输入插入结点的父亲的关键字：");
            scanf("%d", &e);
            printf("插入结点作为 左孩子(0)/右孩子(1)/根节点(-1)：");
            scanf("%d", &LR);
            printf("插入结点的值：");
            scanf("%d %s", &value.key, value.others);
            fl = InsertNode(T, e, LR, value);
            if (fl == OK)
                printf("插入成功！\n");
            else if (fl == ERROR && flag == 1)
                printf("新增结点关键字重复，插入失败！\n");
            else if (fl == ERROR && flag == 0)
                printf("无法找到父亲结点，插入失败！\n");
            break;
        }
        case 10:{
            fl = 0;
            T1 = NULL; T2 = NULL;
            printf("请输入想要在线二叉树中删除的结点关键字：");
            scanf("%d", &e);
            fl = DeleteNode(T, e);
            if (fl == OK)
                printf("结点关键字为%d的结点已从二叉树中删除。\n", e);
            else if (fl == ERROR)
                printf("二叉树中不含有关键字为%d的结点，删除失败！\n", e);
            break;
        }

        case 11:{
            printf("先序遍历二叉树：\n");
            fl = PreOrderTraverse(*T, visit);
            if (fl != OK)
                printf("不能遍历不存在的二叉树！\n");
            else printf("\n");
            break;
        }
        case 12:{
            printf("中序遍历二叉树：\n");
            fl = InOrderTraverse(*T, visit);
            if (fl != OK)
                printf("不能遍历不存在的二叉树！\n");
            else printf("\n");
            break;
            }
        case 13:{
            printf("后序遍历二叉树：\n");
            fl = PostOrderTraverse(*T, visit);
            if (fl != OK)
                printf("不能遍历不存在的二叉树！\n");
            else printf("\n");
            break;
            }
        case 14:{
            printf("层序遍历二叉树：\n");
            for (i = 0;i < 100;i++)
                A[i] = NULL;
            i = 0;z = 1;
            fl = LevelOrderTraverse(*T, visit);
            if (fl != OK)
                printf("不能遍历不存在的二叉树！\n");
            else printf("\n");
            break;
            }

        case 15:{
            fl=MaxPathSum(*T);
            printf("最大路径和为%d.\n",fl);
            break;
        }
        case 16:{
            int e1,e2;
            printf("请输入想要操作的两个结点的关键字：\n");
            scanf("%d %d",&e1,&e2);
            BiTree R=LowestCommonAncestor(*T,e1,e2);
            if(R==NULL)
            printf("线性表不存在！\n");
            else
            printf("公共祖先关键字为%d。\n",R->data.key);
            break;
        }
        case 17:{
            fl=InvertTree(T);
            if(fl==INFEASIBLE)
            printf("二叉树不存在！\n");
            if(fl==OK)
            printf("已成功翻转。\n");
            break;
        }
        case 18:{
            fl=SaveBiTree(*T,"C:\\Users\\ffsyd\\Desktop\\vscode\\ffsyd.txt");
            if(fl==INFEASIBLE)
            printf("线性表不存在！\n");
            if(fl==OK)
            printf("已成功写入文件。\n");
            break;
        }

        case 19:{
            fl=LoadBiTree(T,"C:\\Users\\ffsyd\\Desktop\\vscode\\ffsyd.txt");
            if(fl==ERROR)
            printf("线性表已存在！\n");
            if(fl==OK)
            printf("已成功录出文件。\n");
            break;
        }
        case 20:{
            char name[30];
            printf("请输入树的名字：");
            scanf("%s",name);
            
            j=AddList(&Lists,name);
            if (j==OK) 
            {
                printf("成功添加树\n");
            }
            else printf("失败\n");
            break;
        }
        case 24:{
            printf("请输入需要操作的树的序号\n");
            scanf("%d",&i);

            fl=change(&T,i,&Lists);
            if(fl==OK)
            printf("已成功切换\n");
            else
            printf("未成功切换.\n");
            break;
        }
        case 0:
        break;
        }//end of switch

    }//end of while

}//end of main()

