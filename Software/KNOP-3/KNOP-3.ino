/* -----------------  Dit is de code voor knop 3.         ----------------- */
/* -----------------  Deze bevindt zich bij de magazijn   ----------------- */

#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//////////////////////////////////////// DEFINITIES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
#define I2C_SDA 6
#define I2C_SCL 7
TwoWire I2CBME = TwoWire(0);
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     4 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//ints & bool
int btn1 = 0;
int btn2 = 0;
int btn3 = 0;
int btn4 = 0;
const int btn1P = 21;
const int btn2P = 20;
const int btn3P = 1;
const int btn4P = 2;
bool noQueue = false;
bool btn1Pressed = false;
bool btn2Pressed = false;
bool btn3Pressed = false;
bool btn4Pressed = false;


//wifi setup
const char* ssid = "SSID";                  // De WiFi-SSID
const char* password = "PASSWORD";          // De WiFi-wachtwoord
const char* telnetServerIP = "IP";          // IP van de Telnet-server
const uint16_t telnetServerPort = 0000;     // Telnet-server poort
const char* telnetWW = "PASSWORD";          // De wachtwoord van de Telnet-server 
WiFiClient telnetClient;
bool isTelnetVerbonden = false;

// tijdscontrole instellen
unsigned long laatsteQuerytijd = 0;
const unsigned long QUERY_INTERVAL = 1000; // nu staat de Query op elke 1 seconde



//////////////////////////////////////// SETUP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
void setup() {
  Serial.begin(9600);
  pinMode(btn1P, INPUT);
  pinMode(btn2P, INPUT);
  pinMode(btn3P, INPUT);
  pinMode(btn4P, INPUT);
  
//scherm instellen
  Wire.begin(I2C_SDA, I2C_SCL);
  I2CBME.begin(I2C_SDA, I2C_SCL, 400000);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Laat logo zien
  display.display();
  delay(4000); // Pauze voor 2 secondes
  // Clear the buffer
  display.clearDisplay();
  display.display();
  delay(700);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

// Verbinden met het WiFi-netwerk
  Serial.print("Verbinding maken met WiFi-netwerk: ");
  Serial.println(ssid);
  display.setCursor(0,0);
  display.println("Verbinden met wifi:");
  display.println(ssid);
  display.display();
  delay(1000);
  display.clearDisplay();
  WiFi.begin(ssid, password);


  // Wacht tot er verbinding is met het WiFi-netwerk
  int wifiConnectAttempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    display.setTextSize(2);
    wifiConnectAttempts++;
    Serial.print("Poging ");
    display.setCursor(0,0);
    display.print("Poging ");
    display.display();
    Serial.print(wifiConnectAttempts);
    display.println(wifiConnectAttempts); display.println("");
    Serial.println("Nog steeds aan het proberen om verbinding te maken...");
    display.setTextSize(1);
    display.println(": Nog steeds aan het proberen om verbinding te maken...");
    display.display();
    display.clearDisplay();
    delay(500);
  }

  Serial.println("WiFi-verbinding is tot stand gebracht.");
  Serial.print("IP-adres van ESP32: ");
  Serial.println(WiFi.localIP());
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.println("WiFi-verbinding is tot stand gebracht.");
  display.print("IP-adres van ESP32: ");
  display.println(WiFi.localIP());
  display.display();
  delay(3000);


  // Verbinden met de Telnet-server
  connectToTelnetServer();
}

//////////////////////////////////////// SPECIALLE VOIDS (VOOR RECALL) \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
void countdown() { //countdown voor het opnieuw probere te verbinden met de telnet server
  int c = 10;
  while (c >= 0){
    Serial.println(c);
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2);
    display.println("Telnet verbinding mislukt");
    display.setTextSize(1);
    display.print("Opniew proberen in:");
    display.println(c);
    display.display();
    c = c - 1;
    delay(1000);
  }
  connectToTelnetServer();
}

void connectToTelnetServer() {
  display.setTextSize(2);
  Serial.print("Proberen te verbinden met Telnet-server op ");
  Serial.print(telnetServerIP);
  Serial.print(":");
  Serial.println(telnetServerPort);
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Proberen te verbinden met Telnet-server op ");
  display.print(telnetServerIP);
  display.print(":");
  display.println(telnetServerPort);
  display.display();
  delay(500);

  if (telnetClient.connect(telnetServerIP, telnetServerPort)) {
    Serial.println("Succesvol verbonden met de Telnet-server!");
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Succesvol verbonden met de Telnet-server!");
    // Stuur het wachtwoord naar de Telnet-server
    telnetClient.println(telnetWW);
    Serial.println("Wachtwoord verstuurd naar Telnet-server.");
    display.println("Wachtwoord verstuurd naar Telnet-server.");
    display.display();
    isTelnetVerbonden = true;
    delay(400);
    Serial.println("succes 1");
    loop();
  } else {
    Serial.println("Verbinding met Telnet-server mislukt. Controleer IP en poort.");
    countdown();
    isTelnetVerbonden = false;
  }
}


