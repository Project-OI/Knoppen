/* -----------------  Dit is de code voor knop 1   ----------------- */
/* -----------------        ----------------- */
/* -----------------                                    ----------------- */


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
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define LOGO_HEIGHT   64
#define LOGO_WIDTH    128

//ints
int a = 2;
int btn1 = 0;
const int btn1P = 21;

//wifi setup
const char* ssid = "SSID";                  // De WiFi-SSID
const char* password = "PASSWORD";          // De WiFi-wachtwoord
const char* telnetServerIP = "IP";          // IP van de Telnet-server
const uint16_t telnetServerPort = 0000;     // Telnet-server poort
const char* telnetWW = "PASSWORD";          // De wachtwoord van de Telnet-server 
WiFiClient telnetClient;

bool isTelnetVerbonden = false;



//////////////////////////////////////// SETUP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
void setup() {
  Serial.begin(9600);
  
//scherm instellen
  Wire.begin(I2C_SDA, I2C_SCL);
  I2CBME.begin(I2C_SDA, I2C_SCL, 400000);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Laat logo zien
  display.display();
  delay(4000); // Pause for 2 seconds
  // Clear the buffer
  display.clearDisplay();
  display.display();
  delay(700);
  pinMode(btn1P, INPUT);
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
  delay(700);
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

void connectToTelnetServer() {
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
  } else {
    Serial.println("Verbinding met Telnet-server mislukt. Controleer IP en poort.");
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Verbinding met Telnet-server mislukt. Controleer IP en poort.");
    display.display();
    isTelnetVerbonden = false;
  }
}


//////////////////////////////////////// CODE VOOR HET HELE SYSTEEM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
void loop() {
  btn1 = digitalRead(btn1P);

// Check of de ESP32 nog steeds verbonden is met de Telnet-server
  if (telnetClient.connected() && isTelnetVerbonden) {
    if (btn1 == 1){
      Serial.println("btn 1 pressed");
      btn1a();
      delay(20);
    }
    // Ontvang data van de Telnet-server en stuur naar de seriële monitor voor het testen (!niet in eindcode!)
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
  }

  delay(10);
}




void btn1a(void) {
  String message1 = "doTaskInstant playInstant vineboom.wav";
  telnetClient.println(message1);  // Stuur input via Telnet
  Serial.println("AMR:");
  Serial.print("wordt geroepen");
  String message2 = "goto R2-D2"; 
  telnetClient.println(message2);
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("AMR");
  display.print("wordt geroepen");
  display.display();
  delay(200);
}