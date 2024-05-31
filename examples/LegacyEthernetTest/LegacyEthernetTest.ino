/**
 * LegacyEthernetTest from NetApiHelpers library
 * modified for the EthernetESP32 library.
 */

#include <EthernetESP32.h>
#include <MacAddress.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF };

W5500Driver driver;
//ENC28J60Driver driver;
//EMACDriver driver(ETH_PHY_LAN8720);

void setup() {

  Serial.begin(115200);
  delay(500);
  while (!Serial);
  Serial.println("START");
  Serial.println();

	Ethernet.init(driver);

  Serial.println("Attempting to connect with DHCP ...");
  testEthernet(true);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  Serial.println("Checking link ...");
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("\tretry...");
    delay(500);
  }
  switch (Ethernet.linkStatus()) {
    case LinkOFF:
      Serial.println("\tEthernet cable is not connected.");
      break;
    case LinkON:
      Serial.println("\tEthernet cable is connected.");
      break;
    default:
      Serial.println("\tLink state unknown.");
      break;
  }
  Serial.println();

  IPAddress ip = Ethernet.localIP();
  IPAddress gw = Ethernet.gatewayIP();
  IPAddress mask = Ethernet.subnetMask();
  IPAddress dns = Ethernet.dnsServerIP();

  const char *serverName = "www.google.com";
  Serial.print("Resolve IP for ");
  Serial.println(serverName);
  IPAddress resolvedIP;
  if (Ethernet.hostByName("www.google.com", resolvedIP) == 1) {
    Serial.print("\t");
    Serial.println(resolvedIP);
  } else {
    Serial.println("\t...ERROR");
  }
  Serial.println();

  Ethernet.end();

//  IPAddress ip(192, 168, 1, 177);
//  IPAddress gw(192, 168, 1, 1);
//  IPAddress dns(192, 168, 1, 1);
//  IPAddress mask(255, 255, 255, 0);

  Serial.print("Configuring static IP ");
  ip[3] = 177;
  Serial.println(ip);
  Ethernet.begin(mac, ip, dns, gw, mask);
  if (Ethernet.dnsServerIP() == mask) {
    Serial.println("ERROR: begin() has wrong ordering of parameters");
    while (true) {
      delay(1);
    }
  }
  testEthernet(false);
  if (ip != Ethernet.localIP()) {
    Serial.println("ERROR: Static IP was not used.");
    while (true) {
      delay(1);
    }
  }

  IPAddress dnsIP2(8, 8, 8, 8);
  Ethernet.setDnsServerIP(dnsIP2);
  if (Ethernet.dnsServerIP() != dnsIP2) {
    Serial.print("ERROR: DNS IP setter or getter error. dnsServerIP() returned ");
    Serial.println(Ethernet.dnsServerIP());
    Serial.println();
  }
  Ethernet.setDnsServerIP(dns);

  Ethernet.end();

  Serial.print("Configuring with automatic gateway, DNS and mask with static IP "); // <-------
  ip[3] = 178;
  Serial.println(ip);
  Ethernet.begin(mac, ip);
  testEthernet(false);
  if (ip != Ethernet.localIP()) {
    Serial.println("ERROR: Static IP was not used.");
    while (true) {
      delay(1);
    }
  }
  IPAddress autoIP(ip);
  autoIP[3] = 1;
  IPAddress defaultMask(255, 255, 255, 0);
  if (Ethernet.gatewayIP() == autoIP && Ethernet.dnsServerIP() == autoIP && Ethernet.subnetMask() == defaultMask) {
    Serial.println("Automatic config values are OK");
  } else {
    Serial.println("ERROR: Automatic config values are wrong");
    if (Ethernet.gatewayIP() != autoIP) {
      Serial.print("\tgateway IP Address: ");
      Serial.println(Ethernet.gatewayIP());
    }
    if (Ethernet.subnetMask() != defaultMask) {
      Serial.print("\tsubnet IP mask: ");
      Serial.println(Ethernet.subnetMask());
    }
    if (Ethernet.dnsServerIP() != autoIP) {
      Serial.print("\tDNS server: ");
      Serial.println(Ethernet.dnsServerIP());
    }
  }
  Serial.println();

  Ethernet.end();

  Serial.println("Attempting to connect with DHCP again ...");
  testEthernet(true);

  if (ip == Ethernet.localIP()) {
    Serial.println("ERROR: The IP didn't change from static IP to DHCP assigned IP.");
  } else {
    Serial.println("Check on router the hostname and MAC address.");
  }
  Serial.println();

  Serial.println("END");
}

void loop() {
  delay(1);
}

void testEthernet(bool dhcp) {

  if (dhcp) {
    Ethernet.setHostname("arduino");
    if (!Ethernet.begin(mac)) {
      Serial.println("\t...ERROR");
      while (true) {
        delay(1);
      }
    } else {
      Serial.println("\t...success");
    }
  }
  Serial.println();

  printEthernetStatus();
  Serial.println();

  Serial.print("Attempt to connect to port 80 on ");
  Serial.println(Ethernet.gatewayIP());
  EthernetClient client;
  if (client.connect(Ethernet.gatewayIP(), 80)) {
    Serial.println("\t...success");
  } else {
    Serial.println("\t...ERROR");
  }
  client.stop();
  Serial.println();
}

void printEthernetStatus() {

  byte mac[6];
  Ethernet.MACAddress(mac);
  Serial.print("MAC: ");
  Serial.println(MacAddress(mac));
  if (mac[0] & 1) { // unicast bit is set
    Serial.println("\t is the ordering of the MAC address bytes reversed?");
  }

  Serial.print("IP Address: ");
  Serial.println(Ethernet.localIP());

  Serial.print("gateway IP Address: ");
  Serial.println(Ethernet.gatewayIP());

  Serial.print("subnet IP mask: ");
  Serial.println(Ethernet.subnetMask());

  Serial.print("DNS server: ");
  IPAddress dns = Ethernet.dnsServerIP();
  if (dns == INADDR_NONE) {
    Serial.println("not set");
  } else {
    Serial.println(dns);
  }
}