//////////////////////////////////////// CODE VOOR HET HELE SYSTEEM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
void loop() {
  btn1 = digitalRead(btn1P);
  btn2 = digitalRead(btn2P);
  btn3 = digitalRead(btn3P);
  btn4 = digitalRead(btn4P);
  
  //checked of telnet nogsteeds verbonden is
  if (telnetClient.connected() && isTelnetVerbonden) { 
    // Ontvang data van de Telnet-server 
    if (telnetClient.available()) {
      String telnetResponse = telnetClient.readString();
      Serial.print("Ontvangen van Telnet: ");
      Serial.println(telnetResponse);
      display.clearDisplay();
      display.setCursor(0,0);
      display.setTextSize(2);
      display.println("AMR Zegt: ");
      display.setTextSize(1);
      display.println(telnetResponse);
      display.display();
      delay(500);
    }
    
    // Voeg tijdscontrole toe voor query
    if (millis() - laatsteQuerytijd >= QUERY_INTERVAL) {
      String messageQueue = "outputQuery o16";
      telnetClient.println(messageQueue);
      delay(100);
      String telnetResponse = telnetClient.readString();
      telnetResponse.trim();
      
      if (telnetResponse == "Output: o16 off"){
        noQueue = true;
        Serial.println("geen queue");
      } else if (telnetResponse == "Output: o16 on"){
        noQueue = false;
        Serial.println("AMR is bezig");
        display.clearDisplay();
        display.setCursor(0,0);
        display.setTextSize(2);
        display.println("AMR: ");
        display.println("is bezig");
        display.display();
        delay(200);
      } else {
        Serial.println("Onverwachte response ontvangen");
      }
      
    
      laatsteQuerytijd = millis();// Update de laatste query tijd
    }

    if (btn1 == 1 && btn2 == 0 && btn3 == 0 && btn4 == 0){
      btn1Pressed = true;
    }
    if (btn2 == 1 && btn1 == 0 && btn3 == 0 && btn4 == 0){
      btn2Pressed = true;
    }
    if (btn3 == 1 && btn1 == 0 && btn2 == 0 && btn4 == 0){
      btn3Pressed = true;
    }
    if (btn4 == 1 && btn1 == 0 && btn2 == 0 && btn3 == 0){
      btn4Pressed = true;
    }

    if (btn1Pressed && noQueue){
      btn1a();
      Serial.println("btn1Pressed && noQueue");
    }
    if (btn2Pressed && noQueue){
      btn2a();
      Serial.println("btn2Pressed && noQueue");
    }
    if (btn3Pressed && noQueue){
      btn3a();
      Serial.println("btn3Pressed && noQueue");
    }
    if (btn4Pressed && noQueue){
      btn4a();
      Serial.println("btn4Pressed && noQueue");
    }
  } else { //probeert opnieuw te verbinden met telnet
    Serial.println("Verbinding met Telnet-server mislukt. Controleer IP en poort.");
    countdown();
    isTelnetVerbonden = false;
  }
  delay(10);
}

//////////////////////////////////////// CODE VOOR KNOP COMMANDO \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
void btn1a(void) {
  String message1 = "outputOn o16";
  telnetClient.println(message1);  // Stuur input via Telnet
  Serial.println("AMR:");
  Serial.print("wordt geroepen");
  String message2 = "patrolOnce BTN-M1"; //Stuurt een commando om de route BTN-M1 te starten
  telnetClient.println(message2);
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("AMR");
  display.print("wordt geroepen");
  display.display();
  delay(6000);
  btn1Pressed = false;
}

void btn2a(void) {
  String message1 = "outputOn o16";
  telnetClient.println(message1);  // Stuur input via Telnet
  Serial.println("AMR:");
  Serial.print("wordt geroepen");
  String message2 = "patrolOnce BTN-M2"; 
  telnetClient.println(message2);
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("AMR");
  display.print("wordt geroepen");
  display.display();
  delay(6000);
  String message3 = "outputOff o16";
  telnetClient.println(message3);
  btn2Pressed = false;
}

void btn3a(void) {
  String message1 = "outputOn o16";
  telnetClient.println(message1);  // Stuur input via Telnet
  Serial.println("AMR:");
  Serial.print("wordt geroepen");
  String message2 = "patrolOnce BTN-M2"; 
  telnetClient.println(message2);
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("AMR");
  display.print("wordt geroepen");
  display.display();
  delay(6000);
  btn3Pressed = false;
}

void btn4a(void) {
  String message1 = "outputOn o16";
  telnetClient.println(message1);  // Stuur input via Telnet
  Serial.println("AMR:");
  Serial.print("wordt geroepen");
  String message2 = "patrolOnce BTN-M2"; 
  telnetClient.println(message2);
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("AMR");
  display.print("wordt geroepen");
  display.display();
  delay(6000);
  btn4Pressed = false;
}