#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include "config.h"
#include "html.h"

const int port = HTTP_PORT;
const char* ssid1 = HOME_WIFI_SSID;
const char* passwd1 = HOME_WIFI_PASSWORD;
const char* ssid2 = MATT_HOTSPOT_SSID;
const char* passwd2 = MATT_HOTSPOT_PASSWORD;

int RED_LIGHT = D5;
int YEL_LIGHT = D6;
int GRN_LIGHT = D7;

bool cycle = false;
unsigned long currentTime;
unsigned long prevTime = 0;

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(port);

void rootPage() {
  server.send(200, "text/html", HOME_page);
  logRequest("200", "");
}

void notFound() {
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
  logRequest("302", "Rerouted to \"/\"");
}

void getStatus() {
  String color = getLED();
  server.send(200, "application/json", getColorJson(color));
}

void redOn() {
  cycle = false;
  String old_color = getLED();
  setLED("red");
  String new_color = getLED();
  String msg = "Changed light from " + old_color + " to " + new_color;
  server.send(200, "application/json", getColorJson(new_color));
  logRequest("200", msg);
}

void yellowOn() {
  cycle = false;
  String old_color = getLED();
  setLED("yellow");
  String new_color = getLED();
  String msg = "Changed light from " + old_color + " to " + new_color;
  server.send(200, "application/json", getColorJson(new_color));
  logRequest("200", msg);
}

void greenOn() {
  cycle = false;
  String old_color = getLED();
  setLED("green");
  String new_color = getLED();
  String msg = "Changed light from " + old_color + " to " + new_color;
  server.send(200, "application/json", getColorJson(new_color));
  logRequest("200", msg);
}

void turnOffLights() {
  cycle = false;
  String old_color = getLED();
  setLED("off");
  String new_color = getLED();
  String msg = "Changed light from " + old_color + " to " + new_color;
  server.send(200, "application/json", getColorJson(new_color));
  logRequest("200", msg);
}

void cycleLights() {
  setLED("red");
  prevTime = millis();
  cycle = true;
  String color = getLED();
  server.send(200, "application/json", getColorJson(color));
  logRequest("200", "Turned on Auto Mode");
}

void setLED(String color) {
  digitalWrite(RED_LIGHT, (color == "red"));
  digitalWrite(YEL_LIGHT, (color == "yellow"));
  digitalWrite(GRN_LIGHT, (color == "green"));
}

String getLED() {
  String res = "";

  if (digitalRead(RED_LIGHT)) return "red";
  if (digitalRead(YEL_LIGHT)) return "yellow";
  if (digitalRead(GRN_LIGHT)) return "green";

  return "off";
}

String getColorJson(String _color) {
  String res = "{ \"color\": \"" + _color + "\"}";
  return res;
}

void logRequest(String request_status, String additional_info) {
  String clientIP = server.client().remoteIP().toString();
  String reqType = (server.method() == HTTP_GET)? "GET" : "POST";
  String reqURI = server.uri();
  String addInfoTabs = (reqURI.length() < 11)? "\t\t" : "\t";
  String msg = clientIP + "\t" + reqType + "\t" + request_status + "\t" + reqURI + addInfoTabs + additional_info;
  Serial.println(msg);
}

void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(ssid1, passwd1);
  wifiMulti.addAP(ssid2, passwd2);
  
  Serial.print("\nConnecting");
  
  while (wifiMulti.run() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected to ");
  Serial.print(WiFi.SSID());
  Serial.print(", IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, rootPage);
  server.on("/api/status", HTTP_GET, getStatus);
  server.on("/api/set-red", HTTP_POST, redOn);
  server.on("/api/set-yellow", HTTP_POST, yellowOn);
  server.on("/api/set-green", HTTP_POST, greenOn);
  server.on("/api/lights-out", HTTP_POST, turnOffLights);
  server.on("/api/cycle", HTTP_POST, cycleLights);
  server.on("/api/set-red", HTTP_GET, redOn);
  server.on("/api/set-yellow", HTTP_GET, yellowOn);
  server.on("/api/set-green", HTTP_GET, greenOn);
  server.on("/api/lights-out", HTTP_GET, turnOffLights);
  server.on("/api/cycle", HTTP_GET, cycleLights);

  server.onNotFound(notFound);

  server.begin();
  Serial.print("Web Server Listening on Port ");
  Serial.println(HTTP_PORT);

  pinMode(RED_LIGHT, OUTPUT);
  pinMode(YEL_LIGHT, OUTPUT);
  pinMode(GRN_LIGHT, OUTPUT);
}

void loop() {
  server.handleClient();

  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("WiFi not connected!");
    delay(1000);
  }

  currentTime = (millis() - prevTime)/1000;

  if (cycle && currentTime < 1) setLED("red");
  if (cycle && currentTime >= 1 && currentTime < 2) setLED("off");
  if (cycle && currentTime >= 2) prevTime = millis();
}
