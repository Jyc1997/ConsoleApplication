// snack.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include <stdio.h>  
#include <stdlib.h>  
#include <conio.h>  
#include <string.h>  
#include <time.h>
#include <windows.h>
#include<GL/glut.h>
#define M 15
#define N 30
void createfood();
void poisongrass();
void gunpowder();
void gotoxy(int x, int y);
void choose1();
int speed;
int gnum;
int bnum;
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
}food, grass, boom;

struct Snack *h;
struct Snack *t;
char key = 'd', lastkey = 'd';
char map[M][N];                     //�����ͼ

void before_start()
{
	int i;
	printf("choose level:(input 1(easy) 2(normal) 3(hard)):\n");
	scanf_s("%d", &i);
	switch (i)
	{
	case 1:
		speed = 5;
		break;
	case 2:
		speed = 3;
		break;
	case 3:
		speed = 1;
	default:;
	}
	system("cls");
}



void start()                          //��Ϸ����
{
	before_start();
	memset(map, '.', sizeof(map));    //����ͼ
	int i, j;                         //��ӡ��ͼ
	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++) {
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
	createfood();                        //���ɵ���
	poisongrass();
	gunpowder();
	map[h->x][h->y] = '*';
	gotoxy(h->x, h->y);                 //��ӡ��
	printf("%c", '*');
	map[t->x][t->y] = '*';
	gotoxy(t->x, t->y);
	printf("%c", '*');
	gotoxy(M + 1, 0);
}
void start_again()
{
	system("cls");
	int i, j;
	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++) {
			printf("%c", map[i][j]);
		}
		printf("\n");
	}
	struct Snack*p;
	p = h;
	while (p->next != NULL)
	{
		gotoxy(p->x, p->y);
		printf("%c", '*');
		p = p->next;
	}
	gotoxy(M + 1, 0);
}
void savefile()                         //�浵
{
	struct Snack*temp;
	temp = h;
	FILE *fp;
	errno_t err;
	err = fopen_s(&fp, "load", "w");
	if (err != 0) {
		printf("open error");
		fclose(fp);
		return;
	}
	fwrite(&key, sizeof(char), 1, fp);
	fwrite(&food, sizeof(food), 1, fp);
	fwrite(&grass, sizeof(grass), 1, fp);
	fwrite(&boom, sizeof(food), 1, fp);
	fwrite(map, sizeof(map), 1, fp);
	while (temp != NULL) {
		fwrite(temp, sizeof(struct Snack), 1, fp);
		temp = temp->next;
	}
	fclose(fp);
	printf("save over");
	_getch();
	exit(0);
}
int readfile()                          //����
{
	struct Snack*temp;
	FILE *fp;
	errno_t err;
	err = fopen_s(&fp, "load", "r");
	if (err != 0) {
		printf("no file");
		fclose(fp);
		return 1;
	}
	h = (struct Snack*)malloc(sizeof(snack));
	fread(&key, sizeof(char), 1, fp);
	fread(&food, sizeof(food), 1, fp);
	fread(&grass, sizeof(food), 1, fp);
	fread(&boom, sizeof(food), 1, fp);
	fread(map, sizeof(map), 1, fp);
	fread(h, sizeof(snack), 1, fp);
	h->before = NULL;
	h->next = NULL;
	t = h;
	while (!feof(fp))
	{
		temp = (struct Snack*)malloc(sizeof(snack));
		fread(temp, sizeof(snack), 1, fp);
		temp->before = t;
		t->next = temp;
		temp->next = NULL;
		t = temp;
	}
	fclose(fp);
	start_again();
	return 0;
}
void choose1()                    //�жϴ�
{
	char i;
	int flag;
	gotoxy(M + 1, 0);
	printf("you want to save?");
	i = _getch();
	if (i == 'y' || i == 'Y')
		savefile();
	else
		return;
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
			printf("you bite yourself\n");
			printf("game over\n");
			return 1;
		}
	}
}
int gameover2()
{
	if (h->x >= M || h->y >= N)
	{
		system("cls");
		printf("game over\n");
		return 1;
	}
	else if (h->x < 0 || h->y < 0)
	{
		system("cls");
		printf("you strike the wall\n");
		printf("game over\n");
		return 1;
	}
	else return 0;
}
int gameover3()
{
	struct Snack*p = h;
	int i = 1;
	while (p->next != NULL)
	{
		i++;
		p = p->next;
	}
	if (i == 1)
	{
		system("cls");
		printf("you are too short\n");
		printf("game over\n");
		return 1;
	}
	else
		return 0;
}
void checkfood()                     //���ʳ��
{

	if (h->x == food.x&&h->y == food.y)
		food.flag = 0;
}

