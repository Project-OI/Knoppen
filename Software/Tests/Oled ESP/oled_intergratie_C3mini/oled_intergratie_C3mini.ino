/* -----------------  Dit is een code om te testen hoe de 128x32 oled scherm en   ----------------- */
/* -----------------  knoppen geïntegreerd kunnen worden met een ESPC3 mini. Als  ----------------- */
/* -----------------  knop 1 of 2 wordt ingedrukt zal het scherm een ander tekst  ----------------- */
/* -----------------  laten zien                                                  ----------------- */


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//defines voor het scherm
#define LOGO_HEIGHT   32
#define LOGO_WIDTH    128
#define I2C_SDA 6
#define I2C_SCL 7
TwoWire I2CBME = TwoWire(0);

//ints
int a = 2;
int btn1 = 0;
int btn2 = 0;
const int btn1P = 21;
const int btn2P = 20;


void setup() {
  Serial.begin(9600);
  
  //scherm instellen
  Wire.begin(I2C_SDA, I2C_SCL);
  I2CBME.begin(I2C_SDA, I2C_SCL, 400000);
  pinMode(btn1P, INPUT);
  pinMode(btn2P, INPUT);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Laat logo zien
  display.display();
  delay(3000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.display();
  delay(700);

}

void loop() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  btn1 = digitalRead(btn1P);
  btn2 = digitalRead(btn2P);

  if (btn1 == 1){
    Serial.println("btn 1 pressed");
    btn1a();
    delay(2000);
    display.clearDisplay();
    display.display();
  }
  if (btn2 == 1){
    Serial.println("btn 2 pressed");
    btn2a();
    delay(2000);
    display.clearDisplay();
    display.display();
  }

  delay(10);
}


void btn1a(void) {
  display.setCursor(0,0);
  display.println("AMR:");
  display.display();
  display.setTextSize(1);
  display.print("wordt geroepen");
  display.display();
  delay(4000);
  Serial.println("btn1 klaar");
}

void btn2a(void) {
  display.setCursor(0,0);
  display.println("AMR:");
  display.display();
  display.setTextSize(1);
  display.println("vervoerdt bak naar vak 1");
  display.display();
  delay(4000);
  Serial.println("btn2 klaar");
}