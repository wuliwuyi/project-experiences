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

typedef struct element {           //����Ԫ��
	int flag;                   //�ж��Ƿ�ɾ����0����δ��ɾ�����������ִ���ɾ��ʱ�ĵݹ������
	int value;                  //��¼Ԫ��(������)
	struct element* nextnode;          //��һԪ�ر���
}element;

typedef struct clause {   //��¼�Ӿ�
	int flag;                   //����Ӿ���٣�mark=0��ʾ�Ӿ�Ϊ��,���������ʾɾ��ʱ�ĵݹ����
	int literal_num;                    //��¼�Ӿ���Ԫ�صĸ���
	struct clause* nextcla;   	//ָ����һ���Ӿ�
	struct element* first;      //ָ���һ��Ԫ��
}clause;

typedef struct problem {                //SAT����
	int vexnum, clanum;            //��¼������Ԫ����vexnum���Ӿ�����clanum
	struct clause* root;     //root����в��������
	int element[MAX_NUM + 1];		//��Ԫ�س��ֵĴ��������޵��Ӿ��Ҫѡ��һ��ɾ���ؼ���ʱʹ�ã�
	int ans[MAX_NUM + 1];			//���н�ʱ�ģ���	 
}problem; 

typedef struct sudproblem {     //��������
	int original[10][10];       //��¼ԭʼ��ֵ
	int finalans[10][10];       //��
	int flag[10][10];			//��ֹ������λ��
	int num;                    //��0����
}sud;