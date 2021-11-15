#include <player.hpp>
#include<iostream>
#include<string>
Player::Player()
{
  shipDirection = "";
  //shipPosition = "";
  shipRow = 0;
}

void Player::createBoard()
{
  for(int x = 1; x < m_rows; x++)
  {
    for(int y = 1; y < m_cols; y++)
    {
      gameBoard[x][y] = '#'; //BLANK BOARD SPACE
      attackBoard[x][y] = '#'; //BLANK BOARD SPACE
    }
  }
}

void Player::printBoard()
{
  int rowLabel = 1;
  std::cout << "\tA\tB\tC\tD\tE\tF\tG\tH\n"; //COLUMN LABELS

  for(int x = 1; x < m_rows; x++)
  {
    std::cout << rowLabel << "\t";
    rowLabel++;

    for(int y = 1; y < m_cols; y++)
    {
      std::cout << gameBoard[x][y] << "\t";
    }

    std::cout << std::endl;
  }
}

char Player::find(int row, char col) //will return the value of the board at the specified location (ex: find(3,C))
{
  //to do
  if (row >= 1 && row < 9)
  {
    if(col == 'A' || col == 'B' || col == 'C' || col == 'D' || col == 'E' || col == 'F' || col == 'G' || col == 'H')
    {
        if(col == 'A')
        {
            return (gameBoard[row][1]);
        }

        if(col == 'B')
        {
            return (gameBoard[row][2]);
        }

        if(col == 'C')
        {
            return (gameBoard[row][3]);
        }

        if(col == 'D')
        {
            return (gameBoard[row][4]);
        }

        if(col == 'E')
        {
            return (gameBoard[row][5]);
        }

        if(col == 'F')
        {
           return (gameBoard[row][6]);
        }

        if(col == 'G')
        {
            return (gameBoard[row][7]);
        }

        if(col == 'H')
        {
            return (gameBoard[row][8]);
        }
      }
      else
      {
        return 'F';
      }
    }
}

char Player::findOnAttackBoard(int row, char col)
{
  if (row >= 1 && row < 9)
  {
    if(col == 'A' || col == 'B' || col == 'C' || col == 'D' || col == 'E' || col == 'F' || col == 'G' || col == 'H')
    {
        if(col == 'A')
        {
            return (attackBoard[row][1]);
        }

        if(col == 'B')
        {
            return (attackBoard[row][2]);
        }

        if(col == 'C')
        {
            return (attackBoard[row][3]);
        }

        if(col == 'D')
        {
            return (attackBoard[row][4]);
        }

        if(col == 'E')
        {
            return (attackBoard[row][5]);
        }

        if(col == 'F')
        {
           return (attackBoard[row][6]);
        }

        if(col == 'G')
        {
            return (attackBoard[row][7]);
        }

        if(col == 'H')
        {
            return (attackBoard[row][8]);
        }
      }
      else
      {
        return 'F';
      }
    }
}

void Player::setShipCount(int numShips)
{
  m_shipCount = numShips;
}

void Player::fire(int row, char col)
{
  bool retry = true;
  while(retry == true)
  {
    char potentialRetry = findOnAttackBoard(row, col);
    if (potentialRetry == 'M')
    {
      std::cout << "You have already missed at this location, please retry:\n";
      while(col!='A'&&col!='B'&&col!='C'&&col!='D'&&col!='E'&&col!='F'&&col!='G'&&col!='H')
      {
        std::cout << "Player 1, which column will you fire at (A - H)? ";
        isColumn(col);
      }
      row = 0;
      while(row <= 0 || row > 8)
      {
        std::cout << "Player 1, which row will you fire at (1 - 8)? ";
        isRow(row);
      }
    }
    else
    {
      retry = false;
    }
  }

  char location = find(row, col);
  char location2 = findOnAttackBoard(row, col);
  //do a find on attackBoard, if it is an M then allow the player to refire
  if(location == 'F')
  {
    std::cout<<"Location is invalid "<<std::endl;
  }
  if(location == '#')
  {
    std::cout << "You have missed at location " << col << row << "\n";
    attackBoard[row][convertColumn(col)] = 'M'; // UPDATES ATTACK BOARD
    return;
  }
  else if(location == 'M')
  {
    std::cout << "You already missed at location " << col << row <<'\n';
    return;
  }
  else if(location2 == 'H' || location2 == 'X')
  {
    std::cout << "You already hit at location " << col << row <<'\n';
    return;
  }
  else if(location == 'S')
  {
    for(int i = 0; i < m_ships.size(); i++)
    {
      if(m_ships[i].checkForHit(convertColumn(col), row) == true)
      {
        std::cout << "You have hit at location " << col << row << "\n";
        attackBoard[row][convertColumn(col)] = 'H'; //updates attack board

        if(m_ships[i].getLength() == 0)
        {
          m_shipCount--; //Decreases the ship count once the ship is erased from the vector
          std::cout << "You sunk a ship! \n";
          std::vector<int> shipCoords = m_ships[i].returnCoordinates();
          for(int i = 0; i < shipCoords.size(); i = i+2)
          {
            gameBoard[shipCoords[i]][shipCoords[i+1]] = 'X';
          }
        }
      }
    }
  }
}



