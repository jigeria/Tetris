#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define COMMAND_SIZE 256
#define MAIN_X 12 
#define MAIN_Y 20
#define GAME_SIZE_WIDTH 10
#define GAME_SIZE_HEIGHT 19 
#define X 5
#define Y 4

#define WALL 1 // 벽
#define BLOCK_POSSIBLE 0 // 움직일 수 있는 공간
#define BLOCK_FIXED -1 // 고정된 블럭
#define ACTIVITY_BLOCK 10 //실제 블럭
#define BLOCK_INITALIZATION  9 //블럭 배열 중 실제 블럭이 아닌 공간

#define BLOCK_SIZE_Y 4
#define BLOCK_SIZE_X 3

typedef struct
{
	int block[4][3];
	int xPosition;
	int yPosition;
	int type;

}BLOCK;

typedef struct
{
	int score;
	int speed;
	int level;

}GAMEINFORMATION;

void gotoxy(int x, int y);
void mainTitle();
void setMap(int [][MAIN_X], int [][MAIN_X]);
void makeMap(int[][MAIN_X], int [][MAIN_X], BLOCK * blockPointer);
void makeBlock(BLOCK *);
void reset(int[][MAIN_X], int[][MAIN_X], BLOCK * blockPointer);
void dropBlock(BLOCK *, int[][MAIN_X], int[][MAIN_X]);
void crushBlcok(BLOCK *, int[][MAIN_X], int[][MAIN_X]);


int main()
{
	int x = 5;
	int y = 4;
	int mainOriginal[MAIN_Y][MAIN_X];
	int mainCopy[MAIN_Y][MAIN_X];
	BLOCK *blockPointer;
	blockPointer = (BLOCK*)malloc(sizeof(BLOCK));

	srand(time(NULL));

	mainTitle();

	reset(mainOriginal, mainCopy, blockPointer);
	
	while (1)
	{
		makeMap(mainOriginal, mainCopy, blockPointer);
		Sleep(1000);
		
		dropBlock(blockPointer, mainOriginal, mainCopy);
	}
	
	getchar();

	return 0;
}

void gotoxy(int x, int y)
{
	COORD CursorPosition = {2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
}

void mainTitle()
{
	int x = 5;
	int y = 4;

	char command[COMMAND_SIZE] = { '\0', };
	int lines = 30;
	int cols = 80;
	sprintf(command, "mode con: lines=%d cols=%d", lines, cols);
	system(command);

	gotoxy(x, y + 0); printf("■■■■■■■■■■■"); Sleep(100);
	gotoxy(x, y + 1); printf("■■■■■■■■■■■"); Sleep(100);
	gotoxy(x, y + 2); printf("■■"); Sleep(100);
	gotoxy(x + 9, y + 2); printf("■■"); Sleep(100);
	gotoxy(x, y + 3); printf("■■■■■■■■■■■"); Sleep(100);
	gotoxy(x, y + 4); printf("■■■■■■■■■■■"); Sleep(100);
	gotoxy(x, y + 5); printf("MADE BY : sang-min"); Sleep(100);
	gotoxy(x + 2, y + 2); printf(" T E T R I S"); Sleep(100);
	gotoxy(x, y + 7); printf("Please Enter Any Key to Start.."); Sleep(100);
	gotoxy(x, y + 9); printf("  ↑   : Turn"); Sleep(100);
	gotoxy(x, y + 10); printf("←  → : Left / Right"); Sleep(100);
	gotoxy(x, y + 11); printf("  ↓   : Soft Drop"); Sleep(100);
	gotoxy(x, y + 12); printf(" SPACE : Hard Drop"); Sleep(100);
	gotoxy(x, y + 13); printf("   P   : Pause"); Sleep(100);
	gotoxy(x, y + 14); printf("  ESC  : Quit"); Sleep(100);
	gotoxy(0, 0);

	_kbhit();
	_getch();
	//while (_kbhit()) _getch();
	
	system("cls");
}

void setMap(int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X])
{
	int i, j;

	for (i = 0; i < MAIN_Y; i++)
	{
		for (j = 0; j < MAIN_X; j++)
		{
			mainCopy[i][j] = 100;
			mainOriginal[i][j] = 0;
		}
	}

	for (i = 0; i < MAIN_Y; i++)
	{
		mainOriginal[i][0] = WALL;
		mainOriginal[i][MAIN_X-1] = WALL;
		
	}

	for (i = 1; i < MAIN_X-1; i++)
	{
		mainOriginal[MAIN_Y - 1][i] = WALL;
	}
		
	Sleep(1000);
	system("cls");
}

