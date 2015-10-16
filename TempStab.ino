// Copyright (c) 14-05-2015
// Temperature Stabilizer
// Luqman Sahidin - Release for public domain
// lsahidin@yahoo.com

/*-----( Import needed libraries )-----*/
#include <dht11.h>
#include <Wire.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
dht11 DHT11;

/*-----( Declare Constants, Pin Numbers )-----*/
#define DHT11PIN 7
#define LED_PIN  6
#define BUTT_UP  A3
#define BUTT_DOWN  A2
#define RELAY  A0

int count_dlay;
int frame,
    butt_up, 
    butt_down,
    pressed = 0;
byte value;
char derajat = 0xDF; // karakter derajat
// lcd_putc(224); //ascii alpha
// lcd_putc(226); //ascii beta

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  //Serial.begin(9600); //(Remove all 'Serial' commands if not needed)
  lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines, turn on backlight
  lcd.backlight();
  
  // Initialize PIN
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUTT_UP, INPUT);
  pinMode(BUTT_DOWN, INPUT);
  
  // Print a message "HELLO" to the LCD.
  // EEPROM.write(0, 30); // untuk EEPROM Memory Tesss..
  lcd.print("PENETASAN TELUR");
  lcd.setCursor(0, 1);
  lcd.print("- Yugan Studio -");
  // Set Memory
  value = EEPROM.read(0);
  delay(2000);
  
  lcd.clear();
  
  lcd.print(" Initializing");
  lcd.setCursor(0, 1);
  lcd.print(" please wait... ");
  
  count_dlay = 0;
  digitalWrite(LED_PIN, HIGH);

}/*--(end setup )---*/

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{   
  int chk = DHT11.read(DHT11PIN);
  butt_up = readButtons(BUTT_UP);
  butt_down = readButtons(BUTT_DOWN);
  
  //Debug only
  /*switch (chk)
  {
    case 0: Serial.println("OK"); break;
    case -1: Serial.println("Checksum error"); break;
    case -2: Serial.println("Time out error"); break;
    default: Serial.println("Unknown error"); break;
  }*/
  
  if (chk == 0)
  {
    if (count_dlay == 20)
    {
      if (frame == 0)
      {
        lcd.clear();
        lcd.print("Maximum Temp ");
        update_meas();
        //lcd.setCursor(15,0);
        //lcd.print("C");
        lcd.setCursor(0, 1);
        lcd.print("Current => ");
        lcd.setCursor(13,1);
        lcd.print("C");
        
        frame = 1; // trigger to exit reload the frame
      }
    }
  
  // Tombol Kepencet kah? already Butt..
  if (pressed == 0)
  {
    if (butt_up == 1)
    {
      EEPROM.write(0, value + 1);
      value = value + 1;
      update_meas();
    }
    
    if (butt_down == 1)
    {
      EEPROM.write(0, value - 1);
      value = value - 1;
      update_meas();
    }
  }
  
  //restore button state
  if (butt_up == 0 && butt_down == 0)
  {
    pressed = 0;
  }
  else
  {
    pressed = 1;
  }
  
 //Here you go... take the suhu & Display it.
  if (count_dlay == 20)
  {    
    lcd.setCursor(11, 1);
    lcd.print((float)DHT11.temperature, 0);
    digitalWrite(LED_PIN, HIGH);
    delay(20);
    digitalWrite(LED_PIN, LOW);
    count_dlay = 0;
   }

  if ((float)DHT11.temperature >= value)
  {
    //RELAY ON
    digitalWrite(RELAY, HIGH);
  }
  else if ((float)DHT11.temperature <= value - 2)
  {
    //RELAY OFF
    digitalWrite(RELAY, LOW);
  }
    
  count_dlay++;
  delay(20);
  }
}
/* --(end main loop )-- */

/* --(functions)-- */
void update_meas()
{
  lcd.setCursor(13,0);
  lcd.print("   ");
  lcd.setCursor(13,0);
  lcd.print(value, DEC);
  lcd.print("C");
}

int readButtons(int pin)
{
  int butt, b = 0;
  butt = analogRead(pin);
  
  if (butt < 1020)
  {
    b = 0;
  }
  else
    if (butt > 1020 && butt < 1024)
    {
      b = 1;
    }
  return b;
}

/* --(end of functions)-- */
