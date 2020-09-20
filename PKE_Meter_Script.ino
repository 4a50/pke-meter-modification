#define NUMELEMENTS(x) (sizeof(x) / sizeof(x[0]))
#include "OLED_Driver.h"
#include "OLED_GFX.h"
#include <SPI.h>
#include <Wire.h>

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define DKGREEN 0x09E0//1300
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GRAY    0x2945//0x738E
OLED_GFX oled = OLED_GFX();
char pkeOut[6];
int pkeOutLength = 0;
float potRead = 0.0f;
int interval = 1000;
//Timer var setup
int maxInterval = 500;
int minInterval = 5;//20
uint8_t screen1StateTracker;
uint8_t pkeWingLightTracker;
int minWingSpan = 545;
int maxWingSpan = 2400;
int pkeWingControlPos = minWingSpan;
int prevPkeWingControlPos = 0;
unsigned long prevMilliSec = 0;
const int pkeWingLights[] = {0, 1, 2, 3, 4, 5, 6};//{2, 0, 5, 3, 6, 4, 1}; //Set the order of light off with pinNums
int servoPin = 22;
//
void setup()  {
  for (int i = 0; i < 7; i++) {pinMode(pkeWingLights[i], OUTPUT);}
  //Servo Setup
  
  pinMode(servoPin, OUTPUT);
  
  //Init GPIO
  pinMode(oled_cs, OUTPUT);
  pinMode(oled_rst, OUTPUT);
  pinMode(oled_dc, OUTPUT);
  //Init UART
  Serial.begin(115200);
#if INTERFACE_4WIRE_SPI
  //Init SPI
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();
#elif INTERFACE_3WIRE_SPI
  pinMode(oled_sck, OUTPUT);
  pinMode(oled_din, OUTPUT);
#endif




//

//Screen Opening Display
  oled.Device_Init();
  oled.Clear_Screen();
  noGhostLogo(RED, WHITE, 12);
  
  oled.Set_Color(WHITE);
  oled.print_String(25, 95, "Ghostsmashers", FONT_5X8);
  delay(500);
  oled.Set_Color(CYAN);
  oled.print_String(22, 105, "PKE Meter v1.01", FONT_5X8);
  delay(20);
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(servoPin, LOW);
  delay(500);
  oled.Set_Color(YELLOW);
  oled.print_String(30, 115, "(c)2020 4a50", FONT_5X8);
  delay (3000);
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(2000);
  digitalWrite(servoPin, LOW);
  delay(20);
  oled.Clear_Screen();
}


void loop() { 
    
   
   unsigned long currentMilliSec = millis();
   float pkeReading = 0.0; 
   int pkePotRead = 0;   
   
   //potentiometer //Max Value is 660 potRead will be a percentage
   pkePotRead = (analogRead(A1)/660);
   potRead = (analogRead(A1) / 660.0f);    
   if (potRead >= 1.0000){ potRead = .999f;}   
   pkeReading = potRead * 100.0f;  
   
   //Setting the interval for the lights and screen circles
    interval = maxInterval - ((maxInterval - minInterval)* potRead);  //minInterval - ((maxInterval - minInterval)* potRead);
    if (interval >=1000){interval = 1000;}
    
    pkeWingControlPos = minWingSpan + ((maxWingSpan - minWingSpan)* potRead);  //minInterval - ((maxInterval - minInterval)* pkePotRead);
    if (pkeWingControlPos >=2400){pkeWingControlPos = 2400;}
    else if (pkeWingControlPos <= 545){pkeWingControlPos = 545;}
    //if (pkeWingControlPos % 20 == 0){pkeServo.writeMicroseconds(pkeWingControlPos); }
    
    Serial.print("PotRead: "); Serial.print(potRead);
    Serial.print("\tInterval: ");Serial.print(interval);
    
   
    
    
    
   
   //
   screen1(screen1StateTracker, pkeReading);
   currentMilliSec = millis();
   //This area is controlled by the interval loop.  Only update when currentMill > prev Mill
   if (currentMilliSec - prevMilliSec >= interval){    
    
//    Serial.print("PKE Reading: ");
//    Serial.println(String(pkeReading));
      
      if (pkeWingLightTracker == 0){ 
        digitalWrite(pkeWingLights[pkeWingLightTracker], HIGH);
        digitalWrite(pkeWingLights[6], LOW);
      }        
        else {
          digitalWrite(pkeWingLights[pkeWingLightTracker], HIGH);
          digitalWrite(pkeWingLights[pkeWingLightTracker - 1], LOW);}       
   
   
      
                 
      
      
   
    pkeWingLightTracker++;
    if (pkeWingLightTracker >=7) {pkeWingLightTracker = 0;}
    screen1StateTracker++;
    if (screen1StateTracker >= 5) {screen1StateTracker = 0;}
    prevMilliSec = currentMilliSec;
}
   
  Serial.print("\tServo Position: "); Serial.println(pkeWingControlPos);
  
  delay(5);
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(pkeWingControlPos);
  digitalWrite(servoPin, LOW);
  delay(5);
  prevPkeWingControlPos = pkeWingControlPos;
}

  
  /////////////////////////////////////////////


