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
} TElemType; //������������Ͷ���

typedef struct BiTNode{  //����������Ķ���
    TElemType  data;
    struct BiTNode *lchild,*rchild;
} BiTNode, *BiTree;
typedef struct{  //��������
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
int gjz[20][100]={0};//��¼ÿ�����Ĺؼ��֣�����
int q=0,j=0,i,z;
int flag=0;//����Ƿ�Ϊ��,����Ƿ��ظ�

int num=0;//��¼���Ƶ��������
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
{//���ݴ���֦�Ķ������ȸ���������definition����һ�ö������������ڵ�ָ�븳ֵ��T������OK���������ͬ�Ĺؼ��֣�����ERROR��
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
        j++;//j��ȫ�ֱ���������������Ĺؼ���
        strcpy((*T)->data.others,definition[q++].others);
        (*T)->lchild=NULL;
        (*T)->rchild=NULL;
    }
    CreateBiTree(&(*T)->lchild,definition);
    CreateBiTree(&(*T)->rchild,definition);
}

//����
status DestroyBTree(BiTree *T)	
	{
	        if(*T)
	        {   //����ǰ������
	                if((*T)->lchild)
	                {   //���ж��Ƿ������ӣ�����������������
	                        DestroyBTree(&(*T)->lchild);   //�ݹ���������
	                }
	                if((*T)->rchild)
	                {   //���������ж��Ƿ����Һ��ӣ������������Һ���
	                        DestroyBTree(&(*T)->rchild);   //�ݹ������Һ���
	                }
	                free(*T);   //����û�����ͷŵ�ǰ���
	                *T=NULL;   //������ÿ�
	                /*��ͨ��free�����ͷſռ�󣬽�ָ�������ÿ�*/
                    return OK;
	        }
	        return INFEASIBLE;
    }
//�п�
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
		flag = 1;//flag��ȫ�ֱ��������뺯��ǰ����Ϊ0������Ϊ������1
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
//�����������óɿգ���ɾ�����н�㣬�ͷŽ��ռ�
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
//�������T�����
{
  int ansl,ansr,ans;
    if(T){
        ansl=BiTreeDepth(T->lchild);
        ansr=BiTreeDepth(T->rchild);
        ans=ansl>ansr?ansl:ansr;//�ĸ��󷵻��ĸ�
        return ans+1;
    }
    return 0;
}

BiTNode* LocateNode(BiTree T,KeyType e)
//���ҽ��
{
       if(!T)//�жϲ�����
        return NULL;
    if(T->data.key==e)
        return T;
    BiTNode* an;
    if(T){
        an=LocateNode(T->lchild,e);
        if(an)//�������NULL������ans
            return an;
        an=LocateNode(T->rchild,e);
        if(an)
            return an;
    }
    return an;
}

