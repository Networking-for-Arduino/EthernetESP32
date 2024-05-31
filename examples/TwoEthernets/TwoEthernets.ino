/**
 * Example for two Ethernet instances
 */

#include <EthernetESP32.h>
#include <MacAddress.h>

W5500Driver driver;
W5500Driver driver1(16);

EthernetClass Ethernet1;

void setup() {

  Serial.begin(115200);
  while (!Serial);

  Ethernet.init(driver);
  Ethernet1.init(driver1);

  Serial.println("Attempting to connect with DHCP (eth0) ...");
  if (!Ethernet.begin()) {
    Serial.println("\t...ERROR (eth0)");
    while (true) {
      delay(1);
    }
  } else {
    Serial.println("\t...success (eth0)");
    printEthernetStatus(Ethernet);
  }

  Serial.println();

  Serial.println("Attempting to connect with DHCP (eth1) ... ");
  if (!Ethernet1.begin()) {
    Serial.println("\t...ERROR (eth1)");
    while (true) {
      delay(1);
    }
  } else {
    Serial.println("\t...success (eth1)");
    printEthernetStatus(Ethernet1);
  }
}

void loop() {
  delay(1);
}


void printEthernetStatus(EthernetClass& eth) {

  byte mac[6];
  eth.MACAddress(mac);
  Serial.print("MAC: ");
  Serial.println(MacAddress(mac));
  if (mac[0] & 1) { // unicast bit is set
    Serial.println("\t is the ordering of the MAC address bytes reversed?");
  }

  Serial.print("IP Address: ");
  Serial.println(eth.localIP());

  Serial.print("gateway IP Address: ");
  Serial.println(eth.gatewayIP());

  Serial.print("subnet IP mask: ");
  Serial.println(eth.subnetMask());

  Serial.print("DNS server: ");
  IPAddress dns = eth.dnsServerIP();
  if (dns == INADDR_NONE) {
    Serial.println("not set");
  } else {
    Serial.println(dns);
  }
}
