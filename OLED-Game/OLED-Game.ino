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
#define bit8       A7
#define bit4       A6
#define bit2       A5
#define bit1       A4
#define butUp      A3
#define butLeft    A2
#define butDown    A1
#define butRight   A0

  float pi = 3.141592;
  long startTime = millis();
  long startTime2 = millis();
  byte jumping;
  int sc;
  byte died = false;
  byte started = false;
  byte started2 = false;
  long score = 0;
  int time;
  int w = 128;
  int h = 64;
  int cx = w/2;
  int cy = h/2;
  int i;
  int A;
  int B;
  int Type;
  int c=0;
  
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
  pinMode(A7,INPUT_PULLUP);
  pinMode(A6,INPUT_PULLUP);
  pinMode(A5,INPUT_PULLUP);
  pinMode(A4,INPUT_PULLUP);
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
  int b1 = (1-digitalRead(bit1));
  int b2 = (1-digitalRead(bit2))*2;
  int b4 = (1-digitalRead(bit4))*4;
  int b8 = (1-digitalRead(bit8))*8;

   //if you only want the game, remove everything from the loop function and replace it with run(); 
   switch(b1+b2){
   case 0:Run();break;
   case 1:eng3d();break;
   case 2:mandelbrot();break;
   }
}

void eng3d() {
  byte up =  (!digitalRead(butUp));
  byte dow = (!digitalRead(butDown));
  byte lef = (!digitalRead(butLeft));
  byte rig = (!digitalRead(butRight));
  byte cen = (!digitalRead(butCenter));
  
  if (up)A+=4;
  if (dow)A-=4;
  if (lef)B-=4;
  if (rig)B+=4;
  if (lef&&rig) Type++;
  if (up&&dow) Type--;
  engine(A,B,64+Type,1);
  display.display();
  display.clearDisplay();
  i++;
}

void mandelbrot(){
    float width = 128;
    float height = 128;
    float max = c+10;
    c++;

    for (int row = 0; row < 64; row++) {
      for (int col = 0; col < 128; col++) {
        
        double c_re = (col - width/2)/(pow(2,c%27)-1);
        double c_im = (row - height/2)/(pow(2,c%27))+1;
        
        double x = 0,y = 0;
        int iteration = 0;
        
        while (x*x+y*y <= 4 && iteration < max) {
          double x_new = x*x - y*y + c_re;
          y = 2*x*y + c_im;
          x = x_new;

          //y += 1;
          
          iteration++;
        }
        if (iteration < max) display.drawPixel(col, row, iteration%2);
        else{display.drawPixel(col, row, 0);}
      }
    }
    //draws pixels to screen when finished rendering frame
    display.display();
  }

