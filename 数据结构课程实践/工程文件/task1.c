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
typedef int ElemType; //鏁版嵁鍏冪礌绫诲瀷瀹氫箟

#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
typedef int ElemType;
typedef struct{  //椤哄簭琛紙椤哄簭缁撴瀯锛夌殑瀹氫箟
	      ElemType * elem;
	      int length;
	      int listsize;
         }SqList;

typedef struct{  //绾挎€ц〃鐨勭鐞嗚〃瀹氫箟
     struct { char name[30];
     		  SqList L;	
      } elem[10];
      int length;
      int listsize;
 }LISTS;
status InitList(SqList *K)// 绾挎€ц〃L涓嶅瓨鍦?锛屾瀯閫犱竴涓?绌虹殑绾挎€ц〃锛岃繑鍥濷K锛屽惁鍒欒繑鍥濱NFEASIBLE銆?
{ 
    if((*K).elem==NULL)//涓嶅瓨鍦?
    {
        (*K).elem=(ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));//鍒嗛厤绌洪棿
        if(!(*K).elem)
        exit(OVERFLOW);//鏈?鎴愬姛锛岄€€鍑?
        (*K).length=0;
        (*K).listsize=LIST_INIT_SIZE;
        return OK;
    }
    if((*K).elem!=NULL)//瀛樺湪
    return INFEASIBLE;
}

status DestroyList(SqList *L)
// 濡傛灉绾挎€ц〃L瀛樺湪锛岄攢姣佺嚎鎬ц〃L锛岄噴鏀炬暟鎹?鍏冪礌鐨勭┖闂达紝杩斿洖OK锛屽惁鍒欒繑鍥濱NFEASIBLE銆?
{
    if((*L).elem)//瀛樺湪
    {
        free((*L).elem);//閲婃斁绌洪棿
        (*L).elem=NULL;
        (*L).length=0;
        (*L).listsize=0;
        return OK;
    }
    return INFEASIBLE;
}

