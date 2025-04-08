#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<windows.h>
#include<time.h>
#define MAX_STNUM 20000
#define MAX_NUM 2000
#define ERROR 0
#define OK 1
#define false 0
#define true 1
typedef int status;

typedef struct element {           //保存元素
	int flag;                   //判断是否被删除（0代表未被删除，其余数字代表被删除时的递归层数）
	int value;                  //记录元素(有正负)
	struct element* nextnode;          //下一元素变量
}element;

typedef struct clause {   //记录子句
	int flag;                   //标记子句真假，mark=0表示子句为真,其他情况表示删除时的递归深度
	int literal_num;                    //记录子句中元素的个数
	struct clause* nextcla;   	//指向下一个子句
	struct element* first;      //指向第一个元素
}clause;

typedef struct problem {                //SAT问题
	int vexnum, clanum;            //记录布尔变元数量vexnum、子句数量clanum
	struct clause* root;     //root结点中不存放数据
	int element[MAX_NUM + 1];		//各元素出现的次数（在无单子句而要选下一个删除关键字时使用）
	int ans[MAX_NUM + 1];			//（有解时的）解	 
}problem; 

typedef struct sudproblem {     //数独问题
	int original[10][10];       //记录原始数值
	int finalans[10][10];       //答案
	int flag[10][10];			//防止玩家输错位置
	int num;                    //非0个数
}sud;