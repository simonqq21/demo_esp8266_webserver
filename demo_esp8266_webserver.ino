#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h>
#include <LittleFS.h>

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
String html;
char json[256];
char buff[32];

// status variables 
bool ledBits[] = {false, false, false};
bool ledBlinks[] = {false, false, false};
unsigned long lastTimeBlinked[] = {0, 0, 0};
int onIntervals[] = {1000, 1000, 1000}, offIntervals[] = {1000, 3000, 7000};
uint32_t lastTimeUpdated = 0;
bool buttonBits[] = {false};

// LED pins
// #define LED1 2 
int LEDS[3] = {2, 3, 4};
int BUTTONS[1] = {5}; 
int globalLEDIndex = 0; 
int globalBtnIndex = 0; 

// current time 
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000; 

File indexPage; 

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
  
  if (!LittleFS.begin()) {
    Serial.println("An error occured while mounting LittleFS.");
  }
  
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
  server.on("/led", controlLED);
  server.on("/ledstatus", updateLEDStatus);
  server.on("/button", sendBtnJSON);
  server.begin();
}

void loop() {
  // blink the LEDs that are supposed to blink 
  for (int i=0;i<3;i++)
    blinkLED(i);

  // read the buttons 
  for (int i=0;i<1;i++) 
    buttonBits[i] = digitalRead(BUTTONS[i]);
  server.handleClient();
}

void sendWebpage() {
  Serial.println("Sending web page");
  indexPage = LittleFS.open("/index.html", "r");
  if (!indexPage) {
    Serial.println("Failed to open file for reading."); 
  }
  html = "";
  while (indexPage.available()) {
    html += (char) indexPage.read();
  }
  server.send(200, "text/html", html);
  indexPage.close();
}

// button json format 
// {
//     type: "button",
//     index: ${index},
//     state: ${state}
// }
void sendBtnJSON() {
  globalBtnIndex = server.arg("index").toInt();
  strcpy(json, "{\n");
  sprintf(buff, "\"type\": \"button\",\n");
  strcat(json, buff);
  sprintf(buff, "\"index\": %d,\n", globalBtnIndex);
  strcat(json, buff);
  sprintf(buff, "\"state\": %d\n}", buttonBits[globalBtnIndex]);
  strcat(json, buff);
  Serial.println(json);
  server.send(200, "application/json", json);
}

// LED json format 
//{
//    type: "led",
//    "index": ${index},
//    "state": ${state},
//    "blink": ${blink}
//}
void controlLED() {
  int index = server.arg("index").toInt();
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
    ledBits[index] = false;
    ledBlinks[index] = true;
  }
  Serial.print("Index=");
  Serial.println(index);
  Serial.print("State=");
  Serial.println(ledBits[index]);
  Serial.println();
  digitalWrite(LEDS[index], ledBits[index]);
  sendLEDJSON(index);
}

void sendLEDJSON(int index) {
  strcpy(json, "{\n");
  sprintf(buff, "\"type\": \"led\",\n");
  strcat(json, buff);
  sprintf(buff, "\"index\": %d,\n", index);
  strcat(json, buff);
  sprintf(buff, "\"state\": %d,\n", ledBits[index]);
  strcat(json, buff);
  sprintf(buff, "\"blink\": %d\n}", ledBlinks[index]);
  strcat(json, buff);
  Serial.println(json);
  server.send(200, "application/json", json);
}

void updateLEDStatus() {
  int ledIndex = server.arg("index").toInt();
  sendLEDJSON(ledIndex);
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
