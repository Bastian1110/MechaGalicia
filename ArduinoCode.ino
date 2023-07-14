
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char *ssid = "hotspot";
const char *password = "12345678";

WebServer server(80);

const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/html", "Smart car server up and ready! 🥰");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void handleMovement(){
  int n_args = server.args();
  if(n_args != 1){
    server.send(400, "text/plain", "Bad request :c");
  }
  String dat = server.arg(0);
  if (dat == "FORWARD" ){
    neopixelWrite(RGB_BUILTIN,142,9,246);
  }
  else if (dat == "STOP"){
    neopixelWrite(RGB_BUILTIN,246,9,9);
  }
  else if (dat == "LEFT"){
    neopixelWrite(RGB_BUILTIN,16,82,223);
  }
  else{
    neopixelWrite(RGB_BUILTIN,220,232,21);
  }

  
  Serial.println(dat); // Send data tu arduino
  server.send(200, "text/plain", "Todo ok :D");
}

void setup(void) {
  pinMode(led, OUTPUT);
  pinMode(RGB_BUILTIN, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    neopixelWrite(RGB_BUILTIN,64,0,0);
    delay(300);
    neopixelWrite(RGB_BUILTIN,0,0,0);
    delay(300);
  }
  neopixelWrite(RGB_BUILTIN,0,64,0);

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/move", handleMovement);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
