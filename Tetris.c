#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
//#include <conio.h>
#include <time.h>

#define COMMAND_SIZE 256
#define MAIN_X 12 
#define MAIN_Y 23
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
#define PAUSE 112
#define QUIT 27
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
	char id[20];
	int score;
	int speed;
	int level;
	int highScore;
	int dataCount;

}GAMEINFORMATION;

void gameStart(int[][MAIN_X], int[][MAIN_X], BLOCK * blockPointer, GAMEINFORMATION *);
void inputId(GAMEINFORMATION *);
void gotoxy(int x, int y);
void mainTitle();
void setMap(int [][MAIN_X], int [][MAIN_X], GAMEINFORMATION *);
void makeMap(int[][MAIN_X], int [][MAIN_X], BLOCK * blockPointer, GAMEINFORMATION *);
void makeBlock(BLOCK *, int[][MAIN_X], int[][MAIN_X], GAMEINFORMATION *);
void reset(int[][MAIN_X], int[][MAIN_X], BLOCK * blockPointer, GAMEINFORMATION *);
void dropBlock(BLOCK *, int[][MAIN_X], int[][MAIN_X], GAMEINFORMATION *);
int crushBlcok(BLOCK *, int[][MAIN_X], int[][MAIN_X], int, GAMEINFORMATION*);
void inputKey(BLOCK *, int[][MAIN_X], int[][MAIN_X], GAMEINFORMATION *);
void moveBlock(BLOCK *, int[][MAIN_X], int[][MAIN_X], int, GAMEINFORMATION *);
void rotaitonBlock(BLOCK *);
void checkLine(int[][MAIN_X], int [][MAIN_X], BLOCK *, GAMEINFORMATION *);
void gameOver(int[][MAIN_X], int[][MAIN_X], BLOCK * blockPointer, GAMEINFORMATION *);
void saveScore(GAMEINFORMATION *);

int main()
{
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

	int x = 5;
	int y = 4;
	int i = 0;
	int mainOriginal[MAIN_Y][MAIN_X];
	int mainCopy[MAIN_Y][MAIN_X];
	BLOCK *blockPointer;
	blockPointer = (BLOCK*)malloc(sizeof(BLOCK));
	GAMEINFORMATION *infomationPointer = (GAMEINFORMATION*)malloc(sizeof(GAMEINFORMATION));

	srand(time(NULL));

	mainTitle();
	inputId(infomationPointer);

	gameStart(mainOriginal, mainCopy, blockPointer, infomationPointer);

	return 0;
}

void gotoxy(int x, int y)
{
	COORD CursorPosition = {2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
}

void saveScore(GAMEINFORMATION * infoPointer)
{
	FILE * idFPointer;
	FILE * copyPointer;

	char ID[20] = {'0'};
	int score = 0;

	idFPointer = fopen("Data.txt", "r");
	copyPointer = fopen("copyData.txt", "w");

	while (1)
	{
		fscanf(idFPointer, "%s %d", ID, &score);
		
		if (feof(idFPointer))
			break;
		if(strcmp(infoPointer->id, ID) != 0)
			fprintf(copyPointer, "%s %d \n", ID, score);
	}

	fprintf(copyPointer, "%s %d \n", infoPointer->id, infoPointer->highScore);

	fclose(idFPointer);
	fclose(copyPointer);

	idFPointer = fopen("Data.txt", "w");
	copyPointer = fopen("copyData.txt", "r");

	while (1)
	{
		fscanf(copyPointer, "%s %d", ID, &score);

		if (feof(copyPointer))
			break;

		fprintf(idFPointer, "%s %d \n", ID, score);
	}

	fclose(idFPointer);
	fclose(copyPointer);
}

void gameStart(int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X], BLOCK * blockPointer, GAMEINFORMATION *infoPointer)
{
	reset(mainOriginal, mainCopy, blockPointer, infoPointer);

	while (1)
	{
		for (int i = 0; i < 5; i++)
		{
			inputKey(blockPointer, mainOriginal, mainCopy, infoPointer);
			makeMap(mainOriginal, mainCopy, blockPointer, infoPointer);

			Sleep(150);
		}

		dropBlock(blockPointer, mainOriginal, mainCopy, infoPointer);
	}

}

