/*
  my_machine.h - machine-level feature configuration

  Target machine : Genmitsu 3020 Pro Max V2 (CNC milling, PCB routing)
  Target board   : BIGTREETECH Octopus Pro v1.1 (STM32H723ZET6)

  This file is honored by the build system because OVERRIDE_MY_MACHINE
  is NOT defined in platformio.ini. Do not add that flag or this file
  will be silently ignored and all features below will be disabled.

  Part of grblHAL
  Copyright (c) 2021-2025 Terje Io
  Copyright (c) 2025 zinoalex (3020Max adaptation)

  grblHAL is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the
  Free Software Foundation, either version 3 of the License, or (at your
  option) any later version.
*/

// -------------------------------------------------------------------------------
// Board selection
// -------------------------------------------------------------------------------
// BOARD_BTT_OCTOPUS_PRO is set via -D flag in platformio.ini so that
// the correct board map (boards/btt_octopus_pro_map.h) is always picked
// for this repo. Do NOT uncomment a different board here.
// -------------------------------------------------------------------------------
//#define BOARD_BTT_SKR_30
//#define BOARD_BTT_OCTOPUS_MAX
//#define BOARD_WEACT_MINI_H743
//#define BOARD_REFERENCE
//#define BOARD_MY_MACHINE

// Nucleo dev-kit shim — kept from upstream, not relevant for Octopus Pro
#if defined(NUCLEO_H743) || defined(NUCLEO_H723)
#define IS_NUCLEO_DEVKIT 1
#else
#define IS_NUCLEO_DEVKIT 0
#endif

// -------------------------------------------------------------------------------
// USB CDC
// -------------------------------------------------------------------------------
// USB_SERIAL_CDC=1 is injected by the [usb_h723] section in platformio.ini.
// Do not redefine here.
// -------------------------------------------------------------------------------
#if !IS_NUCLEO_DEVKIT && !defined(USB_SERIAL_CDC)
//#define USB_SERIAL_CDC          1
#endif

// -------------------------------------------------------------------------------
// Spindle plugin selection
// -------------------------------------------------------------------------------
// Current spindle: 48 V DC brushed motor, ON/OFF only, via SSR-40DD relay.
// The default grblHAL driver spindle handles pure ON/OFF on SPINDLE_ENABLE
// (mapped to FAN4 / PD14 in the board map). No PWM plugin is needed.
//
// Future options:
//   PWM-capable motor driver or laser module:
//     #define SPINDLE0_ENABLE   SPINDLE_PWM0
//   Huanyang VFD via Modbus RS-485:
//     #define SPINDLE0_ENABLE   SPINDLE_HUANYANG1
//     #define MODBUS_ENABLE     1
// -------------------------------------------------------------------------------
//#define SPINDLE0_ENABLE         SPINDLE_PWM0
//#define SPINDLE1_ENABLE         SPINDLE_NONE
//#define SPINDLE2_ENABLE         SPINDLE_NONE
//#define SPINDLE3_ENABLE         SPINDLE_NONE
//#define SPINDLE_OFFSET          1

// -------------------------------------------------------------------------------
// WiFi WebUI via ESP3D (Wemos D1 Mini on USART3)
// -------------------------------------------------------------------------------
// A Wemos D1 Mini flashed with ESP3D 3.x is wired to USART3 of the Octopus Pro:
//   Octopus TX (PD8) -> Wemos RX
//   Octopus RX (PD9) -> Wemos TX
//   GND              -> GND
//   5 V breakout     -> Wemos 5 V
//
// Default baud rate on both sides: 115200 — must match ESP3D serial settings.
//
// WEBUI_ENABLE=3  : enable the ESP3D-WEBUI plugin (serial/UART bridge mode)
// SDCARD_ENABLE=1 : required — WebUI serves files from the SD card
// SERIAL2_ENABLE=1: opens USART3 in the grblHAL HAL layer
// -------------------------------------------------------------------------------
#define WEBUI_ENABLE            3
#define SDCARD_ENABLE           1
#define SERIAL2_ENABLE          1

// Enable authentication once the WebUI is up and running, then set credentials
// through the ESP3D web interface or serial commands.
//#define WEBUI_AUTH_ENABLE       1

// Store WebUI index files in MCU flash instead of SD card (saves SD space but
// requires a rebuild every time WebUI is updated).
//#define WEBUI_INFLASH           1

// -------------------------------------------------------------------------------
// Onboard Ethernet (NOT available on Octopus Pro)
// -------------------------------------------------------------------------------
// The Octopus Pro has no Ethernet PHY. Network access is provided exclusively
// by the external Wemos D1 Mini running ESP3D (see WEBUI section above).
// Do NOT enable ETHERNET_ENABLE — it will compile but produce dead code.
// -------------------------------------------------------------------------------
//#define ETHERNET_ENABLE         1
//#define _WIZCHIP_            5500


