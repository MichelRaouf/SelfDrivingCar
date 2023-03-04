#include <Arduino_FreeRTOS.h>

#include "Adafruit_GFX.h"
  #include "MCUFRIEND_kbv.h"
  MCUFRIEND_kbv tft;
  #include "Fonts/FreeSans9pt7b.h"
  #include "Fonts/FreeSans12pt7b.h"
  #include "Fonts/FreeSerif12pt7b.h"
  #include "FreeDefaultFonts.h"
  #define PI 3.1415926535897932384626433832795
  #include "TouchScreen.h"
  #include <stdint.h>


  #define YP A2
  #define XM A3
  #define YM 8
  #define XP 9

  #include "SoftwareSerial.h"
  #include "DFRobotDFPlayerMini.h"
  SoftwareSerial mySerial(51,53 );
  DFRobotDFPlayerMini myDFPlayer;
  # define Start_Byte 0x7E
  # define Version_Byte 0xFF
  # define Command_Length 0x06
  # define End_Byte 0xEF
  # define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]

  # define ACTIVATED LOW

  boolean isPlaying = true;
  int song=1;
  int maxSong=0;
  int col[8];
  
  void vTaskTouchScreen(void *pvParameters);


  void setup () {
  
    mySerial.begin(9600);
    Serial.begin(115200);

    Serial.println();
    Serial.println(F("DFRobot DFPlayer Mini Demo"));
    Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

    if (!myDFPlayer.begin(mySerial)) {  //Use softwareSerial to communicate with mp3.
      Serial.println(F("Unable to begin:"));
      Serial.println(F("1.Please recheck the connection!"));
      Serial.println(F("2.Please insert the SD card!"));
      while(true);
    }
    delay(1000);
    playFirst();
    maxSong=myDFPlayer.readFileCounts();
    isPlaying = true;
    
  //   // touch screen
    tft.reset();
  // Serial.begin(9600);
    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.setRotation(1);
    tft.invertDisplay(true);
    tft.fillScreen(0xffff);

    col[0] = tft.color565(155, 0, 50);
    tft.drawRect(60,200,100,65,col[0]);
    tft.drawRect(190,200,100,65,col[0]);
    tft.drawRect(320,200,100,65,col[0]);
    //tft.drawRect(190,100,100,65,col[0]);

    tft.setCursor(85,220);
    tft.setTextColor(0xF80000);
    tft.setTextSize(2);
    tft.print("Prev");

    tft.setCursor(215,220);
    tft.setTextColor(0xF80000);
    tft.setTextSize(2);
    tft.print("Pause");

  tft.fillRect(215,120,70,50, 0xFFFF);
          tft.setCursor(215,120);
          tft.setTextColor(0xF80000);
          tft.setTextSize(2);
          tft.print("Song ");
          tft.setCursor(270,120);
          tft.setTextColor(0xF80000);
          tft.setTextSize(2);
          tft.print(song);


    tft.setCursor(345,220);
    tft.setTextColor(0xF80000);
    tft.setTextSize(2);
    tft.print("Next");
  maxSong=myDFPlayer.readFileCounts();
  
      xTaskCreate(vTaskTouchScreen,"Touchscreen and Action",900,NULL,3,NULL);


  }

  void playFirst()
  {
    execute_CMD(0x3F, 0, 0);
    delay(500);
    setVolume(25);
    delay(500);
    execute_CMD(0x11,0,1); 
    delay(500);
  }

  void pause()
  {
    execute_CMD(0x0E,0,0);
    delay(500);

  }

  void play()
  {
    execute_CMD(0x0D,0,1); 
    delay(500);

  }


  void playNext()
  {
    execute_CMD(0x01,0,1);
    delay(500);

    


  }

  void playPrevious()
  {
    execute_CMD(0x02,0,1);
    delay(500);


  }

  void setVolume(int volume)
  {
    execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
    delay(2000);
  }

  void execute_CMD(byte CMD, byte Par1, byte Par2){ // Excecute the command and parameters
  // Calculate the checksum (2 bytes)
  word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
  // Build the command line
  byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
  Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
  //Send the command line to the module
  for (byte k=0; k<10; k++)
  {
  mySerial.write( Command_line[k]);
  }
  }




  void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg)
  {
  int16_t x1, y1;
  uint16_t wid, ht;
  tft.setFont(f);
  tft.setCursor(x, y);
  tft.setTextColor(0x0000);
  tft.setTextSize(sz);
  tft.print(msg);
  }




  void loop() {
      
  }
  
  
  
void vTaskTouchScreen(void *pvParameters){
     while(true){
    TouchScreen ts=TouchScreen(XP,YP,XM,YM,300);
    TSPoint p=ts.getPoint();
    
   

    if (p.z>ts.pressureThreshhold)
    {
      Serial.print("X= ");
      Serial.print(p.x);

      Serial.print("Y= ");
      Serial.print(p.y);

      Serial.print("Pressure ");
      Serial.print(p.z);
      Serial.println();

        
      if (p.x>=170 && p.x<=355 && p.y>=616 && p.y<=786){
        //collision previous rect 
    
            playPrevious();
      if(song==1)
      song=maxSong;
      else
      song=song-1;
            pinMode(XM,OUTPUT);
          pinMode(YP,OUTPUT);
          tft.fillRect(215,120,70,50, 0xFFFF);
          tft.setCursor(215,120);
          tft.setTextColor(0xF80000);
          tft.setTextSize(2);
          tft.print("Song ");
          tft.setCursor(270,120);
          tft.setTextColor(0xF80000);
          tft.setTextSize(2);
          tft.print(song);
    

        
      }
      
      else if (p.x>=405 && p.x<=589 && p.y>=616 && p.y<=786){
        //collision middle rect 
          pinMode(XM,OUTPUT);
          pinMode(YP,OUTPUT);
        if(isPlaying){ 
          //toggle from playing to paused.
          tft.fillRect(195,205,90,55, 0xFFFF);
          tft.setCursor(215,220);
          tft.setTextColor(0xF80000);
          tft.setTextSize(2);
          //tft.print("Pause");
          tft.print("Play");
                Serial.println("pause");
          pause();
          isPlaying = false;
        }
        else{ 
          //toggle from paused to playing.
          tft.fillRect(195,205,90,55, 0xFFFF);
          tft.setCursor(215,220);
          tft.setTextColor(0xF80000);
          tft.setTextSize(2);
          //tft.print("Play"); 
          tft.print("Pause");
                Serial.println("play");
          isPlaying = true;
          play();
        }

      }
      else if(p.x>=641 && p.x<=830 && p.y>=616 && p.y<=786){
        //toggle the next song..
        Serial.println("Next");
          isPlaying = true;
          playNext();
    if(song==maxSong){
        song=1;
      }
      else
      song=song+1;
  pinMode(XM,OUTPUT);
          pinMode(YP,OUTPUT);
          tft.fillRect(215,120,70,50, 0xFFFF);
          tft.setCursor(215,120);
          tft.setTextColor(0xF80000);
          tft.setTextSize(2);
          tft.print("Song ");
          tft.setCursor(270,120);
          tft.setTextColor(0xF80000);
          tft.setTextSize(2);
          tft.print(song);
    
        }

  
      delay(400);
      
    }   
    }
  }