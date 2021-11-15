#ifndef Ship_h
#define Ship_h
#include <limits>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>

class Ship
{
public:
  /**
  * @pre: valid ship object is created
  * @post: initializes shipLength and Vector Pairs for a ship
  * @param: int length - length of the ship
  * @return: nothing
  **/
  Ship(int length);

  /**
  * @pre: valid ship object exists
  * @post: returns length of a ship
  * @param: none
  * @return: integer length of ship
  **/
  int getLength();

  /**
  * @pre: valid row and column passed in
  * @post: returns length of a ship
  * @param: int col - column of coordinate
  * @param: int row - row of coordinate
  * @return: none
  **/
  void addCoordinate(int col, int row);

  /**
  * @pre: valid row and column passed in
  * @post: returns boolean value based on if hit or not
  * @param: int col - column of coordinate
  * @param: int row - row of coordinate
  * @return: boolean true/false of hit
  **/
  bool checkForHit(int col, int row);

  /**
  * @pre: boards exist and ship coordinates have been updated
  * @post: returns vector integer of coordinates
  * @param: none
  * @return: vector integer of coordinates
  **/
  std::vector<int> returnCoordinates();

  /**
  * @pre: int provided input
  * @post: verifies if int is okay or not
  * @param: int& checkInt - int to check if int or not
  * @return: nothing
  **/
  void isInt(int& checkInt);

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
  int m_shipLength = 0;
  std::vector< std::pair<int,int> > m_pairs; //this is a list of ordered pairs
};
#endif /* Ship_h */