void createfood()                 //����ʳ��
{
	srand(time(0));
	while (food.flag == 0) {
		food.x = rand() % M - 2 + 1;
		food.y = rand() % N - 2 + 1;
		if (map[food.x][food.y] == '.')
		{
			map[food.x][food.y] = '0';
			gotoxy(food.x, food.y);
			printf("0");
			gotoxy(M + 1, 0);
			food.flag = 1;
		}
	}
}
void checkgrass()
{
	if (h->x == grass.x&&h->y == grass.y)
		grass.flag--;
}
void poisongrass()        //����
{
	srand(time(0));
	while (grass.flag == 0)
	{
		grass.x = rand() % M - 2 + 1;
		grass.y = rand() % N - 2 + 1;
		if (map[grass.x][grass.y] == '.')
		{
			map[grass.x][grass.y] = '#';
			gotoxy(grass.x, grass.y);
			printf("#");
			gotoxy(M + 1, 0);
			grass.flag = 1;
		}
	}
}
void checkboom()
{
	if (h->x == boom.x&&h->y == boom.y)
		boom.flag = 0;
}
void gunpowder()                        //ըҩ
{
	srand(time(0));
	while (boom.flag == 0)
	{
		boom.x = rand() % M - 2 + 1;
		boom.y = rand() % N - 2 + 1;
		if (map[boom.x][boom.y] == '.')
		{
			map[boom.x][boom.y] = '&';
			gotoxy(boom.x, boom.y);
			printf("&");
			gotoxy(M + 1, 0);
			boom.flag = 1;
		}
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
	else if (a == ' ') {           //��ͣ
		gotoxy(M + 1, 0);
		_getch();
		key = lastkey;
	}
	else if (a == 'm') {
		choose1();
		key = lastkey;
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
	else if (key == 's') {
		temp->x++;
	}
	temp->next = h;
	temp->before = NULL;
	h->before = temp;
	h = temp;
	map[h->x][h->y] = '*';
	gotoxy(h->x, h->y);
	printf("*");
}
void dispelhead()                //��ͷ
{
	struct Snack*p;
	p = t->before;
	p->next = NULL;
	map[t->x][t->y] = '.';
	gotoxy(t->x, t->y);
	printf(".");
	free(t);
	t = p;
}
void cuthalf()                  //��һ��
{
	struct Snack*p = h;
	int i = 1;
	while (p->next != NULL)
	{
		i++;
		p = p->next;
	}
	i = i / 2;
	if (i == 1)
	{
		dispelhead();
		return;
	}
	p = h;
	while (i != 0)
	{
		p = p->next;
		i--;
	}
	t = p;
	p = p->next;
	t->next = NULL;
	struct Snack*f;
	f = p;
	while (p->next != NULL)
	{
		gotoxy(p->x, p->y);
		printf(".");
		map[p->x][p->y] = '.';
		f = p->next;
		free(p);
		p = f;
	}
	gotoxy(f->x, f->y);
	printf(".");
	map[f->x][f->y] = '.';
	free(f);
}
void move()                     //���ƶ�
{
	struct Snack*p;
	p = (struct Snack*)malloc(sizeof(snack));
	if (grass.flag == 0)
	{
		dispelhead();
		map[grass.x][grass.y] = '.';
		poisongrass();
	}
	if (food.flag == 0) {
		addhead();
		map[food.x][food.y] = '.';
		createfood();
	}
	if (boom.flag == 0) {
		cuthalf();
		map[boom.x][boom.y] = '.';
		gunpowder();
	}
	else
	{
		addhead();
		p = t->before;
		p->next = NULL;
		map[t->x][t->y] = '.';
		gotoxy(t->x, t->y);
		printf(".");
		free(t);
		t = p;
	}
}

int main()
{
	char a;
	printf("Do you want to read file?");
	a = _getch();
	system("cls");
	if (a == 'y' || a == 'Y')
	{
		readfile();
		if (readfile() == 1)
			return 0;
	}
	else
		start();
	while (1) {
		Sleep(speed * 100);
		if (_kbhit() != 0) {
			a = _getch();
			checkgo(a);
		}
		move();
		checkgrass();
		checkfood();
		checkboom();
		if (gameover() == 1 || gameover2() == 1 || gameover3() == 1) break;
	}

	system("pause");
	return 0;
}

void gotoxy(int x, int y)
{
	COORD coord = { y,x };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}