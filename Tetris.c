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
#define BLOCK_SIZE_X 4

#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define UP 72 // rotaion 회전
#define SPACE 32 // hard drop
#define KEY_CHECK 224

typedef struct
{
	int block[BLOCK_SIZE_Y][BLOCK_SIZE_X];
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
int crushBlcok(BLOCK *, int[][MAIN_X], int[][MAIN_X], int);
void inputKey(BLOCK *, int[][MAIN_X], int[][MAIN_X]);
void moveBlock(BLOCK *, int[][MAIN_X], int[][MAIN_X], int);


int main()
{
	int x = 5;
	int y = 4;
	int i = 0;
	int mainOriginal[MAIN_Y][MAIN_X];
	int mainCopy[MAIN_Y][MAIN_X];
	BLOCK *blockPointer;
	blockPointer = (BLOCK*)malloc(sizeof(BLOCK));

	srand(time(NULL));

	mainTitle();

	reset(mainOriginal, mainCopy, blockPointer);
	
	while (1)
	{
		for (i = 0; i < 5; i++)
		{
			inputKey(blockPointer, mainOriginal, mainCopy);
			makeMap(mainOriginal, mainCopy, blockPointer);
			
			Sleep(150);
		}
		
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
		
	system("cls");
}

void makeMap(int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X], BLOCK * blockPointer)
{
	int i, j;

	for (i = 0; i < BLOCK_SIZE_Y; i++)
	{
		for (j = 0; j < BLOCK_SIZE_X; j++)
		{		
			if(blockPointer->block[i][j] == ACTIVITY_BLOCK || blockPointer->block[i][j] == BLOCK_FIXED)
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

			if (mainOriginal[i][j] == ACTIVITY_BLOCK)
				mainOriginal[i][j] = BLOCK_POSSIBLE;

		
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

		for (i = 0; i < BLOCK_SIZE_X-1; i++)
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
	moveBlock(blockPointer, mainOriginal, mainCopy, DOWN);
	//blockPointer->yPosition += 1;

	crushBlcok(blockPointer, mainOriginal, mainCopy, DOWN);
}

int crushBlcok(BLOCK * blockPointer, int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X], int directionKey)
{
	int i, j;
	int count = 0;
	int testMain1[MAIN_Y][MAIN_X];
	int testMain2[MAIN_Y][MAIN_X];

	for (i = 0; i < MAIN_Y; i++)
	{
		for (j = 0; j < MAIN_X; j++)
		{
			testMain1[i][j] = mainCopy[i][j];
			testMain2[i][j] = mainCopy[i][j];
		}
	}

	for (i = 0; i < BLOCK_SIZE_Y; i++)
	{
		for (j = 0; j < BLOCK_SIZE_X; j++)
		{
			if (blockPointer->block[i][j] == ACTIVITY_BLOCK)
			{	  
				testMain2[i + blockPointer->yPosition][j + blockPointer->xPosition] = blockPointer->block[i][j];
				testMain1[i + blockPointer->yPosition][j + blockPointer->xPosition] = blockPointer->block[i][j];
			}
		}
	}

	switch (directionKey)
	{
	case LEFT:

		blockPointer->xPosition -= 1;

		for (i = 0; i < BLOCK_SIZE_Y; i++)
		{
			for (j = 0; j < BLOCK_SIZE_X; j++)
			{
				if (blockPointer->block[i][j] == ACTIVITY_BLOCK)
				{
					testMain2[i + blockPointer->yPosition][j + blockPointer->xPosition] = blockPointer->block[i][j];
				}

			}
		}

		for (i = 0; i < MAIN_Y; i++)
		{
			for (j = 0; j < MAIN_X; j++)
			{
				if (testMain1[i][j] != testMain2[i][j])
				{
					if (testMain1[i][j] == BLOCK_FIXED || testMain1[i][j] == WALL)
						count = 1;
				}
			}
		}
		
		blockPointer->xPosition += 1;
		
		if (count == 1 || blockPointer->xPosition == 0)
			return 1;
		
			
		

		break;

	case RIGHT:

		blockPointer->xPosition += 1;

		for (i = 0; i < BLOCK_SIZE_Y; i++)
		{
			for (j = 0; j < BLOCK_SIZE_X; j++)
			{
				if (blockPointer->block[i][j] == ACTIVITY_BLOCK)
				{
					testMain2[i + blockPointer->yPosition][j + blockPointer->xPosition] = blockPointer->block[i][j];
				}

			}
		}

		for (i = 0; i < MAIN_Y; i++)
		{
			for (j = 0; j < MAIN_X; j++)
			{
				if (testMain1[i][j] != testMain2[i][j])
				{
					if (testMain1[i][j] == BLOCK_FIXED || testMain1[i][j] == WALL)
						count = 1;
				}
			}
		}
		blockPointer->xPosition -= 1;

		if (count == 1 || blockPointer->xPosition == 11)
			return 1;
		

		break;

	case DOWN:
		blockPointer->yPosition += 1;

		for (i = 0; i < BLOCK_SIZE_Y; i++)
		{
			for (j = 0; j < BLOCK_SIZE_X; j++)
			{
				if (blockPointer->block[i][j] == ACTIVITY_BLOCK)
				{
					testMain2[i + blockPointer->yPosition][j + blockPointer->xPosition] = blockPointer->block[i][j];
				}
				
			}
		}

		for (i = 0; i < MAIN_Y; i++)
		{
			for (j = 0; j < MAIN_X; j++)
			{
				if (testMain1[i][j] != testMain2[i][j])
				{
					if (testMain1[i][j] == BLOCK_FIXED || testMain1[i][j] == WALL)
						count = 1;
				
				}
			}
		}
		
		blockPointer->yPosition -= 1;

		if (count == 1)
		{
			
			for (i = 0; i < BLOCK_SIZE_Y; i++)
			{
				for (j = 0; j < BLOCK_SIZE_X; j++)
				{
					if (blockPointer->block[i][j] == ACTIVITY_BLOCK)
						blockPointer->block[i][j] = BLOCK_FIXED;
				}
			}
			makeMap(mainOriginal, mainCopy, blockPointer);
			makeBlock(blockPointer);

			if (blockPointer->yPosition == 1)
			{

				Sleep(10000);

				exit(0);
			}

			return 1;
		}

		

		break;

		
	default:
		break;
	}

	return 0;
}

void inputKey(BLOCK * blockPointer, int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X])
{
	int key = 0;
	
	if (_kbhit())
	{
		key = _getch();

		if (key == KEY_CHECK)
		{
			do {
				key = _getch();

			} while (key == KEY_CHECK);

			switch (key)
			{
			case LEFT:

				moveBlock(blockPointer, mainOriginal, mainCopy, LEFT);
				//crushBlcok(blockPointer, mainOriginal, mainCopy, LEFT);
				break;

			case RIGHT:

				moveBlock(blockPointer, mainOriginal, mainCopy, RIGHT);
				//crushBlcok(blockPointer, mainOriginal, mainCopy, RIGHT);
				break;

			case DOWN:

				moveBlock(blockPointer, mainOriginal, mainCopy, DOWN);
				crushBlcok(blockPointer, mainOriginal, mainCopy, DOWN);
				break;

			case UP:

				break;
			}
		}
		else if(key != KEY_CHECK)
		{
			switch (key)
			{

			case SPACE:

				while (1)
				{
					if (crushBlcok(blockPointer, mainOriginal, mainCopy, DOWN) == 1)
						break;

					moveBlock(blockPointer, mainOriginal, mainCopy, DOWN);
				}

				break;
			}
		}
	}
	
	
}

void moveBlock(BLOCK *blockPointer, int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X], int directionKey)
{
	int i, j;

	switch (directionKey)
	{
	case LEFT:

		if(!crushBlcok(blockPointer, mainOriginal, mainCopy, LEFT))
			blockPointer->xPosition -= 1;

		break;

	case RIGHT:

		if(!crushBlcok(blockPointer, mainOriginal, mainCopy, RIGHT))
			blockPointer->xPosition += 1;

		break;

	case DOWN:

		for (i = 0; i < BLOCK_SIZE_Y; i++)
		{
			for (j = 0; j < BLOCK_SIZE_X; j++)
			{
				if (blockPointer->block[i][j] == ACTIVITY_BLOCK)
					mainOriginal[i + blockPointer->yPosition][j + blockPointer->xPosition] = BLOCK_POSSIBLE;

			}
		}
		blockPointer->yPosition += 1;

		break;

	default:
		break;
	}
}
