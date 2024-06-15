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

#include "EthDriver.h"

#include <Arduino.h>

void EthDriver::begin() {
  if (mac == NULL) {
    mac = newMAC();
    phy = newPHY();
  }
}
void EthDriver::end() {
  if (mac != NULL) {
    mac->del(mac);
    mac = NULL;
  }
  if (phy != NULL) {
    phy->del(phy);
    phy = NULL;
  }
}

void EthDriver::setPhyAddress(int32_t addr) {
  phyAddr = addr;
}

EthDriver::~EthDriver() {
  end();
};

void* eth_spi_init(const void *ctx) {
  return (void *)ctx;
}

esp_err_t eth_spi_deinit(void *ctx) {
  return ESP_OK;
}

esp_err_t eth_spi_read(void *ctx, uint32_t cmd, uint32_t addr, void *data, uint32_t data_len) {
  return ((EthSpiDriver*) ctx)->read(cmd, addr, data, data_len);
}

esp_err_t eth_spi_write(void *ctx, uint32_t cmd, uint32_t addr, const void *data, uint32_t data_len) {
  return ((EthSpiDriver*) ctx)->write(cmd, addr, data, data_len);
}

void EthSpiDriver::initCustomSPI(eth_spi_custom_driver_config_t& customSPI) {
  customSPI.config = this;
  customSPI.init = eth_spi_init;
  customSPI.deinit = eth_spi_deinit;
  customSPI.read = eth_spi_read;
  customSPI.write = eth_spi_write;
}