void screen1(int lightIndex, float pkeReading){
  
  if (lightIndex == 0){
    oled.Set_Color(BLACK);
    oled.Fill_Circle(120, 58, 8);  //5
    
    
    oled.Set_Color(GRAY);
    oled.Draw_Line(126,0,126,128); //vertline
    oled.Draw_Line(105,63,128,63);   //horizline
    
    oled.Set_Color(GREEN);
    oled.Fill_Circle(21, 21, 8); //1
  }
  if (lightIndex == 1){
    oled.Set_Color(GREEN);
    oled.Fill_Circle(53, 21, 8); //2
    oled.Set_Color(BLACK);
    oled.Fill_Circle(21, 21, 8); //1
    //gridLines(21, GRAY);
    oled.Set_Color(GRAY);
    oled.Draw_Line(21,0,21,42); //vertline
    oled.Draw_Line(0,21,42,21);   //horizline
  }
  if (lightIndex == 2){
    oled.Set_Color(GREEN);
    oled.Fill_Circle(84, 21, 8); //3
    oled.Set_Color(BLACK);
    oled.Fill_Circle(53, 21, 8); //2
    //gridLines(21, GRAY);
    oled.Set_Color(GRAY);
    oled.Draw_Line(0,21,63,21);   //horizline
  }
  if (lightIndex == 3){
  oled.Set_Color(GREEN);
  oled.Fill_Circle(105, 42, 8);  //4
  oled.Set_Color(BLACK);
  oled.Fill_Circle(84, 21, 8); //3
  //gridLines(21, GRAY);
  oled.Set_Color(GRAY);
  oled.Draw_Line(84,0,84,42); //vertline
  oled.Draw_Line(63,21,105,21);   //horizline
  }
  if (lightIndex == 4){
  oled.Set_Color(BLACK);
  oled.Fill_Circle(105, 42, 8);  //4
  gridLines(21, GRAY);
  oled.Set_Color(GREEN);
  oled.Fill_Circle(120, 58, 8);  //5
  
  //gridLines(21, GRAY);
  oled.Set_Color(GRAY);
  oled.Draw_Line(105,21,105,63); //vertline
  oled.Draw_Line(84,42,126,42);   //horizline
  

  //displayVar5X8 (65, 112, WHITE, "PKE: ");
  //displayVar5X8 (90, 112, WHITE, String(pkeReading));
  }
  oled.Set_Color(WHITE);
  oled.print_String(65, 112, "PKE:", FONT_5X8);  
  displayVar5X8 (100, 112, pkeReading);
}
void displayVar5X8 (uint16_t x, uint16_t y, float pkeReading){
  String s =  String(pkeReading); //This is how to display strings on the screen through variable.  TODO: make textArray and str_len global.
  pkeOutLength = s.length() + 1;           //
  s.toCharArray(pkeOut, pkeOutLength);
  oled.print_String(90, 112, pkeOut, FONT_5X8);
  }
