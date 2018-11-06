#define SIZE 4          // size of board
#define MAX_BOMBS 4     // max number of bombs allowed

int matrix[SIZE][SIZE];  // board
bool nextMove = false;

void setup() {
 Serial.begin(9600); 
 randomSeed(analogRead(0));
 createMap();
 setRandomBombs();
}

void loop() {
  if (Serial.available() > 0) {
    int c = Serial.read();
    nextMove = userEnterMove(c);
  }
  if (nextMove) {
    printMap();
    nextMove = false;
  }
}

void createMap() {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      matrix[i][j] = 0;
    }
  }
}

void printMap() {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (matrix[i][j] == 0) {
         Serial.print("*");
      } else if (matrix[i][j] == 1) {
        Serial.print("x");
      } else if (matrix[i][j] == 2) {
        Serial.print("o");
      }
    }
    Serial.println();  
  }
}

bool userEnterMove(int c) {
  Serial.println(c);
  if (c == 10) {
    return true;
  } else {
    return false;
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
