/*
 * EE 299 Lab 3 Master Code
 * 
 * Feifan Qiao, Bert Zhao, Mitchell Szeto
 * 
 * This program is the master version of the two-player game.
 * Game board for player 1 is stored in this program.
 * Bombs placed by player 1 is transferred to the slave board.
 * Player starts on the top-left corner, if he hits a bomb,
 * he loses one life. The person first reaches the bottom-right
 * corner alive wins.
 * 
 * Last modified: 11/13/2018
*/
#include <Wire.h>

#define SIZE 4          
#define MAX_BOMBS 4      // max number of bombs allowed
#define MAX_LIVES 3      // max number of lives
#define LEFTBRACKET 60   // ascii value for left bracket
#define RIGHTBRACKET 62  // ascii value for right bracket
#define COMMA 44         // ascii value for comma
#define CONVERT 48       // difference between ascii value and actual value

// Matrix key
// 0 = empty space
// 1 = hidden bomb
// 2 = hit bomb
// 3+ = player position
int matrix[SIZE][SIZE];
int matrix2[SIZE][SIZE];  // temporily stores the bomb locations
bool nextMove = false;
int limit = 4;
int nextX = 0;
int nextY = 0;
int curX = 0;
int curY = 0;
int life = MAX_LIVES;
int bIndex = 0;
int prevData = 0;
int row = -1;
int col = -1;
int x = 0;
int z = 0;
bool status1First = true;
bool status2First = true;
// 0--player 1 setting up bombs, 1--player 2 setting up bombs, 
// 2--current game active, 3--current game not active
int gameStatus = 0;   


void setup() {
 Serial.begin(9600);
 Wire.begin();
 Serial.println("Player 1 now setting bombs");
 Serial.println("Enter in the form <row,col>");
 Serial.println("Please use zero indexing"); 
}

void loop() {
  if (gameStatus == 0) {                // Player 1 is now setting bombs
    if (Serial.available() > 0) {
      int data = Serial.read();
      if (prevData == LEFTBRACKET) {
        row = data - CONVERT;
      } else if (prevData == COMMA) {
        col = data - CONVERT;
      }
      if (data == RIGHTBRACKET) {
        matrix2[row][col] = 1;
        if (setBombs(row, col)) {
          Serial.print("Row: ");
          Serial.println(row);
          Serial.print("Col: ");
          Serial.println(col);
          Serial.println();
          bIndex++;
          Wire.beginTransmission(4);
          Wire.write(8);                // command for giving out player 1 bomb location
          Wire.write(row);
          Wire.write(col);
          Wire.endTransmission(4);
        }      
        row = -1;
        col = -1;
      }
      prevData = data;
    }
    if (bIndex == (MAX_BOMBS)) {
      gameStatus = 1;
    }
  } else if (gameStatus == 1) {          // Player 2 is now setting bombs
    if (status1First) {
      Wire.beginTransmission(4);
      Wire.write(9);                     // 9 is the command to signal player 2 setting bomb
      Wire.endTransmission(4);
      status1First = false;
      delay(20000);                      // delay 20s, wait for player 2 to finish inputting bombs
      for (int i = 0; i < 16; i++) {
        Wire.requestFrom(4,1);
        while (Wire.available()) {
          matrix[x][z] = Wire.read();     // x represents row while z represents column
          z++;
          if (z == SIZE) {
            z = 0;
            x++;
          }
        }
      }
      gameStatus = 2;
    }
  } else if (gameStatus == 2) {        // game is actively played
    if (status2First) {                // this part is only run one time
      createMap();
      Serial.print("you have ");
      Serial.print(life);
      Serial.println(" lives");
      status2First = false;
      Wire.beginTransmission(4);
      Wire.write(7);                   // command for gameStatus 2
      Wire.endTransmission(4);
    }
    if (Serial.available() > 0) {
      // user enters wasd for direction
      char c = Serial.read();
      nextMove = userEnterMove(c);
      char dir = moveCmd(c);

      // determines next coordinate based on direction
      // (up, down, left, or right)
      if (dir == 'u' || dir == 'd') {
        nextX += playerXPos(dir); // change X direction
      } else if (dir == 'l' || dir == 'r') {
        nextY += playerYPos(dir); // change Y direction
      }
        // move player based on current and next position
        movePlayer();
        curX = nextX;
        curY = nextY;
    }
    if (nextMove) {                                    // if user entered a move last loop, draw the new map on the next loop
      printMap();
      nextMove = false;
      if ((curX == SIZE - 1) && (curY == SIZE - 1)) { // if user reaches the opposite corner
        Serial.println("Player 1 won");
        gameStatus = 3;
        Wire.beginTransmission(4);
        Wire.write(6);                                // command for end of game situation
        Wire.write('w');                              // signals a victory for player 1
        Wire.endTransmission(4);
      } else if (life == 0) {                         // if the user runs out of lives
        Serial.println("game over");
        Wire.beginTransmission(4);
        Wire.write(6);                                // command for end of game situation
        Wire.write('l');                              // signals a defeat for player 1
        Wire.endTransmission(4);
        gameStatus = 3;
      }
    }
  }
}

