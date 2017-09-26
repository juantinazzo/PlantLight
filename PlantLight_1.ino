#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ClickEncoder.h>             //Libreria necesaria para el rotary encoder. Para descargarla: https://github.com/0xPIT/encoder/tree/arduino
#include <TimerOne.h>                 //Libreria necesaria para el rotary encoder

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

float ver=0.3;
int R=0,G=0,B=0,W=0,All=0;
bool mainscreen=1,msp=0,F1=0;
bool arrow[16][2]={0};
int possel=0;
int PR=6,PG=5,PB=3,PW=11;
byte buffer1[4];
ClickEncoder *encoder; 
void timerIsr() {
  encoder->service();
}
long tiempo,tiempo2;

#define der 4                                                                        //Pin del rotary encoder de giro hacia la derecha
#define izq 7                                                                        //Pin del rotary encoder de giro hacia la izquierda
#define pulsador 8

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
  Serial.begin(9600);
  Serial.setTimeout(100);
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
  pinMode(der,INPUT);                                         //Seteamos los pines de salida del encoder
  pinMode(izq,INPUT);
  Timer1.initialize(1000); 
  Timer1.attachInterrupt(timerIsr); 
  encoder = new ClickEncoder(der, izq, pulsador); 
  encoder->setAccelerationEnabled(0);
  delay(2000);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Initializing...");
  lcd.setCursor(0,1);
  for(int i=1;i<16;i++){
    lcd.print("-");
    delay(100);
  }
   F1=1;
   Valor();
   lcd.clear();

    R=127;
    G=127;
    B=127;
    W=127;
    F1=1;

   tiempo=millis();
   tiempo2=tiempo;
   MainScreenUpdate();
}


void loop()
{
  Encoder(); 
  //Valor();    
    if(mainscreen){
          if(!msp) MainScreenPrepare();
          if((Valor()||SerialLeer())&&(tiempo+20<millis())){
            MainScreenUpdate();
            tiempo=millis();
          }
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
    boton();    
    msp=1;
}

void MainScreenUpdate(){
    lcd.setCursor(3,0);   
    lcd.print(R/2.55,0);
    lcd.print("%");
    if(R/2.55<10) lcd.print("  ");
    else if (R/2.55<100) lcd.print(" ");
    lcd.setCursor(11,0);   
    lcd.print(G/2.55,0);
    lcd.print("%");
    if(G/2.55<10) lcd.print("  ");
    else if (G/2.55<100) lcd.print(" ");
    lcd.setCursor(3,1);   
    lcd.print(B/2.55,0);
    lcd.print("%");
    if(B/2.55<10) lcd.print("  ");
    else if (B/2.55<100) lcd.print(" ");
    lcd.setCursor(11,1);   
    lcd.print(W/2.55,0);
    lcd.print("%");
    if(W/2.55<10) lcd.print("  ");
    else if (W/2.55<100) lcd.print(" ");
}

void NewArrow(int y, int x, bool dir)
{
    lcd.setCursor(y,x);
    arrow[y][x]=1;
    lcd.write((uint8_t)dir);
}

void DeleteArrow(int y, int x)
{
    lcd.setCursor(y,x);
    lcd.print(" ");
    arrow[y][x]=0;
}

void UpdatePWM(int Rpwm, int Gpwm, int Bpwm, int Wpwm)
{
    analogWrite(PR,Rpwm);
    analogWrite(PG,Gpwm);
    analogWrite(PB,Bpwm);
    analogWrite(PW,Wpwm);
}

void boton()
{
  possel+=1;
  if(possel>4){ possel=0;}
  switch(possel){
    case 0:
       NewArrow(0,0,1);
       NewArrow(8,0,1);
       NewArrow(0,1,1);
       NewArrow(8,1,1);
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

void Encoder()
{
    ClickEncoder::Button b = encoder->getButton();
    if ((b != ClickEncoder::Open)&&(tiempo2+300<millis())) {
         if(b==ClickEncoder::DoubleClicked) boton();
         tiempo2=millis();
    }
}

bool Valor()
{
  int vale = encoder->getValue();
  if(vale!=0||F1){
       switch(possel){
          case 0:
                R+=vale;
                G+=vale;
                B+=vale;
                W+=vale;
             break;
          case 1:
                R+=vale;
                break;
           case 2:
                G+=vale;
                break;
           case 3:
               B+=vale;
               break;
           case 4:
               W+=vale;
               break;
        }
      
      if(R>255)R=255;
      if(G>255)G=255;
      if(B>255)B=255;  
      if(W>255)W=255;
      if(R<0) R=0;
      if(G<0) G=0;
      if(B<0) B=0;
      if(W<0) W=0;
  
      UpdatePWM(R,G,B,W);
      F1=0;
      return 1;
    }  
  else return 0;
}

bool SerialLeer()
{
    if(Serial.available() > 0){
      Serial.readBytesUntil('\n',buffer1,4);
      R=buffer1[0];
      G=buffer1[1];
      B=buffer1[2];
      W=buffer1[3];
      UpdatePWM(R,G,B,W);
      return 1;
    }
    else return 0;
}