status ClearList(SqList *L)
// 濡傛灉绾挎€ц〃L瀛樺湪锛屽垹闄ょ嚎鎬ц〃L涓?鐨勬墍鏈夊厓绱狅紝杩斿洖OK锛屽惁鍒欒繑鍥濱NFEASIBLE銆?
{
    if((*L).elem)//鍒ゆ柇瀛樺湪
    {
        for(int i=0;i<=(*L).length;i++)//姣忎釜鍏冪礌閮界瓑浜?0
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
// 濡傛灉绾挎€ц〃L瀛樺湪锛屽垽鏂?绾挎€ц〃L鏄?鍚︿负绌猴紝绌哄氨杩斿洖TRUE锛屽惁鍒欒繑鍥濬ALSE锛涘?傛灉绾挎€ц〃L涓嶅瓨鍦?锛岃繑鍥濱NFEASIBLE銆?
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
// 濡傛灉绾挎€ц〃L瀛樺湪锛岃繑鍥炵嚎鎬ц〃L鐨勯暱搴︼紝鍚﹀垯杩斿洖INFEASIBLE銆?
{
    if(L.elem)
    {
        return L.length;
    }
    else
    return INFEASIBLE;
}

status GetElem(SqList L,int i,ElemType *e)
// 濡傛灉绾挎€ц〃L瀛樺湪锛岃幏鍙栫嚎鎬ц〃L鐨勭?琲涓?鍏冪礌锛屼繚瀛樺湪e涓?锛岃繑鍥濷K锛涘?傛灉i涓嶅悎娉曪紝杩斿洖ERROR锛涘?傛灉绾挎€ц〃L涓嶅瓨鍦?锛岃繑鍥濱NFEASIBLE銆?
{
    if(L.elem)
    {
        if(i<1||i>L.length)//褰搃涓嶇?﹀悎瑕佹眰鏃讹紝閿欒??
        return 0;
        (*e)=L.elem[i-1];//璧嬪€肩粰e
        return OK;
    }
    else
    return INFEASIBLE;
}

int LocateElem(SqList L,ElemType e)
// 濡傛灉绾挎€ц〃L瀛樺湪锛屾煡鎵惧厓绱爀鍦ㄧ嚎鎬ц〃L涓?鐨勪綅缃?搴忓彿骞惰繑鍥炶?ュ簭鍙凤紱濡傛灉e涓嶅瓨鍦?锛岃繑鍥?0锛涘綋绾挎€ц〃L涓嶅瓨鍦ㄦ椂锛岃繑鍥濱NFEASIBLE锛堝嵆-1锛夈€?
{
    if(L.elem)
    {
        ElemType *p;
        int i=0;
        p=L.elem;
        for(;i<=L.length;i++)
        {
            if(*(p+i)==e)//鑻?p+i鎸囧悜鍏冪礌=e锛岃繑鍥炰綅缃?
            return i+1;
        }
        return ERROR;
    }
    return INFEASIBLE;
}

status PriorElem(SqList L,ElemType e,ElemType *pre)
// 濡傛灉绾挎€ц〃L瀛樺湪锛岃幏鍙栫嚎鎬ц〃L涓?鍏冪礌e鐨勫墠椹憋紝淇濆瓨鍦╬re涓?锛岃繑鍥濷K锛涘?傛灉娌℃湁鍓嶉┍锛岃繑鍥濫RROR锛涘?傛灉绾挎€ц〃L涓嶅瓨鍦?锛岃繑鍥濱NFEASIBLE銆?
{
  if(L.elem)
    {
        int i=0;
        ElemType *p;
        p=L.elem;
        for(;i<L.length;i++)
        {
            if(*(p+i)==e) //p+i鎸囧悜鍏冪礌=e
            {if(i==0)//鏄?绗?涓€涓?鍏冪礌锛屾棤鍓嶉┍
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
// 濡傛灉绾挎€ц〃L瀛樺湪锛岃幏鍙栫嚎鎬ц〃L鍏冪礌e鐨勫悗缁э紝淇濆瓨鍦╪ext涓?锛岃繑鍥濷K锛涘?傛灉娌℃湁鍚庣户锛岃繑鍥濫RROR锛涘?傛灉绾挎€ц〃L涓嶅瓨鍦?锛岃繑鍥濱NFEASIBLE銆?
{
    if(L.elem)
    {
        int i;
        ElemType *p;
        p=L.elem;
        for(i=0;i<L.length-1;i++)//涓嶄細閬嶅巻鍒版渶鍚庝竴涓?
        {
            if(*(p+i)==e)//p+i鎸囧悜鍏冪礌=e
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
// 濡傛灉绾挎€ц〃L瀛樺湪锛屽皢鍏冪礌e鎻掑叆鍒扮嚎鎬ц〃L鐨勭?琲涓?鍏冪礌涔嬪墠锛岃繑鍥濷K锛涘綋鎻掑叆浣嶇疆涓嶆?ｇ‘鏃讹紝杩斿洖ERROR锛涘?傛灉绾挎€ц〃L涓嶅瓨鍦?锛岃繑鍥濱NFEASIBLE銆?
{
    if((*L).elem)
    {
        if(i<1||i>(*L).length+1)//鎻掑叆浣嶇疆涓嶆?ｇ‘
        return 0;
        if((*L).length>=(*L).listsize)
        {
            ElemType *newbase=(ElemType*)malloc(LIST_INIT_SIZE *sizeof(ElemType));//鍒嗛厤绌洪棿
            if(!newbase)
            exit -2;//绌洪棿鍒嗛厤澶辫触锛岄敊璇?
            (*L).listsize+=LISTINCREMENT;
        }
        ElemType *q=&((*L).elem[i-1]);//绗琲涓?鍏冪礌浣嶇疆璧嬪€肩粰q
        for(int *p=&((*L).elem[(*L).length-1]);p>=q;--p)
        {  
            *(p+1)=*p;//姣忎釜寰€鍚庣Щ涓€涓?
        }
        *q=e;//鍔犲叆鏂板€煎埌绗琲涓?鍏冪礌
        ++(*L).length;//闀垮害+1
        return OK;
    }
    return INFEASIBLE;
}

status ListDelete(SqList *L,int i,ElemType *e)
// 濡傛灉绾挎€ц〃L瀛樺湪锛屽垹闄ょ嚎鎬ц〃L鐨勭?琲涓?鍏冪礌锛屽苟淇濆瓨鍦╡涓?锛岃繑鍥濷K锛涘綋鍒犻櫎浣嶇疆涓嶆?ｇ‘鏃讹紝杩斿洖ERROR锛涘?傛灉绾挎€ц〃L涓嶅瓨鍦?锛岃繑鍥濱NFEASIBLE銆?
{
    if((*L).elem)
    {
        if(i<1||i>(*L).length)//浣嶇疆涓嶆?ｇ‘
        return 0;
        ElemType *p;
        p=(*L).elem;
        (*e)=*(p+i-1);//璧嬪€肩粰e
        for(int j=i-1;j<(*L).length-1;j++)
        {
            *(p+j)=*(p+j+1);//寰€鍓嶇Щ
        }
        (*L).length-=1;
        return OK;
    }
    return INFEASIBLE;
}

status ListTraverse(SqList L)
// 濡傛灉绾挎€ц〃L瀛樺湪锛屼緷娆℃樉绀虹嚎鎬ц〃涓?鐨勫厓绱狅紝姣忎釜鍏冪礌闂寸┖涓€鏍硷紝杩斿洖OK锛涘?傛灉绾挎€ц〃L涓嶅瓨鍦?锛岃繑鍥濱NFEASIBLE銆?
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
// 濡傛灉绾挎€ц〃L瀛樺湪锛屽皢绾挎€ц〃L鐨勭殑鍏冪礌鍐欏埌FileName鏂囦欢涓?锛岃繑鍥濷K锛屽惁鍒欒繑鍥濱NFEASIBLE銆?
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
// 濡傛灉绾挎€ц〃L涓嶅瓨鍦?锛屽皢FileName鏂囦欢涓?鐨勬暟鎹?璇诲叆鍒扮嚎鎬ц〃L涓?锛岃繑鍥濷K锛屽惁鍒欒繑鍥濱NFEASIBLE銆?
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
            fscanf(fp,"%d",&(*L).elem[i]);//璇诲叆
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
// 鍙?闇€瑕佸湪Lists涓?澧炲姞涓€涓?鍚嶇О涓篖istName鐨勭┖绾挎€ц〃锛岀嚎鎬ц〃鏁版嵁鍙堝悗鍙版祴璇曠▼搴忔彃鍏ャ€?
{
    strcpy((*Lists).elem[(*Lists).length].name,ListName);
    (*Lists).elem[(*Lists).length].L.elem=(ElemType*)malloc(LIST_INIT_SIZE*sizeof(ElemType));
    (*Lists).elem[(*Lists).length].L.length=0;
    (*Lists).elem[(*Lists).length].L.listsize=LIST_INIT_SIZE;
    (*Lists).length+=1;
    return OK;
}

status RemoveList(LISTS *Lists,char ListName[])
// Lists涓?鍒犻櫎涓€涓?鍚嶇О涓篖istName鐨勭嚎鎬ц〃
{
    int i,*q,m;
    m=(*Lists).length;
    for(i=0;i<(*Lists).length;i++)
    {
        for(i=0;i<(*Lists).length;i++)
    {
        for(m=0;ListName[m]!='\0';m++)//瀵圭収鍚嶇О
        {
            if((*Lists).elem[i].name[m]==ListName[m])
        {
            continue;
        }
        else
        break;
        }
        if(ListName[m]=='\0')//鑻ュ悕绉板畬鍏ㄤ竴鏍凤紝姝ゆ椂瀵瑰簲瀛楃?︾┖
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
// 鍦↙ists涓?鏌ユ壘涓€涓?鍚嶇О涓篖istName鐨勭嚎鎬ц〃锛屾垚鍔熻繑鍥為€昏緫搴忓彿锛屽惁鍒欒繑鍥?0
{
    int i,m;
    for(i=0;i<Lists.length;i++)
    {
        for(m=0;ListName[m]!='\0';m++)
        {
            if(Lists.elem[i].name[m]==ListName[m])//瀵圭収鍚嶇О
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
        if(max < sum)//濡傛灉鍔犱笅涓€涓?鏁板悗鍜屽彉澶?
            max = sum;
        if(sum < 0)//绗?涓€涓?灏辨槸璐熸暟
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
        if(sum==e)//涓€涓?鏁板€兼湰韬?绛変簬缁欏畾鍊?
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

status SortList(SqList *L)  //鍗囧簭鎺掑垪
{
    if(!(*L).length)//鏃犳暟缁?
    {
        return ERROR;
    }
    int i, j, k;
    int t;
    for(i = 0; i <(*L).length-1; ++i)
    {
        k = i;//k闅廼寰€鍚庢壘
        for(j = i+1; j < (*L).length; ++j)
            if((*L).elem[j] < (*L).elem[k])//i鍜宨鍚庨潰鐨勬瘮杈?
                k = j;//杈冨皬鐨勪綅缃?缁檏
        if(k != i)//浜ゆ崲
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
	printf("    	  1. InitList初始化线性表       7. LocateElem查找元素\n");
	printf("    	  2. DestroyList销毁线性表      8. PriorElem获取前驱元素\n");
	printf("    	  3. ClearList清空线性表        9. NextElem获取后继元素 \n");
	printf("    	  4. ListEmpty判空线性表        10.ListInsert插入元素\n");
	printf("    	  5. ListLength线性表长度       11.ListDelete删除元素\n");
	printf("    	  6. GetElem获取元素            12.ListTrabverse遍历元素\n");
	printf("    	  13.SaveList写入文件           14.LoadList读出文件\n");
	printf("    	  15.AddList增添线性表          16.RemoveList移除线性表\n");
	printf("    	  17.LocateList查找线性表       18.MaxSubArray最大连续子数组和\n");
    printf("    	  19.SubArrayNum和为K的子数组   20.SortList线性表排序 \n");
    printf("    	  21.change切换                 0. Exit\n");
	printf("-------------------------------------------------\n");
	printf("    请选择你的操作[0~21]:");

    scanf("%d",&n);
    switch(n)
    {
        case 1:{
            j=InitList(K);
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
            printf("未完成初始化\n");
            break;
                }
        
        case 2:{
            j=DestroyList(K);
            if (j==INFEASIBLE) 
            {
                printf("线性表不存在\n");
                break;
            }
            if (j==OK) 
            {
                printf("线性表已销毁\n");
            }
            else printf("未成功销毁\n");
            break;
        }
    
        case 3:{
            j=ClearList(K);
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
            j=ListEmpty(*K);
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
            j=ListLength(*K);
            if (j==INFEASIBLE) 
            {
                printf("线性表不存在\n");
            }
            else
            {printf("线性表长度为%d\n",j);}
            break;
        }
    
        case 6:{
            printf("请输入：位置\n");
            scanf("%d",&i);
            j=GetElem(*K,i,&e);
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
            printf("请输入想要查找的元素\n");
            scanf("%d",&e);
            j=LocateElem(*K,e); 
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
            j=PriorElem(*K,e,&pre);
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
            j=NextElem(*K,e,&next);
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
            printf("请输入：插入位置 插入元素");
            scanf("%d %d",&i,&e);
            j=ListInsert(K,i,e);
            printf("%s\n", j==OK? "已成功插入\n" : j==ERROR? "未成功插入（位置错误）\n" : "线性表不存在\n");
            break;
        }

        case 11:{
            printf("请输入需要删除的元素位置\n");
            scanf("%d",&i);
            j=ListDelete(K,i,&e);
            if (j==INFEASIBLE) 
            {
                printf("线性表不存在\n");
                break;
            }
            if (j==OK) 
            {
                printf("已删除位置为%d的元素%d\n",i,e);
            }
            else printf("未成功删除（位置错误）\n");
            break;
        }

        case 12:{
            j=ListTraverse(*K); 
            if (j==INFEASIBLE) 
            {
                printf("线性表不存在\n");
                break;
            }
            if (j==OK) 
            {
                printf("\n遍历完成\n");
            }
            else printf("未成功遍历\n");
            break;
        }
    
        case 13:{
            j=SaveList(*K,"C:\\Users\\ffsyd\\Desktop\\vscode\\ffsyd.txt");
        if (j==INFEASIBLE) 
            {
                printf("线性表不存在\n");
                break;
            }
            if (j==OK) 
            {
                printf("已成功写入文件\n");
            }
            else printf("未成功写入文件\n");
            break;
        }
    
        case 14:{
            j=LoadList(K,"C:\\Users\\ffsyd\\Desktop\\vscode\\ffsyd.txt");
        if (j==INFEASIBLE) 
            {
                printf("线性表已存在\n");
                break;
            }
            if (j==OK) 
            {
                printf("已成功读出文件到线性表\n");
            }
            else printf("未成功读出文件\n");
            break;

        }

        case 15:{
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

        case 16:{
            char name[30];
            printf("请输入需删除线性表的名字\n");
            scanf("%s",name);
            j=RemoveList(&Lists,name);
            if (j==OK) 
            {
                printf("已成功删除\n");
            }
            else printf("未找到该线性表\n");
            break;
        }

        case 17:{
            char name[30];
            printf("请输入想查找的线性表的名字\n");
            scanf("%s",name);
            j=LocateList(Lists,name);
            if (j==INFEASIBLE) 
            {
                printf("线性表不存在");
                break;
            }
            else if(j==ERROR) 
            {
                printf("未找到该线性表\n");
            }
            else 
            {
                printf("该线性表的逻辑位置为%d\n",j);
            }
            break;
        }
    
        case 18:{
            j=maxSubArray(K);
            printf("最大子数组和为%d\n",j);
            break;
        }
    
        case 19:{
            printf("请输入希望的和值：");
            scanf("%d",&e);
            j=subarraySum(K,e);
            printf("和为%d的子数组个数为%d\nOK\n",e,j);
            break;
        }

         case 20:{
        	if(SortList(K)!=ERROR)	
			printf("已完成排序\n");	
			else printf("未完成排序\n");
			break;}

        case 21:{
            printf("请输入希望控制的线性表序号\n");
            scanf("%d",&i);
            j=change(&K,i,&Lists);
            if(j==OK)
            printf("成功切换\n");
            else
            printf("未成功切换");
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