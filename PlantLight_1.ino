#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ClickEncoder.h>             //Libreria necesaria para el rotary encoder. Para descargarla: https://github.com/0xPIT/encoder/tree/arduino
#include <TimerOne.h>                 //Libreria necesaria para el rotary encoder

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

float ver=0.1;
int R=0,G=0,B=0,W=0,All=0;
bool mainscreen=1,msp=0;
bool arrow[16][2]={0};
int possel=0;
int PR=6,PG=9,PB=10,PW=11;
ClickEncoder *encoder; 
void timerIsr() {
  encoder->service();
}
#define der 8                                                                        //Pin del rotary encoder de giro hacia la derecha
#define izq 7                                                                        //Pin del rotary encoder de giro hacia la izquierda
#define pulsador 4

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
  pinMode(13,OUTPUT);
 // pinMode(2,INPUT_PULLUP);
  pinMode(der,INPUT);                                         //Seteamos los pines de salida del encoder
  pinMode(izq,INPUT);
  //pinMode(3,INPUT_PULLUP);
  //attachInterrupt(0,boton,FALLING);
  //attachInterrupt(1,led,FALLING);
  Timer1.attachInterrupt(timerIsr); 
  encoder = new ClickEncoder(der, izq, pulsador); 
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

    R=255;
    G=255;
    B=255;
    W=255;

}


void loop()
{
  Encoder(); 
      
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
    lcd.setCursor(1,0);   
    lcd.print("R:");
    lcd.setCursor(9,0);   
    lcd.print("G:");
    lcd.setCursor(1,1);   
    lcd.print("B:");
    lcd.setCursor(9,1);   
    lcd.print("W:");
        
    msp=1;
    
         
}

void MainScreenUpdate(){
    lcd.setCursor(3,0);   
    lcd.print(R/2.55,0);
    lcd.print("%");
    lcd.setCursor(11,0);   
    lcd.print(G/2.55,0);
    lcd.print("%");
    lcd.setCursor(3,1);   
    lcd.print(B/2.55,0);
    lcd.print("%");
    lcd.setCursor(11,1);   
    lcd.print(W/2.55,0);
    lcd.print("%");
}

void NewArrow(int y, int x, bool dir){

    lcd.setCursor(y,x);
    arrow[y][x]=1;
    lcd.write((uint8_t)dir);
}

void DeleteArrow(int y, int x){

    lcd.setCursor(y,x);
    lcd.print(" ");
    arrow[y][x]=0;
}

void UpdatePWM(int Rpwm, int Gpwm, int Bpwm, int Wpwm){
    analogWrite(PR,Rpwm);
    analogWrite(PG,Gpwm);
    analogWrite(PB,Bpwm);
    analogWrite(PW,Wpwm);
}

void boton(){
  possel+=1;
  if(possel>4){ possel=0;}
  switch(possel){
    case 0:
       NewArrow(0,0,1);
       NewArrow(8,0,1);
       NewArrow(0,1,1);
       NewArrow(8,1,1);
       digitalWrite(13,!digitalRead(13));
       break;
    case 1:
       NewArrow(0,0,1); 
       DeleteArrow(8,0);
       DeleteArrow(0,1);
       DeleteArrow(8,1);
       break;
     case 2:
       NewArrow(8,0,1); 
       DeleteArrow(0,0);
       break;
     case 3:
         NewArrow(0,1,1); 
         DeleteArrow(8,0);
         break;
     case 4:
       NewArrow(8,1,1); 
       DeleteArrow(0,1);
       break;
  }

 
}

void led(){
  digitalWrite(13,!digitalRead(13));
}

void Encoder(){
    ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
       boton();
      
    }
  }


