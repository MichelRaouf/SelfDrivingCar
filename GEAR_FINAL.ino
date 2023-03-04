#include <Arduino_FreeRTOS.h>
void JoystickDetectionAndActionTask(void *pvParameters);
void lightDetection(void *pvParameters);


int SW = 12;
int xPosition = 0;
int yPosition = 0;
int SW_state = 0;
int mapX = 0;
int mapY = 0;
int GearState=0;
int buzzerPin=9;
const int lightLedPin1 =10;
const int lightLedPin2 =11;
const int lightSensorPin = A2;
const int VRxPin = A0;
const int VRyPin = A1;
int segPins[] = {4, 5, 8, 7, 6, 3, 2, 13 };   // { a b c d e f g . )

  byte segCode[4][8] = {
// //  a  b  c  d  e  f  g  .
   { 1, 1, 0, 0, 1, 1, 1, 0} , // p 
   { 0, 0, 0, 0, 1, 0, 1, 0} ,//r
  { 0, 0, 1, 0, 1, 0, 1, 0} ,  //n
   { 0, 1, 1, 1, 1, 0, 1, 0} , // d
   

 };


void displayGear(int g)
 {
   for (int i=0; i < 8; i++)
   {
 digitalWrite(segPins[i], segCode[g][i]);
   }
 }



void JoystickDetectionAndActionTask(void *pvParameters){ // Round Robin priority 3
   TickType_t xDelay1s=pdMS_TO_TICKS(200);
          Serial.println("joystick");
TickType_t xLastWakeTime;

xLastWakeTime=xTaskGetTickCount();
while(1){
   xPosition = analogRead(VRxPin);



   yPosition = analogRead(VRyPin);
   SW_state = digitalRead(SW);
   

   mapX = map(xPosition, 0, 1023, -512, 512);
   mapY = map(yPosition, 0, 1023, -512, 512);
  if(SW_state==LOW){
    digitalWrite(buzzerPin,HIGH);
  }
  else{
     digitalWrite(buzzerPin,LOW);
  }
 
if(mapY<-200){
  if(GearState!=0){
          Serial.println("decrement");

    GearState=GearState-1;

  };
}
else{
  if(mapY>200)
{
  if(GearState!=3){
      Serial.println("increment");

    GearState=GearState+1;
    };

}


}
      Serial.print("gear: ");
  Serial.print(GearState);

  Serial.print("X: ");
  Serial.print(mapX);
  Serial.print(" | Y: ");
  Serial.print(mapY);
  Serial.print(" | Button: ");
  Serial.println(SW_state);
  displayGear(GearState);
  vTaskDelayUntil(&xLastWakeTime,xDelay1s);

}
  

}

void lightDetection(void *pvParameters){
  TickType_t xDelay1s=pdMS_TO_TICKS(200);
  TickType_t xLastWakeTime;
xLastWakeTime=xTaskGetTickCount();

for(;;){
  int lightSensorStatus = analogRead(lightSensorPin);
   
  int power = ((lightSensorStatus-100)/600.0)*255;

  if(power>255) power =255;
  if(power<0) power =0;
  
 
  analogWrite(lightLedPin1,power);
    analogWrite(lightLedPin2,power);



    vTaskDelayUntil(&xLastWakeTime,xDelay1s);
    }



}

void setup() {
  Serial.begin(9600); 

  // segment and gear
  for (int i=0; i < 8; i++)
 {
     pinMode(segPins[i], OUTPUT);
   }

  
  pinMode(VRxPin, INPUT);
  pinMode(VRyPin, INPUT);
  pinMode(SW, INPUT_PULLUP); 
  pinMode(buzzerPin,OUTPUT);


  pinMode(lightLedPin1, OUTPUT);
  pinMode(lightLedPin2, OUTPUT);

  pinMode(lightSensorPin, INPUT);

    // settup tasks
  // xTaskCreate(IRDetectionTask,"IR Detection",128,NULL,3,NULL);
  // xTaskCreate(TouchScreenDetectionAndActionTask,"Touch Screen Detection",128,NULL,3,NULL);
  xTaskCreate(JoystickDetectionAndActionTask,"Joystick Detection and Action",500,NULL,3,NULL);
  // xTaskCreate(CarTurnTask,"IR Action",128,NULL,1,&CarTurnHandle);
  xTaskCreate(lightDetection,"Light Detection and Action",128,NULL,2,NULL);

  
  
}



void loop() {
  // displayGear(GearState);
  // xPosition = analogRead(VRxPin);
  // yPosition = analogRead(VRyPin);
  // SW_state = digitalRead(SW);
  // mapX = map(xPosition, 0, 1023, -512, 512);
  // mapY = map(yPosition, 0, 1023, -512, 512);
  // int ldrStatus = analogRead(ldrPin);
  // int power = ((ldrStatus-50)/600.0)*255;
  // if(power>255) {
  //   power =255;
  // }
  // if(power<0) {
  //   power = 0;
  // }
  // analogWrite(ledPin1, power);
  // analogWrite(ledPin2, power);

  // if(SW_state == LOW){
  //   digitalWrite(buzzer, HIGH);
  // } else {
  //    digitalWrite(buzzer, LOW);
  // }
  // if(mapY < -200) {
  //   if(Gear != 0 ){
  //     Gear = Gear-1;
  //   }
  // } else {
  //   if(mapY > 200) {
  //     if(Gear != 3) {
  //       Gear = Gear + 1;
  //       }
  //  }
  // } 
  // delay(200);
}