void inputId(GAMEINFORMATION * infoPointer)
{
	char id[20] = {'0'};
	int score = 0;

	gotoxy(MAIN_X, MAIN_Y / 2);
	printf("ID : ");
	fgets(id, 20, stdin);

	for (int i = 0; i < sizeof(id); i++)
	{
		if (id[i] == '\n')
		{
			id[i] = '\0';
			break;
		}
			
	}

	FILE *idFPointer;
	idFPointer = fopen("Data.txt", "r");

	infoPointer->highScore = 0;
	infoPointer->dataCount = 0;

	while (1)
	{
		fscanf(idFPointer, "%s", infoPointer->id);
		fscanf(idFPointer, "%d", &score);

		if (feof(idFPointer))
		{
			fclose(idFPointer);
			idFPointer = fopen("Data.txt", "a");
			fprintf(idFPointer, "%s %d \n", id, score);
			
			strcpy(infoPointer->id, id);
			break;
		}
			
		if (strcmp(id, infoPointer->id) == 0)
		{
			infoPointer->highScore = score;
			strcpy(infoPointer->id, id);
			infoPointer->dataCount = 1;
		}
			
	}
	
	fclose(idFPointer);
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

void setMap(int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X], GAMEINFORMATION *infoPointer)
{
	int i, j;
	infoPointer->level = 1;
	infoPointer->score = 0;
	infoPointer->speed = 1;

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

	gotoxy(MAIN_X + 10, 6);
	printf("HighScore : %d", infoPointer->highScore);

	gotoxy(MAIN_X + 10, 8);
	printf("level : %d", infoPointer->level);

	gotoxy(MAIN_X + 10, 10);
	printf("score : %d", infoPointer->score);

}

void makeMap(int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X], BLOCK * blockPointer, GAMEINFORMATION *infoPointer)
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

	checkLine(mainOriginal, mainCopy, blockPointer, infoPointer);
}

void makeBlock(BLOCK * blockPointer, int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X], GAMEINFORMATION *infoPointer)
{
	int i = 0, j = 0;
	
	for (i = 0; i < MAIN_X; i++)
	{
		if (mainCopy[2][i] == BLOCK_FIXED)
		{
			gameOver(mainOriginal, mainCopy, blockPointer, infoPointer);
		}
			
	}

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
		
		blockPointer->block[0][2] = ACTIVITY_BLOCK;
		blockPointer->block[1][2] = ACTIVITY_BLOCK;
		blockPointer->block[2][2] = ACTIVITY_BLOCK;
		blockPointer->block[2][1] = ACTIVITY_BLOCK;

		blockPointer->xPosition -= 1;

		break;

	case 1:		//L			2							

		blockPointer->block[0][1] = ACTIVITY_BLOCK;
		blockPointer->block[1][1] = ACTIVITY_BLOCK;
		blockPointer->block[2][1] = ACTIVITY_BLOCK;
		blockPointer->block[2][2] = ACTIVITY_BLOCK;

		blockPointer->xPosition -= 1;

		break;

	case 2:		//S										
		
		blockPointer->block[0][1] = ACTIVITY_BLOCK;
		blockPointer->block[1][1] = ACTIVITY_BLOCK;
		blockPointer->block[1][2] = ACTIVITY_BLOCK;
		blockPointer->block[2][2] = ACTIVITY_BLOCK;
		
		blockPointer->xPosition -= 1;
		break;

	case 3:			//O									

		blockPointer->block[1][1] = ACTIVITY_BLOCK;
		blockPointer->block[1][2] = ACTIVITY_BLOCK;
		blockPointer->block[2][1] = ACTIVITY_BLOCK;
		blockPointer->block[2][2] = ACTIVITY_BLOCK;

		blockPointer->xPosition -= 1;

		break;

	case 4:				//Z								

		blockPointer->block[0][2] = ACTIVITY_BLOCK;
		blockPointer->block[1][2] = ACTIVITY_BLOCK;
		blockPointer->block[1][1] = ACTIVITY_BLOCK;
		blockPointer->block[2][1] = ACTIVITY_BLOCK;

		blockPointer->xPosition -= 1;
		break;

	case 5:			//T									

		for (i = 0; i < BLOCK_SIZE_X-1; i++)
			blockPointer->block[0][i] = ACTIVITY_BLOCK;
		blockPointer->block[1][1] = ACTIVITY_BLOCK;

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

void reset(int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X], BLOCK * blockPointer, GAMEINFORMATION * infoPointer)
{
	setMap(mainOriginal, mainCopy, infoPointer);
	makeBlock(blockPointer, mainOriginal, mainCopy, infoPointer);
	makeMap(mainOriginal, mainCopy, blockPointer, infoPointer);
}