void noGhostLogo(uint16_t color1, uint16_t color2, uint8_t xOffset){
Serial.println("Ghostsmashers");
//No
oled.Set_Color(color1);
oled.Draw_Line(63 + xOffset,8,66 + xOffset,8);
oled.Draw_Line(60 + xOffset,9,70 + xOffset,9);
oled.Draw_Line(60 + xOffset,10,72 + xOffset,10);
oled.Draw_Line(39 + xOffset,11,41 + xOffset,11);
oled.Draw_Line(60 + xOffset,11,75 + xOffset,11);
oled.Draw_Line(38 + xOffset,12,40 + xOffset,12);
oled.Draw_Line(60 + xOffset,12,77 + xOffset,12);
oled.Draw_Line(36 + xOffset,13,40 + xOffset,13);
oled.Draw_Line(60 + xOffset,13,79 + xOffset,13);
oled.Draw_Line(34 + xOffset,14,39 + xOffset,14);
oled.Draw_Line(60 + xOffset,14,80 + xOffset,14);
oled.Draw_Line(33 + xOffset,15,38 + xOffset,15);
oled.Draw_Line(60 + xOffset,15,82 + xOffset,15);
oled.Draw_Line(32 + xOffset,16,37 + xOffset,16);
oled.Draw_Line(60 + xOffset,16,83 + xOffset,16);
oled.Draw_Line(31 + xOffset,17,36 + xOffset,17);
oled.Draw_Line(60 + xOffset,17,84 + xOffset,17);
oled.Draw_Line(30 + xOffset,18,35 + xOffset,18);
oled.Draw_Line(60 + xOffset,18,85 + xOffset,18);
oled.Draw_Line(29 + xOffset,19,35 + xOffset,19);
oled.Draw_Line(61 + xOffset,19,86 + xOffset,19);
oled.Draw_Line(28 + xOffset,20,34 + xOffset,20);
oled.Draw_Line(61 + xOffset,20,87 + xOffset,20);
oled.Draw_Line(27 + xOffset,21,33 + xOffset,21);
oled.Draw_Line(66 + xOffset,21,88 + xOffset,21);
oled.Draw_Line(26 + xOffset,22,32 + xOffset,22);
oled.Draw_Line(68 + xOffset,22,89 + xOffset,22);
oled.Draw_Line(25 + xOffset,23,32 + xOffset,23);
oled.Draw_Line(70 + xOffset,23,90 + xOffset,23);
oled.Draw_Line(25 + xOffset,24,32 + xOffset,24);
oled.Draw_Line(72 + xOffset,24,91 + xOffset,24);
oled.Draw_Line(24 + xOffset,25,32 + xOffset,25);
oled.Draw_Line(73 + xOffset,25,91 + xOffset,25);
oled.Draw_Line(23 + xOffset,26,32 + xOffset,26);
oled.Draw_Line(75 + xOffset,26,92 + xOffset,26);
oled.Draw_Line(23 + xOffset,27,32 + xOffset,27);
oled.Draw_Line(76 + xOffset,27,92 + xOffset,27);
oled.Draw_Line(22 + xOffset,28,32 + xOffset,28);
oled.Draw_Line(77 + xOffset,28,93 + xOffset,28);
oled.Draw_Line(22 + xOffset,29,33 + xOffset,29);
oled.Draw_Line(77 + xOffset,29,93 + xOffset,29);
oled.Draw_Line(21 + xOffset,30,33 + xOffset,30);
oled.Draw_Line(75 + xOffset,30,94 + xOffset,30);
oled.Draw_Line(21 + xOffset,31,34 + xOffset,31);
oled.Draw_Line(74 + xOffset,31,94 + xOffset,31);
oled.Draw_Line(20 + xOffset,32,32 + xOffset,32);
oled.Draw_Line(72 + xOffset,32,95 + xOffset,32);
oled.Draw_Line(20 + xOffset,33,31 + xOffset,33);
oled.Draw_Line(70 + xOffset,33,95 + xOffset,33);
oled.Draw_Line(20 + xOffset,34,30 + xOffset,34);
oled.Draw_Line(68 + xOffset,34,96 + xOffset,34);
oled.Draw_Line(19 + xOffset,35,29 + xOffset,35);
oled.Draw_Line(67 + xOffset,35,96 + xOffset,35);
oled.Draw_Line(19 + xOffset,36,28 + xOffset,36);
oled.Draw_Line(65 + xOffset,36,96 + xOffset,36);
oled.Draw_Line(19 + xOffset,37,27 + xOffset,37);
oled.Draw_Line(63 + xOffset,37,96 + xOffset,37);
oled.Draw_Line(18 + xOffset,38,26 + xOffset,38);
oled.Draw_Line(61 + xOffset,38,97 + xOffset,38);
oled.Draw_Line(18 + xOffset,39,25 + xOffset,39);
oled.Draw_Line(59 + xOffset,39,95 + xOffset,39);
oled.Draw_Line(18 + xOffset,40,24 + xOffset,40);
oled.Draw_Line(58 + xOffset,40,93 + xOffset,40);
oled.Draw_Line(18 + xOffset,41,23 + xOffset,41);
oled.Draw_Line(56 + xOffset,41,83 + xOffset,41);
oled.Draw_Line(85 + xOffset,41,91 + xOffset,41);
oled.Draw_Line(18 + xOffset,42,22 + xOffset,42);
oled.Draw_Line(54 + xOffset,42,81 + xOffset,42);
oled.Draw_Line(86 + xOffset,42,87 + xOffset,42);
oled.Draw_Line(18 + xOffset,43,21 + xOffset,43);
oled.Draw_Line(52 + xOffset,43,79 + xOffset,43);
oled.Draw_Line(50 + xOffset,44,77 + xOffset,44);
oled.Draw_Line(97 + xOffset,44,98 + xOffset,44);
oled.Draw_Line(49 + xOffset,45,76 + xOffset,45);
oled.Draw_Line(96 + xOffset,45,98 + xOffset,45);
oled.Draw_Line(47 + xOffset,46,74 + xOffset,46);
oled.Draw_Line(95 + xOffset,46,98 + xOffset,46);
oled.Draw_Line(45 + xOffset,47,72 + xOffset,47);
oled.Draw_Line(96 + xOffset,47,98 + xOffset,47);
oled.Draw_Line(43 + xOffset,48,70 + xOffset,48);
oled.Draw_Line(41 + xOffset,49,68 + xOffset,49);
oled.Draw_Line(39 + xOffset,50,66 + xOffset,50);
oled.Draw_Line(38 + xOffset,51,65 + xOffset,51);
oled.Draw_Line(36 + xOffset,52,63 + xOffset,52);
oled.Draw_Line(29 + xOffset,53,30 + xOffset,53);
oled.Draw_Line(34 + xOffset,53,62 + xOffset,53);
oled.Draw_Line(96 + xOffset,53,97 + xOffset,53);
oled.Draw_Line(27 + xOffset,54,31 + xOffset,54);
oled.Draw_Line(32 + xOffset,54,60 + xOffset,54);
oled.Draw_Line(24 + xOffset,55,58 + xOffset,55);
oled.Draw_Line(84 + xOffset,55,91 + xOffset,55);
oled.Draw_Line(24 + xOffset,56,56 + xOffset,56);
oled.Draw_Line(84 + xOffset,56,93 + xOffset,56);
oled.Draw_Line(18 + xOffset,57,19 + xOffset,57);
oled.Draw_Line(24 + xOffset,57,54 + xOffset,57);
oled.Draw_Line(83 + xOffset,57,94 + xOffset,57);
oled.Draw_Line(19 + xOffset,58,20 + xOffset,58);
oled.Draw_Line(24 + xOffset,58,52 + xOffset,58);
oled.Draw_Line(83 + xOffset,58,95 + xOffset,58);
oled.Draw_Line(19 + xOffset,59,22 + xOffset,59);
oled.Draw_Line(23 + xOffset,59,50 + xOffset,59);
oled.Draw_Line(82 + xOffset,59,96 + xOffset,59);
oled.Draw_Line(19 + xOffset,60,48 + xOffset,60);
oled.Draw_Line(82 + xOffset,60,96 + xOffset,60);
oled.Draw_Line(20 + xOffset,61,46 + xOffset,61);
oled.Draw_Line(81 + xOffset,61,95 + xOffset,61);
oled.Draw_Line(20 + xOffset,62,44 + xOffset,62);
oled.Draw_Line(81 + xOffset,62,95 + xOffset,62);
oled.Draw_Line(21 + xOffset,63,43 + xOffset,63);
oled.Draw_Line(80 + xOffset,63,94 + xOffset,63);
oled.Draw_Line(21 + xOffset,64,41 + xOffset,64);
oled.Draw_Line(80 + xOffset,64,94 + xOffset,64);
oled.Draw_Line(21 + xOffset,65,39 + xOffset,65);
oled.Draw_Line(79 + xOffset,65,93 + xOffset,65);
oled.Draw_Line(22 + xOffset,66,38 + xOffset,66);
oled.Draw_Line(78 + xOffset,66,93 + xOffset,66);
oled.Draw_Line(23 + xOffset,67,39 + xOffset,67);
oled.Draw_Line(77 + xOffset,67,92 + xOffset,67);
oled.Draw_Line(23 + xOffset,68,40 + xOffset,68);
oled.Draw_Line(76 + xOffset,68,92 + xOffset,68);
oled.Draw_Line(24 + xOffset,69,40 + xOffset,69);
oled.Draw_Line(74 + xOffset,69,91 + xOffset,69);
oled.Draw_Line(24 + xOffset,70,42 + xOffset,70);
oled.Draw_Line(73 + xOffset,70,90 + xOffset,70);
oled.Draw_Line(25 + xOffset,71,44 + xOffset,71);
oled.Draw_Line(71 + xOffset,71,90 + xOffset,71);
oled.Draw_Line(26 + xOffset,72,46 + xOffset,72);
oled.Draw_Line(70 + xOffset,72,89 + xOffset,72);
oled.Draw_Line(27 + xOffset,73,48 + xOffset,73);
oled.Draw_Line(67 + xOffset,73,88 + xOffset,73);
oled.Draw_Line(27 + xOffset,74,52 + xOffset,74);
oled.Draw_Line(63 + xOffset,74,88 + xOffset,74);
oled.Draw_Line(28 + xOffset,75,87 + xOffset,75);
oled.Draw_Line(29 + xOffset,76,86 + xOffset,76);
oled.Draw_Line(30 + xOffset,77,85 + xOffset,77);
oled.Draw_Line(32 + xOffset,78,84 + xOffset,78);
oled.Draw_Line(33 + xOffset,79,82 + xOffset,79);
oled.Draw_Line(34 + xOffset,80,81 + xOffset,80);
oled.Draw_Line(36 + xOffset,81,80 + xOffset,81);
oled.Draw_Line(37 + xOffset,82,78 + xOffset,82);
oled.Draw_Line(39 + xOffset,83,76 + xOffset,83);
oled.Draw_Line(41 + xOffset,84,74 + xOffset,84);
oled.Draw_Line(44 + xOffset,85,71 + xOffset,85);
oled.Draw_Line(46 + xOffset,86,69 + xOffset,86);
oled.Draw_Line(51 + xOffset,87,64 + xOffset,87);
oled.Set_Color(color2);
oled.Draw_Line(57 + xOffset,0,58 + xOffset,0);
oled.Draw_Line(56 + xOffset,1,59 + xOffset,1);
oled.Draw_Line(54 + xOffset,2,60 + xOffset,2);
oled.Draw_Line(52 + xOffset,3,61 + xOffset,3);
oled.Draw_Line(50 + xOffset,4,61 + xOffset,4);
oled.Draw_Line(47 + xOffset,5,62 + xOffset,5);
oled.Draw_Line(45 + xOffset,6,65 + xOffset,6);
oled.Draw_Line(44 + xOffset,7,57 + xOffset,7);
oled.Draw_Line(58 + xOffset,7,63 + xOffset,7);
oled.Draw_Line(43 + xOffset,8,58 + xOffset,8);
oled.Draw_Line(59 + xOffset,8,60 + xOffset,8);
oled.Draw_Line(43 + xOffset,9,45 + xOffset,9);
oled.Draw_Line(47 + xOffset,9,52 + xOffset,9);
oled.Draw_Line(54 + xOffset,9,59 + xOffset,9);
oled.Draw_Line(42 + xOffset,10,45 + xOffset,10);
oled.Draw_Line(46 + xOffset,10,53 + xOffset,10);
oled.Draw_Line(55 + xOffset,10,59 + xOffset,10);
oled.Draw_Line(42 + xOffset,11,54 + xOffset,11);
oled.Draw_Line(55 + xOffset,11,59 + xOffset,11);
oled.Draw_Line(41 + xOffset,12,46 + xOffset,12);
oled.Draw_Line(47 + xOffset,12,59 + xOffset,12);
oled.Draw_Line(41 + xOffset,13,45 + xOffset,13);
oled.Draw_Line(48 + xOffset,13,52 + xOffset,13);
oled.Draw_Line(54 + xOffset,13,59 + xOffset,13);
oled.Draw_Line(40 + xOffset,14,45 + xOffset,14);
oled.Draw_Line(48 + xOffset,14,51 + xOffset,14);
oled.Draw_Line(54 + xOffset,14,59 + xOffset,14);
oled.Draw_Line(39 + xOffset,15,46 + xOffset,15);
oled.Draw_Line(47 + xOffset,15,51 + xOffset,15);
oled.Draw_Line(53 + xOffset,15,59 + xOffset,15);
oled.Draw_Line(39 + xOffset,16,59 + xOffset,16);
oled.Draw_Line(38 + xOffset,17,59 + xOffset,17);
oled.Draw_Line(37 + xOffset,18,59 + xOffset,18);
oled.Draw_Line(36 + xOffset,19,60 + xOffset,19);
oled.Draw_Line(35 + xOffset,20,60 + xOffset,20);
oled.Draw_Line(34 + xOffset,21,47 + xOffset,21);
oled.Draw_Line(48 + xOffset,21,49 + xOffset,21);
oled.Draw_Line(50 + xOffset,21,61 + xOffset,21);
oled.Draw_Line(34 + xOffset,22,47 + xOffset,22);
oled.Draw_Line(50 + xOffset,22,61 + xOffset,22);
oled.Draw_Line(33 + xOffset,23,62 + xOffset,23);
oled.Draw_Line(33 + xOffset,24,62 + xOffset,24);
oled.Draw_Line(33 + xOffset,25,62 + xOffset,25);
oled.Draw_Line(33 + xOffset,26,46 + xOffset,26);
oled.Draw_Line(48 + xOffset,26,62 + xOffset,26);
oled.Draw_Line(33 + xOffset,27,45 + xOffset,27);
oled.Draw_Line(49 + xOffset,27,62 + xOffset,27);
oled.Draw_Line(33 + xOffset,28,45 + xOffset,28);
oled.Draw_Line(49 + xOffset,28,62 + xOffset,28);
oled.Draw_Line(63 + xOffset,28,70 + xOffset,28);
oled.Draw_Line(34 + xOffset,29,45 + xOffset,29);
oled.Draw_Line(49 + xOffset,29,62 + xOffset,29);
oled.Draw_Line(63 + xOffset,29,72 + xOffset,29);
oled.Draw_Line(34 + xOffset,30,46 + xOffset,30);
oled.Draw_Line(48 + xOffset,30,62 + xOffset,30);
oled.Draw_Line(63 + xOffset,30,74 + xOffset,30);
oled.Draw_Line(35 + xOffset,31,61 + xOffset,31);
oled.Draw_Line(62 + xOffset,31,74 + xOffset,31);
oled.Draw_Line(36 + xOffset,32,60 + xOffset,32);
oled.Draw_Line(61 + xOffset,32,72 + xOffset,32);
oled.Draw_Line(32 + xOffset,33,59 + xOffset,33);
oled.Draw_Line(61 + xOffset,33,70 + xOffset,33);
oled.Draw_Line(31 + xOffset,34,45 + xOffset,34);
oled.Draw_Line(49 + xOffset,34,59 + xOffset,34);
oled.Draw_Line(60 + xOffset,34,68 + xOffset,34);
oled.Draw_Line(30 + xOffset,35,47 + xOffset,35);
oled.Draw_Line(48 + xOffset,35,67 + xOffset,35);
oled.Draw_Line(29 + xOffset,36,65 + xOffset,36);
oled.Draw_Line(28 + xOffset,37,63 + xOffset,37);
oled.Draw_Line(3 + xOffset,38,7 + xOffset,38);
oled.Draw_Line(27 + xOffset,38,61 + xOffset,38);
oled.Draw_Line(4 + xOffset,39,9 + xOffset,39);
oled.Draw_Line(26 + xOffset,39,59 + xOffset,39);
oled.Draw_Line(98 + xOffset,39,101 + xOffset,39);
oled.Draw_Line(6 + xOffset,40,11 + xOffset,40);
oled.Draw_Line(25 + xOffset,40,58 + xOffset,40);
oled.Draw_Line(95 + xOffset,40,100 + xOffset,40);
oled.Draw_Line(7 + xOffset,41,13 + xOffset,41);
oled.Draw_Line(24 + xOffset,41,56 + xOffset,41);
oled.Draw_Line(93 + xOffset,41,99 + xOffset,41);
oled.Draw_Line(8 + xOffset,42,15 + xOffset,42);
oled.Draw_Line(23 + xOffset,42,54 + xOffset,42);
oled.Draw_Line(81 + xOffset,42,83 + xOffset,42);
oled.Draw_Line(90 + xOffset,42,98 + xOffset,42);
oled.Draw_Line(6 + xOffset,43,8 + xOffset,43);
oled.Draw_Line(9 + xOffset,43,16 + xOffset,43);
oled.Draw_Line(22 + xOffset,43,52 + xOffset,43);
oled.Draw_Line(79 + xOffset,43,86 + xOffset,43);
oled.Draw_Line(87 + xOffset,43,97 + xOffset,43);
oled.Draw_Line(5 + xOffset,44,18 + xOffset,44);
oled.Draw_Line(21 + xOffset,44,50 + xOffset,44);
oled.Draw_Line(77 + xOffset,44,95 + xOffset,44);
oled.Draw_Line(2 + xOffset,45,49 + xOffset,45);
oled.Draw_Line(76 + xOffset,45,95 + xOffset,45);
oled.Draw_Line(1 + xOffset,46,47 + xOffset,46);
oled.Draw_Line(74 + xOffset,46,94 + xOffset,46);
oled.Draw_Line(9 + xOffset,47,45 + xOffset,47);
oled.Draw_Line(72 + xOffset,47,73 + xOffset,47);
oled.Draw_Line(75 + xOffset,47,94 + xOffset,47);
oled.Draw_Line(10 + xOffset,48,43 + xOffset,48);
oled.Draw_Line(70 + xOffset,48,74 + xOffset,48);
oled.Draw_Line(76 + xOffset,48,96 + xOffset,48);
oled.Draw_Line(7 + xOffset,49,35 + xOffset,49);
oled.Draw_Line(38 + xOffset,49,41 + xOffset,49);
oled.Draw_Line(68 + xOffset,49,76 + xOffset,49);
oled.Draw_Line(77 + xOffset,49,100 + xOffset,49);
oled.Draw_Line(6 + xOffset,50,32 + xOffset,50);
oled.Draw_Line(37 + xOffset,50,39 + xOffset,50);
oled.Draw_Line(66 + xOffset,50,76 + xOffset,50);
oled.Draw_Line(77 + xOffset,50,103 + xOffset,50);
oled.Draw_Line(6 + xOffset,51,9 + xOffset,51);
oled.Draw_Line(11 + xOffset,51,30 + xOffset,51);
oled.Draw_Line(37 + xOffset,51,38 + xOffset,51);
oled.Draw_Line(65 + xOffset,51,75 + xOffset,51);
oled.Draw_Line(76 + xOffset,51,104 + xOffset,51);
oled.Draw_Line(13 + xOffset,52,29 + xOffset,52);
oled.Draw_Line(63 + xOffset,52,74 + xOffset,52);
oled.Draw_Line(75 + xOffset,52,94 + xOffset,52);
oled.Draw_Line(15 + xOffset,53,27 + xOffset,53);
oled.Draw_Line(61 + xOffset,53,73 + xOffset,53);
oled.Draw_Line(74 + xOffset,53,82 + xOffset,53);
oled.Draw_Line(84 + xOffset,53,95 + xOffset,53);
oled.Draw_Line(17 + xOffset,54,24 + xOffset,54);
oled.Draw_Line(60 + xOffset,54,73 + xOffset,54);
oled.Draw_Line(74 + xOffset,54,80 + xOffset,54);
oled.Draw_Line(83 + xOffset,54,84 + xOffset,54);
oled.Draw_Line(91 + xOffset,54,96 + xOffset,54);
oled.Draw_Line(19 + xOffset,55,23 + xOffset,55);
oled.Draw_Line(58 + xOffset,55,74 + xOffset,55);
oled.Draw_Line(75 + xOffset,55,77 + xOffset,55);
oled.Draw_Line(80 + xOffset,55,84 + xOffset,55);
oled.Draw_Line(93 + xOffset,55,97 + xOffset,55);
oled.Draw_Line(19 + xOffset,56,23 + xOffset,56);
oled.Draw_Line(56 + xOffset,56,75 + xOffset,56);
oled.Draw_Line(77 + xOffset,56,84 + xOffset,56);
oled.Draw_Line(94 + xOffset,56,98 + xOffset,56);
oled.Draw_Line(20 + xOffset,57,23 + xOffset,57);
oled.Draw_Line(54 + xOffset,57,83 + xOffset,57);
oled.Draw_Line(95 + xOffset,57,98 + xOffset,57);
oled.Draw_Line(22 + xOffset,58,23 + xOffset,58);
oled.Draw_Line(52 + xOffset,58,83 + xOffset,58);
oled.Draw_Line(50 + xOffset,59,82 + xOffset,59);
oled.Draw_Line(48 + xOffset,60,82 + xOffset,60);
oled.Draw_Line(46 + xOffset,61,81 + xOffset,61);
oled.Draw_Line(47 + xOffset,62,81 + xOffset,62);
oled.Draw_Line(49 + xOffset,63,80 + xOffset,63);
oled.Draw_Line(51 + xOffset,64,80 + xOffset,64);
oled.Draw_Line(47 + xOffset,65,50 + xOffset,65);
oled.Draw_Line(54 + xOffset,65,79 + xOffset,65);
oled.Draw_Line(46 + xOffset,66,53 + xOffset,66);
oled.Draw_Line(56 + xOffset,66,78 + xOffset,66);
oled.Draw_Line(45 + xOffset,67,56 + xOffset,67);
oled.Draw_Line(60 + xOffset,67,77 + xOffset,67);
oled.Draw_Line(44 + xOffset,68,76 + xOffset,68);
oled.Draw_Line(43 + xOffset,69,74 + xOffset,69);
oled.Draw_Line(42 + xOffset,70,73 + xOffset,70);
oled.Draw_Line(44 + xOffset,71,71 + xOffset,71);
oled.Draw_Line(46 + xOffset,72,70 + xOffset,72);
oled.Draw_Line(48 + xOffset,73,67 + xOffset,73);
oled.Draw_Line(52 + xOffset,74,63 + xOffset,74);

}
void gridLines(int lines, uint16_t color){
  oled.Set_Color(color);
  for(int y = 0; y <=128; y += lines ){
    oled.Draw_Line(0,y,128,y);
  }
  for(int x = 0; x <=128; x += lines ){
    oled.Draw_Line(x,0,x,128);
  }  
}


