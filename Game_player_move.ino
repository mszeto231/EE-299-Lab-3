#define SIZE 4

int matrix[4][4];
bool nextMove = false;
//char dir = 'a';
int playerX = 0;
int playerY = 0;
void setup() {
 Serial.begin(9600); 
 createMap(matrix, playerX, playerY);
 printMap(matrix);
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    
    nextMove = userEnterMove(c);
    char dir = moveCmd(c);
    if (dir == 'u' || dir == 'd') {
      playerX += playerXPos(dir, playerX);
    } else if (dir == 'l' || dir == 'r') {
      playerY += playerYPos(dir, playerY);
    }
    createMap(matrix, playerX, playerY);
  }
  if (nextMove) {
    printMap(matrix);
    nextMove = false;
  }
}

void createMap(int matrix[][SIZE], int playerX, int playerY) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (i == playerX && j == playerY) {
        matrix[i][j] = 2;
      } else {
        matrix[i][j] = 0;
      }
    }
  }
}

void printMap(int matrix[][SIZE]) {
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
  //Serial.println();
  if (c == 10) {
    return true;
  } else {
    return false;
  }
}

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

int playerXPos(char dir, int x) {
  if ((dir == 'u') && (x > 0)) {
    return -1;
  } else if ((dir == 'd') && (x < 3)) {
    return 1;
  } else {
    return 0;
  }
}

int playerYPos(char dir, int y) {
  if ((dir == 'l') && (y > 0)) {
    return -1;
  } else if ((dir == 'r') && (y < 3)) {
    return 1;
  } else {
    return 0;
  }
}
