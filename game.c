#include<stdio.h>
#include<stdlib.h>
#include<time.h>

struct gameBoardCell 
{
	int positionHeight;
	int positionWidth;
	char character;
}
getUserInput();

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

int WINLENGTH = 3;
int HEIGHT = 3;
int WIDTH = 3;
int CURRENTLEVEL = 0;

void run();
void Display(char**);
void calculateNewGameConditions();
int checkBoard(char**, char, int);
char **initializeBoard();
struct gameBoardCell getRandomAIInput(char**);

int main() 
{
	time_t t;

	/* Intializes random number generator */
   	srand((unsigned) time(&t));

	char continuePrompt;
	again:
	run();
	printf("\nType 'n' and press enter to continue\nor anything else and enter to quit: ");
	scanf("%s", &continuePrompt);

	if(continuePrompt == 'n')
	{
		if (CURRENTLEVEL == 9)
		{
			printf("\nYou've WON!\nPress any key and enter to exit the game");
			scanf("%s", &continuePrompt);
			exit(0);
		}
		calculateNewGameConditions();	
		system("clear");
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
	system("clear");
	int turnCount = 0;
	struct gameBoardCell cellInfo;
	char **board = initializeBoard();
	Display(board);
	
	nextTurn:
	// Check if its users turn or AIs turn
	if (turnCount % 2 == 0) // if players turn
		cellInfo = getUserInput(board, turnCount);
	else // if AI turn
		cellInfo = getRandomAIInput(board);
	board[cellInfo.positionHeight][cellInfo.positionWidth] = cellInfo.character;
	system("clear");
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

void calculateNewGameConditions()
{
	CURRENTLEVEL++;

	int increaseHeightOrWidth = rand() % 2;
	if (WIDTH - HEIGHT > 1)
		HEIGHT++;
	else if (HEIGHT - WIDTH > 1)
		WIDTH++;
       	else if (increaseHeightOrWidth == 0)	
		HEIGHT++;
	else
		WIDTH++;

	if (WIDTH > WINLENGTH && HEIGHT > WINLENGTH && (WIDTH + HEIGHT + 1 > (WINLENGTH + 1) * 2))
		WINLENGTH++;

	// also add ai difficulty improvement
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

int checkIfPlayerWon(int score, char input)
{
	if (score >= WINLENGTH)
	{
		printf("Player %c WINS!!!", input);
		return 1;
	}

	return 0;
}

int checkBoard(char **board, char input, int turnCount)
{
	int i,j,k;

	int rowCounter = 0;
	int columnCounter = 0;

	int upLeftCounter = 0;
	int upRightCounter = 0;
	int downLeftCounter = 0;
	int downRightCounter = 0;
	
	// Check for column matches
	for (i = 0; i < WIDTH; i++)
	{
		columnCounter = 0;

		for(j = 0; j < HEIGHT; j++)
		{
			if(board[j][i] == input)
				columnCounter++;
			else
				columnCounter = 0;

			if (checkIfPlayerWon(columnCounter, input) == 1)
                        	return 1;
		}
	}
	
	for (i = 0; i < HEIGHT; i++)
	{
		rowCounter = 0;

		for (j = 0; j < WIDTH; j++)
		{
			upLeftCounter = 0;   	
                        upRightCounter = 0;
                        downLeftCounter = 0;
                        downRightCounter = 0;

			// Check for row match
			if (board[i][j] == input)
				rowCounter++;
			else
				rowCounter = 0;

			if (checkIfPlayerWon(rowCounter, input))
                                return 1;

			for (k = 0; k < WINLENGTH; k++)
			{
				// Check up left diagonal match
				if (i - k >= 0 && j - k >= 0)
				{
					if (board[i-k][j-k] == input)
						upLeftCounter++;
					else
						upLeftCounter = 0;
				}
				// Check up right diagonal match
				if (i - k >= 0 && j + k < WIDTH)
				{
	                       		if (board[i-k][j+k] == input)
						upRightCounter++;
					else
						upRightCounter = 0;
				}
				// Check down left diagonal match
				if (i + k < HEIGHT && j - k >= 0)
				{
					if (board[i+k][j-k] == input)
						downLeftCounter++;
					else
						downLeftCounter = 0;
				}
				// Check down right diagonal match
				if (i + k < HEIGHT && j + k < WIDTH)
				{	
					if (board[i+k][j+k] == input)
						downRightCounter++;
					else 
						downRightCounter = 0;
				}
			}
			if (upLeftCounter >= WINLENGTH || upRightCounter >= WINLENGTH
                        || downLeftCounter >= WINLENGTH || downRightCounter >= WINLENGTH)
                	{
        	                 printf("Player %c WINS!!!", input);
	                         return 1;
	                }
		}
	}

	// Check draw
	if (turnCount == WIDTH * HEIGHT - 1)
	{
		printf("DRAW GAME");
		return 1;
	}

	return 0;
}

struct gameBoardCell getRandomAIInput(char** board)
{
	struct gameBoardCell cellInfo;

	int randHeight = rand() % HEIGHT;
	int randWidth = rand() % WIDTH;

	while (board[randHeight][randWidth] == '1' || board[randHeight][randWidth] == '2')
	{
		randHeight = rand() % HEIGHT;
		randWidth = rand() % WIDTH;
	}

	cellInfo.positionHeight = randHeight;
	cellInfo.positionWidth = randWidth;
	cellInfo.character = P2TOKEN;
	
	return cellInfo;
}

// Take user input
struct gameBoardCell getUserInput(char **board, int turnCount)
{
	int i, j;
	char value;
	struct gameBoardCell cellInfo;
	
	inputAgain:
	if (turnCount%2 == 0)
		printf("\nEnter your choice Player %c:", P1TOKEN);
	else 
		printf("\nEnter your choice Player %c:", P2TOKEN);

	scanf("%s", &value);

	if (value == P1TOKEN || value == P2TOKEN)
	{
		printf("\nInput not valid try again");
		goto inputAgain;
	}	

	for (i = 0; i < HEIGHT; i++)
		for (j = 0; j < WIDTH; j++)
			if (value == board[i][j])
			{
				cellInfo.positionHeight = i;
				cellInfo.positionWidth = j;
				if (turnCount%2 == 0)
					cellInfo.character = P1TOKEN;
				else
					cellInfo.character = P2TOKEN;
				return cellInfo;
			}
			else
			{
				cellInfo.positionHeight = -1;
				cellInfo.positionWidth = -1;
				cellInfo.character = ' ';
			}

	if (cellInfo.positionHeight == -1)
	{
		printf("\nInput not valid try again");
	        goto inputAgain;
	}
	return cellInfo;
}

void Indent()
{
	printf("\t\t\t ");
	for (int i = 0; i < WIDTH - 3; i++)
		printf("   ");
}


void Display(char **board) 
{
	int i,j;

	Indent();
	printf("%s-- MATCH  CHAMPS --%s\n\n", CYN, RESET);
	
	Indent();
	printf("      Level %s%d%s\n", YEL, CURRENTLEVEL + 1, RESET); 
	Indent();
	printf("   Match %s%d%s to win\n\n", GRN, WINLENGTH, RESET);

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
			else if (board[i][j] == P2TOKEN)
				printf("|  %s%c%s  |", BLU, board[i][j], RESET);
			else
				printf("|  %c  |", board[i][j]);
		}

		if (i == HEIGHT - 1)
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