void dropBlock(BLOCK * blockPointer, int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X], GAMEINFORMATION *infoPointer)
{
	moveBlock(blockPointer, mainOriginal, mainCopy, DOWN, infoPointer);
	//blockPointer->yPosition += 1;

	crushBlcok(blockPointer, mainOriginal, mainCopy, DOWN, infoPointer);
}

int crushBlcok(BLOCK * blockPointer, int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X], int directionKey, GAMEINFORMATION *infoPointer)
{
	int i, j;
	int k = 3;
	int count = 0;
	int testMain1[MAIN_Y][MAIN_X];
	int testMain2[MAIN_Y][MAIN_X];
	int testBlock[BLOCK_SIZE_Y][BLOCK_SIZE_X];

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
			for (i = 0; i < 5; i++)
				inputKey(blockPointer, mainOriginal, mainCopy, infoPointer);

			for (i = 0; i < BLOCK_SIZE_Y; i++)
			{
				for (j = 0; j < BLOCK_SIZE_X; j++)
				{
					if (blockPointer->block[i][j] == ACTIVITY_BLOCK)
						blockPointer->block[i][j] = BLOCK_FIXED;
				}
			}
			makeMap(mainOriginal, mainCopy, blockPointer, infoPointer);
			makeBlock(blockPointer, mainOriginal ,mainCopy, infoPointer);

			return 1;
		}

		break;

	case UP:

		for (i = 0; i < BLOCK_SIZE_Y; i++)
		{
			for (j = 0; j < BLOCK_SIZE_X; j++)
			{
				testBlock[j][k] = blockPointer->block[i][j];
			}
			k--;
		}

		for (i = 0; i < BLOCK_SIZE_Y; i++)
		{
			for (j = 0; j < BLOCK_SIZE_X; j++)
			{
				if (testBlock[i][j] == ACTIVITY_BLOCK)
				{
					testMain2[i + blockPointer->yPosition][j + blockPointer->xPosition] = testBlock[i][j];
				}

			}
		}

		for (i = 0; i < MAIN_Y; i++)
		{
			for (j = 0; j < MAIN_X; j++)
			{
				if (testMain1[i][j] != testMain2[i][j])
				{
					if (testMain1[i][j] == WALL || testMain1[i][j] == BLOCK_FIXED)
						count = 1;
				}
			}
		}
		
		if (count == 1)
			return 1;

		break;
		
	default:
		break;
	}

	return 0;
}

void inputKey(BLOCK * blockPointer, int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X], GAMEINFORMATION *infoPointer)
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

				moveBlock(blockPointer, mainOriginal, mainCopy, LEFT, infoPointer);
				break;

			case RIGHT:

				moveBlock(blockPointer, mainOriginal, mainCopy, RIGHT, infoPointer);
				break; 

			case DOWN:

				moveBlock(blockPointer, mainOriginal, mainCopy, DOWN, infoPointer);
				crushBlcok(blockPointer, mainOriginal, mainCopy, DOWN, infoPointer);
				break;

			case UP:

				if (!crushBlcok(blockPointer, mainOriginal, mainCopy, UP, infoPointer))
					rotaitonBlock(blockPointer);

				break;
			}
		}
		else if (key != KEY_CHECK)
		{
			switch (key)
			{

			case SPACE:

				while (1)
				{
					if (crushBlcok(blockPointer, mainOriginal, mainCopy, DOWN, infoPointer) == 1)
						break;

					moveBlock(blockPointer, mainOriginal, mainCopy, DOWN, infoPointer);
				}
				break;

			case PAUSE:
				
				do {
					key = _getch();

				} while (key == KEY_CHECK);

				while (1)
				{
					if (_kbhit())
					{
						key = _getch();
						if (key != KEY_CHECK)
							if (key == PAUSE)
								break;
					}
						break;

					Sleep(1);
				}
				break;

			case QUIT:
				
					saveScore(infoPointer);

				exit(0);
				break;
			}
		}
	}
}