status Assign(BiTree *T,KeyType e,TElemType value)
//ʵ�ֽ�㸳ֵ����������ͨ������������������ʵ�ֱ�������
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
//ʵ�ֻ���ֵܽ��
{
    if(!T||T->lchild==NULL&&T->rchild==NULL)
        return NULL;
  
    if(T->lchild&&T->lchild->data.key==e)
        return T->rchild;
    if(T->rchild&&T->rchild->data.key==e)//���ȱ�֤���Һ��ӵĴ�����������data
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
//�����㡣
{
    BiTree cur=LocateNode(*T,e);
    if(cur==NULL)
        return ERROR;  
        z=0;
    traverse(*T);
    for(int m=0;m<z;m++)
    {
        if(gjz[num][m]==c.key)//���ؼ����Ƿ��ظ�
        {flag=1;//������ظ�
        return ERROR;}
    }
    if(LR==0){//��ڵ�
        BiTree p=(BiTree)malloc(sizeof(BiTNode));
        p->data=c;
        p->rchild=cur->lchild;
        p->lchild=NULL;
        cur->lchild=p;
    }
    else if(LR==1){//�ҽڵ�
        BiTree p=(BiTree)malloc(sizeof(BiTNode));
        p->data=c;
        p->rchild=cur->rchild;
        p->lchild=NULL;
        cur->rchild=p;
    }
    else if(LR==-1){//���ڵ�
        BiTree p=(BiTree)malloc(sizeof(BiTNode));
        p->data.key=c.key;
        strcpy(p->data.others,c.others);
        p->rchild=*T;
        p->lchild=NULL;//���õĽڵ��NULL
        *T=p;
    }
    return OK;
}

//ɾ���ڵ�
BiTNode* LocateFather(BiTree T,KeyType e)
//����˫�׽��
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
//ɾ����㡣��������ͨ������������������ʵ�ֱ�������
{
        BiTree cur=LocateFather(*T,e);//�ҵ�����ڵ�ĸ��ڵ㷽�����ɾ������
    BiTree root=LocateNode(*T,e);//���ڵ�����
    if(cur==NULL&&root==NULL)
        return ERROR;//��ɾ���ڵ㲻����
    BiTree l;
    if(cur==NULL){//���ڵ㲻���ڣ�������
        l=root;
        if(l->lchild==NULL&&l->rchild==NULL){//�����ӽڵ㶼����
            free(l);
            *T=NULL;
        }
        else if(l->lchild==NULL&&l->rchild!=NULL){//�������ӽڵ�
            *T=l->rchild;
            free(l);
        }
        else if(l->rchild==NULL&&l->lchild!=NULL){//�������ӽڵ�
            *T=l->lchild;
            free(l);
        }
        else if(l->lchild&&l->rchild){//�����ӽڵ㶼����
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
        l=cur->lchild;//lָ�����ӽڵ�
        if(l!=NULL&&l->data.key==e){
            if(l->lchild==NULL&&l->rchild==NULL){//���ӽ��
                free(l);
                cur->lchild=NULL;
            }
            else if(l->lchild==NULL&&l->rchild!=NULL){//�������ӽڵ�
                cur->lchild=l->rchild;//�����ӽڵ����ȥ
                free(l);
            }
            else if(l->rchild==NULL&&l->lchild!=NULL){//�������ӽڵ�
                cur->lchild=l->lchild;//�����ӽڵ㼰������������
                free(l);
            }
            else if(l->lchild&&l->rchild){//����
                cur->lchild=l->lchild;//���ӽڵ㼰����������
                cur=cur->lchild;
                while(cur->rchild!=NULL){
                    cur=cur->rchild;//���ӽڵ�����
                }
                cur->rchild=l->rchild;
                free(l);
            }
            return OK;
        }   
        l=cur->rchild;//������������ȣ�������ͬ
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

//���ֱ�������
void visit(BiTree T)//���ʽ��
{
    printf(" %d,%s", T->data.key, T->data.others);
}
status PreOrderTraverse(BiTree T,void (*visit)(BiTree))
//�������������T
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
//�������������T
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
//�������������T
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
//���·����
{
	int ans=MPS(T);
	return ans;
}

BiTNode* LowestCommonAncestor(BiTree T,KeyType e1,KeyType e2)
//�����������
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
//��ת������
{
	if(!*T)  return INFEASIBLE;
	BiTNode*t;//����
	t=(*T)->lchild;
	(*T)->lchild=(*T)->rchild;
	(*T)->rchild=t;
	InvertTree(&(*T)->lchild);//���ӽڵ����
	InvertTree(&(*T)->rchild);
	return OK;
}

status SaveBiTree(BiTree T, char FileName[])
//���������Ľ������д�뵽�ļ�FileName��
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
//�����ļ�FileName�Ľ�����ݣ�����������
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
// �����
{
    
    strcpy((*Lists).elem[(*Lists).length].name,ListName);
    i=0;j=0;q=0;
        printf("��������������Ķ���������������У�\n");
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

status change(BiTree **T,int i,LISTS *Lists)//ѡ����Ҫ���ƵĶ�����
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
    printf("     1. CreateBiTree����������      2. DestroyBTree���ٶ�����\n");
    printf("     3. ClearBiTree ��ն�����      4. BiTreeEmpty �пն�����\n");
    printf("     5. BiTreeDepth ���������      6. LocateNode  ���ҽ��\n");
    printf("     7. Assign      ��㸳ֵ        8. GetSibling  ����ֵܽڵ�\n");
    printf("     9. InsertNode  ������        10. DeleteNode ɾ���ڵ�\n");
    printf("     11. PreOrderTraverseǰ�����   12.InOrderTraverse�������\n");
    printf("     13.PostOrderTraverse�������   14.LevelOrderTraverse�������\n");
    printf("     15.MaxPathSum���·����        16.LowestCommonAncestor�����������\n");
    printf("     17.InvertTree��ת������        \n");
    printf("     18.SaveBiTree  д���ļ�        19.LoadBiTreeд���ļ�\n");
    printf("     20.AddBiTree         21.RemoveBiTree\n");
    printf("     22.LocateBiTree      23.TreesTraverse\n");
    printf("     24.changeѡ����Ҫ���Ƶ���\n");
    printf("     0. Exit\n");
    printf("------------------------------------------------\n");
    LISTS Lists;
    Lists.length=0;
    TElemType value;
    BiTree *T;
    int op=1;//ѡ����
    int fl,LR;//fl��¼��������ֵ
    int e;//e����������ҵĽ��Ĺؼ���
    BiTree T1,T2;
    while (op) {
        printf("��������Ҫ�Ĳ�����\n");
        scanf("%d", &op);
        switch (op)
        {
        case 1: {
            for(int m=0;m<i;m++)
            {
                definition[m].key=0;
                for(int w=0;definition[m].others[w]!='\0';w++)
                definition[m].others[w]='\0';
            }//����
            i=0;//i��ȫ�ֱ�������ͷ����definition��������
            j=0;//j��ȫ�ֱ�������������gjz��Ĺؼ��֣����Ӧ������
            q=0;//q��ȫ�ֱ������ں����д�ͷ����definition�������ݣ�Ӧ������5
            fl=0;
            printf("��������������Ķ���������������У�\n");
            do {
	        scanf("%d %s",&definition[i].key,definition[i].others);
            } while (definition[i++].key!=-1);
            fl=CreateBiTree(T,definition);
            if (fl == OK)
                printf("�����������ɹ���\n");
            else if (fl== ERROR)
                printf("�ؼ��ֲ�Ψһ\n");
            else if (fl == OVERFLOW)
                printf("�����\n");
            break;
            }
        case 2 :{
            fl=DestroyBTree(T);
            if(fl==OK)
            printf("�ѳɹ����ٶ�������\n");
            if(fl==INFEASIBLE)
            printf("�������ٲ����ڵĶ�������\n");
            break;
        }

        case 3:{
            if (ClearBiTree(T) == OK)
                printf("�ɹ���ն�������\n");
            else
                printf("���ܶԲ����ڵĶ�����������ղ�����\n");           
            break;
        }

        case 4:{
            flag=0;//��־������Ϊ�ջ���1
            fl=BiTreeEmpty(*T);
            if(fl==INFEASIBLE)
            printf("�����������ڣ�\n");
            else if(flag==0)
            printf("������Ϊ�ա�\n");
            else
            printf("��������Ϊ�ա�\n");
            break;
        }

        case 5:{
            int depth = 0;
            depth=BiTreeDepth(*T);
            if(depth==0)
            printf("�����������ڣ�\n");
            else
            printf("�����������Ϊ%d\n", depth);
            break;
        }
        case 6:{
            T1 = NULL;
            printf("��������Ҫ���ҵĽ��Ĺؼ��֣�");
            scanf("%d", &e);
            T1=LocateNode(*T, e);
            if (T1) printf("�ؼ���Ϊ%d�Ľ��%d, %s\n", e, T1->data.key, T1->data.others);
            else printf("����ʧ�ܣ������ڹؼ���Ϊ%d�Ľ��\n", e);
            break;
        }
        
        case 7:{
            fl = 0;T1 = NULL;
            printf("��������Ҫ�޸ĵĽ��Ĺؼ��֣�");
            scanf("%d", &e);
            printf("������ֵ�޸�Ϊ��");
            scanf("%d %s", &value.key, value.others);
            fl = Assign(T, e, value);
            if (fl == ERROR)
                printf("��ֵ����ʧ�ܣ�\n");
            else if (fl == OK)
                printf("�ѽ��ؼ���Ϊ%d�Ľ��ֵ�޸�Ϊ %d,%s\n", e, value.key, value.others);
            break;
        }
        
        case 8:{
            T1 = NULL; T2 = NULL;
            printf("��������Ҫ�Ӷ������л���ֵܽ��Ĺؼ��֣�");
            scanf("%d", &e);
            T2=GetSibling(*T, e);
            if (T2) printf("�ؼ���Ϊ%d�Ľ����ֵܽ��Ϊ%d,%s\n", e, T2->data.key, T2->data.others);
            else printf("�ؼ���Ϊ%d�Ľ�����ֵܽ��\n", e);
            break;
        }
        
        case 9:{
            flag = 0;
            T1 = NULL;
            printf("�����������ĸ��׵Ĺؼ��֣�");
            scanf("%d", &e);
            printf("��������Ϊ ����(0)/�Һ���(1)/���ڵ�(-1)��");
            scanf("%d", &LR);
            printf("�������ֵ��");
            scanf("%d %s", &value.key, value.others);
            fl = InsertNode(T, e, LR, value);
            if (fl == OK)
                printf("����ɹ���\n");
            else if (fl == ERROR && flag == 1)
                printf("�������ؼ����ظ�������ʧ�ܣ�\n");
            else if (fl == ERROR && flag == 0)
                printf("�޷��ҵ����׽�㣬����ʧ�ܣ�\n");
            break;
        }
        case 10:{
            fl = 0;
            T1 = NULL; T2 = NULL;
            printf("��������Ҫ���߶�������ɾ���Ľ��ؼ��֣�");
            scanf("%d", &e);
            fl = DeleteNode(T, e);
            if (fl == OK)
                printf("���ؼ���Ϊ%d�Ľ���ѴӶ�������ɾ����\n", e);
            else if (fl == ERROR)
                printf("�������в����йؼ���Ϊ%d�Ľ�㣬ɾ��ʧ�ܣ�\n", e);
            break;
        }

        case 11:{
            printf("���������������\n");
            fl = PreOrderTraverse(*T, visit);
            if (fl != OK)
                printf("���ܱ��������ڵĶ�������\n");
            else printf("\n");
            break;
        }
        case 12:{
            printf("���������������\n");
            fl = InOrderTraverse(*T, visit);
            if (fl != OK)
                printf("���ܱ��������ڵĶ�������\n");
            else printf("\n");
            break;
            }
        case 13:{
            printf("���������������\n");
            fl = PostOrderTraverse(*T, visit);
            if (fl != OK)
                printf("���ܱ��������ڵĶ�������\n");
            else printf("\n");
            break;
            }
        case 14:{
            printf("���������������\n");
            for (i = 0;i < 100;i++)
                A[i] = NULL;
            i = 0;z = 1;
            fl = LevelOrderTraverse(*T, visit);
            if (fl != OK)
                printf("���ܱ��������ڵĶ�������\n");
            else printf("\n");
            break;
            }

        case 15:{
            fl=MaxPathSum(*T);
            printf("���·����Ϊ%d.\n",fl);
            break;
        }
        case 16:{
            int e1,e2;
            printf("��������Ҫ�������������Ĺؼ��֣�\n");
            scanf("%d %d",&e1,&e2);
            BiTree R=LowestCommonAncestor(*T,e1,e2);
            if(R==NULL)
            printf("���Ա����ڣ�\n");
            else
            printf("�������ȹؼ���Ϊ%d��\n",R->data.key);
            break;
        }
        case 17:{
            fl=InvertTree(T);
            if(fl==INFEASIBLE)
            printf("�����������ڣ�\n");
            if(fl==OK)
            printf("�ѳɹ���ת��\n");
            break;
        }
        case 18:{
            fl=SaveBiTree(*T,"C:\\Users\\ffsyd\\Desktop\\vscode\\ffsyd.txt");
            if(fl==INFEASIBLE)
            printf("���Ա����ڣ�\n");
            if(fl==OK)
            printf("�ѳɹ�д���ļ���\n");
            break;
        }

        case 19:{
            fl=LoadBiTree(T,"C:\\Users\\ffsyd\\Desktop\\vscode\\ffsyd.txt");
            if(fl==ERROR)
            printf("���Ա��Ѵ��ڣ�\n");
            if(fl==OK)
            printf("�ѳɹ�¼���ļ���\n");
            break;
        }
        case 20:{
            char name[30];
            printf("�������������֣�");
            scanf("%s",name);
            
            j=AddList(&Lists,name);
            if (j==OK) 
            {
                printf("�ɹ������\n");
            }
            else printf("ʧ��\n");
            break;
        }
        case 24:{
            printf("��������Ҫ�������������\n");
            scanf("%d",&i);

            fl=change(&T,i,&Lists);
            if(fl==OK)
            printf("�ѳɹ��л�\n");
            else
            printf("δ�ɹ��л�.\n");
            break;
        }
        case 0:
        break;
        }//end of switch

    }//end of while

}//end of main()

