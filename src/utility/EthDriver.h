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

#ifndef _ETH_DRIVER_H_
#define _ETH_DRIVER_H_

#include "esp_system.h"
#include "esp_eth.h"
#include "SPI.h"

#ifndef ETH_PHY_SPI_FREQ_MHZ
#define ETH_PHY_SPI_FREQ_MHZ 20
#endif

class EthDriver {
public:

  virtual ~EthDriver();

  void begin();
  void end();

  void setPhyAddress(int32_t addr);

  virtual bool usesIRQ() = 0;

protected:
  virtual esp_eth_mac_t* newMAC() = 0;
  virtual esp_eth_phy_t* newPHY() = 0;

  friend class EthernetClass;

  int32_t phyAddr = ESP_ETH_PHY_ADDR_AUTO;

  esp_eth_mac_t* mac = NULL;
  esp_eth_phy_t* phy = NULL;
};

class EthSpiDriver : public EthDriver {
public:

  EthSpiDriver(int8_t cs = SS, int8_t irq = -1, int8_t rst = -1) {
    pinCS = cs;
    pinIRQ = irq;
    pinRst = rst;
  }

  void setSPI(SPIClass &spiObj) {
    spi = &spiObj;
  }

  void setSpiFreq(uint8_t freqMHz) {
    spiFreq = freqMHz;
  }

  virtual bool usesIRQ() {
    return (pinIRQ >= 0);
  }

  virtual bool read(uint32_t cmd, uint32_t addr, void *data, uint32_t data_len) = 0;
  virtual bool write(uint32_t cmd, uint32_t addr, const void *data, uint32_t data_len) = 0;

protected:
  void initCustomSPI(eth_spi_custom_driver_config_t& customSPI);

  SPIClass* spi = &SPI;
  uint8_t spiFreq = ETH_PHY_SPI_FREQ_MHZ;
  int8_t pinCS;
  int8_t pinIRQ;
  int8_t pinRst;
};

void* eth_spi_init(const void *ctx);
esp_err_t eth_spi_deinit(void *ctx);
esp_err_t eth_spi_read(void *ctx, uint32_t cmd, uint32_t addr, void *data, uint32_t data_len);
esp_err_t eth_spi_write(void *ctx, uint32_t cmd, uint32_t addr, const void *data, uint32_t data_len);

#endif
