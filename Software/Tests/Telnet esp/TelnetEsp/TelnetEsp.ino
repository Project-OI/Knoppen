/* -----------------  De eerste code om een de ESPC3 mini met de AMR te verbinden ----------------- */
/* -----------------  via de AMR Telnet connectie. Met de code kun je in de       ----------------- */
/* -----------------  Serial Monitor berichten versturen van de ESP naar de AMR   ----------------- */
/* -----------------  via Telnet.                                                  ----------------- */

#include <WiFi.h>

const char* ssid = "SSID";      // De WiFi-SSID
const char* password = "PASSWORD";  // De WiFi-wachtwoord
const char* telnetServerIP = "IP";  // IP van de Telnet-server
const uint16_t telnetServerPort = 0000;      // Telnet-server poort
WiFiClient telnetClient;


void setup() {
  Serial.begin(9600);
  Serial.println("ESP32 opstarten...");

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
  Serial.print("Proberen te verbinden met Telnet-server op ");
  Serial.print(telnetServerIP);
  Serial.print(":");
  Serial.println(telnetServerPort);

  if (telnetClient.connect(telnetServerIP, telnetServerPort)) {
    Serial.println("Succesvol verbonden met de Telnet-server!");
  } else {
    Serial.println("Verbinding met Telnet-server mislukt. Controleer IP en poort.");
  }
}

void loop() {
  // Check of de ESP32 nog steeds verbonden is met de Telnet-server
  if (telnetClient.connected()) {
    // Controleer of er input is van de seriële monitor
    if (Serial.available()) {
      String serialInput = Serial.readString();
      Serial.print("Versturen naar Telnet: ");
      Serial.println(serialInput);
      telnetClient.print(serialInput);  // Stuur input via Telnet
    }

    // Ontvang data van de Telnet-server en stuur naar de seriële monitor
    if (telnetClient.available()) {
      String telnetResponse = telnetClient.readString();
      Serial.print("Ontvangen van Telnet: ");
      Serial.println(telnetResponse);
    }

    
  } else {
    Serial.println("Verbinding met de Telnet-server verloren.");
    delay(1000);  // Wacht even en probeer opnieuw te verbinden
    Serial.print("Opnieuw proberen te verbinden met de Telnet-server op ");
    Serial.print(telnetServerIP);
    Serial.print(":");
    Serial.println(telnetServerPort);

    if (telnetClient.connect(telnetServerIP, telnetServerPort)) {
      Serial.println("Opnieuw verbonden met de Telnet-server.");
    } else {
      Serial.println("Opnieuw verbinden met Telnet-server mislukt.");
    }
  }
}
