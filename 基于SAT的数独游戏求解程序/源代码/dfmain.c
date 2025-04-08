#include"dfSAT.c"
struct problem qq;	          //记录问题
sud sudo;
char FileName[100] = { '\0' };

int main(void) 
{
	int choice =1;
	while (choice) {
		printf("---------------------------------------------------\n");
		printf(" 1.SAT SAT问题     2.Sudoku 蜂窝数独    0.Exit 退出                 \n");
		printf("---------------------------------------------------\n");
		scanf("%d",&choice);
		if (choice == 1) {		//SAT求解 
			fflush(stdin);
			SATQuestion();		//进入SAT模块 
		}
		else if (choice == 2) {	//数独问题 
			fflush(stdin);
			Sudoku();
			
		}
		else if (choice != 0) {
			fflush(stdin);
			printf("输入错误！请重新输入1或2或0！");
			getchar(); getchar();
		}
	}
	free(root);
	return 0;
}

