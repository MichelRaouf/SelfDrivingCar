#include <Wire.h> 
#include <AFMotor.h>
#include <Arduino_FreeRTOS.h>
void vTaskCar(void *pvParameters);

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
const int buzzer=38;
// front left
const int FrontLeftForwardPin = 3;
const int FrontLeftBackwardPin = 4;
const int FrontLeftEnablePin = 2;

// front right
const int FrontRightForwardPin = 5;
const int FrontRightBackwardPin = 6;
const int FrontRightEnablePin = 7;

// back left
const int BackLeftForwardPin = 9;
const int BackLeftBackwardPin = 10;
const int BackLeftEnablePin = 8;

// back right
const int BackRightForwardPin = 12;
const int BackRightBackwardPin = 11;
const int BackRightEnablePin = 13;

int leftSensor = 22;
int rightSensor = 52;
int middleSensor = 40;
bool finishLine = false;
int carSpeed = 55;

void setup() {
  pinMode(buzzer,OUTPUT);
  motor1.setSpeed(carSpeed);
  motor2.setSpeed(carSpeed);
  motor3.setSpeed(carSpeed);
  motor4.setSpeed(carSpeed);
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  xTaskCreate(vTaskCar,"Car Detection and Action",128,NULL,2,NULL);

}

void stop(){

  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);

}

void drive(){
  
  motor1.run(FORWARD); //backLeft
  motor2.run(FORWARD); //backRight
  motor3.run(FORWARD); //frontright
  motor4.run(FORWARD); //frontLeft

}

void goRight(){
  motor1.run(FORWARD); //backLeft
  motor2.run(BACKWARD); //backRight
  motor3.run(BACKWARD); //frontright
  motor4.run(FORWARD); //frontLeft

}

void goLeft(){

  motor1.run(BACKWARD); //backLeft
  motor2.run(FORWARD); //backRight
  motor3.run(FORWARD); //frontright
  motor4.run(BACKWARD); //frontLeft

}


void vTaskCar(void *pvParameters){
TickType_t xDelay200ms=pdMS_TO_TICKS(200);
TickType_t xDelay50ms=pdMS_TO_TICKS(50);

  TickType_t xLastWakeTime=xTaskGetTickCount();
   while(1){
  int leftData = digitalRead(leftSensor);
  int rightData = digitalRead(rightSensor);
  int middleData = digitalRead(middleSensor);
  if(middleData == 1 && !finishLine){
  finishLine = true;
    stop();
  }
  if(leftData == 1 && rightData == 1 && !finishLine){
    finishLine=true;
    stop();
  }
  if(leftData == 1 && !finishLine){
    if(rightData == 1 && !finishLine){
      finishLine=true;
      stop();
    } else {
      digitalWrite(buzzer,HIGH);
      stop();
      //delay(50);
      vTaskDelayUntil(&xLastWakeTime,xDelay50ms);
      goRight();
            vTaskDelayUntil(&xLastWakeTime,xDelay200ms);
            digitalWrite(buzzer,LOW);

      //delay(200);
    }
  }
  if(rightData == 1 && !finishLine){
    if(leftData == 1 && !finishLine){
      finishLine = true;
      stop();
    } else {
            digitalWrite(buzzer,HIGH);

      stop();
     // delay(50);
            vTaskDelayUntil(&xLastWakeTime,xDelay50ms);

      goLeft();
            vTaskDelayUntil(&xLastWakeTime,xDelay200ms);
                  digitalWrite(buzzer,LOW);


     // delay(200);
    }
  }
  if(!finishLine){
    drive();
  }
   }
}

void loop() {
 
}