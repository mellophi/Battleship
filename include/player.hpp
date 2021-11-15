#ifndef Player_h
#define Player_h
#include <ship.hpp>

class Player
{
public:
  /**
  * @pre: valid player object is created
  * @post: initializes shipDirection and shipRow
  * @param: none
  * @return: nothing
  **/
  Player();

  /**
  * @pre: valid player object is created
  * @post: creates the board for the respective player
  * @param: none
  * @return: nothing
  **/
  void createBoard();

  /**
  * @pre: valid player object is created
  * @post: prints board to the respective player
  * @param: none
  * @return: nothing
  **/
  void printBoard();

  /**
  * @pre: valid player object is created
  * @post: changes locations on players attack board depending on where the player chose to attack
  * @param: row - the row where the player wants to attack
  * @param: col - the column where the player wants to attack
  * @return: nothing
  **/
  void fire(int row, char col);

  /**
  * @pre: verify input from user, and make sure the input for column is upper case.
  * @post: find the location that user input
  * @param: int row of coordinate
  * @param: char column of coordinate
  * @return: char array
  **/
  char find(int row, char col);

  /**
  * @pre: verify input from user, and make sure the input for column is upper case.
  * @post: find the location that user inputs (on attack board))
  * @param: int row and char column
  * @return: char
  **/
  char findOnAttackBoard(int row, char col);

  /**
  * @pre: valid player object is created
  * @post: setting number of ships for our private member variable
  * @param: numShips - the number of ships the player is going to be placing on the board
  * @return: nothing
  **/
  void setShipCount(int numShips);

  /**
  * @pre: valid player object is created
  * @post: sets the ships on the board and changes all valid coordinates to an 'S'
  * @param: numbShips - the number of ships the player is going to be placing on the board
  * @return: nothing
  **/
  void addShip(int numbShips);

  /**
  * @pre: valid player object is created
  * @post: checks if the coordinates passed in are all within the board and of char '#'
  * @param: shipRow - passed in row to check, shipColumn - passed in column to check, shipDirection - direction to check if valid, shipSize - size of the current ship of which to check
  * @return: true or false depending on if the corresponding coordinates are valid
  **/
  bool validCoordinate(int shipRow, char shipColumn, std::string shipDirection, int shipSize);

  /**
  * @pre: valid player object is created
  * @post: lets us know the respective int for every columns char
  * @param: col - the column we want to convert to an integer
  * @return: returns a char's corresponding int
  **/
  int convertColumn(char col);

  /**
  * @pre: valid player object is created
  * @post: tells us if this player won
  * @param: none
  * @return: weather or not this player won
  **/
  bool checkForWin();

  /**
  * @pre: valid player object and attack board is created
  * @post: prints the attack board for the respective player
  * @param: none
  * @return: nothing
  **/
  void printAttackBoard();

  /**
  * @pre: int provided input
  * @post: verifies if int is okay or not
  * @param: int& checkInt - int to check if int or not
  * @return: nothing
  **/
  void isInt(int& checkInt);

  /**
  * @pre: char provided input
  * @post: verifies if char is okay or not
  * @param: char& checkChar - character to check if char or not
  * @return: nothing
  **/
  void isChar(char& checkChar);

  /**
  * @pre: int provided input
  * @post: verifies if row integer is okay or not
  * @param: int& checkRow - int to check if row is int or not
  * @return: nothing
  **/
  void isRow(int& checkRow);

  /**
  * @pre: char provided input
  * @post: verifies if column character is okay or not
  * @param: char& checkColumn - int to check if column is char or not
  * @return: nothing
  **/
  void isColumn(char& checkColumn);


private:
  const static int m_rows = 9;
  const static int m_cols = 9;
  char gameBoard[m_cols][m_rows];
  char attackBoard[m_cols][m_rows];
  int m_shipCount = 0;
  std::vector<Ship> m_ships;
  std::string shipDirection;
  int shipRow;
  char shipColumn;

  /**
  * @pre: valid player object exists
  * @post: changes all string to upper case
  * @param: std::string& word
  * @return: nothing
  **/
  void changeCase(std::string& word);
};
#endif /* Player_h */