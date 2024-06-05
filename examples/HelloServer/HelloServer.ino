/*
 * HelloServer example from the ESP32 WebServer library modified for Ethernet.
 */

#include <EthernetESP32.h>
#include <WebServer.h>
#include <ESPmDNS.h>

W5500Driver driver;
//ENC28J60Driver driver;
//EMACDriver driver(ETH_PHY_LAN8720);

WebServer server(80);

const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp32!");
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

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);

  Serial.begin(115200);
  delay(500);
  while (!Serial);

  Ethernet.init(driver);

  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin()) {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  } else {
    Serial.println("Failed to configure Ethernet using DHCP");
    while (true) {
      delay(1);
    }
  }

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);  //allow the cpu to switch to other tasks
}
