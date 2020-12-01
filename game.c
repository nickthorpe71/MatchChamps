#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct gameBoardCell
{
  int heightPosition;
  int widthPosition;
  char playerToken;
  int score;
} getUserInput();

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

const char HUMANTOKEN = '1';
const char AITOKEN = '2';

int WINLENGTH = 3;
int HEIGHT = 3;
int WIDTH = 3;
int CURRENTLEVEL = 0;

void run();
char **initializeBoard();
void Display(char **);
void calculateNewGameConditions();
int checkVictory(char **, char);
int isCellEmpty(char);
struct gameBoardCell getRandomAIInput(char **);
struct gameBoardCell aiMove(char **, char);
struct gameBoardCell miniMaxBestMove(char **, char, struct gameBoardCell *, int);

int main()
{
  time_t t;

  /* Intializes random number generator */
  srand((unsigned)time(&t));

  char continuePrompt;
again:
  run();
  printf("\nType 'n' and press enter to continue\nor anything else and enter to quit: ");
  scanf("%s", &continuePrompt);

  if (continuePrompt == 'n')
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
  struct gameBoardCell nextMove;
  char **board = initializeBoard();
  Display(board);

nextTurn:
  // Check if its users turn or AIs turn
  if (turnCount % 2 == 0) // if players turn
    nextMove = getUserInput(board, turnCount);
  else // if AI turn
    nextMove = aiMove(board, AITOKEN);
  printf("nmHeight: %d, nmWidth: %d", nextMove.heightPosition, nextMove.widthPosition);
  board[nextMove.heightPosition][nextMove.widthPosition] = nextMove.playerToken;
  system("clear");
  Display(board);
  int checkWinner = checkVictory(board, nextMove.playerToken);
  if (checkWinner != 0)
  {
    // free board memory
    for (int i = 0; i < HEIGHT; i++)
    {
      free(board[i]);
    }
    free(board);

    if (checkWinner == 1)
      printf("Human WON!");
    else if (checkWinner == 2)
      printf("AI WON!!");
    else
      printf("DRAWWWW");
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
  int i, j;

  char currentNumberAsChar = 'a';
  char **board = malloc(sizeof(char *) * HEIGHT);

  for (i = 0; i < HEIGHT; i++)
    board[i] = malloc(sizeof(char *) * WIDTH);

  for (i = 0; i < HEIGHT; i++)
    for (j = 0; j < WIDTH; j++)
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
    if (input == HUMANTOKEN)
      return 1; // P1 (Human) Wins
    return 2;   // P2 (AI) Wins
  }

  return 0;
}

int checkVictory(char **board, char playerToken)
{
  int i, j, k;

  int rowCounter = 0;
  int columnCounter = 0;

  int upLeftCounter = 0;
  int upRightCounter = 0;
  int downLeftCounter = 0;
  int downRightCounter = 0;

  int occupiedCellCount = 0;

  // Check for column matches
  for (i = 0; i < WIDTH; i++)
  {
    columnCounter = 0;

    for (j = 0; j < HEIGHT; j++)
    {
      if (board[j][i] == playerToken)
        columnCounter++;
      else
        columnCounter = 0;

      if (checkIfPlayerWon(columnCounter, playerToken) == 1)
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
      if (board[i][j] == playerToken)
        rowCounter++;
      else
        rowCounter = 0;

      if (checkIfPlayerWon(rowCounter, playerToken))
        return 1;

      for (k = 0; k < WINLENGTH; k++)
      {
        // Check up left diagonal match
        if (i - k >= 0 && j - k >= 0)
        {
          if (board[i - k][j - k] == playerToken)
            upLeftCounter++;
          else
            upLeftCounter = 0;
        }
        // Check up right diagonal match
        if (i - k >= 0 && j + k < WIDTH)
        {
          if (board[i - k][j + k] == playerToken)
            upRightCounter++;
          else
            upRightCounter = 0;
        }
        // Check down left diagonal match
        if (i + k < HEIGHT && j - k >= 0)
        {
          if (board[i + k][j - k] == playerToken)
            downLeftCounter++;
          else
            downLeftCounter = 0;
        }
        // Check down right diagonal match
        if (i + k < HEIGHT && j + k < WIDTH)
        {
          if (board[i + k][j + k] == playerToken)
            downRightCounter++;
          else
            downRightCounter = 0;
        }
      }
      if (upLeftCounter >= WINLENGTH || upRightCounter >= WINLENGTH || downLeftCounter >= WINLENGTH || downRightCounter >= WINLENGTH)
      {
        if (playerToken == HUMANTOKEN)
          return 1; // P1 (Human) Wins
        return 2;   // P2 (AI) Wins
      }
    }
  }

  // Check draw
  for (i = 0; i < WIDTH; i++)
    for (j = 0; j < HEIGHT; j++)
      if (isCellEmpty(board[i][j]) == 0)
        occupiedCellCount++;

  if (occupiedCellCount == WIDTH * HEIGHT)
  {
    return 3;
  }

  return 0;
}