bool Player::checkForWin()
{
  if(m_shipCount == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void Player::addShip(int numShips)
{
  for(int i = 1; i <= numShips; i++)
  {
    //following code asks user in which direction he would like the ship to be placed
    bool correctInput = false;
    while(correctInput == false)
    {

      std::cout << "\n\nWhich direction would you like to place ship " << i <<"?\nREMINDER: SHIP LENGTH IS BASED ON SHIP NUMBER \n(Horizontal(H) or vertical (V))\n\n>";
      std::cin >> shipDirection;
      changeCase(shipDirection);

      //asks user where bottom-most or left-most coordinate of his ship placement
      if(shipDirection == "HORIZONTAL" || shipDirection == "H")
      {
        std::cout << "What is the left-most column position you would like your ship to be placed? (A-H)\n>";
        isChar(shipColumn);
        std::cout << "What is the left-most row position you would like your ship to be placed? (1-8)\n>";
        isInt(shipRow);
        shipColumn = toupper(shipColumn);

        if(validCoordinate(shipRow, shipColumn, shipDirection, i) == true) //this needs to check if ALL shipPosition are valid
        {
          correctInput = true;
          Ship tempShip(i);
          for(int j = 1; j <= i; j++)
          {
            gameBoard[shipRow][(convertColumn(shipColumn))+j-1] = 'S';
            tempShip.addCoordinate(shipRow, (convertColumn(shipColumn))+j-1);
          }
          m_ships.push_back(tempShip);
        }
        else
        {
          std::cout << "Sorry, invalid coordinate.\n";
        }
      }

      else if(shipDirection == "VERTICAL" || shipDirection == "V")
      {
        std::cout << "What is the bottom-most column position you would like your ship to be placed? (A-H)\n>";
        isChar(shipColumn);
        std::cout << "What is the bottom-most row position you would like your ship to be placed? (1-8)\n>";
        isInt(shipRow);
        shipColumn = toupper(shipColumn);


        if(validCoordinate(shipRow, shipColumn, shipDirection, i) == true)
        {
          correctInput = true;
          Ship tempShip(i);
          for(int j = 1; j <= i; j++)
          {
            gameBoard[shipRow-j+1][convertColumn(shipColumn)] = 'S';
            tempShip.addCoordinate(shipRow-j+1, (convertColumn(shipColumn)));
          }
          m_ships.push_back(tempShip);
        }
        else
        {
          std::cout << "Sorry, invalid coordinate.\n";
        }
      }
      else
      {
        std::cout << "Please enter a valid ship direction\n";
      }

    } //end while loop
  } //end for loop
} //end addShip()

bool Player::validCoordinate(int shipRow, char shipColumn, std::string shipDirection, int shipSize)
{
  int goodCord = 0;
  if((shipColumn == 'A' || shipColumn == 'B' || shipColumn == 'C' || shipColumn == 'D' || shipColumn == 'E' || shipColumn == 'F' || shipColumn == 'G' || shipColumn == 'H') && (shipRow == 1 || shipRow == 2 || shipRow == 3 || shipRow == 4 || shipRow == 5 || shipRow == 6 || shipRow == 7 || shipRow == 8))
  {
    if(shipDirection == "HORIZONTAL" || shipDirection == "H")
    {
      for(int i = 1; i <= shipSize; i++)
      {
        if(gameBoard[shipRow][(convertColumn(shipColumn))+i-1] == '#')
        {
          goodCord++;
        }
      }
    }
    else if(shipDirection == "VERTICAL" || shipDirection == "V")
    {
      for(int i = 1; i <= shipSize; i++)
      {
        if(gameBoard[shipRow-i+1][convertColumn(shipColumn)] == '#')
        {
          goodCord++;
        }
      }
    }
  }
  if(goodCord == shipSize)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void Player::changeCase(std::string &word)
{
    int length = 0;
    std::locale loc;
    length = word.length();
    std::string swap = word;
    for (int i = 0; i < length; i++)
    {
        char upper = toupper(word[i]);
        swap[i] = upper;
    }
    word = swap;
}

int Player::convertColumn(char col)
{
  int num = 0;
  if(col == 'A' || col == 'B' || col == 'C' || col == 'D' || col == 'E' || col == 'F' || col == 'G' || col == 'H')
  {
      if(col == 'A')
      {
        num = 1;
          return (num);
      }

      if(col == 'B')
      {
        num =2;
          return (num);
      }

      if(col == 'C')
      {
        num =3;
          return (num);
      }

      if(col == 'D')
      {
        num = 4;
          return (num);
      }

      if(col == 'E')
      {
        num = 5;
          return (num);
      }

      if(col == 'F')
      {
        num = 6;
         return (num);
      }

      if(col == 'G')
      {
        num = 7;
          return (num);
      }

      if(col == 'H')
      {
        num = 8;
          return (num);
      }
    }
}

void Player::printAttackBoard()
{
  int rowLabel = 1;
  std::cout << "\tA\tB\tC\tD\tE\tF\tG\tH\n"; //COLUMN LABELS

  for(int x = 1; x < m_rows; x++)
  {
    std::cout << rowLabel << "\t";
    rowLabel++;

    for(int y = 1; y < m_cols; y++)
    {
      std::cout << attackBoard[x][y] << "\t";
    }

    std::cout << std::endl;
  }
}

void Player::isInt(int& checkInt)
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

void Player::isChar(char& checkChar)
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

void Player::isRow(int& checkRow)
{
  std::cin >> checkRow;

  while (std::cin.fail())
  {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "ERROR: Please enter an integer (1 - 8): ";
    std::cin >> checkRow;
  }
}

void Player::isColumn(char& checkColumn)
{
  std::cin >> checkColumn;

  while (std::cin.fail())
  {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "ERROR: Please enter a letter (A - H): ";
    std::cin >> checkColumn;
  }
  toupper(checkColumn);
}