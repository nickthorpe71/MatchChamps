#include<stdio.h>
#include<stdlib.h>

struct gameBoardCell 
{
	int positionHeight;
	int positionWidth;
	char character;
}
inputValue();

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

const char P1TOKEN = '1';
const char P2TOKEN = '2';

const int WINLENGTH = 3;
const int HEIGHT = 4;
const int WIDTH = 4;

void run();
void Display(char**);
int checkBoard(char**, char, int);
char **initializeBoard();

int main() 
{
	char reStart;
	again:
	run();
	printf("\nTo play again press 1 and enter \notherwise hit any key: ");
	scanf("%s", &reStart);
	if(reStart == '1')
	{
		system("cls");
		goto again;
	}	
	else
	{
		exit(0);
	}
	return 0;	
}

void run() 
{
	int turnCount = 0;
	struct gameBoardCell cellInfo;
	char **board = initializeBoard();
	Display(board);
	
	nextTurn:
	cellInfo = inputValue(board, turnCount);
	board[cellInfo.positionHeight][cellInfo.positionWidth] = cellInfo.character;
	system("cls");
	Display(board);
	if(checkBoard(board, cellInfo.character, turnCount)==1)
	{
		// free board memory
		for(int i = 0; i < HEIGHT; i++)
		{
			free(board[i]);
		}
		free(board);
	}
	else
	{
		turnCount++;
		goto nextTurn;
	}
}

char **initializeBoard()
{
	int i,j;

	char currentNumberAsChar = 'a';
	char **board = malloc(sizeof(char*) * HEIGHT);

	for (i = 0; i < HEIGHT; i++) 
		board[i] = malloc(sizeof(char*) * WIDTH);

	for (i = 0; i < HEIGHT; i++)
		for(j = 0; j < WIDTH; j++)
		{
			board[i][j] = currentNumberAsChar;
			if (currentNumberAsChar == 'z')
				currentNumberAsChar = 'A';
			else
				currentNumberAsChar++;
		}

	return board;
}

int checkBoard(char **board, char input, int turnCount)
{
	// TODO: rework checkBoard into one effecitent loop
	int i,j;
	
	for (i = 0; i < HEIGHT; i++)
	{
		int rowCounter = 0;
		int columnCounter = 0;
		for (j = 0; j < WIDTH; j++)
		{
			if (board[i][j] == input)
			{
				rowCounter++;
			}
			if (board[j][i] == input)
			{
				columnCounter++;
			}
		}

		if (rowCounter == WINLENGTH || columnCounter == WINLENGTH)
		{
			printf("%c WINS!!!", input);
		        return 1;
		}
	}

	// Diaglonals
	// TODO: set these checks in a loop to check regardless of size
	// see if its possible to incorporate into the loop above (likely is)
	if (board[0][0] == input && board[1][1] == input && board[2][2] == input)
	{
		printf("%c WINS!!!", input);
		return 1;
	}
	else if (board[0][2] == input && board[1][1] == input && board[2][0] == input)
	{		
		printf("%c WINS!!!", input);
		return 1;
	}
	// Check draw
	else if (turnCount == WIDTH * HEIGHT - 1)
	{
		printf("DRAW GAME");
		return 1;
	}

	return 0;
}

// take input
struct gameBoardCell inputValue(char **board, int turnCount)
{
	int i, j;
	char value;
	struct gameBoardCell cellInfo;
	
	inputAgain:
	if (turnCount%2 == 0)
	{
		printf("\nEnter your choice Player %c:", P1TOKEN);
	} 
	else 
	{
		printf("\nEnter your choice Player %c:", P2TOKEN);
	}

	scanf("%s", &value); 

	for(i = 0; i < HEIGHT; i++)
	{
		for(j = 0; j < WIDTH; j++)
		{
			if(value == board[i][j])
			{
				cellInfo.positionHeight = i;
				cellInfo.positionWidth = j;
				if(turnCount%2 == 0)
				{
					cellInfo.character = P1TOKEN;
				}
				else
				{
					cellInfo.character = P2TOKEN;
				}
				return cellInfo;
			}
			else
			{
				cellInfo.positionHeight = -1;
				cellInfo.positionWidth = -1;
				cellInfo.character = ' ';
			}
		}
	}
	if(cellInfo.positionHeight == -1)
	{
		printf("\nInput is not valid");
	        goto inputAgain;
	}
	return cellInfo;
}


void Display(char **board) 
{
	int i,j;

	printf("\n");

	for (i = 0; i < WIDTH - 1; i++)
		printf("\t    ");

	printf("%sTIC     TAC     MOE%s\n\n", CYN, RESET);
	
	for (i = 0; i < WIDTH - 1; i++)
		printf("\t    ");

	printf(" Length to win : %s%d%s\n\n", GRN, WINLENGTH, RESET);

	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
			if (j == 0)
				printf("\n\t\t\t|-----|");
			else
				printf("|-----|");

		for (j = 0; j < WIDTH; j++)
		{
			if (j == 0)
                		printf("\n\t\t\t");

			if (board[i][j] == P1TOKEN)
				printf("|  %s%c%s  |", RED, board[i][j], RESET);	
			else if(board[i][j] == P2TOKEN)
				printf("|  %s%c%s  |", BLU, board[i][j], RESET);
			else
				printf("|  %c  |", board[i][j]);
		}

		if (i == WIDTH - 1)
		{
			for (j = 0; j < WIDTH; j++)
                		if (j == 0)
                			printf("\n\t\t\t|-----|");
                		else
                			printf("|-----|");
			printf("\n");
		}

	}
}
