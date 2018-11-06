#define SIZE 4

int matrix[4][4];
bool nextMove = false;
void setup() {
 Serial.begin(9600); 
 createMap(matrix);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    int c = Serial.read();
    nextMove = userEnterMove(c);
  }
  if (nextMove) {
    printMap(matrix);
    nextMove = false;
  }
}

void createMap(int matrix[][SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      matrix[i][j] = 0;
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
  Serial.println(c);
  if (c == 10) {
    return true;
  } else {
    return false;
  }
}