//void textTest(){
//  for (int y = 0; y <= 128; y++){
//  displayVar8X16(0, y * 12, MAGENTA, "01234567890123456");
//  delay(250);
//  }
//}
//  }
//}
//void testlines(void)  {
//
//  oled.Set_Color(RED);
//  oled.Clear_Screen();
//  for (uint16_t x=0; x <= SSD1351_WIDTH - 1; x+=6)  {
//    oled.Draw_Line(0, 0, x, SSD1351_HEIGHT - 1);
//    delay(10);
//  }
//  for (uint16_t y=0; y < SSD1351_HEIGHT - 1; y+=6)  {
//    oled.Draw_Line(0, 0, SSD1351_WIDTH - 1, y);
//    delay(10);
//  }
//  
//  oled.Set_Color(YELLOW);
//  oled.Clear_Screen();
//  for (uint16_t x=0; x < SSD1351_WIDTH - 1; x+=6) {
//    oled.Draw_Line(SSD1351_WIDTH - 1, 0, x, SSD1351_HEIGHT - 1);
//    delay(10);
//  }
//  for (uint16_t y=0; y < SSD1351_HEIGHT - 1; y+=6) {
//    oled.Draw_Line(SSD1351_WIDTH - 1, 0, 0, y);
//    delay(10);
//  }
//   
//  oled.Set_Color(BLUE);
//  oled.Clear_Screen();
//  for (uint16_t x=0; x < SSD1351_WIDTH - 1; x+=6) {
//    oled.Draw_Line(0, SSD1351_HEIGHT - 1, x, 0);
//    delay(10);
//  }
//  for (uint16_t y=0; y < SSD1351_HEIGHT - 1; y+=6) {
//    oled.Draw_Line(0, SSD1351_HEIGHT - 1, SSD1351_WIDTH - 1, y);
//    delay(10);
//  }
//
//  oled.Set_Color(GREEN);
//  oled.Clear_Screen();
//  for (uint16_t x=0; x < SSD1351_WIDTH - 1; x+=6) {
//  oled.Draw_Line(SSD1351_WIDTH - 1, SSD1351_HEIGHT - 1, x, 0 );
//    delay(10);
//  }
//  for (uint16_t y=0; y < SSD1351_HEIGHT - 1; y+=6) {
//    oled.Draw_Line(SSD1351_WIDTH - 1, SSD1351_HEIGHT - 1, 0, y);
//    delay(10);
//  }
//}
//
//void lcdTestPattern(void)
//{
//  uint32_t i,j;
//  oled.Set_Coordinate(0, 0);
//  
//  for(i=0;i<128;i++)  {
//    for(j=0;j<128;j++)  {
//      if(i<16)  {
//        oled.Set_Color(RED);
//        oled.Write_Data(color_byte[0]);
//        oled.Write_Data(color_byte[1]);
//      }
//      else if(i<32) {
//        oled.Set_Color(YELLOW);
//        oled.Write_Data(color_byte[0]);
//        oled.Write_Data(color_byte[1]);
//      }
//      else if(i<48) {
//        oled.Set_Color(GREEN);
//        oled.Write_Data(color_byte[0]);
//        oled.Write_Data(color_byte[1]);
//      }
//      else if(i<64) {
//        oled.Set_Color(CYAN);
//        oled.Write_Data(color_byte[0]);
//        oled.Write_Data(color_byte[1]);
//      }
//      else if(i<80) {
//        oled.Set_Color(BLUE);
//        oled.Write_Data(color_byte[0]);
//        oled.Write_Data(color_byte[1]);
//      }
//      else if(i<96) {
//        oled.Set_Color(MAGENTA);
//        oled.Write_Data(color_byte[0]);
//        oled.Write_Data(color_byte[1]);
//      }
//      else if(i<112)  {
//        oled.Set_Color(BLACK);
//        oled.Write_Data(color_byte[0]);
//        oled.Write_Data(color_byte[1]);
//      }
//      else {
//        oled.Set_Color(WHITE);
//        oled.Write_Data(color_byte[0]);
//        oled.Write_Data(color_byte[1]);
//      }
//    }
//  }
//}

