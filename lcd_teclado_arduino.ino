#include <Keypad_I2C.h>
#include "Wire.h"
#include "LiquidCrystal_I2C.h"

#define lcd_addr 0x27     // I2C address of typical I2C LCD Backpack
#define keypad_addr 0x20  // I2C address of I2C Expander module (A0-A1-A2 dip switch to off position)

// LCD Pins to I2C LCD Backpack - These are default for HD44780 LCD's
#define Rs_pin 0
#define Rw_pin 1
#define En_pin 2
#define BACKLIGHT_PIN 3
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7


const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

//byte ledPin = 13;
// Keypad pins connected to the I2C-Expander pins P0-P6
byte rowPins[ROWS] = {4, 5, 6, 7}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {3, 2, 1, 0};    // connect to the column pinouts of the keypad 

// Create instance for LCD called: i2c_lcd
LiquidCrystal_I2C i2c_lcd(lcd_addr,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

// Create instance of the Keypad name I2C_Keypad and using the PCF8574 chip
Keypad_I2C keypad = Keypad_I2C( makeKeymap(keys), rowPins, colPins, ROWS, COLS, keypad_addr, PCF8574 );
//Keypad_I2C keypad = Keypad_I2C( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR );


//boolean blink = false;
//boolean ledPin_state;
int Cursor = 0;
int col = 0;
int dos = 0;
char hora_comida[5];
char hora_orina[5];
char hora_evacuar[5];
char hora_farmaco[5];
void setup(){
    Serial.begin(9600);
    //pinMode(ledPin, OUTPUT);              // Sets the digital pin as output.
    //digitalWrite(ledPin, HIGH);           // Turn the LED on.
    //ledPin_state = digitalRead(ledPin);   // Store initial LED state. HIGH when LED is on.
    //keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
    keypad.addEventListener(keypadEvent); //add an event listener for this keypad

    i2c_lcd.begin (20,4); //  our LCD is a 16x2, change for your LCD if needed
    // LCD Backlight ON
    i2c_lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
    i2c_lcd.setBacklight(HIGH); 
    i2c_lcd.clear(); // Clear the LCD screen

    keypad.begin();
    
    //lcd.init();
    //lcd.backlight();
    i2c_lcd.setCursor(0,0);
    i2c_lcd.print("Hora comida=");
    i2c_lcd.setCursor(0,1);
    i2c_lcd.print("Hora orina=");
    i2c_lcd.setCursor(0,2);
    i2c_lcd.print("Hora evacuar=");
    i2c_lcd.setCursor(0,3);
    i2c_lcd.print("Hora farmaco=");
}

void loop(){
    char key = keypad.getKey();
    if (key) {
      if (Cursor == 0){
        Serial.println("comida");
        Serial.println(hora_comida);
      }
      else if (Cursor == 1){
        Serial.println("orina");
        Serial.println(hora_orina);
      }
      else if (Cursor == 2){
        Serial.println("evacuar");
        Serial.println(hora_evacuar);
      }
      else if (Cursor == 3){
        Serial.println("farmaco");
        Serial.println(hora_farmaco);
      }
    }
}

// Taking care of some special events.
void keypadEvent(KeypadEvent key){
    switch (keypad.getState()){
    case PRESSED:
        if (key == 'A'){
          Serial.println("Presionado A");
          Cursor = 0;
          col = 12;
          dos = 0;
        }
        else if (key == 'B'){
          Serial.println("Presionado B");
          Cursor = 1;
          col = 11;
          dos = 0;
        }
        else if (key == 'C'){
          Serial.println("Presionado C");
          Cursor = 2;
          col = 13;
          dos = 0;
        }
        else if (key == 'D'){
          Serial.println("Presionado D");
          Cursor = 3;
          col = 13;
          dos = 0;
        }
        else {
          if (dos < 5){
            if (Cursor == 0){
              hora_comida[dos]= key;
            }
            else if (Cursor == 1){
              hora_orina[dos]=key;
            }
            else if (Cursor == 2){
              hora_evacuar[dos]=key;
            }
            else if (Cursor == 3){
              hora_farmaco[dos]=key;
            }
            i2c_lcd.setCursor(col,Cursor);
            i2c_lcd.print(key);
            col = col + 1;
            dos = dos + 1;
            if (dos == 2){
              if (Cursor == 0){
                hora_comida[dos]= ':';
              }
              else if (Cursor == 1){
                hora_orina[dos]=':';
              }
              else if (Cursor == 2){
                hora_evacuar[dos]=':';
              }
              else if (Cursor == 3){
                hora_farmaco[dos]=':';
              }
              i2c_lcd.setCursor(col,Cursor);
              i2c_lcd.print(":");
              col = col + 1;
              dos = dos + 1;
              }
             }

         }
        if (key == '#') {
            Serial.println("Presionado");
            }
        break;
    }
}