void engine(int angleX,int angleY,float camZoom,byte state){

 //doesnt work yet
 //cylinder generator
 /* float verts[8][3];
  int joints[8][2];
  int length = 1;
  int radius = 1;
  int segments = 4;

  for(int i=0;i<2;i++){
    for(int j=0;j<segments;j++){
      float x;
      float y;
      float z;

      x = (-length/2)+i*length;
      y = radius * sin(2*pi/segments*j);
      z = radius * cos(2*pi/segments*j);

      verts[j*(i+1)][0]=x;
      verts[j*(i+1)][1]=y;
      verts[j*(i+1)][2]=z;

      joints[j*(i+1)][0]=(j-1)%4;
      joints[j*(i+1)][1]=j;
    }
  }*/
   //octahedron
   //float verts[6][3] = {{0,-2,0}, {0,0,-2}, {-2,0,0}, {0,0,2},{2,0,0},{0,2,0}};
   //int joints[12][2] = {{0,1},{1,2},{2,3},{3,4},{4,5},{5,1},{5,2},{5,3},{0,2},{0,3},{0,4},{4,1}};
   
   
   //cube
   float verts[8][3] = {{-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1},{-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1}};
   int joints[12][2] = {{0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},{0,4},{1,5},{2,6},{7,3}};


  /* //Triangular Pyramid
   float verts[4][3] = {{0,-2,0},{-1.5,1,-1.5},{1.5,1,-1.5},{0,1,1.5}};
   int joints[6][2] = {{0,1},{1,2},{2,3},{3,0},{3,1},{2,0}};
   */
   
//this for loop rotates the object by editing the vertex coordinates 
for (int i=0;i<(sizeof(verts)/sizeof(verts[0]));i++){

   //getting the vertex coordinates
   float x = verts[i][0];
   float y = verts[i][1];
   float z = verts[i][2];

   float xx = x;
   float yy = y;
   float zz = z;

//converts the angle in degree to radian
float xangle = (angleX%360) / (180 / pi);
float yangle = (angleY%360) / (180 / pi);

//calculates the vertex positions for rotation
x = (cos(yangle)* xx + sin(yangle)*sin(xangle)*yy - sin(yangle)*cos(xangle)*zz);

y = 0 + cos(xangle)*yy + sin(xangle)*zz;

z = (sin(yangle)*xx + cos(yangle)* - sin(xangle)*yy + cos(yangle)*cos(xangle)*zz) ;
   
      verts[i][0] += x - verts[i][0];
      verts[i][1] += y - verts[i][1];
      verts[i][2] += z - verts[i][2];

}
     //this for loop converts 3d vertices into 2d and draws edges between them
      for (int i=0;i<(sizeof(joints)/sizeof(joints[1]));i++){

        //camera offset
        float camX = 0;
        float camY = 0;
        float camZ = 5;

        //getting the vertex coordinates
        float x = verts[joints[i][0]][0]+camX;
        float y = verts[joints[i][0]][1]+camY;
        float z = verts[joints[i][0]][2]+camZ;

        float x2 = verts[joints[i][1]][0]+camX;
        float y2 = verts[joints[i][1]][1]+camY;
        float z2 = verts[joints[i][1]][2]+camZ;


        //this gives depth to the z axis
        float f = camZoom/z;
        x=x*f;
        y=y*f;

        float f2 = camZoom/z2;
        x2=x2*f2;
        y2=y2*f2;

        //draws the lines
        display.drawLine(x+cx,y+cy,x2+cx,y2+cy,state);
  }
}

//----------------run game---------------//

void Run(){

    boolean pressed = !digitalRead(butCenter);

    display.setCursor(40,0);

    //resets time and sets started to true
    if (pressed){
      if (!started) startTime2 = millis();
      started = true;
    }

    //this counts the milliseconds 
    float time = (millis()-startTime2);
    float tim = time*2;

    //this stops increasing the speed when the time is more then 50000
    if (time > 50000)time = tim; else time *= 1+time/200000;
    score = time/100;

    //if started equals true
    if (started){
      
      //this integer changes from 0 to 35 and back to 0 in a smooth way for the jumping animation
      int jumP = jump(pressed&&started2&&time>300,35,1.6);

      //this prevents jumping if the game hasn't started yet
      if (started2&&time<300)display.drawBitmap(30, 48,  player, 16, 16, 1);

      //this is for the movement of the obstacle
      int move = 128-((int(time/7)))%144;

      //detects if the player has colided with the obstacle
      boolean collisionx = move <= 48-jumP&&20+move >= 48-jumP;

      //draws player and obstacle
      display.drawBitmap(30, 48-jumP,  player, 16, 16, 1);
      display.fillRect(move,46,16,16,1);

      started2 = true;

      //sets died to true if the player has colided with the obstacle 
      if (collisionx&&jumP < 16)died = true;

      //draws score to screen
      display.println(score);
    }else{
      //draws stars screen
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(40,0);
      display.println("play");
      display.drawBitmap(30, 48,  player, 16, 16, 1);
    }
    
    if (!died)sc = score;

    //displays text and score when died equals true
    if (died)diedtxt(); 

    //refreshes screen when died equals false
    if (!died){
      display.display();
      display.clearDisplay();
    }
}


int jump(boolean trigjump,int height,float speed){
    //count milliseconds
    long time = (millis()-startTime);
    
    //this latches and resets time when trigjump equals true
    if (trigjump && !jumping){
      jumping = true;
      startTime = millis();
    }
    
    //time = time * speed
    time *= speed;

    //stops jumping if time equals 1000
    if (time >= 1000){
      jumping = false;
    }
    
    //create jump physics
    if (jumping){
      float jump = height * (float)sin(((PI*2)/1000)*(time/2));
      return jump;
    }else return 0;
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