//JPJ EDIT
//

//void testdrawrects(void)  {
//  oled.Clear_Screen();
//  for (uint16_t x=0; x < SSD1351_HEIGHT - 1; x+=6)  {
//    oled.Draw_Rect((SSD1351_WIDTH-1)/2 - x/2, (SSD1351_HEIGHT-1)/2 - x/2 , x, x);
//    delay(10);
//  }
//}
//
//void testfillrects(uint16_t color1, uint16_t color2)  {
//  
//  uint16_t x = SSD1351_HEIGHT - 1;
//  oled.Clear_Screen();
//  oled.Set_Color(color1);
//  oled.Set_FillColor(color2);
//  for(; x > 6; x-=6)  {
//    oled.Fill_Rect((SSD1351_WIDTH-1)/2 - x/2, (SSD1351_HEIGHT-1)/2 - x/2 , x, x);
//  oled.Draw_Rect((SSD1351_WIDTH-1)/2 - x/2, (SSD1351_HEIGHT-1)/2 - x/2 , x, x);
//  }
//}
//
//void testfillcircles(uint16_t radius, uint16_t color)  {
//  
//  oled.Set_Color(color);
//
//  oled.Fill_Circle(64, 64, radius);
//  }
//
//
//void testdrawcircles(uint16_t color)  {
//  
//  uint16_t r = 0;
//  oled.Set_Color(color);
//  
//  for (; r < SSD1351_WIDTH/2; r+=4)  {
//    oled.Draw_Circle(64, 64, r);
//    delay(10);
//  }
//}
//
//void testroundrects(void) {
//
//  int color = 100;
//  int x = 0, y = 0;
//  int w = SSD1351_WIDTH - 1, h = SSD1351_HEIGHT - 1;
//  
//  oled.Clear_Screen();
//  
//  for(int i = 0 ; i <= 20; i++) {
//
//    oled.Draw_RoundRect(x, y, w, h, 5);
//    x += 2;
//    y += 3;
//    w -= 4;
//    h -= 6;
//    color += 1100;
//    oled.Set_Color(color);
//  }
//}
//
//void testtriangles(void)  {
//  oled.Clear_Screen();
//  int color = 0xF800;
//  int t;
//  int w = SSD1351_WIDTH/2;
//  int x = SSD1351_HEIGHT-1;
//  int y = 0;
//  int z = SSD1351_WIDTH;
//  for(t = 0 ; t <= 15; t+=1) {
//    oled.Draw_Triangle(w, y, y, x, z, x);
//    x-=4;
//    y+=4;
//    z-=4;
//    color+=100;
//    oled.Set_Color(color);
//  }
//}
