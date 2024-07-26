#include <Arduino.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);



//......................................colors


#define needleColor 0x07E0

//............................dont edit this
int cx=120;
int cy=200;
int r=120;
int ir=118;


float x[360]; //outer points of Speed gaouges
float y[360];
float px[360]; //ineer point of Speed gaouges
float py[360];
float lx[360]; //text of Speed gaouges
float ly[360];
float nx[360]; //needle low of Speed gaouges
float ny[360];


double rad=0.01745;
unsigned short color1;
unsigned short color2;
float sA;
float rA;
int blinkPeriod=500;
unsigned long currentTimeL=0;
unsigned long currentTimeR=0;
int brightnesses[5]={40,80,120,150,240};
int selectedBrightness=3;
int deb1=0;
int deb2=0;
int debB=0;





// .........important variables
bool leftPointer=0;
bool rightPointer=0;
bool braking;
int lights=0;  //0 is lights off, 1 is short light, 2 is long lights
float speedAngle=0; //...speed variable 0-240
float rpmAngle=5;  //.....RPM variable 0-9

void setup() {



  tft.init();

    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    sprite.createSprite(320,300);
    sprite.setSwapBytes(true);
    sprite.setTextDatum(4);
    sprite.setTextColor(TFT_WHITE,TFT_BLACK);
    sprite.setTextDatum(4);





  int a=120;
  for(int i=0;i<360;i++)
    {
       x[i]=((r-10)*cos(rad*a))+cx;
       y[i]=((r-10)*sin(rad*a))+cy;
       px[i]=((r-14)*cos(rad*a))+cx;
       py[i]=((r-14)*sin(rad*a))+cy;
       lx[i]=((r-24)*cos(rad*a))+cx;
       ly[i]=((r-24)*sin(rad*a))+cy;
       nx[i]=((r-36)*cos(rad*a))+cx;
       ny[i]=((r-36)*sin(rad*a))+cy;


       a++;
       if(a==360)
       a=0;
    }

}



void draw()
{
  sprite.fillSprite(TFT_BLACK);



   for(int i=0;i<selectedBrightness;i++)
   sprite.fillSmoothRoundRect(8+(i*4),6,2,9,1,TFT_RED,TFT_BLACK);

   sprite.fillSmoothCircle(cx, cy, r+2, TFT_BLACK);
   sprite.fillSmoothCircle(320-cx, cy, r+2, TFT_BLACK);





  sprite.drawSmoothArc(cx, cy, r, ir, 30, 330, TFT_YELLOW, TFT_BLACK);

  sprite.drawSmoothArc(cx, cy, r-5, r-6, 30, 330, TFT_WHITE, TFT_BLACK);







   //.....................................................draw GAUGES
     for(int i=0;i<26;i++){
     if(i<20) {color1=TFT_YELLOW; color2=TFT_WHITE;} else {color1=TFT_RED; color2=TFT_RED;}

  if(i%2==0) {
  sprite.drawWedgeLine(x[i*12],y[i*12],px[i*12],py[i*12],2,1,color1);
  sprite.setTextColor(color2,TFT_BLACK);
  sprite.drawString(String(i*2),lx[i*12],ly[i*12], 4);
  }else
  sprite.drawWedgeLine(x[i*12],y[i*12],px[i*12],py[i*12],1,1,color2);
  }



  // ........................................needles draw
   sA=speedAngle*1.2;

   sprite.drawWedgeLine(px[(int)sA],py[(int)sA],nx[(int)sA],ny[(int)sA],2,2,needleColor);



  //.....................................drawing  TEXT
  sprite.setTextColor(TFT_WHITE,TFT_BLACK);
 sprite.drawString(String((int)speedAngle),cx,cy,6);

  sprite.drawString("KM/H",cx,cy+16);




   //..............................................push Sprite to screen
   sprite.pushSprite(0,10);

}



void loop() {




  draw();



}