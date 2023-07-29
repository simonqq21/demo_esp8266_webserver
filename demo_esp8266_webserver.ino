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
bool ledBits[] = {false, false, false};
bool ledBlinks[] = {false, false, false};
unsigned long lastTimeBlinked[] = {0, 0, 0};
int onIntervals[] = {1000, 1000, 1000}, offIntervals[] = {1500, 2000, 3000};
uint32_t lastTimeUpdated = 0;

// LED pins
// #define LED1 2 
int LEDS[3] = {2, 3, 4};
int BUTTONS[1] = {5}; 
int globalLEDIndex = 0; 

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
    pinMode(pin, INPUT_PULLUP);
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
  for (int i=0;i<3;i++)
    blinkLED(i);
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
  sprintf(buff, "\"index\": %d,\n", globalLEDIndex);
  strcat(json, buff);
  sprintf(buff, "\"state\": %d,\n", ledBits[globalLEDIndex]);
  strcat(json, buff);
  sprintf(buff, "\"blink\": %d\n}", ledBlinks[globalLEDIndex]);
  strcat(json, buff);
  Serial.println(json);
  server.send(200, "application/json", json);
}

void controlLED() {
  int index = server.arg("index").toInt();
  globalLEDIndex = index;
  String state = server.arg("state");
  if (state == "0") {
    ledBlinks[index] = false;
    ledBits[index] = false;
  }
  else if (state == "1") {
    ledBlinks[index] = false;
    ledBits[index] = true;
  }
//    blink if state == 2  
  else if (state == "2") {
    ledBlinks[index] = true;
  }
  Serial.print("Index=");
  Serial.println(index);
  Serial.print("State=");
  Serial.println(ledBits[index]);
  Serial.println();
  digitalWrite(LEDS[index], ledBits[index]);
  sendJSON();
}

void blinkLED(int index) {
  if (ledBlinks[index]) {
    
    if (millis() - lastTimeBlinked[index] > offIntervals[index] && !ledBits[index]) {
      lastTimeBlinked[index] = millis();
      ledBits[index] = true;
      digitalWrite(LEDS[index], ledBits[index]);
    }
    if (millis() - lastTimeBlinked[index] > onIntervals[index] && ledBits[index]) {
      lastTimeBlinked[index] = millis();
      ledBits[index] = false;
      digitalWrite(LEDS[index], ledBits[index]);
    }
  } 
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
