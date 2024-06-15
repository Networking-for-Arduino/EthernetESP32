EthernetESP32 is an alternative to the ESP32 platform's bundled Ethernet library with additional support for ENC28J60.

EthernetESP32 library has the legacy Arduino Ethernet API as the classic Arduino Ethernet library.

EthernetESP32 requires ESP32 platform version 3 or higher.


## SPI modules

EthernetESP32 supports W5500, ENC28J60, DM9051 and KSZ8851SNL SPI Ethernet modules. 

### Wiring

Best option is to wire the SPI modules to default SPI pins of your ESP32 module or development board. The default pins are specified in `pins_arduino.h` for the ESP32 variant in variants folder of the platform.  For classic ESP32 without pin remapping the default pins are MOSI 23, MISO 19, SCK 18 and CS 5.

If you use other pins for the SPI object, add `Setup.begin(sck, miso, mosi);` in `setup()`, before `Ethernet.begin`.

It is possible to use other CS pin. The library supports INT pin of the modules for faster processing of received packets. Hardware reset is supported if the reset pin of the SPI module is wired to an IO pin.

### In sketch

The driver for specific Ethernet SPI module must be instanced as global in sketch and provide to Ethernet object with `Ethernet.init(driver);` before `Ethernet.begin`.

Example for default pins:

```
#include <EthernetESP32.h>

W5500Driver driver;

void setup() {

  Ethernet.init(driver);

  Ethernet.begin(); // begin with default MAC address and DHCP IP
  
```

There are 4 drivers for SPI modules: W5500Driver, ENC28J60Driver, DM9051Driver and KSZ8851SNLDriver.

The constructors for the drivers have 3 optional parameters CS pin, INT pin and reset pin. Default CS pin is the default SPI SS pin. Default value for the INT pin and reset pin is -1, which tells the library to not to use interrupt and hardware reset.

Examples:

```
W5500Driver driver(4); // CS pin is 4
W5500Driver driver(4, 26); // CS pin is 4, INT pin is 26
W5500Driver driver(4, 26, 27); // CS pin is 4, INT pin is 26, reset pin is 27
W5500Driver driver(SS, -1, 27); // CS pin is default, INT is not used and reset pin is 27
```

To use other SPIClass instance set it on driver with `setSPI`. To change the SPI frequency use `setSpiFreq`. Default frequency is 20 MHz. Sometimes automatic PHY address detection doesn't work, then use `setPhyAddr` to set the right PHY address. 

Example with all options specified (using HSPI as SPI1 on FSPI pins):

```
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF };
W5500Driver driver(5, 26, 27);
SPIClass SPI1(HSPI);

void setup() {

  SPI1.begin(18, 19, 23);

  driver.setSPI(SPI1);
  driver.setSpiFreq(10);
  driver.setPhyAddress(0);

  Ethernet.init(driver);

  Ethernet.begin(mac);

```

## PHY modules

The EthernetESP32 library supports PHY modules with ESP32 Ethernet peripheral. EMAC is available only on classic ESP32. Supported PHY modules are: LAN8720, TLK110, RTL8201, DP83848 and  KSZ80XX series.

### Wiring

Dedicated pins are RMII_TX_EN  21, RMII_TX0 19, RMII_TX1 22, RMII_RX0 25, RMII_RX1_EN 26, RMII_CRS_DV 27. MDC and MDIO pins can be any pin. The default in the library is MDC 23 and MDIO 18. Additionally power pin can be specified for hardware reset.

Clock pin may be input or output. IO 0 is default. For output clock pins 16 and 17 can be used too.

### In sketch

There is one driver class EMACDriver for PHY modules on the Ethernet peripheral. The specific PHY module is specified as first parameter of the constructor. The constants are  `ETH_PHY_LAN8720`, `ETH_PHY_TLK110`, `ETH_PHY_RTL8201`, `ETH_PHY_DP83848`,
and  `ETH_PHY_KSZ80XX`.

The driver must be instanced as global in sketch and provide to Ethernet object with `Ethernet.init(driver);` before `Ethernet.begin`.

Example for default pins:

```
#include <EthernetESP32.h>

EMACDriver driver(ETH_PHY_LAN8720);

void setup() {

  Ethernet.init(driver);

  Ethernet.begin();

```

The EMACDriver has optional parameters:

* **mdcPin** - default is 23
* **mdioPin** - default is 18
* **powerPin** - default is -1 (pin not used)
* **clockMode** - values from enum emac_rmii_clock_gpio_t, default is `EMAC_APPL_CLK_OUT_GPIO`,
* **clockMode** - values from enum emac_rmii_clock_mode_t, default is `EMAC_CLK_EXT_IN`

The values for clockPin and clockMode are [enums from ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_eth.html?highlight=rmii#id14).

Sometimes automatic PHY address detection doesn't work, then use `setPhyAddr` to set the right PHY address. 

## The Ethernet object

### MAC address

By default the library uses a MAC address from the range of MAC address assigned to your ESP32 module. You can specify custom MAC address as (first) parameter of `Ethernet.begin` as in the Arduino Ethernet library.

### Static IP configuration

As in the Arduino Ethernet library static IP configuration is specified with `Ethernet.begin(ip, dns, gateway, netmask)` or `Ethernet.begin(mac, ip, dns, gateway, netmask)`.

## Implementation details

The EthernetESP32 library wraps drivers provided by the ESP-IDF framework. The ENC29J60 driver included in the library is from ESP-IDF examples.

The Ethernet object inherits from NetworkInterface class from the Network library of the ESP32 Arduino platform. It adds Arduino Ethernet API methods not implemented in NetworkInterface.

The integration of the IDF drivers with NetworkInterface is code adopted from ETH.cpp of the bundled Ethernet library.

EthernetClient, EthernetServer and EthernetUDP are macros aliasing NetworkClient, NetworkServer and NetworkUDP from the Network library (as are WiFiCllent, WiFiServer and WiFiUDP in the WiFi library).

Network modules tested with the library are SPI modules W5500 and ENC28J60 and a LAN8720 PHY module.
