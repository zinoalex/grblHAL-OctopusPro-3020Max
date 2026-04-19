/*
  my_machine.h - configuration for STM32H7xx processors

  Part of grblHAL

  Copyright (c) 2021-2025 Terje Io

  grblHAL is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  grblHAL is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with grblHAL. If not, see <http://www.gnu.org/licenses/>.
*/

// NOTE: Only one board may be enabled!
// If none is enabled pin mappings from generic_map.h will be used.
//#define BOARD_PROTONEER_3XX   // For use with a Nucleo-F756ZG board.
//#define BOARD_GENERIC_UNO     // For use with a Nucleo-F756ZG board.
//#define BOARD_BTT_SKR_30      // BTT SKR V3 board.
//#define BOARD_BTT_OCTOPUS_MAX // BTT Octopus Max board.
//#define BOARD_WEACT_MINI_H743 // WeAct MiniSTM32H743 board.
//#define BOARD_REFERENCE       // grblHAL reference board map.
//#define BOARD_MY_MACHINE      // Add my_machine_map.h before enabling this!

#if defined(NUCLEO_H743) || defined(NUCLEO_H723)
#define IS_NUCLEO_DEVKIT 1
#else
#define IS_NUCLEO_DEVKIT 0
#endif

// Configuration
// Uncomment to enable.

#if !IS_NUCLEO_DEVKIT && !defined(USB_SERIAL_CDC)
//#define USB_SERIAL_CDC          1 // Serial communication via native USB.
#endif

// Spindle selection:
//#define SPINDLE0_ENABLE         SPINDLE_HUANYANG1
//#define SPINDLE1_ENABLE         SPINDLE_PWM0
//#define SPINDLE2_ENABLE         SPINDLE_NONE
//#define SPINDLE3_ENABLE         SPINDLE_NONE
//#define SPINDLE_OFFSET          1

// **********************
// WiFi via ESP3D-WEBUI (Wemos D1 Mini on USART3: TX=PD8, RX=PD9)
#define WEBUI_ENABLE            3 // ESP3D-WEBUI plugin via UART-connected ESP8266
#define SDCARD_ENABLE           1 // Required by WEBUI
#define SERIAL2_ENABLE          1 // Enable USART3 for Wemos D1 Mini

//#define MODBUS_ENABLE           1
//#define WEBUI_AUTH_ENABLE       1 // Enable ESP3D-WEBUI authentication.
//#define WEBUI_INFLASH           1 // Store WebUI files in flash instead of on SD card.
//#define ETHERNET_ENABLE         1
//#define _WIZCHIP_            5500
//#define BLUETOOTH_ENABLE        2
//#define MPG_ENABLE              1
//#define KEYPAD_ENABLE           1
//#define DISPLAY_ENABLE          1
//#define ODOMETER_ENABLE         1
//#define PPI_ENABLE              1
//#define LASER_COOLANT_ENABLE    1
//#define LASER_OVD_ENABLE        1
//#define LB_CLUSTERS_ENABLE      1
//#define OPENPNP_ENABLE          1
//#define FANS_ENABLE             1
//#define EMBROIDERY_ENABLE       1
//#define PLASMA_ENABLE           1
//#define TRINAMIC_ENABLE      2130
//#define TRINAMIC_ENABLE      5160
//#define TRINAMIC_R_SENSE      110
//#define TRINAMIC_I2C            1
//#define TRINAMIC_DEV            1
//#define EEPROM_ENABLE          16
//#define EEPROM_IS_FRAM          1
//#define ESTOP_ENABLE            0
//#define RGB_LED_ENABLE          2
//#define PWM_SERVO_ENABLE        1
//#define BLTOUCH_ENABLE          1
//#define EVENTOUT_ENABLE         1
//#define ESP_AT_ENABLE           1
//#define FEED_OVERRIDE_ENABLE    1
//#define HOMING_PULLOFF_ENABLE   1

// IO expanders:
//#define MCP3221_ENABLE          1
//#define PCA9654E_ENABLE         1

// Optional control signals:
//#define PROBE_ENABLE            0
//#define SAFETY_DOOR_ENABLE      1
//#define MOTOR_FAULT_ENABLE      1
//#define MOTOR_WARNING_ENABLE    1
//#define PROBE_DISCONNECT_ENABLE 1
//#define STOP_DISABLE_ENABLE     1
//#define BLOCK_DELETE_ENABLE     1
//#define SINGLE_BLOCK_ENABLE     1
//#define LIMITS_OVERRIDE_ENABLE  1

/**/

// Ganged axes:
//#define X_GANGED            1
//#define X_AUTO_SQUARE       1
//#define Y_GANGED            1
//#define Y_AUTO_SQUARE       1
//#define Z_GANGED            1
//#define Z_AUTO_SQUARE       1
//#define X_GANGED_LIM_MAX    1
//#define Y_GANGED_LIM_MAX    1
//#define Z_GANGED_LIM_MAX    1

#if ETHERNET_ENABLE || WEBUI_ENABLE
#define TELNET_ENABLE           1 // Telnet daemon
#define WEBSOCKET_ENABLE        1 // Websocket daemon
#define HTTP_ENABLE             1 // HTTP daemon
//#define MDNS_ENABLE           1
//#define SSDP_ENABLE           1
//#define MQTT_ENABLE           1
#if SDCARD_ENABLE || WEBUI_ENABLE
#define FTP_ENABLE              1 // FTP daemon
//#define WEBDAV_ENABLE         1
#endif

// Network settings (defaults shown, uncomment to override):
//#define NETWORK_HOSTNAME        "grblHAL"
//#define NETWORK_IPMODE          1 // 0 = static, 1 = DHCP, 2 = AutoIP
//#define NETWORK_IP              "192.168.5.1"
//#define NETWORK_GATEWAY         "192.168.5.1"
//#define NETWORK_MASK            "255.255.255.0"
//#define NETWORK_FTP_PORT        21
//#define NETWORK_TELNET_PORT     23
//#define NETWORK_HTTP_PORT       80
#if HTTP_ENABLE
//#define NETWORK_WEBSOCKET_PORT  81
#else
//#define NETWORK_WEBSOCKET_PORT  80
#endif
#endif
