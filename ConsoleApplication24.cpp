// ConsoleApplication24.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <stdio.h>  
#include <stdlib.h>  
#include <conio.h>  
#include <string.h>  
#include <time.h>
#include <windows.h>
void createfood();
void gotoxy(int x, int y);
struct Snack    //������
{
	int x;
	int y;
	struct Snack *next;
	struct Snack *before;
}snack;
struct Food                            //����ʳ��
{
	int x;
	int y;
	int flag = 0;
}food;
struct Snack *h;
struct Snack *t;
char key = 'd', lastkey = 'd';
char map[20][20];                     //�����ͼ

void start()                          //��Ϸ����
{
	memset(map, '.', sizeof(map));    //����ͼ
	int i, j;                         //��ӡ��ͼ
	for (i = 0;i < 20; i++)
	{
		for (j = 0;j < 20;j++) {
			printf("%c", map[i][j]);
		}
		printf("\n");
	}
	h = (struct Snack*)malloc(sizeof(snack));  //��ʼ����
	t = (struct Snack*)malloc(sizeof(snack));
	t->x = 9;
	t->y = 9;
	t->next = NULL;
	t->before = h;
	h->x = 10;
	h->y = 9;
	h->next = t;
	h->before = NULL;
	createfood();
	gotoxy(h->x, h->y);                 //��ӡ��
	printf("%c", '*');
	gotoxy(t->x, t->y);
	printf("%c", '*');
	gotoxy(21, 0);
}
int gameover()           //ɱ��
{
	struct Snack *p;
	p = h;
	while (p->next != NULL)
	{
		p = p->next;
		if (p->x == h->x&&p->y == h->y)
		{
			system("cls");
			printf("game over\n");
			return 1;
		}
	}
}
int gameover2()
{
	 if (h->x >= 19 || h->y >= 19)
	{
		system("cls");
		printf("game over\n");
		return 1;
	}
	else if (h->x <= 0 || h->y <= 0)
	{
		system("cls");
		printf("game over\n");
		return 1;
	}
	else return 0;
}

void checkfood()                     //���ʳ��
{
	struct Snack*p;
	p = h;
	if (p->x == food.x&&p->y == food.y)
		food.flag = 0;
}

void createfood()                 //����ʳ��
{
	srand(time(0));
	while(food.flag == 0) {
		food.x = rand() % 18 + 1;
		food.y = rand() % 18 + 1;
		if (map[food.x][food.y] == '.')
		{
			gotoxy(food.x, food.y);
			printf("0");
			gotoxy(21, 0);
			food.flag = 1;
		}
		else
			continue;
	}
}
void checkgo(char a)          //������뷽��
{
	if (a == 'w' && lastkey != 's') {
		key = 'w';
	}
	else if (a == 's' && lastkey != 'w') {
		key = 's';
	}
	else if (a == 'a'&& lastkey != 'd') {
		key = 'a';
	}
	else if (a == 'd'&& lastkey != 'a') {
		key = 'd';
	}
	lastkey = key;
}

void addhead()                 //����ͷ��
{
	struct Snack* temp;
	temp = (struct Snack*)malloc(sizeof(snack));
	temp->x = h->x;
	temp->y = h->y;
	if (key == 'a') {
		temp->y--;
	}
	else if (key == 'd') {
		temp->y++;
	}
	else if (key == 'w') {
		temp->x--;
	}
	else {
		temp->x++;
	}
	temp->next = h;
	temp->before = NULL;
	h->before = temp;
	h = temp;
	gotoxy(h->x, h->y);
	printf("*");
}
void move()                     //���ƶ�
{
	struct Snack*p;
	p = (struct Snack*)malloc(sizeof(snack));
	if (food.flag==0) {
		addhead();
		createfood();
	}
	else
	{
		addhead();
		p = t->before;
		p->next = NULL;
		gotoxy(t->x, t->y);
		printf(".");
		free(t);
		t = p;
	}
}

int main()
{
	char a;
	start();
	while (1) {
		Sleep(500);
		if (_kbhit() != 0) {
			a = _getch();
			checkgo(a);
		}
		move();
		checkfood();

		if (gameover() == 1||gameover2()==1) break;
	}

	system("pause");
	return 0;
}

void gotoxy(int x, int y)
{
	COORD coord = { y,x };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}