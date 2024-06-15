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

#include "W5500Driver.h"

#include "esp_eth_mac.h"
#include <Arduino.h>

esp_eth_mac_t* W5500Driver::newMAC() {

  pinMode(pinCS, OUTPUT);
  digitalWrite(pinCS, HIGH);

  spi->begin();

  eth_w5500_config_t mac_config;
  mac_config.int_gpio_num = digitalPinToGPIONumber(pinIRQ);
  mac_config.poll_period_ms = (pinIRQ < 0) ? 10 : 0;
  initCustomSPI(mac_config.custom_spi_driver);

  eth_mac_config_t eth_mac_config = ETH_MAC_DEFAULT_CONFIG();
  return esp_eth_mac_new_w5500(&mac_config, &eth_mac_config);
}

esp_eth_phy_t* W5500Driver::newPHY() {
  eth_phy_config_t phy_config = ETH_PHY_DEFAULT_CONFIG();
  phy_config.phy_addr = phyAddr;
  phy_config.reset_gpio_num = digitalPinToGPIONumber(pinRst);

  return esp_eth_phy_new_w5500(&phy_config);
}

bool W5500Driver::read(uint32_t cmd, uint32_t addr, void* data, uint32_t data_len) {
  spi->beginTransaction(SPISettings(1000000L * spiFreq, MSBFIRST, SPI_MODE0));
  digitalWrite(pinCS, LOW);

  spi->write16(cmd);
  spi->write(addr);
  spi->transferBytes(NULL, (uint8_t*) data, data_len);

  digitalWrite(pinCS, HIGH);
  spi->endTransaction();
  return ESP_OK;
}

bool W5500Driver::write(uint32_t cmd, uint32_t addr, const void* data, uint32_t data_len) {
  spi->beginTransaction(SPISettings(1000000L * spiFreq, MSBFIRST, SPI_MODE0));
  digitalWrite(pinCS, LOW);

  spi->write16(cmd);
  spi->write(addr);
  spi->writeBytes((const uint8_t*) data, data_len);

  digitalWrite(pinCS, HIGH);
  spi->endTransaction();
  return ESP_OK;
}
