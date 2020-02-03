#include <Wire.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include "RoboGUI_pouch.h"
#include <ros.h>
#include <std_msgs/Uint8.h>
#include <std_msgs/String.h>

ros::NodeHandle nh;

extern "C" {
  #include "utility/twi.h"
}

#define TCAADDR 0x71

Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();

enum State state;

boolean bootingFlag,
  idleFlag,
  turningLeftFlag,
  turningRightFlag,
  batteryFlag,
  emergencyFlag;

void emergencyText( const std_msgs::String& emergencyMsg){
  Serial.print(emergencyMsg.data);
  //Test for output when LCD arrives
}

void batteryValue( const std_msgs::Uint8& batteryLevel){
  if(batteryLevel>60){
    batteryState = battery_5;
  }
  else{
    batteryState = battery_2;
  }
}

ros::Subscriber<std_msgs::String> sub("Emergency", &emergencyText);
ros::Subscriber<std_msgs::Uint8> sub("Battery", &batteryValue);

void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}
  
void setup() {

  nh.initNode();
  nh.subscribe(sub);
  
  while (!Serial);
    delay(1000);

  Wire.begin();
  
  Serial.begin(9600);

  for (uint8_t t=0; t<8; t++) {
    tcaselect(t);
    Serial.print("TCA Port #"); Serial.println(t);

    for (uint8_t addr = 0; addr<=127; addr++) {
      if (addr == TCAADDR) continue;
      
      uint8_t data;
      if (! twi_writeTo(addr, &data, 0, 1, 1)) {
         Serial.print("Found I2C 0x");  Serial.println(addr,HEX);
      }
    }
  }
  Serial.println("\ndone");

  tcaselect(0);
  matrix.begin(0x70);
  matrix.clear();
  matrix.writeDisplay();
  tcaselect(1);
  matrix.begin(0x70);
  matrix.clear();
  matrix.writeDisplay();
  tcaselect(2);
  matrix.begin(0x70);
  matrix.clear();
  matrix.writeDisplay();
  tcaselect(3);
  matrix.begin(0x70);
  matrix.clear();
  matrix.writeDisplay();
  tcaselect(4);
  matrix.begin(0x70);
  matrix.clear();
  matrix.writeDisplay();
  
  state = BATTERY;
  batteryState = battery_6;
}

void loop() {
  /*
  if (Serial.available())  {
    Serial.write(Serial.read());//send what has been received
    Serial.println();   //print line feed character
  }
  */
  nh.spinOnce();
  delay(1);
  
  switch(state){
    case BOOTING:
    //for(int i = 0; i < 5; i++){
      //tcaselect(i);
      bootingFlag = true;
      while(bootingFlag){
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.drawBitmap(0, 0, boot0, 8, 8, LED_GREEN);
          matrix.writeDisplay();
        }
        delay(200);
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.drawBitmap(0, 0, boot1, 8, 8, LED_GREEN);
          matrix.writeDisplay();
        }
        delay(200);
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.drawBitmap(0, 0, boot2, 8, 8, LED_GREEN);
          matrix.writeDisplay();
        }
        delay(200);
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.drawBitmap(0, 0, boot3, 8, 8, LED_GREEN);
          matrix.writeDisplay();
        }
        delay(200);
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.drawBitmap(0, 0, boot4, 8, 8, LED_GREEN);
          matrix.writeDisplay();
        }
        delay(200);
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.drawBitmap(0, 0, boot5, 8, 8, LED_GREEN);
          matrix.writeDisplay();
        }
        delay(200);
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.drawBitmap(0, 0, boot6, 8, 8, LED_GREEN);
          matrix.writeDisplay();
        }
        delay(200);
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.drawBitmap(0, 0, boot7, 8, 8, LED_GREEN);
          matrix.writeDisplay();
        }
        delay(200);
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.drawBitmap(0, 0, boot8, 8, 8, LED_GREEN);
          matrix.writeDisplay();
        }
        delay(200);
        bootingFlag = false;
      }
    //}
    break;
    
    case IDLING:
    break;
    
    case TURNING_LEFT:
      turningLeftFlag = true;
      while(turningLeftFlag){
        matrix.clear();
        matrix.drawBitmap(0, 0, turn_left, 8, 8, LED_YELLOW);
        matrix.writeDisplay();
        delay(300);
        matrix.clear();
        matrix.writeDisplay();
        delay(300);
        turningLeftFlag = false;
      }
    break;
    
    case TURNING_RIGHT:
      turningRightFlag = true;
      while(turningRightFlag){
        matrix.clear();
        matrix.writeDisplay();
        delay(300);
        matrix.drawBitmap(0, 0, turn_right, 8, 8, LED_YELLOW);
        matrix.writeDisplay();
        delay(300);
        turningRightFlag = false;
      }
    break;
    
    case BATTERY:
      batteryFlag = true;
      while(batteryFlag){

        matrix.clear();
        matrix.drawBitmap(0, 0, batteryState, 8, 8, LED_GREEN);
        matrix.writeDisplay();
        delay(1000);

//        matrix.clear();
//        matrix.drawBitmap(0, 0, battery_6, 8, 8, LED_GREEN);
//        matrix.writeDisplay();
//        delay(1000);
//        
//        matrix.clear();
//        matrix.drawBitmap(0, 0, battery_5, 8, 8, LED_GREEN);
//        matrix.writeDisplay();
//        delay(1000);
//        
//        matrix.clear();
//        matrix.drawBitmap(0, 0, battery_4, 8, 8, LED_GREEN);
//        matrix.writeDisplay();
//        delay(1000);
//        
//        matrix.clear();
//        matrix.drawBitmap(0, 0, battery_3, 8, 8, LED_YELLOW);
//        matrix.writeDisplay();
//        delay(1000);
//        
//        matrix.clear();
//        matrix.drawBitmap(0, 0, battery_2, 8, 8, LED_YELLOW);
//        matrix.writeDisplay();
//        delay(1000);
//        
//        matrix.clear();
//        matrix.drawBitmap(0, 0, battery_1, 8, 8, LED_RED);
//        matrix.writeDisplay();
//        delay(1000);
//        
//        for(int i = 0; i < 5; i++){
//          matrix.clear();
//          matrix.writeDisplay();
//          delay(200);
//          matrix.drawBitmap(0, 0, battery_0, 8, 8, LED_RED);
//          matrix.writeDisplay();
//          delay(200);
        }
        batteryFlag = false;
      }
    break;
    
    case EMERGENCY:
      emergencyFlag = true;
      while(emergencyFlag){
        matrix.clear();
        matrix.writeDisplay();
        delay(200);
        matrix.drawBitmap(0, 0, emergency_alert, 8, 8, LED_RED);
        matrix.writeDisplay();
        delay(200);
        emergencyFlag = false;
      }
    break;

    default:
      Serial.print("default error");
    break;
  }
  
}
