#include <player.hpp>

/**
* @pre: int provided input
* @post: verifies if int is okay or not
* @param: int& checkInt - int to check if int or not
* @return: nothing
**/
void isInt(int& checkInt)
{
  std::cin >> checkInt;

  while(std::cin.fail())
  {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "ERROR: Please enter an integer (1 - 5): ";
    std::cin >> checkInt;
  }
}

/**
* @pre: int provided input
* @post: verifies if row integer is okay or not
* @param: int& checkRow - int to check if row is int or not
* @return: nothing
**/
void isRow(int& checkRow)
{
  std::cin >> checkRow;

  while(std::cin.fail())
  {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "ERROR: Please enter an integer (1 - 8): ";
    std::cin >> checkRow;
  }
}

/**
* @pre: char provided input
* @post: verifies if column character is okay or not
* @param: char& checkColumn - int to check if column is char or not
* @return: nothing
**/
void isColumn(char& checkColumn)
{
  std::cin >> checkColumn;

  while(std::cin.fail())
  {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "ERROR: Please enter a letter (A - H): ";
    std::cin >> checkColumn;
  }
  toupper(checkColumn);
}

/**
* @pre: char provided input
* @post: verifies if char is okay or not
* @param: char& checkChar - character to check if char or not
* @return: nothing
**/
void isChar(char& checkChar)
{
  std::cin >> checkChar;
  int temp = int(checkChar);

  while(temp < 65 || temp > 72)
  {
    std::cout << "ERROR: Please enter a column character (A - H): ";
    std::cin >> checkChar;
    temp = int(checkChar);
  }
}

int main(int argc, const char* argv[])
{
  std::cout << "\n\nWelcome to Battleship!\n\n";
  int numberShips = 0;

  while(numberShips <= 0 || numberShips > 5)
  {
    std::cout << "How many ships do you want to play with (1 - 5)? ";
    isInt(numberShips);
  }

  Player player1;
  player1.setShipCount(numberShips);
  Player player2;
  player2.setShipCount(numberShips);
  player1.createBoard();
  std::cout << "\n\nPLAYER 1'S GAME BOARD:\n\n";
  player1.printBoard();
  player2.createBoard();
  std::cout << "\n\nPLAYER 2'S GAME BOARD:\n\n";
  player2.printBoard();
  std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n====================\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPlayer 2 look away, it's Player 1's turn to add ships \n\n\n====================\n\n\n";
  std::cout << "\nPlayer 1: ";
  player1.addShip(numberShips);
  std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n====================\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPlayer 1 look away, it's Player 2's turn to add ships \n\n\n====================\n\n\n";
  std::cout << "\nPlayer 2: ";
  player2.addShip(numberShips);

  bool gameOver = false;
  int row;
  char column;
  while (!gameOver)
  {
    std::cout << "\n====================\nPlayer 2 look away\n\n\n====================\n\n\n\n\n\n\n\n";

    std::cout << "\n\nPLAYER 1'S ATTACK BOARD:\n";
    player2.printAttackBoard();
    std::cout << "\nPLAYER 1'S BOARD:\n";
    player1.printBoard();
    column = 'Z';
    while (column!='A'&&column!='B'&&column!='C'&&column!='D'&&column!='E'&&column!='F'&&column!='G'&&column!='H')
    {
      std::cout << "Player 1, which column will you fire at (A - H)? ";
      isColumn(column);
    }
    row = 0;
    while (row <= 0 || row > 8)
    {
      std::cout << "Player 1, which row will you fire at (1 - 8)? ";
      isRow(row);
    }
    player2.fire(row, column); //player 1 attacks player 2's game board

    if(player2.checkForWin())
    {
      gameOver = true;
      std::cout << "\n\nPlayer 1 sunk all of Player 2's battleships! Player 1 wins! \n";
      player2.printAttackBoard();
      break;
    }


    std::cout << "\n====================\nPlayer 1 look away\n\n\n====================\n\n\n\n\n\n\n\n";

    std::cout << "\n\nPLAYER 2'S ATTACK BOARD:\n";
    player1.printAttackBoard();
    std::cout << "\nPLAYER 2'S BOARD:\n";
    player2.printBoard();
    column = 'Z';
    while (column!='A'&&column!='B'&&column!='C'&&column!='D'&&column!='E'&&column!='F'&&column!='G'&&column!='H')
    {
      std::cout << "Player 2, which column will you fire at (A - H)? ";
      isColumn(column);
    }
    row = 0;
    while (row <= 0 || row > 8)
    {
      std::cout << "Player 2, which row will you fire at (1 - 8)? ";
      isRow(row);
    }
    player1.fire(row, column); //player 2 attacks player 1's game board

    if(player1.checkForWin())
    {
      gameOver = true;
      std::cout << "\n\nPlayer 2 sunk all of Player 1's battleships! Player 2 wins! \n";
      player1.printAttackBoard();
    }
  }
  return 0;
}