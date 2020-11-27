#include<stdio.h>
#include<stdlib.h>

struct gameBoardCell 
{
	int positionHeight;
	int positionWidth;
	char character;
}
inputValue();

const int WINLENGTH = 3;
const int HEIGHT = 3;
const int WIDTH = 3;

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
	int currentNumber = 1;
	char **board = malloc(sizeof(char*) * HEIGHT); 
	for(int i = 0; i < HEIGHT; i++) 
	{
		board[i] = malloc(sizeof(char*) * WIDTH);
	}

	for(int i = 0; i < HEIGHT; i++)
	{
		for(int j = 0; j < WIDTH; j++)
		{
			char currentNumberAsChar = currentNumber + '0';
			board[i][j] = currentNumberAsChar;
			currentNumber++;
		}
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
		printf("\nEnter your choice X:");
	} 
	else 
	{
		printf("\nEnter your choice O:");
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
					cellInfo.character = 'X';
				}
				else
				{
					cellInfo.character = 'O';
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
	printf("\t\t\tTIC    TAC    TOE\n");
	printf( "\n\t\t\t     |     |     ");
	printf("\n\t\t\t  %c  |  %c  |  %c  ",board[0][0],board[0][1],board[0][2]);
	printf( "\n\t\t\t-----|-----|-----");
	printf("\n\t\t\t  %c  |  %c  |  %c  ",board[1][0],board[1][1],board[1][2]);
        printf( "\n\t\t\t-----|-----|-----");	
	printf("\n\t\t\t  %c  |  %c  |  %c  ",board[2][0],board[2][1],board[2][2]);
	printf( "\n\t\t\t     |     |     \n");
	printf("Player 1 Symbol: X\n");
	printf("Player 2 Symbol: O\n");
}