void moveBlock(BLOCK *blockPointer, int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X], int directionKey, GAMEINFORMATION *infoPointer)
{
	int i, j;

	switch (directionKey)
	{
	case LEFT:

		if(!crushBlcok(blockPointer, mainOriginal, mainCopy, LEFT, infoPointer))
			blockPointer->xPosition -= 1;

		break;

	case RIGHT:

		if(!crushBlcok(blockPointer, mainOriginal, mainCopy, RIGHT, infoPointer))
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
		blockPointer->yPosition += infoPointer->speed;

		break;

	default:
		break;
	}
}

void rotaitonBlock(BLOCK *blockPointer)
{
	int i, j;
	int k = 3;
	int copyArr[BLOCK_SIZE_Y][BLOCK_SIZE_X];

	for (i = 0; i < BLOCK_SIZE_Y; i++)
	{
		for (j = 0; j < BLOCK_SIZE_X; j++)
		{
			copyArr[j][k] = blockPointer->block[i][j];
		}
		k--;
	}

	for (i = 0; i < BLOCK_SIZE_Y; i++)
	{
		for (j = 0; j < BLOCK_SIZE_X; j++)
		{
			blockPointer->block[i][j] = copyArr[i][j];
		}
	}
}

void checkLine(int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X], BLOCK *blockPointer, GAMEINFORMATION *infoPointer)
{
	int count = 0;
	int i, j;
	int checkPosition = 0;
	int testScore = 0;

	for (i = MAIN_Y; i > 3; i--)
	{
		count = 0;

		for (j = 0; j < MAIN_X; j++)
		{
			if (mainCopy[i][j] == BLOCK_FIXED)
				count++;
		}
		if (count == MAIN_X - 2)
			break;
	}

	checkPosition = i;

	if (count == MAIN_X - 2)
	{
		for (i = checkPosition -1; i > 3; i--)
		{
			for (j = 0; j < MAIN_X; j++)
			{
				if (mainCopy[i][j] != WALL)
				{
					mainOriginal[i + 1][j] = mainOriginal[i][j];
				}
			}
		}

		makeMap(mainOriginal, mainCopy, blockPointer, infoPointer);
		
		infoPointer->score += 50;
		if(infoPointer->highScore < infoPointer->score)
			infoPointer->highScore = infoPointer->score;
	}

	infoPointer->level = infoPointer->score / 500;

	switch (infoPointer->level)
	{
	case 1:
		infoPointer->speed = 1;
		break;

	case 2:
		infoPointer->speed = 2;
		break;

	case 3:
		infoPointer->speed = 3;
		break;
	}
	
	if (!infoPointer->dataCount)
	{
		gotoxy(MAIN_X + 10, 6);
		printf("HighScore : %d", infoPointer->highScore);
	}
	
	gotoxy(MAIN_X + 10, 8);
	printf("level : %d", infoPointer->level);
	
	gotoxy(MAIN_X + 10, 10);
	printf("score : %d", infoPointer->score);

	

}

void gameOver(int mainOriginal[][MAIN_X], int mainCopy[][MAIN_X], BLOCK * blockPointer, GAMEINFORMATION *infoPointer)
{
	int i;

	gotoxy(MAIN_X, MAIN_Y / 2);
	printf("■■■■■■■■■■■■■■■■■■");
	Sleep(100);

	for (i = 0; i < 6; i++)
	{
		gotoxy(MAIN_X, (MAIN_Y / 2) + i);
		printf("■");

		gotoxy(MAIN_X + (MAIN_Y/2) + 6, (MAIN_Y / 2) + i);
		printf("■");

		Sleep(10);
	}

	gotoxy(MAIN_X + 2, MAIN_Y / 2 + 2);
	printf("+------------------------+");
	//Sleep(100);

	gotoxy(MAIN_Y + -8, MAIN_Y / 2 + 3);
	printf("G  A  M  E  O  V  E  R !!");
	Sleep(100);

	gotoxy(MAIN_X + 2, MAIN_Y / 2 + 4);
	printf("+------------------------+");
	//Sleep(100);

	Sleep(100);
	gotoxy(MAIN_X, (MAIN_Y / 2) + 6);
	printf("■■■■■■■■■■■■■■■■■■");

	saveScore(infoPointer);

	gotoxy(MAIN_X, MAIN_Y / 2 + 7);
	printf("\n종료하시겠습니까? Y/N ");

	switch (getchar())
	{
	case 'Y':
	case 'y':

		exit(0);

		break;

	case'N':
	case 'n':

		gameStart(mainOriginal, mainCopy, blockPointer, infoPointer);

		break;
	default:

		break;
	}
}
