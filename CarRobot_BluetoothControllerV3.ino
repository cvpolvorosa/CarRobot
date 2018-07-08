/* This sketch is intended for the e-Gizmo MOBOT, accompanied by an Android phone application.
   The smartphone acts as the remote controller for the mobile robot, which communicates
   through bluetooth. It can command the robot to go forward, left, right, reverse, go on auto-pilot,
   or line follower mode. While on auto-pilot, the robot uses its front infrared sensors for collision avoidance.
   While on line follower mode, the robot uses its infrared line tracking sensors to follow a black path.

   Requirements: e-Gizmo MOBOT, HC-06 bluetooth module, Android smartphone
   Code by: Carl Vincent M. Polvorosa
*/
char data; //variable to store the data from the android application


//Refers to the state of the IR sensors
int sensorRight = HIGH, sensorMid = HIGH, sensorLeft = HIGH;

//Sets the pins for the wheels and sensors
byte leftWheel = 9;
byte rightWheel = 10;

//Refers to the directions of the wheels; HIGH = forward, LOW = backward
byte leftDirect = 8;
byte rightDirect = 11;

byte lineSensor1 = 5;
byte lineSensor2 = 6;
byte lineSensor3 = 7;

byte rightSensor = 4;
byte midSensor = 3;
byte leftSensor = 2;

int randNum;
boolean isAutoPilotTrue = false;
boolean isLineFollowTrue = false;
void setup() {
  Serial.begin(38400);
  pinMode(lineSensor1, INPUT);
  pinMode(lineSensor2, INPUT);
  pinMode(lineSensor3, INPUT);

  pinMode(rightSensor, INPUT);
  pinMode(midSensor, INPUT);
  pinMode(leftSensor, INPUT);

  pinMode(leftDirect, OUTPUT);
  pinMode(leftWheel, OUTPUT);
  pinMode(rightWheel, OUTPUT);
  pinMode(rightDirect, OUTPUT);

  digitalWrite(leftDirect, HIGH);
  digitalWrite(rightDirect, HIGH);
}
void loop() {
  //Reads the data from the android app if it's available
  if (Serial.available() > 0) {
    data = Serial.read();
    if (data == 'w') {
      goForward();
    } else if (data == 'd') {
      turnRight();
    } else if (data == 'a') {
      turnLeft();
    } else if (data == 's') {
      reverse();
    } else if (data == 'z') {
      brake();
    } else if (data == '1') {
      isAutoPilotTrue = true;
    } else if (data == '0') {
      isAutoPilotTrue = false;
      brake();
    } else if (data == '2') {
      isLineFollowTrue = true;
    } else if (data == '3') {
      isLineFollowTrue = false;
      brake();
    }
  }
  autoPilot();
  lineFollow();
}

void goForward() {
  digitalWrite(leftDirect, HIGH);
  digitalWrite(rightDirect, HIGH);
  if (isAutoPilotTrue == true) {
    analogWrite(leftWheel, 120);
    analogWrite(rightWheel, 120);
  } else if (isLineFollowTrue == true) {
    analogWrite(leftWheel, 70);
    analogWrite(rightWheel, 70);
  } else {
    analogWrite(leftWheel, 255);
    analogWrite(rightWheel, 255);
  }
}

void turnLeft() {
  if (isAutoPilotTrue == true) {
    analogWrite(leftWheel, 0);
    analogWrite(rightWheel, 175);
    delay(200);
  } else if (isLineFollowTrue == true) {
    analogWrite(leftWheel, 0);
    analogWrite(rightWheel, 90);
  } else {
    analogWrite(leftWheel, 200);
    analogWrite(rightWheel, 75);
  }
}

void turnRight() {
  if (isAutoPilotTrue == true) {
    analogWrite(leftWheel, 175);
    analogWrite(rightWheel, 0);
    delay(200);
  } else if (isLineFollowTrue == true) {
    analogWrite(leftWheel, 90);
    analogWrite(rightWheel, 0);
  } else {
    analogWrite(leftWheel, 75);
    analogWrite(rightWheel, 200);
  }
}

void reverse() {
  if (isAutoPilotTrue == true) {
    digitalWrite(leftDirect, LOW);
    digitalWrite(rightDirect, LOW);
    delay(750);
    digitalWrite(leftDirect, HIGH);
    digitalWrite(rightDirect, HIGH);
  } else {
    digitalWrite(leftDirect, LOW);
    digitalWrite(rightDirect, LOW);
    analogWrite(leftWheel, 200);
    analogWrite(rightWheel, 200);
  }
}

void brake() {
  analogWrite(leftWheel, 0);
  analogWrite(rightWheel, 0);
}

void autoPilot() {
  if (isAutoPilotTrue == true) {
    //Reads the IR sensors
    sensorRight = digitalRead(rightSensor);
    sensorMid = digitalRead(midSensor);
    sensorLeft = digitalRead(leftSensor);

    int randNum; //Will be used to decide to go left or right if all sensors are LOW

    //If all sensors are HIGH then there are no obstacles detected
    if (sensorRight == HIGH && sensorMid == HIGH && sensorLeft == HIGH) {
      goForward();
    } else if (sensorRight == HIGH && sensorMid == HIGH && sensorLeft == LOW) { //If the left sensor detects an object, the robot turns right
      turnRight();
    } else if (sensorRight == LOW && sensorMid == HIGH && sensorLeft == HIGH) { //If the right sensor detects an object, the robot turns left
      turnLeft();

      //If both the middle and right sensors detect an object, the robot reverses first before turning left to avoid being stuck
    } else if (sensorRight == LOW && sensorMid == LOW && sensorLeft == HIGH) {
      reverse();
      turnLeft();
      //If both the middle and left sensors detect an object, the robot reverses first before turning right to avoid being stuck
    } else if (sensorRight == HIGH && sensorMid == LOW && sensorLeft == LOW) {
      reverse();
      turnRight();
    } else { //If all sensors detect an obstacle, randomly chooses to go either left or right
      reverse();
      randNum = random(2);
      if (randNum == 0) {
        turnLeft();
      } else {
        turnRight();
      }
    }
    delay(20);
  }
}

void lineFollow() {
  if(isLineFollowTrue == true){
  //The sensors will read as LOW if it is on the color black
  sensorRight = digitalRead(lineSensor1);
  sensorMid = digitalRead(lineSensor2);
  sensorLeft = digitalRead(lineSensor3);

  //If all sensor read LOW, keep moving forward
  if (sensorLeft == LOW && sensorMid == LOW && sensorRight == LOW) {
    goForward();
  } else if (sensorLeft == HIGH && sensorMid == LOW && sensorRight == LOW) {
    turnRight();
  } else if (sensorLeft == LOW && sensorMid == LOW && sensorRight == HIGH) {
    turnLeft();
  } else if (sensorLeft == LOW && sensorMid == HIGH && sensorRight == HIGH) {
    turnLeft();
  } else if (sensorLeft == HIGH && sensorMid == HIGH && sensorRight == LOW) {
    turnRight();
  } else if (sensorLeft == HIGH && sensorMid == HIGH && sensorRight == HIGH) {
    brake();
  } else {
    goForward();
  }
  }
}