void makeMap(int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X], BLOCK * blockPointer)
{
	int i, j;

	for (i = 0; i < BLOCK_SIZE_Y; i++)
	{
		for (j = 0; j < BLOCK_SIZE_X; j++)
		{		
			mainOriginal[i + blockPointer->yPosition][j + blockPointer->xPosition] = blockPointer->block[i][j];
		}
	}

	for (i = 0; i < MAIN_Y; i++)
	{
		for (j = 0; j < MAIN_X; j++)
		{
			if (mainOriginal[i][j] != mainCopy[i][j])
			{
				gotoxy(X + j, Y + i);
	
				switch (mainOriginal[i][j]) 
				{
				case BLOCK_POSSIBLE :

					printf(" ");

					break;

				case WALL :	
				
					printf("▧");
					
					break;

				case ACTIVITY_BLOCK :
					
					printf("■");

					break;
					
				case BLOCK_FIXED :

					printf("□");

					break;
				}
			}
		}
	}
	
	for (i = 0; i < MAIN_Y; i++)
	{
		for (j = 0; j < MAIN_X; j++)
		{
			mainCopy[i][j] = mainOriginal[i][j];
		}
	}
}

void makeBlock(BLOCK * blockPointer)
{
	int i = 0, j = 0;
	
	for (i = 0; i < BLOCK_SIZE_Y; i++)
	{
		for (j = 0; j < BLOCK_SIZE_X; j++)
		{
			blockPointer->block[i][j] = 0;
		}
	}

	blockPointer->type = rand() % 7;
	blockPointer->xPosition = (MAIN_X / 2)-1;
	blockPointer->yPosition = 0;

	switch (blockPointer->type)
	{
	case 0:		//J
		
		blockPointer->block[1][1] = ACTIVITY_BLOCK;
		blockPointer->block[2][1] = ACTIVITY_BLOCK;
		blockPointer->block[3][1] = ACTIVITY_BLOCK;
		blockPointer->block[3][0] = ACTIVITY_BLOCK;

		break;

	case 1:		//L										

		blockPointer->block[1][1] = ACTIVITY_BLOCK;
		blockPointer->block[2][1] = ACTIVITY_BLOCK;
		blockPointer->block[3][1] = ACTIVITY_BLOCK;
		blockPointer->block[3][2] = ACTIVITY_BLOCK;

		blockPointer->xPosition -= 1;

		break;

	case 2:		//S										
		
		blockPointer->block[1][0] = ACTIVITY_BLOCK;
		blockPointer->block[2][0] = ACTIVITY_BLOCK;
		blockPointer->block[2][1] = ACTIVITY_BLOCK;
		blockPointer->block[3][1] = ACTIVITY_BLOCK;
		
		break;

	case 3:			//O									

		blockPointer->block[2][0] = ACTIVITY_BLOCK;
		blockPointer->block[2][1] = ACTIVITY_BLOCK;
		blockPointer->block[3][0] = ACTIVITY_BLOCK;
		blockPointer->block[3][1] = ACTIVITY_BLOCK;


		break;

	case 4:				//Z								

		blockPointer->block[1][1] = ACTIVITY_BLOCK;
		blockPointer->block[2][1] = ACTIVITY_BLOCK;
		blockPointer->block[2][0] = ACTIVITY_BLOCK;
		blockPointer->block[3][0] = ACTIVITY_BLOCK;

		break;

	case 5:			//T									

		for (i = 0; i < BLOCK_SIZE_X; i++)
			blockPointer->block[2][i] = ACTIVITY_BLOCK;
		blockPointer->block[3][1] = ACTIVITY_BLOCK;

		blockPointer->xPosition -= 1;

		break;

	case 6:				//I								

		for(i=0; i<BLOCK_SIZE_Y; i++)
			blockPointer->block[i][1] = ACTIVITY_BLOCK;

		break;

	default :
		puts("ERROR!!");

		return;
	}

	
}

void reset(int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X], BLOCK * blockPointer)
{
	setMap(mainOriginal, mainCopy);
	makeBlock(blockPointer);
	makeMap(mainOriginal, mainCopy, blockPointer);
}

void dropBlock(BLOCK * blockPointer, int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X])
{
	blockPointer->yPosition += 1;

	crushBlcok(blockPointer, mainOriginal, mainCopy);
}

void crushBlcok(BLOCK * blockPointer, int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X])
{
	int i, j, k;

	for (k = 0; k < BLOCK_SIZE_X; k++)
	{
		if (mainOriginal[blockPointer->yPosition + BLOCK_SIZE_Y][blockPointer->xPosition+k] == WALL || mainCopy[blockPointer->yPosition + BLOCK_SIZE_Y][blockPointer->xPosition + k] == BLOCK_FIXED)
		{
			for (i = 0; i < BLOCK_SIZE_Y; i++)
			{
				for (j = 0; j < BLOCK_SIZE_X; j++)
				{
					if (blockPointer->block[i][j] == ACTIVITY_BLOCK )
						blockPointer->block[i][j] = BLOCK_FIXED;
				}
			}
	
			makeMap(mainOriginal, mainCopy, blockPointer);
			makeBlock(blockPointer);
		}
	}

	
		
	
}
