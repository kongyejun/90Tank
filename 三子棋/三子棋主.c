#define _CRT_SECURE_NO_WARNINGS
#include"game.h"
void title()
{
	printf("************************\n");
	printf("*****1.pkay  2.quit*****\n");
	printf("************************\n");
}
void game()
{   
	char f = '2';
	//������������
	char arr[ROW][COL] = { 0 };//ROW��ʾ��  COL��ʾ��
	//���ݳ�ʼ��
	Initarr(arr, ROW, COL);
	while (1)
	{ 
		//�����Ļ
		system("CLS");
		//��ӡ����
		QIPAN(arr, ROW, COL);
		//���1����
		player1(arr, ROW, COL);
		//��ӡ����
		system("CLS"); QIPAN(arr, ROW, COL);
		//���2����
		//player2(arr, ROW, COL);
		//��������
		computer(arr, ROW, COL);
		//��Ϸ�ж�
		system("CLS"); QIPAN(arr, ROW, COL);
		f=seetlement(arr,ROW,COL);
		if (f== '#')
		{
			//printf("����ʤ��\n");
			printf("���2ʤ��\n");
			system("pause");
			break;
		}
		else if (f=='*')
		{
			printf("���1ʤ��\n");
			system("pause");
			break;
		}
		else if (f == '0')
		{
			printf("ƽ��\n");
			system("pause");
			break;
		}
	}
	//��ӡ�Ƿ������Ϸ
	printf("��������Ϸ\n");
}
void text()
{
	srand((unsigned int)time(NULL));//ʱ���  �������������ʹÿ����һ�����ᱻ����
	int input = 0;
	printf("��������Ϸ\n");
	do
	{
		title();
		printf("������:>");
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			printf("��������Ϸ\n\n");
			game();
			break;
		case 2:
			printf("�˳���Ϸ�ɹ�\n");
			break;
		default :
			printf("�������,����������\n\n");
		    break;
		}
	} while (input !=2);
}
int main()
{
	system("title  ������");
	system("color 1f");
	text();
	return 0;
}