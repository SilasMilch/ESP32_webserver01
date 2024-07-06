#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

WebServer server(80);

const char* ssid     = "yourSSID";
const char* password = "yourPassword";
int LEDPIN = 32;

String ledState = "OFF";

void setup() {
  Serial.begin(115200);
  pinMode(LEDPIN, OUTPUT);
  connectToWifi();
  beginServer();
}

void loop() {
  server.handleClient();
}

void connectToWifi() {
  WiFi.enableSTA(true);
  delay(2000);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void beginServer() {
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
    Serial.println("Error: Invalid LED Value");
  }
  server.send(200, "text/html", getPage());
}

String getPage() {  //in getPage() kommt die Website rein mittels "page +="";"
  String page = "<!DOCTYPE html>";
  page += "<html>";
  page += "<head>";
  page += "<title>LED Control</title>";
  page += "<style type=\"text/css\">";
  page += "a img{ width: 25%; }";
  page += "</style>";
  page += "</head>";
  page += "<body>";
  page += "<h1>LED Control</h1>";
  page += "<p>LED is currently: " + ledState + "</p>";
  page += "<a href=\"/?LED=1\"><button>Turn On</button></a>";
  page += "<a href=\"/?LED=0\"><button>Turn Off</button></a>";
  page += "<div id=\"instafeed-container\"></div>";
  page += "<script src=\"https://cdn.jsdelivr.net/gh/stevenschobert/instafeed.js@2.0.0rc1/src/instafeed.min.js\"></script>";
  page += "<script type=\"text/javascript\">";
  page += "var userFeed = new Instafeed({";
  page += "get: 'user',";
  page += "target: 'instafeed-container',";
  page += "resolution: 'low_resolution',";
  page += "sortBy: 'most-recent',";
  page += "limit: 1,";
  page += "accessToken: 'YOUR_INSTAGRAM_ACCESS_TOKEN_GOES_HERE'";
  page += "});";
  page += "userFeed.run();";
  page += "</script>";
  page += "</body>";
  page += "</html>";
  return page;
}
