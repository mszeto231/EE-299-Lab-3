#define SIZE 4
#define MAX_BOMBS 4     // max number of bombs allowed
#define MAX_LIVES 3

// Matrix key
// 0 = empty space
// 1 = hidden bomb
// 2 = hit bomb
// 3+ = player position
int matrix[SIZE][SIZE];
bool nextMove = false;
int nextX = 0;
int nextY = 0;
int curX = 0;
int curY = 0;
int life = MAX_LIVES;
bool win = false;
bool gameStatus = true;   // true if current game is active


void setup() {
 Serial.begin(9600); 
 createMap();
 randomSeed(analogRead(0));
 setRandomBombs();
 Serial.print("you have ");
 Serial.print(life);
 Serial.println(" lives");
 printMap();
}

void loop() {
  if (gameStatus) {
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
    if (nextMove) { // if user entered a move last loop, draw the new map on the next loop
      printMap();
      nextMove = false;
      if ((curX == SIZE - 1) && (curY == SIZE - 1)) { // if user reaches the opposite corner
        Serial.println("you won");
        gameStatus = false;
        Serial.println("Enter y to start a new game");
      } else if (life == 0) { // if the user runs out of lives
        Serial.println("game over");
        gameStatus = false;
        Serial.println("Enter y to start a new game");
      }
    }
  } else {
     if (Serial.available() > 0) {
       char c = Serial.read();
       if (c == 'y') {
        newGame();
       }
     }
  }
}

// creats a new map with all empty spaces except for the player at the top left corner
void createMap() {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (i == 0 && j == 0) {
        matrix[i][j] = 3;
      } else {
        matrix[i][j] = 0;
      }
    }
  }
}

// Handles player movement
void movePlayer() {
  if (matrix[nextX][nextY] == 1) { // if player hits a hidden bomb
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

void newGame() {
  nextMove = false;
  nextX = 0;
  nextY = 0;
  curX = 0;
  curY = 0;
  life = MAX_LIVES;
  win = false;
  gameStatus = true;
  createMap();
  randomSeed(analogRead(0));
  setRandomBombs();
  Serial.print("you have ");
  Serial.print(life);
  Serial.println(" lives");
}
