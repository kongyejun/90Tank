#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"game.h"

void Initarr(char arr[ROW][COL], int row, int col)
{
	int i, j;
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)//���ܵ��ڷ���ջ���
			arr[i][j] = ' ';
	}
}
void QIPAN(char arr[ROW][COL], int x, int y)
{
	int i,j;
	for (j = 0; j < x; j++)
	{
		for (i = 0; i < y; i++)
		{
			printf(" %c ", arr[j][i]);//��ӡ��   |   |   ���ĸ�ʽ
			if (i < y - 1)
			{
				printf("|");
			}
		}
		printf("\n");
		if (j != x - 1)//�ж��Ƿ������һ��
		{
			for (i = 0; i < y; i++)//��ӡ---|---|---
			{
				printf("---");
				if (i < y - 1)
				{
					printf("|");
				}
			}
			printf("\n");
		}
	}
}
void player1(char arr[ROW][COL], int row, int col)
{
	int x, y;
	while (1)
	{
		printf("���1 ���������꣺��");
		scanf("%d%d", &x, &y);
		if (x > 0 && x <= row && y>0 && y <= col)
		{
			if (arr[x-1][y-1] != ' ')
			{
				printf("�ø����ѱ�ռ�ã�����������\n");
			}
			else
			{
				arr[x-1][y-1] = '*';
				break;
			}
		}
		else
		{
			printf("���������������������\n");
		}
	}
}
//void player2(char arr[ROW][COL], int row, int col)
//{
//	int x, y;
//	while (1)
//	{
//		printf("���2 ���������꣺��");
//		scanf("%d%d", &x, &y);
//		if (x > 0 && x <= row && y > 0 && y <= col)
//		{
//			if (arr[x - 1][y - 1] != ' ')
//			{
//				printf("�ø����ѱ�ռ�ã�����������\n");
//			}
//			else
//			{
//				arr[x - 1][y - 1] = '#';
//				break;
//			}
//		}
//		else
//		{
//			printf("���������������������\n");
//		}
//	}
//}
void computer(char arr[ROW][COL],int row,int col)
{
	
	while (1)
	{
		int x = rand() % row;
		int y = rand() % col;
		if (arr[x][y] == ' ')
		{
			arr[x][y] = '#';
			break;
		}
	}
}
char seetlement(char arr[ROW][COL], int row, int col)
{
	int i = 0;
	int j = 0;
	for (i = 0, j = 0; i < row; i++, j++)
	{
		if (arr[i][1] == arr[i][2] && arr[i][0] == arr[i][1] && arr[i][1] != ' ')//ˮƽ����ж�
		{
			if (arr[i][1] == '#')
				return '#';//����ӮΪ1
			else
				return '*';//���ӮΪ2
		}
		else if (arr[1][j] == arr[2][j] && arr[0][j] == arr[1][j] && arr[1][j] != ' ')
		{
			if (arr[1][j] == '#')
				return '#';
			else
				return '*';
		}
	}
	if (arr[0][0] == arr[1][1] && arr[2][2] == arr[1][1] && arr[1][1] != ' ')//б������ж�
	{
		if (arr[1][1] == '#')
			return '#';
		else
			return '*';
	}
	if (arr[1][1] == arr[0][2] && arr[2][0] == arr[1][1] && arr[0][2] != ' ')//б������ж�
	{
		if (arr[1][1] == '#')
			return '#';
		else
			return '*';
	}
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			if (arr[i][j] == ' ')
				return '1';
		}
	}
	return '0';//ƽ��Ϊ0
}