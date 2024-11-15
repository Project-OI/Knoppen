/* -----------------  In deze code worden de knoppen en Telnet samengevoegd. Nu   ----------------- */
/* -----------------  zijn er voorgeprogrammeerde berichten/commando's die worden ----------------- */
/* -----------------  gestuurd als er een bepaalde knop wordt ingedrukt.          ----------------- */


#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>


//////////////////////////////////////// DEFINITIES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
#define I2C_SDA 6
#define I2C_SCL 7
TwoWire I2CBME = TwoWire(0);

//ints
int a = 2;
int btn1 = 0;
int btn2 = 0;
const int btn1P = 21;
const int btn2P = 20;

//wifi setup
const char* ssid = "SSID";      // De WiFi-SSID                                       
const char* password = "PASSWORD";  // Het WiFi-wachtwoord                    
const char* telnetServerIP = "SERVERIP";  // IP van de Telnet-server               
const uint16_t telnetServerPort = 0000;      // Telnet-server poort                   
const char* telnetWW = "PASSWORD";  
WiFiClient telnetClient;

bool isTelnetVerbonden = false;



//////////////////////////////////////// SETUP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
void setup() {
  Serial.begin(9600);
  
  //scherm instellen
  Wire.begin(I2C_SDA, I2C_SCL);
  I2CBME.begin(I2C_SDA, I2C_SCL, 400000);
  pinMode(btn1P, INPUT);
  pinMode(btn2P, INPUT);

  // Verbinden met het WiFi-netwerk
  Serial.print("Verbinding maken met WiFi-netwerk: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);


  // Wacht tot er verbinding is met het WiFi-netwerk
  int wifiConnectAttempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    wifiConnectAttempts++;
    Serial.print("Poging ");
    Serial.print(wifiConnectAttempts);
    Serial.println(": Nog steeds aan het proberen om verbinding te maken...");
  }

  Serial.println("WiFi-verbinding is tot stand gebracht.");
  Serial.print("IP-adres van ESP32: ");
  Serial.println(WiFi.localIP());

  // Verbinden met de Telnet-server
  connectToTelnetServer();
}

void connectToTelnetServer() {
  Serial.print("Proberen te verbinden met Telnet-server op ");
  Serial.print(telnetServerIP);
  Serial.print(":");
  Serial.println(telnetServerPort);

  if (telnetClient.connect(telnetServerIP, telnetServerPort)) {
    Serial.println("Succesvol verbonden met de Telnet-server!");

    // Stuur het wachtwoord naar de Telnet-server
    telnetClient.println(telnetWW);
    Serial.println("Wachtwoord verstuurd naar Telnet-server.");
    isTelnetVerbonden = true;
  } else {
    Serial.println("Verbinding met Telnet-server mislukt. Controleer IP en poort.");
    isTelnetVerbonden = false;
  }
}


//////////////////////////////////////// CODE VOOR HET HELE SYSTEEM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
void loop() {
  btn1 = digitalRead(btn1P);
  btn2 = digitalRead(btn2P);

// Check of de ESP32 nog steeds verbonden is met de Telnet-server
  if (telnetClient.connected() && isTelnetVerbonden) {
    if (btn1 == 1){
      Serial.println("btn 1 pressed");
      btn1a();
      delay(20);
    }
    if (btn2 == 1){
      String message2 = "say button 2 pressed";
      telnetClient.println(message2);
      Serial.println("btn 2 pressed");
      btn2a();
      delay(20);
    }
    // Ontvang data van de Telnet-server en stuur naar de seriële monitor voor het testen (!niet in eindcode!)
    if (telnetClient.available()) {
      String telnetResponse = telnetClient.readString();
      Serial.print("Ontvangen van Telnet: ");
      Serial.println(telnetResponse);
    }
  }

  delay(10);
}




void btn1a(void) {
  String message1 = "say i am going to the genesis";
  telnetClient.println(message1);  // Stuur input via Telnet
  Serial.println("AMR:");
  Serial.print("wordt geroepen");
  delay(200);
}

void btn2a(void) {
  delay(200);
}