int isCellEmpty(char token)
{
  if (token == HUMANTOKEN || token == AITOKEN)
    return 1; // false
  return 0;   // true
}

struct gameBoardCell aiMove(char **board, char playerToken)
{

  struct gameBoardCell *moves;
  moves = (struct gameBoardCell *)malloc(sizeof(struct gameBoardCell) * 300000);
  int moveCount = 0;

  struct gameBoardCell bestMove = miniMaxBestMove(board, AITOKEN, moves, moveCount);

  free(moves);
  return bestMove;
}

// MINIMAX
struct gameBoardCell miniMaxBestMove(char **board, char playerToken, struct gameBoardCell *moves, int moveCount)
{
  int i, j;

  struct gameBoardCell nextMove;
  nextMove.playerToken = playerToken;
  nextMove.score = 0;

  // Base case
  // Check to see if game has been won or ended in draw
  int gameStatus = checkVictory(board, playerToken);
  if (gameStatus == 1) // player won
  {
    nextMove.score = -10;
    return nextMove;
  }
  else if (gameStatus == 2) // ai won
  {
    nextMove.score = 10;
    return nextMove;
  }
  else if (gameStatus == 3 || moveCount == 280000) // game is a draw or we cant search further into the future
  {
    return nextMove;
  }

  // Find possible moves
  for (i = 0; i < HEIGHT; i++)
    for (j = 0; j < WIDTH; j++)
    {
      if (isCellEmpty(board[i][j]) == 0)
      {
        nextMove.heightPosition = i;
        nextMove.widthPosition = j;

        char tempHolder = board[i][j];

        board[i][j] = playerToken;
        if (playerToken == AITOKEN)
          nextMove.score = miniMaxBestMove(board, HUMANTOKEN, moves, moveCount).score;
        else
          nextMove.score = miniMaxBestMove(board, AITOKEN, moves, moveCount).score;

        moves[moveCount] = nextMove;
        moveCount++;

        board[i][j] = tempHolder;
      }
    }

  // Select best move
  int bestMoveIndex = 0;
  if (playerToken == AITOKEN)
  {
    int bestScore = -4000000;
    for (i = 0; i < moveCount; i++)
      if (moves[i].score > bestScore)
        bestMoveIndex = i;
  }
  else
  {
    int bestScore = 4000000;
    for (i = 0; i < moveCount; i++)
      if (moves[i].score < bestScore)
        bestMoveIndex = i;
  }

  // Return the best move
  return moves[bestMoveIndex];
}

struct gameBoardCell getRandomAIInput(char **board)
{
  struct gameBoardCell cellInfo;

  int randHeight = rand() % HEIGHT;
  int randWidth = rand() % WIDTH;

  while (board[randHeight][randWidth] == '1' || board[randHeight][randWidth] == '2')
  {
    randHeight = rand() % HEIGHT;
    randWidth = rand() % WIDTH;
  }

  cellInfo.heightPosition = randHeight;
  cellInfo.widthPosition = randWidth;
  cellInfo.playerToken = AITOKEN;

  return cellInfo;
}

// Take user input
struct gameBoardCell getUserInput(char **board, int turnCount)
{
  int i, j;
  char value;
  struct gameBoardCell cellInfo;

inputAgain:
  if (turnCount % 2 == 0)
    printf("\nEnter your choice Player %c:", HUMANTOKEN);
  else
    printf("\nEnter your choice Player %c:", AITOKEN);

  scanf("%s", &value);

  if (value == HUMANTOKEN || value == AITOKEN)
  {
    printf("\nInput not valid try again");
    goto inputAgain;
  }

  for (i = 0; i < HEIGHT; i++)
    for (j = 0; j < WIDTH; j++)
      if (value == board[i][j])
      {
        cellInfo.heightPosition = i;
        cellInfo.widthPosition = j;
        if (turnCount % 2 == 0)
          cellInfo.playerToken = HUMANTOKEN;
        else
          cellInfo.playerToken = AITOKEN;
        return cellInfo;
      }
      else
      {
        cellInfo.heightPosition = -1;
        cellInfo.widthPosition = -1;
        cellInfo.playerToken = ' ';
      }

  if (cellInfo.heightPosition == -1)
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
  int i, j;

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

      if (board[i][j] == HUMANTOKEN)
        printf("|  %s%c%s  |", RED, board[i][j], RESET);
      else if (board[i][j] == AITOKEN)
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
