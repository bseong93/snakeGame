#define _CRT_RAND_S
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define MAP_WIDTH 30
#define MAP_HEIGHT 20
#define MAP_STARTX 5
#define MAP_STARTY 3
#define UP 72	//화살표가 눌리면 _getch()는 224를 첫 번째로 리턴.
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ESC 27
#define PAUSE 112

int nDirect;
int nLength;
int nX[100], nY[100];
int nSpeed;
int nScore;
int key;
int nFoodX;
int nFoodY;

void gotoxy(int x, int y, char *szData)
{
	COORD pos = { x * 2, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("%s", szData);
}
void draw_map();
void title();
void reset();
void move();
void food();
void pause();
void game_over();

void main(int argc, char* argv[])
{
	title();
	while (1)
	{
		Sleep(nSpeed);
		if (_kbhit())	do { key = _getch(); } while (key == 224);
		switch (key)
		{
		case UP:
		case DOWN:
		case LEFT:
		case RIGHT:
			if ((nDirect == UP && key != DOWN) || (nDirect == DOWN && key != UP)
				|| (nDirect == LEFT && key != RIGHT) || (nDirect == RIGHT && key != LEFT))
				nDirect = key;
			//key = 0;
			break;
		case PAUSE:
			pause();
			break;
		case ESC:
			exit(0);
			break;
		}
		move();
	}

}

void draw_map()
{
	int i = 0;

	for (i = 0; i < MAP_WIDTH; i++)
	{
		gotoxy(MAP_STARTX + i, MAP_STARTY, "■");
		gotoxy(MAP_STARTX + i, MAP_STARTY + MAP_HEIGHT - 1, "■");
	}
	for (i = 1; i < MAP_HEIGHT - 1; i++)
	{
		gotoxy(MAP_STARTX, MAP_STARTY + i, "■");
		gotoxy(MAP_STARTX + MAP_WIDTH - 1, MAP_STARTY + i, "■");
	}
}

void title()
{
	draw_map();
	gotoxy(MAP_STARTX + (MAP_WIDTH / 2 - 7), MAP_STARTY + 3, "|----------------------------|");
	gotoxy(MAP_STARTX + (MAP_WIDTH / 2 - 7), MAP_STARTY + 4, "|☆★☆★☆뱀 게임!☆★☆★☆|");
	gotoxy(MAP_STARTX + (MAP_WIDTH / 2 - 7), MAP_STARTY + 5, "|----------------------------|");

	gotoxy(MAP_STARTX + (MAP_WIDTH / 2 - 7), MAP_STARTY + 13, "   ◇ ←,→,↑,↓ : Move    ");
	gotoxy(MAP_STARTX + (MAP_WIDTH / 2) - 7, MAP_STARTY + 14, "   ◇ P : Pause             ");
	gotoxy(MAP_STARTX + (MAP_WIDTH / 2) - 7, MAP_STARTY + 15, "   ◇ ESC : Quit              ");
	gotoxy(MAP_STARTX + (MAP_WIDTH / 2) - 5, MAP_STARTY + 18, "    = Made by BSW =              ");
	while (1)
	{
		if (_kbhit())
		{
			key = _getch();
			if (key == ESC)
			{
				gotoxy(MAP_STARTX, MAP_HEIGHT + 3, "|-----------게임 종료-----------|");
				exit(0);
			}
			else
				break;
		}
		gotoxy(MAP_STARTX + (MAP_WIDTH / 2 - 5), MAP_STARTY + MAP_HEIGHT / 2 - 3, "PRESS ANY KEY TO START");
		Sleep(700);
		gotoxy(MAP_STARTX + (MAP_WIDTH / 2 - 5), MAP_STARTY + MAP_HEIGHT / 2 - 3, "                      ");
		Sleep(700);
	}
	reset();
}

void reset()
{
	int i = 0;

	while (_kbhit()) _getch();
	system("cls");
	draw_map();

	key = 0;
	nSpeed = 100;
	nLength = 5;
	nDirect = LEFT;
	nScore = 0;

	for (i = 0; i < nLength; i++)
	{
		nX[i] = MAP_WIDTH / 2 + i;
		nY[i] = MAP_HEIGHT / 2;
		if (i == 0) gotoxy(MAP_STARTX + nX[i], MAP_STARTY + nY[i], "ㅎ");
		else gotoxy(MAP_STARTX + nX[i], MAP_STARTY + nY[i], "ㅇ");
	}
	food();
}

void move()
{
	int i;

	if (nX[0] == nFoodX && nY[0] == nFoodY)
	{
		nScore += 10;
		nLength++;
		food();
		nX[nLength - 1] = nX[nLength - 2];
		nY[nLength - 1] = nY[nLength - 2];
	}

	if (nX[0] == 0 || nX[0] == MAP_WIDTH - 1 || nY[0] == 0 || nY[0] == MAP_HEIGHT - 1)
	{
		game_over();
		return;
	}

	for (i = 1; i < nLength; i++)
	{
		if (nX[0] == nX[i] && nY[0] == nY[i])
		{
			game_over();
			return;
		}
	}

	gotoxy(MAP_STARTX + nX[nLength - 1], MAP_STARTY + nY[nLength - 1], " ");
	for (i = nLength - 1; i > 0; i--)
	{
		nX[i] = nX[i - 1];
		nY[i] = nY[i - 1];
	}

	gotoxy(MAP_STARTX + nX[0], MAP_STARTY + nY[0], "ㅇ");
	if (nDirect == LEFT) nX[0]--;
	if (nDirect == RIGHT) nX[0]++;
	if (nDirect == UP) nY[0]--;
	if (nDirect == DOWN) nY[0]++;
	gotoxy(MAP_STARTX + nX[0], MAP_STARTY + nY[0], "ㅎ");
}
void food()
{
	int i = 0;

	gotoxy(MAP_STARTX + 5, MAP_STARTY + MAP_HEIGHT, "Your Score : ");
	printf("%d", nScore);
	srand((unsigned)time(NULL));

	while (1)
	{
		nFoodX = rand() % (MAP_WIDTH - 2) + 1;
		nFoodY = rand() % (MAP_HEIGHT - 2) + 1;
		for (i = 0; i < nLength; i++)
		{
			if (nX[i] == nFoodX && nY[i] == nFoodY)
				continue;
		}
		gotoxy(MAP_STARTX + nFoodX, MAP_STARTY + nFoodY, "♣");
		nSpeed -= 3;
		break;
	}
}

void pause()
{
	while (1)
	{
		if (_kbhit())
		{
			do { key = _getch(); } while (key == 224);
			if (key == PAUSE)
				break;
		}
		gotoxy(MAP_STARTX + MAP_WIDTH / 2 - 5, MAP_STARTY, "         PAUSE       ");
		Sleep(500);
		gotoxy(MAP_STARTX + MAP_WIDTH / 2 - 5, MAP_STARTY, "                     ");
		Sleep(500);
	}
	draw_map();
	key = 0;
}

void game_over()
{
	gotoxy(MAP_STARTX + MAP_WIDTH / 2 - 3, MAP_STARTY + MAP_HEIGHT / 2 - 3, "GAME OVER!!!");
	while (1)
	{
		if (_kbhit())
			break;
	}
	while (_kbhit()) _getch();
	system("cls");
	title();
}