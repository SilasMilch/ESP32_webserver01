#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

WebServer server (80);

const char* ssid     = "yourSSID";
const char* password = "yourPassword";
int LEDPIN = 32;

String ledState = "OFF";

void setup() 
{
  Serial.begin(115200);
  pinMode(LEDPIN, OUTPUT);
  connectToWifi();
  beginServer();
}

void loop() {
  server.handleClient();
}

void connectToWifi()
{
  WiFi.enableSTA(true);
  delay(2000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void beginServer()
{
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void handleRoot() { 
  if (server.hasArg("LED")) {
    handleSubmit();
  } else {
    server.send(200, "text/html", getPage());
  }  
}

void handleSubmit() {
  String LEDValue = server.arg("LED");
  Serial.print("Set GPIO ");
  Serial.println(LEDValue);
  if (LEDValue == "1") {
    digitalWrite(LEDPIN, HIGH);
    ledState = "On";
  } else if (LEDValue == "0") {
    digitalWrite(LEDPIN, LOW);
    ledState = "Off";
  } else {
    Serial.println("Error Led Value");
  }
  server.send(200, "text/html", getPage());
}

String getPage() {
  String page = "<!DOCTYPE html>";
  page += "<html>";
  page += "<head>";
  page += "<title>Instafeed on Your Website</title>";
  page += "<style type=\"text/css\">";
  page += "a img{ width: 25%; }";
  page += "</style>";
  page += "</head>";

