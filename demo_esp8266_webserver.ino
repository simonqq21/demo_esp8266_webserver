#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h>
#include "Webpage.h" 

#define LOCAL_SSID "QUE-STARLINK"
#define LOCAL_PASS "Quefamily01259"

// set web server port number to 80
ESP8266WebServer server(80); 

//static IP address configuration 
IPAddress local_IP(192,168,5,75);
IPAddress gateway(192,168,5,1);
IPAddress subnet(255,255,255,0);
IPAddress primaryDNS(8,8,8,8);
IPAddress secondaryDNS(8,8,4,4);

// variable to store the HTTP request
String header; 
char json[2048];
char buff[32];

// status variables 
bool led1Bit = false;
uint32_t lastTimeUpdated = 0;

// LED pins
// #define LED1 2 
int LEDS[3] = {2, 3, 4};
int BUTTONS[1] = {5}; 

// current time 
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  for (int pin: LEDS) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  for (int pin: BUTTONS) {
    pinMode(pin, iNPUT_PULLUP);
  }
  
  //  disable watchdog timer
//  wdt_disable();
  
  Serial.print("Connecting to "); 
  Serial.println(LOCAL_SSID);

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Station failed to configure.");
  }
  WiFi.begin(LOCAL_SSID, LOCAL_PASS); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); 
    Serial.print(".");
  }

  //  print local IP address and start web server 
  printWiFi();

  server.on("/", sendWebpage); 
  server.on("/json", sendJSON);
  server.on("/led", controlLED);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
//  if (millis() - lastTimeUpdated >= 50) {
//    lastTimeUpdated = millis();
//    
//
//  }
  server.handleClient();
}

void sendWebpage() {
  Serial.println("Sending web page");
  server.send(200, "text/html", PAGE_MAIN);
}

// json format 
//{
//    type: "led",
//    index: ${index},
//    state: ${state}
//}
void sendJSON() {
  strcpy(json, "{\n");
  sprintf(buff, "\"type\": \"led\",\n");
  strcat(json, buff);
  sprintf(buff, "\"index\": %d,\n", 1);
  strcat(json, buff);
  sprintf(buff, "\"state\": %d\n}", led1Bit);
  strcat(json, buff);
  Serial.println(json);
  server.send(200, "application/json", json);
}

void controlLED() {
    String index = server.arg("index");
    String state = server.arg("state");
    if (state == "0") {
      led1Bit = false;
    }
    else {
      led1Bit = true;
    }
    Serial.print("Index=");
    Serial.println(index);
    Serial.print("State=");
    Serial.println(led1Bit);
    Serial.println();
    digitalWrite(LED1, led1Bit);
    sendJSON();
}
 
void printWiFi() {
  Serial.println(" ");
  Serial.println("WiFi connected.");
  Serial.print("WiFi SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: "); 
  Serial.println(WiFi.localIP()); 
  long rssi = WiFi.RSSI(); 
  Serial.print("Signal strength (RSSI): "); 
  Serial.print(rssi);
  Serial.println(" dBm");
}
