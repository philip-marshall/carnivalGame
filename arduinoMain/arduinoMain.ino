// Code by Philip Marshall
// carnival game hub for my YouTube video titled "-". Make sure to watch it and subscribe!
// Feel free to disect and use any code in your own project

#include <Servo.h>

// Variable for setting the difficulty and will stop the game from resetting each time it loops
int difficulty = -1;
int pos = 50;
int pos2 = 120;
double pos3 = 70;
double pos4 = 180;

// Array of locks for targets
bool sensorLock[6] = {0,0,0,0,0,0};

// buttons
const int button3 = 13;
const int button2 = 12;
const int button1 = 11;

// sensor variables
const int sensor1 = A0;
const int sensor2 = A1;
const int sensor3 = A2;
const int sensor4 = A3;
const int sensor5 = A4;
const int sensor6 = A5;

// leds
const int led1 = 8;
const int led2 = 7;
const int led3 = 0;
const int led4 = 10;
const int led5 = 9;
const int led6 = 6;

// servo objects
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// sensor values
int sen1Val = 0;
int sen2Val = 0;
int sen3Val = 0;
int sen4Val = 0;
int sen5Val = 0;
int sen6Val = 0;

char serialByte = "0";
int serialInt = 0;
int score = 0;
int scoreHolder = 0;
int isReset = 0;

void setup() {  
  // setting the analog(sensor) pins for input
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);
  pinMode(sensor6, INPUT);

  // Setting the servo pins to output
  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);
  servo4.attach(5);

  servo1.write(80);
  servo2.write(90);
  servo3.write(90);
  servo4.write(pos4);

  // set up the buttons
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);

  // led setup
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
  digitalWrite(led5, HIGH);
  digitalWrite(led6, HIGH);

  // Start the serial com at 9600 baud rate
  Serial.begin(9600);
  Serial.setTimeout(1);
}

void checkForSerial() {
  // Check serial port at the start of each loop for anything from python program
  // send the score when python program sends a '1'
  
  if (Serial.available())
  {
    serialByte = Serial.read();
    serialInt = String(serialByte).toInt();
    if (serialInt == 1)
    {
      Serial.println("score," + String(score));
    }
    else if (serialInt == 2)
    {
      if (difficulty == 1)
      {
        pos4 = pos4 - 6;
      }
      else if (difficulty == 2)
      {
        pos4 = pos4 - 7.2;
      }
      else if (difficulty == 3)
      {
        pos4 = pos4 - 9;
      }
      
      if (pos4 >= 0)
      {
        servo4.write(pos4);
      }
    }
    else if (serialInt == 9)
    {
      Serial.println("final," + String(score));
      isReset = 1;
    }
  }
}

int findLaser() {
  sen1Val = analogRead(sensor1);
  sen2Val = analogRead(sensor2);
  sen3Val = analogRead(sensor3);
  sen4Val = analogRead(sensor4);
  sen5Val = analogRead(sensor5);
  sen6Val = analogRead(sensor6);

  if (sen1Val > 400)
    return 1;
  else if (sen2Val > 400)
    return 2;
  else if (sen3Val > 400)
    return 3;
  else if (sen4Val > 400)
    return 4;
  else if (sen5Val > 400)
    return 5;
  else if (sen6Val > 400)
    return 6;
  else
    return -1;
}

void checkAndResetLocks() {
  if (sensorLock[0] == 1 && sensorLock[1] == 1 && sensorLock[2] == 1 && sensorLock[3] == 1 && sensorLock[4] == 1 && sensorLock[5] == 1)
  {
    for (int i = 0; i < 6; i++)
    {
      sensorLock[i] = 0;
    }
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led6, HIGH);
    delay(100);
  }
}

