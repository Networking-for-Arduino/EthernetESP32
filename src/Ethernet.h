/*
  This file is part of the EthernetESP32 library for Arduino
  https://github.com/Networking-for-Arduino/EthernetESP32
  Copyright 2024 Juraj Andrassy

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _ETHERNET_ESP32_H_
#define _ETHERNET_ESP32_H_

#include "Network.h"
#include "esp_netif.h"
#include "utility/EthDriver.h"

enum EthernetLinkStatus {
  Unknown, LinkON, LinkOFF
};

enum EthernetHardwareStatus {
  EthernetNoHardware, EthernetHardwareFound
};

class EthernetClass : public NetworkInterface {

public:

  EthernetClass();

  void init(EthDriver& ethDriver);

  int begin(uint8_t *mac, unsigned long timeout = 60000);
  void begin(uint8_t *mac, IPAddress ip, IPAddress dns = INADDR_NONE, IPAddress gateway = INADDR_NONE, IPAddress subnet = INADDR_NONE);

  int begin(unsigned long timeout = 60000);
  void begin(IPAddress ip, IPAddress dns = INADDR_NONE, IPAddress gateway = INADDR_NONE, IPAddress subnet = INADDR_NONE);

  void end();
  int maintain();

  // Ethernet API functions
  EthernetLinkStatus linkStatus();
  EthernetHardwareStatus hardwareStatus();

  // legacy API functions
  void MACAddress(uint8_t *mac);
  IPAddress dnsServerIP();
  void setDnsServerIP(const IPAddress dns);

  // API functions missing in NetworkInterface
  void setDNS(IPAddress dns, IPAddress dns2 = INADDR_NONE);
  int hostByName(const char *hostname, IPAddress &result);

  virtual size_t printDriverInfo(Print &out) const;

  void _onEthEvent(int32_t eventId, void *eventData);

  esp_eth_handle_t getEthHandle() {
    return ethHandle;
  }

  uint8_t index = 0;

protected:
  EthDriver* driver = nullptr;
  esp_eth_handle_t ethHandle = NULL;
  esp_event_handler_instance_t _eth_ev_instance = NULL;
  esp_eth_netif_glue_handle_t glueHandle = NULL;

  EthernetHardwareStatus hwStatus = EthernetNoHardware;

  bool beginETH(uint8_t *mac);
};

extern EthernetClass Ethernet;

#define EthernetUDP NetworkUDP
#define EthernetServer NetworkServer
#define EthernetClient NetworkClient

#endif
