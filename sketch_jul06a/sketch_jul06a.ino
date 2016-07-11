// Pin numbers (These numbers are on the Audrino board)
#define CLOCK_1 7
#define CLOCK_2 12
#define DATA_1 8
#define DATA_2 13

void setup() {
  pinMode(CLOCK_1, OUTPUT);
  pinMode(CLOCK_2, OUTPUT);
  pinMode(DATA_1, OUTPUT);
  pinMode(DATA_2, OUTPUT);
  // Serial.begin(9600);
}

int current_row = 1;
int current_numberToDisplay = 0;

void showNumber(int row, int numberToDisplay)
{
  //               0,D,2,3,4,5,6,7,8,D,D,D,2,3,4,5,D,D,D,D,0,1,2,3,4,5,D,7,D,D,0,1,2,D,D,D
  //               0,X,1,2,3,4,5,6,7,X,X,X,8,9,A,B,X,X,X,X,C,D,E,F,0,1,X,2,X,X,3,4,5,6,7,X
  int cycle[36] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int segments[] = { 10,17,20,26,32,37,43,50,55,63, // map a specific digit to the segments
                     0,1,2,3,5,6,-1,   // 0 segments to turn on
                     3,5,-1,           // 1 segments to turn on
                     2,3,4,0,6,-1,     // 2 segments to turn on
                     2,3,4,5,6,-1,     // 3 segments to turn on
                     1,4,3,5,-1,       // 4 segments to turn on
                     2,1,4,5,6,-1,     // 5 segments to turn on
                     2,1,0,6,5,4,-1,   // 6 segments to turn on
                     1,2,3,5,-1,       // 7 segments to turn on
                     1,2,3,4,0,6,5,-1, // 8 segments to turn on
                     1,2,3,4,5,6,-1 }; // 9 segments to turn on
  int pins[] = { 2,3,4,5,6,7,8,13,14,15,20,21,22,23,24,25,27,30,31,32,33 }; // LED driver output bits --> actual segments on the LEDS

  for (int k = 6; k > 0; k--)
  {
    digitalWrite(CLOCK_2, LOW);
    digitalWrite(DATA_2, k == row ? HIGH : LOW);
    delay(1);
    digitalWrite(CLOCK_2, HIGH);
  }

  int remainder = numberToDisplay;

  for (int digitIndex = 2; digitIndex >= 0; digitIndex--)
  {
    int digit = remainder % 10;
    remainder -= digit;
    remainder /= 10;
    int index = segments[digit];
    while (segments[index] != -1)
    {
      int segmentNumber = segments[index];
      int pinIndex = digitIndex * 7 + segmentNumber;
      int pinNumber = pins[pinIndex];
      cycle[pinNumber] = 1;
      index++;
      // Serial.println(pinNumber);
    }
  }
        
  for (int i = 0; i < 36; i++)
  {
    digitalWrite(CLOCK_1, LOW);
    delay(1);
    digitalWrite(DATA_1, cycle[i] == 0 ? LOW : HIGH);
    delay(1);
    digitalWrite(CLOCK_1, HIGH);
  } 
}

void loop()
{
  showNumber(current_row, current_numberToDisplay);
  current_numberToDisplay += 1;
  if (current_numberToDisplay > 999)
  {
    current_numberToDisplay = 0;
    current_row++;
    if (current_row == 7) // current
    {
      current_row = 1;
    }
  }
}