void scoreLaser(int laserID) {
  switch(laserID) {
    case -1:
      break;
    case 1:
      if (sensorLock[0] == 0) {
        sensorLock[0] = 1;
        digitalWrite(led1, LOW);
        score += 1;
        Serial.println("score," + String(score));
      }
      break;
    case 2:
      if (sensorLock[1] == 0) {
        sensorLock[1] = 1;
        digitalWrite(led2, LOW);
        score += 1;
        Serial.println("score," + String(score));
      }
      break;
    case 3:
      if (sensorLock[2] == 0) {
        sensorLock[2] = 1;
        digitalWrite(led3, LOW);
        score += 1;
        Serial.println("score," + String(score));
      }
      break;
    case 4:
      if (sensorLock[3] == 0) {
        sensorLock[3] = 1;
        digitalWrite(led4, LOW);
        score += 1;
        Serial.println("score," + String(score));
      }
      break;
    case 5:
      if (sensorLock[4] == 0) {
        sensorLock[4] = 1;
        digitalWrite(led5, LOW);
        score += 1;
        Serial.println("score," + String(score));
      }
      break;
    case 6:
      if (sensorLock[5] == 0) {
        sensorLock[5] = 1;
        digitalWrite(led6, LOW);
        score += 1;
        Serial.println("score," + String(score));
      }
      break;
    default:
      break;
  }
}

void resetGame() {
  difficulty = -1;
  for (int i = 0; i < 6; i++)
  {
    sensorLock[i] = 0;
  }
  pos4 = 180;
  score = 0;
  servo1.write(80);
  servo2.write(90);
  servo3.write(90);
  servo4.write(pos4);
  digitalWrite(led1, HIGH);
}

void checkIsReset()
{
  if (isReset == 1)
  {
    isReset = 0;
    resetGame();
  }
}

void loop() {
  // Check if all targets have been hit and then reset them
  checkAndResetLocks();
  
  // If the game is in difficulty selection mode
  if (difficulty == -1)
  {
    // reset score incase there is a previous score saved
    score = 0;
    if (digitalRead(button1) == HIGH)
    {
     difficulty = 1;
     Serial.println("difficulty,1");
    }
    else if (digitalRead(button2) == HIGH)
    {
     difficulty = 2;
     Serial.println("difficulty,2");
    }
    else if (digitalRead(button3) == HIGH)
    {
     difficulty = 3;
     Serial.println("difficulty,3");
    }
    delay(100);
  }
  // If a difficulty is selected and the game begins
  else
  {
    if (difficulty == 1)
    {
        scoreLaser(findLaser());
        checkForSerial();
        checkIsReset();
    }
    else if (difficulty == 2)
    {
        for (pos = 50; pos <= 120; pos += 1) { // goes from 50 degrees to 120 degrees
        // in steps of 1 degree
        servo1.write(pos);  // tell servo to go to position in variable 'pos'
        servo2.write(pos2);
        pos2 -= 1;
        scoreLaser(findLaser());
        checkForSerial();
        delay(30);                       // waits 30ms for the servo to reach the position
      }
      for (pos = 120; pos >= 50; pos -= 1) {
        servo1.write(pos);
        servo2.write(pos2);
        pos2+= 1;
        scoreLaser(findLaser());
        checkForSerial();
        delay(30);
      }
      checkIsReset();
    }
    else if (difficulty == 3)
    {
        for (pos = 50; pos <= 120; pos += 1) {
        servo1.write(pos);
        servo2.write(pos2);
        servo3.write(pos3);
        pos2 -= 1;
        pos3 += .5;
        scoreLaser(findLaser());
        checkForSerial();
        delay(30);
      }
      for (pos = 120; pos >= 50; pos -= 1) {
        servo1.write(pos);
        servo2.write(pos2);
        servo3.write(pos3);
        pos2+= 1;
        pos3 -= .5;
        scoreLaser(findLaser());
        checkForSerial();
        delay(30);
      }
      checkIsReset();
    }
    else {
      // Error
      delay(1000);
    }
  }
}
