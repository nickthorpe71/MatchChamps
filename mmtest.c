#include <stdio.h>
#include <stdlib.h>

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

int WINLENGTH = 3;
int HEIGHT = 3;
int WIDTH = 4;

const char HUMANTOKEN = '1';
const char AITOKEN = '2';

char **initializeBoard();
int checkVictory(char **);
int isCellEmpty(char);
void Display(char **);
struct gameBoardCell miniMaxBestMove(char **, char);

int main()
{
  char **board = initializeBoard();

  board[0][0] = '2';
  board[0][1] = '1';
  board[0][2] = '2';
  board[0][3] = '1';
  board[1][0] = '1';
  board[1][1] = '2';
  board[1][2] = '1';
  board[1][3] = '2';
  board[2][0] = '1';
  board[2][1] = 'n';
  board[2][2] = 'o';
  board[2][3] = '2';

  printf("start board\n");
  Display(board);

  struct gameBoardCell bestMove = miniMaxBestMove(board, AITOKEN);
  printf("**------ RESULT -------**\n");
  printf("height: %d\n", bestMove.heightPosition);
  printf("width: %d\n", bestMove.widthPosition);
  printf("player token: %c\n", bestMove.playerToken);
  printf("score: %d\n", bestMove.score);

  return 0;
}

// MINIMAX
struct gameBoardCell miniMaxBestMove(char **board, char playerToken)
{
  printf("------- new func call -------\n");

  struct gameBoardCell moves[HEIGHT * WIDTH + 1];
  int moveCount = 0;

  printf("------- moves array made -------\n");

  struct gameBoardCell tempMove;
  tempMove.playerToken = playerToken;
  tempMove.score = 0;

  printf("------- temp move assigned -------\n");

  // Base case
  // Check to see if game has been won or ended in draw
  int gameStatus = checkVictory(board);

  printf("------- game status checked -------\n");

  printf("\n!!!gameStatus: %d!!!\n", gameStatus);
  printf("\n!!!playerToken: %c!!!\n", playerToken);
  if (gameStatus == 1) // player won
  {
    tempMove.score = -10;
    printf("P WIN - Score: %d\n", tempMove.score);
    return tempMove;
  }
  else if (gameStatus == 2) // ai won
  {
    tempMove.score = 10;
    printf("AI WIN - Score: %d\n", tempMove.score);
    return tempMove;
  }
  else if (gameStatus == 3) // game is a draw
  {
    printf("DRAW - Score: %d\n", tempMove.score);
    return tempMove;
  }

  // Find possible moves
  for (int i = 0; i < HEIGHT; i++)
  {
    printf("currI: %d, ", i);
    for (int j = 0; j < WIDTH; j++)
    {
      if (isCellEmpty(board[i][j]) == 0)
      {
        struct gameBoardCell nextMove;
        nextMove.playerToken = playerToken;

        char tempHolder = board[i][j];

        board[i][j] = playerToken;

        /*collect the score resulted from calling minimax 
      on the opponent of the current player*/
        if (playerToken == AITOKEN)
        {
          nextMove.score = miniMaxBestMove(board, HUMANTOKEN).score;
        }
        else if (playerToken == HUMANTOKEN)
        {
          nextMove.score = miniMaxBestMove(board, AITOKEN).score;
        }

        Display(board);

        // reset the spot to empty
        board[i][j] = tempHolder;

        nextMove.heightPosition = i;
        nextMove.widthPosition = j;

        // push the object to the array
        moves[moveCount] = nextMove;
        moveCount++;
      }
    }
  }

  // printf("Inn score:%d\n", moves[0].score);
  // printf("Inn height:%d\n", moves[0].heightPosition);
  // printf("Inn width:%d\n", moves[0].widthPosition);
  // printf("Inn token:%c\n", moves[0].playerToken);

  printf("---- CHECK MOVES ---- pt:%c move count:%d \n", playerToken, moveCount);

  int bestMoveIndex = 0;
  if (playerToken == AITOKEN)
  {
    int bestScore = -10000;
    for (int i = 0; i < moveCount; i++)
    {
      printf("AI\n");
      printf("Inn score:%d\n", moves[i].score);
      printf("Inn height:%d\n", moves[i].heightPosition);
      printf("Inn width:%d\n", moves[i].widthPosition);
      if (moves[i].score > bestScore)
      {
        bestScore = moves[i].score;
        bestMoveIndex = i;
      }
    }
  }
  else
  {
    int bestScore = 10000;
    for (int i = 0; i < moveCount; i++)
    {
      printf("HUMAN\n");
      printf("Inn score:%d\n", moves[i].score);
      printf("Inn height:%d\n", moves[i].heightPosition);
      printf("Inn width:%d\n", moves[i].widthPosition);
      if (moves[i].score < bestScore)
      {
        bestScore = moves[i].score;
        bestMoveIndex = i;
      }
    }
  }

