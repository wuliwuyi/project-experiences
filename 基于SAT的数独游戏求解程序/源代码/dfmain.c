#include"dfSAT.c"
struct problem qq;	          //��¼����
sud sudo;
char FileName[100] = { '\0' };

int main(void) 
{
	int choice =1;
	while (choice) {
		printf("---------------------------------------------------\n");
		printf(" 1.SAT SAT����     2.Sudoku ��������    0.Exit �˳�                 \n");
		printf("---------------------------------------------------\n");
		scanf("%d",&choice);
		if (choice == 1) {		//SAT��� 
			fflush(stdin);
			SATQuestion();		//����SATģ�� 
		}
		else if (choice == 2) {	//�������� 
			fflush(stdin);
			Sudoku();
			
		}
		else if (choice != 0) {
			fflush(stdin);
			printf("�����������������1��2��0��");
			getchar(); getchar();
		}
	}
	free(root);
	return 0;
}