// -------------------------------------------------------------------------------
// Optional feature plugins
// -------------------------------------------------------------------------------
//#define MPG_ENABLE              1   // MPG / jog pendant over serial
//#define KEYPAD_ENABLE           1   // I2C keypad
//#define DISPLAY_ENABLE          1   // SPI or I2C display
//#define ODOMETER_ENABLE         1   // tool-life / distance tracking
//#define PPI_ENABLE              1   // Pulse-Per-Inch mode (laser)
//#define LASER_COOLANT_ENABLE    1
//#define LASER_OVD_ENABLE        1
//#define LB_CLUSTERS_ENABLE      1
//#define OPENPNP_ENABLE          1
#define FANS_ENABLE             1   // software M106/M107 fan control
//#define PLASMA_ENABLE           1

// -------------------------------------------------------------------------------
// Trinamic driver mode
// -------------------------------------------------------------------------------
// TRINAMIC_ENABLE=2209 is injected by platformio.ini.
// Only override here if you physically swap drivers to a different family.
// -------------------------------------------------------------------------------
//#define TRINAMIC_ENABLE      2130
//#define TRINAMIC_ENABLE      5160
//#define TRINAMIC_R_SENSE      110
//#define TRINAMIC_I2C            1
//#define TRINAMIC_DEV            1

// -------------------------------------------------------------------------------
// Non-volatile storage
// -------------------------------------------------------------------------------
// EEPROM_ENABLE=32 is injected by platformio.ini (emulated in internal flash).
// No external EEPROM chip is required.
// -------------------------------------------------------------------------------
//#define EEPROM_ENABLE          16
//#define EEPROM_IS_FRAM          1

// -------------------------------------------------------------------------------
// Safety and control signals
// -------------------------------------------------------------------------------
//#define ESTOP_ENABLE            0
//#define RGB_LED_ENABLE          2
//#define PWM_SERVO_ENABLE        1
//#define BLTOUCH_ENABLE          1
//#define EVENTOUT_ENABLE         1
//#define ESP_AT_ENABLE           1
//#define FEED_OVERRIDE_ENABLE    1
//#define HOMING_PULLOFF_ENABLE   1

// -------------------------------------------------------------------------------
// IO expanders
// -------------------------------------------------------------------------------
//#define MCP3221_ENABLE          1
//#define PCA9654E_ENABLE         1

// -------------------------------------------------------------------------------
// Optional control signal inputs
// -------------------------------------------------------------------------------
// Enable PROBE_ENABLE once the tool-setter is physically wired to STOP5 (PG13).
// -------------------------------------------------------------------------------
//#define PROBE_ENABLE            0   // uncomment and set to 1 when probe is wired
//#define SAFETY_DOOR_ENABLE      1
//#define MOTOR_FAULT_ENABLE      1
//#define MOTOR_WARNING_ENABLE    1
//#define PROBE_DISCONNECT_ENABLE 1
//#define STOP_DISABLE_ENABLE     1
//#define BLOCK_DELETE_ENABLE     1
//#define SINGLE_BLOCK_ENABLE     1
//#define LIMITS_OVERRIDE_ENABLE  1

// -------------------------------------------------------------------------------
// Ganged / dual-motor axes
// -------------------------------------------------------------------------------
// The 3020 Pro Max V2 has a single motor per axis — leave these disabled.
// -------------------------------------------------------------------------------
//#define X_GANGED            1
//#define X_AUTO_SQUARE       1
//#define Y_GANGED            1
//#define Y_AUTO_SQUARE       1
//#define Z_GANGED            1
//#define Z_AUTO_SQUARE       1
//#define X_GANGED_LIM_MAX    1
//#define Y_GANGED_LIM_MAX    1
//#define Z_GANGED_LIM_MAX    1

// -------------------------------------------------------------------------------
// Network daemon configuration
// -------------------------------------------------------------------------------
// These daemons are automatically enabled when WEBUI_ENABLE is set.
// The network settings below only apply when ETHERNET_ENABLE is used
// (which it is not on this board). IP/hostname are handled by the Wemos/ESP3D.
// -------------------------------------------------------------------------------
#if ETHERNET_ENABLE || WEBUI_ENABLE
#define TELNET_ENABLE           1
#define WEBSOCKET_ENABLE        1
#define HTTP_ENABLE             1
//#define MDNS_ENABLE           1
//#define SSDP_ENABLE           1
//#define MQTT_ENABLE           1
#if SDCARD_ENABLE || WEBUI_ENABLE
#define FTP_ENABLE              1
//#define WEBDAV_ENABLE         1
#endif
//#define NETWORK_HOSTNAME        "grblHAL"
//#define NETWORK_IPMODE          1   // 0=static, 1=DHCP, 2=AutoIP
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
