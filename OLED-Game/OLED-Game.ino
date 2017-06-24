#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// If using software SPI (the default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
#define butCenter  5
#define butUp      A3
#define butLeft    A2
#define butDown    A1
#define butRight   A0

  long startTime = millis();
  long startTime2 = millis();
  boolean jumping;
  int sc;
  boolean died = false;
  boolean started = false;
  boolean started2 = false;
  long score = 0;
  int time;

static const unsigned char PROGMEM player[] =
{ B00000000, B11110000,
  B00000000, B11110000,
  B00000001, B11111000,
  B00000000, B11010000,
  B00000000, B11111000,
  B00000000, B11110000,
  B00000000, B01100000,
  B00000000, B11110000,
  B00000001, B11111000,
  B00000011, B11111100,
  B00000111, B11111110,
  B00000110, B11110110,
  B00000000, B11111000,
  B00000001, B11011010,
  B00000011, B00001110,
  B00000011, B00001100 };

void setup()   {                
  pinMode(5, INPUT_PULLUP);
  pinMode(A3,INPUT_PULLUP);
  pinMode(A2,INPUT_PULLUP);
  pinMode(A1,INPUT_PULLUP);
  pinMode(A0,INPUT_PULLUP);

  setupScreen();
}

void setupScreen()   {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
}
void loop() {
   Run();
}


//----------------run game---------------//

void Run(){

    boolean pressed = !digitalRead(butCenter);

    display.setCursor(40,0);
    
    if (pressed){
      if (!started) startTime2 = millis();
      started = true;
    }

    float time = (millis()-startTime2);
    if (!time > 500000)time *= 1+time/200000; else time *= 1+time/200000;
    score = time/100;
    int jumP;

    //dont even bother trying to read this
    if (started){
      jumP = jump(pressed&&started2&&time>300,35,1.6);
      if (started2&&time<300)display.drawBitmap(30, 48,  player, 16, 16, 1);
      int move = 128-((int(time/7)))%144;
      boolean collisionx = move <= 48-jumP&&20+move >= 48-jumP;
      display.drawBitmap(30, 48-jumP,  player, 16, 16, 1);
      display.fillRect(move,46,16,16,1);
      started2 = true;
      if (collisionx&&jumP < 16)died = true;
      display.println(score);
    }else{
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(40,0);
      display.println("play");
      display.drawBitmap(30, 48,  player, 16, 16, 1);
    }
    
    if (!died)sc = score;
    if (died)diedtxt(); 
    //if (died&&pressed) died = false; 

    if (!died){
      display.display();
      display.clearDisplay();
    }
}

int jump(boolean trigjump,float height,float speed){
    //count milliseconds
    long time = (millis()-startTime);
    
    
    if (trigjump && !jumping){
      jumping = true;
      startTime = millis();
      time = 10;
    }
    
    //time = time * speed
    time *= speed;
    
    if (time >= 1000){
      jumping = false;
    }
    //create jump physics
    if (jumping){
      float jump = height * (float)sin(((PI*2)/1000)*(time/2));
      return jump;
    }//else return 0;
  }

void diedtxt(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(40,0);
  display.clearDisplay();
  display.println("Game");
  display.setCursor(40,16);
  display.println("Over");
  display.setTextSize(3);
  display.setCursor(40,32);
  display.println(sc);
  display.display();
  
}