// creats a new map with the player at the top left corner
void createMap() {
  matrix[0][0] = 3;
}

// Handles player movement
void movePlayer() {
  if (matrix[nextX][nextY] == 1) {          // if player hits a hidden bomb
    life--;
    Serial.print("you have ");
    Serial.print(life);
    Serial.println(" lives remaining");
  }
  if (matrix[curX][curY] == 4) {
    matrix[curX][curY] += 1;
  }
  matrix[nextX][nextY] += 3;
  matrix[curX][curY] -= 3;
}

// Prints the current status of the map to the console
void printMap() {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (matrix[i][j] <= 1) {
        Serial.print("*");
      } else if (matrix[i][j] == 2) {
        Serial.print("x");
      } else if (matrix[i][j] >= 3) {
        Serial.print("o");
      }
    }
    Serial.println();  
  }
}

bool userEnterMove(int c) {
  if (c == 10) {
    Serial.println("Player 2's turn");
    return true;
  } else {
    return false;
  }
}

// Prints direction user moved to console
char moveCmd(char c) {
  char moveDirection = ' ';
  if (c == 'w') {
    moveDirection = 'u';
    Serial.println("up");
  } else if (c == 'a') {
    moveDirection = 'l';
    Serial.println("left");
  } else if (c == 's') {
    moveDirection = 'd';
    Serial.println("down");
  } else if (c == 'd') {
    moveDirection = 'r';
    Serial.println("right");
  }
  return moveDirection;
}

// Returns the direction the user moves in the X direction
int playerXPos(char dir) {
  if ((dir == 'u') && (curX > 0)) {
    return -1;
  } else if ((dir == 'd') && (curX < SIZE - 1)) {
    return 1;
  } else {
    return 0;
  }
}

// Returns the direction the user moves in the Y direction
int playerYPos(char dir) {
  if ((dir == 'l') && (curY > 0)) {
    return -1;
  } else if ((dir == 'r') && (curY < SIZE - 1)) {
    return 1;
  } else {
    return 0;
  }
}

// randomly sets MAX_BOMBS on the board
void setRandomBombs() {
  int numBombs = 0;
  while (numBombs < MAX_BOMBS) {
    if (setBombs(random(0, SIZE), random(0, SIZE))){
      numBombs++;
    }
  }
}

// row: row of board
// col: column of board
// tries to set the given position as a bomb
// returns true if successful and false if not successful
bool setBombs(int row, int col) {
  if (matrix[row][col] == 1 || isCorner(row, col)) {
    return false;
  } else {
    matrix[row][col] = 1;
    return true;
  }
}

// returns true if row and col make a corner
// otherwise, returns false
bool isCorner(int row, int col) {
  return (row == 0 && col == 0) || (row == 0 && col == SIZE - 1) || (row == SIZE - 1 && col == 0) || (row == SIZE -1 && col == SIZE - 1);
}
