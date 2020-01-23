#include <Wire.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include "RoboGUI_pouch.h"

extern "C" {
  #include "utility/twi.h"
}

//I2C address of mux
#define TCAADDR 0x71

Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();

//state variable
enum State state;

//flags for controlling looping of different states; set false to exit current state
boolean bootingFlag,
  idleFlag,
  turningLeftFlag,
  turningRightFlag,
  batteryFlag,
  emergencyFlag;

//selects which I2C mux output to address
void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}
  
void setup() {

  while (!Serial);
    delay(1000);

  Wire.begin();
  
  Serial.begin(9600);
  /*
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
  */
  //initialize each LED matrix through the mux
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

  //state to be decide by ROS input
  state = BATTERY;
}

void loop() {
  //switch case statement for main state machine functionality
  switch(state){
    //boot state
    case BOOTING:
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
    break;

    //idle state
    case IDLING:
    break;

    //left turning state
    case TURNING_LEFT:
      turningLeftFlag = true;
      while(turningLeftFlag){
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.drawBitmap(0, 0, turn_left, 8, 8, LED_YELLOW);
          matrix.writeDisplay();
        }
        delay(300);
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.writeDisplay();
        }
        delay(300);
        turningLeftFlag = false;
      }
    break;

    //right turning state
    case TURNING_RIGHT:
      turningRightFlag = true;
      while(turningRightFlag){
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.drawBitmap(0, 0, turn_right, 8, 8, LED_YELLOW);
          matrix.writeDisplay();
        }
        delay(300);
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.writeDisplay();
        }
        delay(300);
        turningRightFlag = false;
      }
    break;


    //battery display state; needs development to separate different battery states
    case BATTERY:
      batteryFlag = true;
      while(batteryFlag){
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.drawBitmap(0, 0, battery_6, 8, 8, LED_GREEN);
          matrix.writeDisplay();
        }
        delay(1000);

        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.drawBitmap(0, 0, battery_5, 8, 8, LED_GREEN);
          matrix.writeDisplay();
        }
        delay(1000);
        
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.drawBitmap(0, 0, battery_4, 8, 8, LED_GREEN);
          matrix.writeDisplay();
        }
        delay(1000);
        
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.drawBitmap(0, 0, battery_3, 8, 8, LED_GREEN);
          matrix.writeDisplay();
        }
        delay(1000);
        
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.drawBitmap(0, 0, battery_2, 8, 8, LED_GREEN);
          matrix.writeDisplay();
        }
        delay(1000);
        
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.drawBitmap(0, 0, battery_1, 8, 8, LED_GREEN);
          matrix.writeDisplay();
        }
        delay(1000);
        
        for(int i = 0; i < 5; i++){
          for(int j = 0; j < 5; j++){
            tcaselect(j);
            matrix.clear();
            matrix.writeDisplay();
          }
          delay(200);
          for(int j = 0; j < 5; j++){
            tcaselect(j);
            matrix.drawBitmap(0, 0, battery_0, 8, 8, LED_RED);
            matrix.writeDisplay();
          }
          delay(200);
        }
        batteryFlag = false;
      }
    break;

    //emergency/collision state
    case EMERGENCY:
      emergencyFlag = true;
      while(emergencyFlag){
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.clear();
          matrix.writeDisplay();
        }
        delay(200);
        for(int i = 0; i < 5; i++){
          tcaselect(i);
          matrix.drawBitmap(0, 0, emergency_alert, 8, 8, LED_RED);
          matrix.writeDisplay();
        }
        delay(200);
        emergencyFlag = false;
      }
    break;

    //default/error state
    default:
      Serial.print("default error");
    break;
  }
  
}
