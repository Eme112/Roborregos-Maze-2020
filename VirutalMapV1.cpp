#include <iostream>
using namespace std;

int iSize = 11, iOrientation = 1, iRow, iColumn, iTilesLeft = 24;
char map[11][11] = {{'.','W','.','W','.','W','.','W','.','W','.'},
                    {'W','R','W',' ','W',' ','.',' ','.',' ','W'},
                    {'.','.','.','.','.','.','.','.','.','.','.'},
                    {'W',' ','.',' ','W',' ','W',' ','W',' ','W'},
                    {'.','.','.','.','.','.','.','.','.','.','.'},
                    {'W',' ','W',' ','W',' ','W',' ','W',' ','W'},
                    {'.','.','.','.','.','W','.','.','.','.','.'},
                    {'W',' ','.',' ','W',' ','W',' ','W',' ','W'},
                    {'.','.','.','.','.','.','.','.','.','.','.'},
                    {'W',' ','W',' ','.',' ','.',' ','.',' ','W'},
                    {'.','W','.','W','.','W','.','W','.','W','.'}};

void printMap(){
  for(int iX = 0 ; iX < iSize ; iX++){
    for(int iY = 0 ; iY < iSize ; iY++){
      cout << map[iX][iY];
      if(iY < iSize-1) cout << " ";
    }
    cout << endl;
  }
}

void Move(){
  map[iRow][iColumn] = 'O';
  
  switch(iOrientation){
    case 1:
      iRow -= 2;
    break;
    case 2:
      iColumn += 2;
    break;
    case 3:
      iRow += 2;
    break;
    case 4:
      iColumn -= 2;
    break;
  }

  if(map[iRow][iColumn] != 'O'){
    iTilesLeft--;
  }
  map[iRow][iColumn] = 'R';
}

void turnLeft(){
  if(iOrientation == 1){
    iOrientation = 4;
  }
  else iOrientation--;

  Move();
}

void turnRight(){
  if(iOrientation == 4){
    iOrientation = 1;
  }
  else iOrientation++;
  Move();
}

void goStraight(){
  Move();
}

void turnBack(){
  if(iOrientation == 3){
    iOrientation = 1;
  }
  else if(iOrientation == 4){
    iOrientation = 2;
  }
  else iOrientation += 2;

  Move();
}

int main() {
  string next;

  for(int iX = 0 ; iX < iSize ; iX++){
    for(int iY = 0 ; iY < iSize ; iY++){
      if(map[iX][iY] == 'R'){
        iRow = iX;
        iColumn = iY;
        break;
      }
    }
  }

  while(iTilesLeft != 0){
    printMap();
    cout << "Tiles Left: " << iTilesLeft << endl;

    if(iOrientation == 1){
      if(map[iRow][iColumn + 1] != 'W' && iColumn + 1 < iSize){
        cout << "Right, Orientation = " << iOrientation << endl;
        turnRight();
      }
      else if(map[iRow - 1][iColumn] != 'W' && iRow > 1){
        cout << "Straight, Orientation = " << iOrientation << endl;
        goStraight();
      }
      else if(map[iRow][iColumn - 1] != 'W' && iColumn > 1){
        cout << "Left, Orientation = " << iOrientation << endl;
        turnLeft();
      }
      else{
        cout << "Back, Orientation = " << iOrientation << endl;
        turnBack();
      }
    }
    else if(iOrientation == 2){
      if(map[iRow + 1][iColumn] != 'W' && iRow + 1 < iSize){
        cout << "Right, Orientation = " << iOrientation << endl;
        turnRight();
      }
      else if(map[iRow][iColumn + 1] != 'W' && iColumn + 1 < iSize){
        cout << "Straight, Orientation = " << iOrientation << endl;
        goStraight();
      }
      else if(map[iRow - 1][iColumn] != 'W' && iRow > 1){
        cout << "Left, Orientation = " << iOrientation << endl;
        turnLeft();
      }
      else{
        cout << "Back, Orientation = " << iOrientation << endl;
        turnBack();
      }
    }
    else if(iOrientation == 3){
      if(map[iRow][iColumn - 1] != 'W' && iColumn > 1){
        cout << "Right, Orientation = " << iOrientation << endl;
        turnRight();
      }
      else if(map[iRow + 1][iColumn] != 'W' && iRow + 1 < iSize){
        cout << "Straight, Orientation = " << iOrientation << endl;
        goStraight();
      }
      else if(map[iRow][iColumn + 1] != 'W' && iColumn + 1 < iSize){
        cout << "Left, Orientation = " << iOrientation << endl;
        turnLeft();
      }
      else{
        cout << "Back, Orientation = " << iOrientation << endl;
        turnBack();
      }
    }
    else{
      if(map[iRow - 1][iColumn] != 'W' && iRow > 1){
        cout << "Right, Orientation = " << iOrientation << endl;
        turnRight();
      }
      else if(map[iRow][iColumn - 1] != 'W' && iColumn > 1){
        goStraight();
      }
      else if(map[iRow + 1][iColumn] != 'W' && iRow + 1 < iSize){
        cout << "Left, Orientation = " << iOrientation << endl;
        turnLeft();
      }
      else{
        cout << "Back, Orientation = " << iOrientation << endl;
        turnBack();
      }
    }

    cin >> next;
    cout << endl << endl << endl;
  }

  printMap();

  return 0;
}