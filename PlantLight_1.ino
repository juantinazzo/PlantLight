#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

float ver=0.1;
int R=0,G=0,B=0,W=0,All=0;
bool mainscreen=1,msp=0;
bool arrow[2]={16};
int possel=4;
int PR=6,PG=9,PB=10,PW=11;

byte AI1[8] = {
  0b00001,
  0b00010,
  0b00100,
  0b01000,
  0b00100,
  0b00010,
  0b00001,
  0b00000
};

byte AD1[8] = {
  0b10000,
  0b01000,
  0b00100,
  0b00010,
  0b00100,
  0b01000,
  0b10000,
  0b00000
};

void setup()
{
  lcd.init();                      // initialize the lcd 
 // lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("- PlantLight -");
  lcd.setCursor(1,1);
  lcd.print("Ver:");
  lcd.print(ver,1);
  lcd.print(" by CSJ");
  lcd.createChar(0, AI1);
  lcd.createChar(1, AD1);
  pinMode(PR,OUTPUT);
  pinMode(PG,OUTPUT);
  pinMode(PB,OUTPUT);
  pinMode(PW,OUTPUT);
  delay(2000);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Initializing...");
  lcd.setCursor(0,1);
  for(int i=1;i<16;i++){
    lcd.print("-");
    delay(100);
  }
   lcd.clear();
  //lcd.blink();
}


void loop()
{
    if(mainscreen){
        //lcd.noCursor();
          if(!msp){
               MainScreenPrepare();
          }
          MainScreenUpdate();
        //lcd.setCursor(15,0);

      
    }
}

void MainScreenPrepare(){
    lcd.setCursor(0,0);   
    lcd.print("R:");
    lcd.setCursor(7,0);   
    lcd.print("G:");
    lcd.setCursor(0,1);   
    lcd.print("B:");
    lcd.setCursor(7,1);   
    lcd.print("W:");
    MoveArrow()
    
    msp=1;
    
         
}

void MainScreenUpdate(){
    lcd.setCursor(0,1);   
    lcd.print(R/2.55,0);
    lcd.print("%");
    lcd.setCursor(4,1);   
    lcd.print(G/2.55,0);
    lcd.print("%");
    lcd.setCursor(8,1);   
    lcd.print(B/2.55,0);
    lcd.print("%");
    lcd.setCursor(12,1);   
    lcd.print(W/2.55,0);
    lcd.print("%");
}

void MoveArrow(int y, int x, bool dir){
    if(arrow[0]!=16){
    lcd.setCursor(arrow[0],0);
    lcd.print(" ");
    }
    if(arrow[1]!=16){
    lcd.setCursor(arrow[1],1);
    lcd.print(" ");
    }
    lcd.setCursor(y,x);
    arrow[y]=x;
    lcd.write((uint8_t)dir);
}

void UpdatePWM(int Rpwm, int Gpwm, int Bpwm, int Wpwm){
    analogWrite(PR,Rpwm);
    analogWrite(PG,Gpwm);
    analogWrite(PB,Bpwm);
    analogWrite(PW,Wpwm);
}


