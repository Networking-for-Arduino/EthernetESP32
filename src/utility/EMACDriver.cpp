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

#include <Arduino.h>
#include "EMACDriver.h"

#ifdef CONFIG_ETH_USE_ESP32_EMAC

EMACDriver::EMACDriver(EthPhyType phyType, int mdcPin, int mdioPin, int powerPin, emac_rmii_clock_gpio_t clockPin, emac_rmii_clock_mode_t clockMode) {
  type = phyType;
  this->mdcPin = mdcPin;
  this->mdioPin = mdioPin;
  this->powerPin = powerPin;
  this->clockPin = clockPin;
  this->clockMode = clockMode;
}

esp_eth_mac_t* EMACDriver::newMAC() {

  eth_esp32_emac_config_t mac_config = ETH_ESP32_EMAC_DEFAULT_CONFIG();
  mac_config.clock_config.rmii.clock_mode = clockMode;
  mac_config.clock_config.rmii.clock_gpio = clockPin;
  mac_config.smi_mdc_gpio_num = digitalPinToGPIONumber(mdcPin);
  mac_config.smi_mdio_gpio_num = digitalPinToGPIONumber(mdioPin);

  eth_mac_config_t eth_mac_config = ETH_MAC_DEFAULT_CONFIG();
  eth_mac_config.sw_reset_timeout_ms = 1000;

  return esp_eth_mac_new_esp32(&mac_config, &eth_mac_config);
}

esp_eth_phy_t* EMACDriver::newPHY() {
  eth_phy_config_t phy_config = ETH_PHY_DEFAULT_CONFIG();
  phy_config.phy_addr = phyAddr;
  phy_config.reset_gpio_num = digitalPinToGPIONumber(powerPin);

  switch (type) {
  case ETH_PHY_LAN8720:
    return esp_eth_phy_new_lan87xx(&phy_config);
  case ETH_PHY_TLK110:
    return esp_eth_phy_new_ip101(&phy_config);
  case ETH_PHY_RTL8201:
    return esp_eth_phy_new_rtl8201(&phy_config);
  case ETH_PHY_DP83848:
    return esp_eth_phy_new_dp83848(&phy_config);
  case ETH_PHY_KSZ80XX:
    return esp_eth_phy_new_ksz80xx(&phy_config);
  default:
    log_e("Unsupported PHY %d", type);
    return NULL;
  }
}

#endif
