#include <ship.hpp>

Ship::Ship(int length)
{
  m_shipLength = length;
  std::vector<std::pair<int, int>> m_pairs;
}

int Ship::getLength()
{
  return m_shipLength;
}

void Ship::addCoordinate(int row, int col)
{
  std::pair<int,int> newPair;
  newPair.first = row;
  newPair.second = col;
  m_pairs.push_back(newPair);
}

bool Ship::checkForHit(int col, int row)
{
  for(int i = 0; i < m_pairs.size(); i++)
  {
    if(m_pairs[i].first == row && m_pairs[i].second == col)
    {
      m_shipLength--;
      return true;
    }
  }
  return false;
}

std::vector<int> Ship::returnCoordinates()
{
  std::vector<int> coords;
  for(int i = 0; i < m_pairs.size(); i++)
  {
    coords.push_back(m_pairs[i].first);
    coords.push_back(m_pairs[i].second);
  }
  return coords;
}

void Ship::isInt(int& checkInt)
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

void Ship::isRow(int& checkRow)
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

void Ship::isColumn(char& checkColumn)
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