  return moves[bestMoveIndex];
}

int checkVictory(char **board)
{
  int i, j, k;

  int rowCounterHu = 0;
  int columnCounterHu = 0;
  int upLeftCounterHu = 0;
  int upRightCounterHu = 0;
  int downLeftCounterHu = 0;
  int downRightCounterHu = 0;

  int rowCounterAI = 0;
  int columnCounterAI = 0;
  int upLeftCounterAI = 0;
  int upRightCounterAI = 0;
  int downLeftCounterAI = 0;
  int downRightCounterAI = 0;

  int occupiedCellCount = 0;

  // Check for column matches
  for (i = 0; i < WIDTH; i++)
  {
    columnCounterHu = 0;
    columnCounterAI = 0;

    for (j = 0; j < HEIGHT; j++)
    {
      if (board[j][i] == HUMANTOKEN)
        columnCounterHu++;
      else
        columnCounterHu = 0;
      if (board[j][i] == AITOKEN)
        columnCounterAI++;
      else
        columnCounterAI = 0;

      if (columnCounterHu == WINLENGTH)
        return 1; // (Human) Wins
      if (columnCounterAI == WINLENGTH)
        return 2; // (AI) Wins
    }
  }

  printf("------- columns checked -------\n");

  for (i = 0; i < HEIGHT; i++)
  {
    rowCounterHu = 0;
    rowCounterAI = 0;

    for (j = 0; j < WIDTH; j++)
    {
      upLeftCounterHu = 0;
      upRightCounterHu = 0;
      downLeftCounterHu = 0;
      downRightCounterHu = 0;

      upLeftCounterAI = 0;
      upRightCounterAI = 0;
      downLeftCounterAI = 0;
      downRightCounterAI = 0;

      // Check for row match
      if (board[i][j] == HUMANTOKEN)
        rowCounterHu++;
      else
        rowCounterHu = 0;
      if (board[i][j] == AITOKEN)
        rowCounterAI++;
      else
        rowCounterAI = 0;

      if (rowCounterHu == WINLENGTH)
        return 1; // (Human) Wins
      if (rowCounterAI == WINLENGTH)
        return 2; // (AI) Wins

      printf("------- rows checked -------\n");

      for (k = 0; k < WINLENGTH; k++)
      {
        // Check up left diagonal match
        if (i - k >= 0 && j - k >= 0)
        {
          if (board[i - k][j - k] == HUMANTOKEN)
            upLeftCounterHu++;
          else
            upLeftCounterHu = 0;

          if (board[i - k][j - k] == AITOKEN)
            upLeftCounterAI++;
          else
            upLeftCounterAI = 0;
        }
        // Check up right diagonal match
        if (i - k >= 0 && j + k < WIDTH)
        {
          if (board[i - k][j + k] == HUMANTOKEN)
            upRightCounterHu++;
          else
            upRightCounterHu = 0;
          if (board[i - k][j + k] == AITOKEN)
            upRightCounterAI++;
          else
            upRightCounterAI = 0;
        }
        // Check down left diagonal match
        if (i + k < HEIGHT && j - k >= 0)
        {
          if (board[i + k][j - k] == HUMANTOKEN)
            downLeftCounterHu++;
          else
            downLeftCounterHu = 0;
          if (board[i + k][j - k] == AITOKEN)
            downLeftCounterAI++;
          else
            downLeftCounterAI = 0;
        }
        // Check down right diagonal match
        if (i + k < HEIGHT && j + k < WIDTH)
        {
          if (board[i + k][j + k] == HUMANTOKEN)
            downRightCounterHu++;
          else
            downRightCounterHu = 0;
          if (board[i + k][j + k] == AITOKEN)
            downRightCounterAI++;
          else
            downRightCounterAI = 0;
        }
      }
      if (upLeftCounterHu >= WINLENGTH || upRightCounterHu >= WINLENGTH || downLeftCounterHu >= WINLENGTH || downRightCounterHu >= WINLENGTH)
      {
        return 1; // (Human) Wins
      }
      if (upLeftCounterAI >= WINLENGTH || upRightCounterAI >= WINLENGTH || downLeftCounterAI >= WINLENGTH || downRightCounterAI >= WINLENGTH)
      {
        return 2; // (AI) Wins
      }
    }
  }

  printf("------- diagonal checked -------\n");

  // Check draw
  for (i = 0; i < HEIGHT; i++)
    for (j = 0; j < WIDTH; j++)
      if (isCellEmpty(board[i][j]) == 1)
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

void Indent()
{
  printf("\t\t\t ");
  for (int i = 0; i < WIDTH - 3; i++)
    printf("   ");
}

void Display(char **board)
{
  int i, j;

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
