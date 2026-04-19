/*
  btt_octopus_pro_map.h - Board map for BIGTREETECH Octopus Pro v1.1

  Adapted for: Genmitsu 3020 Pro Max V2 CNC
  Changes vs. upstream dresco/STM32H7xx:
    - M5_LIMIT (STOP5 / PG13) repurposed as PROBE input (12 V tolerant, opto-isolated)
    - COOLANT_FLOOD/MIST mapped to HE0/HE1 (high-current MOSFET outputs) for future use
    - Board cooling fan wired directly to a 12 V breakout pin (always-on, no SW control)
    - Laser TTL pin (PB6 / TIM4_CH1) intentionally left unassigned — see "Adding laser"
      section in README.md
    - TMC2209 UART pin assignments retained but flagged as untested by upstream

  Part of grblHAL
  Copyright (c) 2024 Joe Corelli
  Copyright (c) 2024 Jon Escombe
  Copyright (c) 2025 Michael Griffin
  Copyright (c) 2025 zinoalex (3020Max adaptation)

  grblHAL is free software: you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free Software
  Foundation, either version 3 of the License, or (at your option) any later
  version.

  grblHAL is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with
  grblHAL. If not, see <http://www.gnu.org/licenses/>.
*/

#if N_ABC_MOTORS > 8
#error "Octopus Pro v1.1 board map is only configured for a maximum of 8 motors."
#endif

#if !(defined(STM32H723xx)) || HSE_VALUE != 25000000
#error "This board requires STM32H723 with a 25 MHz crystal. Check your build target."
#endif

#define BOARD_NAME "BTT Octopus Pro v1.1 (3020Max CNC)"
#define BOARD_URL  "https://github.com/zinoalex/grblHAL-OctopusPro-3020Max"

// -------------------------------------------------------------------------------
// Serial / bus port assignment
// -------------------------------------------------------------------------------
// SERIAL_PORT  1  — USART1: TX=PA9,  RX=PA10  (TFT connector J70)
//                   When USB_SERIAL_CDC=1 (our build) the USB-C port takes over
//                   as the primary console and USART1 becomes an auxiliary UART.
//
// SERIAL1_PORT 21 — USART2: TX=PD5,  RX=PD6   (onboard ESP-12S / J26 RPi header)
//                   Not used in this build; left mapped for future reference.
//
// SERIAL2_PORT 32 — USART3: TX=PD8,  RX=PD9   (Wemos D1 Mini running ESP3D)
//                   Wemos RX -> PD8, Wemos TX -> PD9.
//
// I2C_PORT     1  — I2C1:   SCL=PB8, SDA=PB9  (I2C expansion header J73)
// SPI_PORT     1  — SPI1:   SCK=PA5, MISO=PA6, MOSI=PA7
// -------------------------------------------------------------------------------
#define SERIAL_PORT                 1       // USART1: TX=PA9,  RX=PA10
#define SERIAL1_PORT                21      // USART2: TX=PD5,  RX=PD6  (unused)
#define SERIAL2_PORT                32      // USART3: TX=PD8,  RX=PD9  (ESP3D)
#define I2C_PORT                    1       // I2C1:   SCL=PB8, SDA=PB9
#define SPI_PORT                    1       // SPI1:   SCK=PA5, MISO=PA6, MOSI=PA7

// Alternate SPI on PB3/PB4/PB5 — only one SPI can be active at a time.
//#define SPI_PORT                  12

//#define TRINAMIC_SOFT_SPI

// -------------------------------------------------------------------------------
// Motor slot to axis mapping
// -------------------------------------------------------------------------------
// MOTOR0  -> X axis
// MOTOR1  -> Y axis
// MOTOR2_1 -> Z axis  (MOTOR2_2 left empty; parallel Z not required)
// MOTOR3..7 -> spare  (future: 4th rotary axis, laser-Z slide, etc.)
// -------------------------------------------------------------------------------

// --- X axis (MOTOR0) ---
#define X_STEP_PORT                 GPIOF
#define X_STEP_PIN                  13
#define X_DIRECTION_PORT            GPIOF
#define X_DIRECTION_PIN             12
#define X_ENABLE_PORT               GPIOF
#define X_ENABLE_PIN                14
#define X_LIMIT_PORT                GPIOG
#define X_LIMIT_PIN                 6       // STOP0 (connector J27)

// --- Y axis (MOTOR1) ---
#define Y_STEP_PORT                 GPIOG
#define Y_STEP_PIN                  0
#define Y_DIRECTION_PORT            GPIOG
#define Y_DIRECTION_PIN             1
#define Y_ENABLE_PORT               GPIOF
#define Y_ENABLE_PIN                15
#define Y_LIMIT_PORT                GPIOG
#define Y_LIMIT_PIN                 9       // STOP1 (connector J29)

// --- Z axis (MOTOR2_1) ---
#define Z_STEP_PORT                 GPIOF
#define Z_STEP_PIN                  11
#define Z_DIRECTION_PORT            GPIOG
#define Z_DIRECTION_PIN             3
#define Z_ENABLE_PORT               GPIOG
#define Z_ENABLE_PIN                5
#define Z_LIMIT_PORT                GPIOG
#define Z_LIMIT_PIN                 10      // STOP2 (connector J31)

#define STEP_OUTMODE                GPIO_SINGLE
#define DIRECTION_OUTMODE           GPIO_SINGLE
#define LIMIT_INMODE                GPIO_SINGLE

// -------------------------------------------------------------------------------
// Spare motor slots (declared for completeness; not used in a 3-axis CNC build)
// -------------------------------------------------------------------------------
#if N_ABC_MOTORS > 0
#define M3_AVAILABLE                        // MOTOR3
#define M3_STEP_PORT                GPIOG
#define M3_STEP_PIN                 4
#define M3_DIRECTION_PORT           GPIOC
#define M3_DIRECTION_PIN            1
#define M3_LIMIT_PORT               GPIOG
#define M3_LIMIT_PIN                11      // STOP3
#define M3_ENABLE_PORT              GPIOA
#define M3_ENABLE_PIN               2
#endif

#if N_ABC_MOTORS > 1
#define M4_AVAILABLE                        // MOTOR4
#define M4_STEP_PORT                GPIOF
#define M4_STEP_PIN                 9
#define M4_DIRECTION_PORT           GPIOF
#define M4_DIRECTION_PIN            10
#define M4_LIMIT_PORT               GPIOG
#define M4_LIMIT_PIN                12      // STOP4
#define M4_ENABLE_PORT              GPIOG
#define M4_ENABLE_PIN               2
#endif

#if N_ABC_MOTORS > 2
#define M5_AVAILABLE                        // MOTOR5
#define M5_STEP_PORT                GPIOC
#define M5_STEP_PIN                 13
#define M5_DIRECTION_PORT           GPIOF
#define M5_DIRECTION_PIN            0
// M5_LIMIT intentionally not defined.
// STOP5 (PG13) is repurposed as the PROBE input (AUXINPUT1).
// If you need M5 limit, move the probe to another STOP pin and restore this.
//#define M5_LIMIT_PORT               GPIOG
//#define M5_LIMIT_PIN                13
#define M5_ENABLE_PORT              GPIOF
#define M5_ENABLE_PIN               1
#endif

#if N_ABC_MOTORS > 3
#define M6_AVAILABLE                        // MOTOR6
#define M6_STEP_PORT                GPIOE
#define M6_STEP_PIN                 2
#define M6_DIRECTION_PORT           GPIOE
#define M6_DIRECTION_PIN            3
#define M6_LIMIT_PORT               GPIOG
#define M6_LIMIT_PIN                14      // STOP6
#define M6_ENABLE_PORT              GPIOD
#define M6_ENABLE_PIN               4
#endif

#if N_ABC_MOTORS > 4
#define M7_AVAILABLE                        // MOTOR7
#define M7_STEP_PORT                GPIOE
#define M7_STEP_PIN                 6
#define M7_DIRECTION_PORT           GPIOA
#define M7_DIRECTION_PIN            14
#define M7_LIMIT_PORT               GPIOG
#define M7_LIMIT_PIN                15      // STOP7
#define M7_ENABLE_PORT              GPIOE
#define M7_ENABLE_PIN               0
#endif

// -------------------------------------------------------------------------------
// Auxiliary outputs (FAN and heater connectors repurposed for CNC I/O)
// -------------------------------------------------------------------------------
// AUXOUTPUT0 FAN0 PA8  TIM1_CH1 — RESERVED for future laser PWM output (5 V TTL)
// AUXOUTPUT1 FAN1 PE5           — free
// AUXOUTPUT2 FAN2 PD12          — free
// AUXOUTPUT3 FAN3 PD13          — free
// AUXOUTPUT4 FAN4 PD14          — SPINDLE_ENABLE -> drives SSR-40DD control input
// AUXOUTPUT5 FAN5 PD15          — SPINDLE_DIR    (unused; spindle is ON/OFF only)
// AUXOUTPUT6 HE0  PA0           — COOLANT_FLOOD  (high-current MOSFET, for future pump)
// AUXOUTPUT7 HE1  PA3           — COOLANT_MIST   (high-current MOSFET, for future valve)
//
// Board cooling fan: wire DIRECTLY to a 12 V breakout pin on the board.
// This keeps the fan always-on whenever the board is powered, without
// consuming a PWM output or requiring software control.
// -------------------------------------------------------------------------------
#define AUXOUTPUT0_PORT             GPIOA   // FAN0 (PA8) — reserved, do not assign
#define AUXOUTPUT0_PIN              8

#define AUXOUTPUT1_PORT             GPIOE   // FAN1
#define AUXOUTPUT1_PIN              5

#define AUXOUTPUT2_PORT             GPIOD   // FAN2
#define AUXOUTPUT2_PIN              12

#define AUXOUTPUT3_PORT             GPIOD   // FAN3
#define AUXOUTPUT3_PIN              13

#define AUXOUTPUT4_PORT             GPIOD   // FAN4 — spindle enable (SSR-40DD)
#define AUXOUTPUT4_PIN              14

#define AUXOUTPUT5_PORT             GPIOD   // FAN5 — spindle direction (unused)
#define AUXOUTPUT5_PIN              15

#define AUXOUTPUT6_PORT             GPIOA   // HE0 — high-current output
#define AUXOUTPUT6_PIN              0

#define AUXOUTPUT7_PORT             GPIOA   // HE1 — high-current output
#define AUXOUTPUT7_PIN              3

// -------------------------------------------------------------------------------
// Spindle pin binding
// -------------------------------------------------------------------------------
// Current setup: 48 V DC brushed spindle, ON/OFF only, driven via SSR-40DD.
// The SSR control input (3-32 V DC) is fed from FAN4 with its voltage-select
// jumper set to 24 V.
// No PWM, no direction reversal needed.
//
// To add a PWM-capable spindle or laser later:
//   - Define AUXOUTPUT8 pointing to PB6 (see the free-pin note at the end)
//   - Route SPINDLE_PWM_PORT/PIN to AUXOUTPUT8
//   - Set SPINDLE0_ENABLE=SPINDLE_PWM0 in my_machine.h and rebuild
// -------------------------------------------------------------------------------
#if DRIVER_SPINDLE_ENABLE & SPINDLE_ENA
#define SPINDLE_ENABLE_PORT         AUXOUTPUT4_PORT
#define SPINDLE_ENABLE_PIN          AUXOUTPUT4_PIN
#endif

#if DRIVER_SPINDLE_ENABLE & SPINDLE_PWM
// If PWM spindle or laser is later enabled, it will land on AUXOUTPUT0 (PA8,
// TIM1_CH1) for a hardware PWM signal — no bit-banging.
#define SPINDLE_PWM_PORT            AUXOUTPUT0_PORT
#define SPINDLE_PWM_PIN             AUXOUTPUT0_PIN
#endif

#if DRIVER_SPINDLE_ENABLE & SPINDLE_DIR
#define SPINDLE_DIRECTION_PORT      AUXOUTPUT5_PORT
#define SPINDLE_DIRECTION_PIN       AUXOUTPUT5_PIN
#endif

// -------------------------------------------------------------------------------
// Coolant outputs
// -------------------------------------------------------------------------------
// Not wired on this build. Definitions kept so that M7/M8/M9 G-code commands
// compile and map to the high-current MOSFET outputs (HE0/HE1) for future use
// (e.g. coolant pump or air-blast solenoid). Enable COOLANT_ENABLE in
// my_machine.h once you wire something up.
// -------------------------------------------------------------------------------
#if COOLANT_ENABLE & COOLANT_FLOOD
#define COOLANT_FLOOD_PORT          AUXOUTPUT6_PORT
#define COOLANT_FLOOD_PIN           AUXOUTPUT6_PIN
#endif
#if COOLANT_ENABLE & COOLANT_MIST
#define COOLANT_MIST_PORT           AUXOUTPUT7_PORT
#define COOLANT_MIST_PIN            AUXOUTPUT7_PIN
#endif

// -------------------------------------------------------------------------------
// Auxiliary inputs
// -------------------------------------------------------------------------------
// AUXINPUT0 PC0  — PWR-DET connector (free; could be used as safety door)
// AUXINPUT1 PG13 — STOP5 -> PROBE input (opto-isolated, 12 V tolerant via EL357C)
// AUXINPUT2 PB7  — PROBE connector right pin (secondary probe or reference)
// AUXINPUT3 PB2  — onboard pushbutton
// AUXINPUT4 PF3  — thermistor TB connector (free; rewired as reset if desired)
// AUXINPUT5 PF4  — thermistor T0 connector (free; rewired as feed-hold if desired)
// AUXINPUT6 PF5  — thermistor T1 connector (free; rewired as cycle-start if desired)
// -------------------------------------------------------------------------------
#define AUXINPUT0_PORT              GPIOC
#define AUXINPUT0_PIN               0

// Probe input on STOP5 — 12 V tolerant through the onboard opto-coupler path.
// Wiring: one alligator clip on the tool shank (through collet), the other on
// a metal reference plate on the work surface. Continuity triggers the probe.
#define AUXINPUT1_PORT              GPIOG
#define AUXINPUT1_PIN               13

#define AUXINPUT2_PORT              GPIOB
#define AUXINPUT2_PIN               7
#define AUXINPUT3_PORT              GPIOB
#define AUXINPUT3_PIN               2
#define AUXINPUT4_PORT              GPIOF
#define AUXINPUT4_PIN               3
#define AUXINPUT5_PORT              GPIOF
#define AUXINPUT5_PIN               4
#define AUXINPUT6_PORT              GPIOF
#define AUXINPUT6_PIN               5

// Analog inputs (thermistor pads reusable as ADC channels if needed)
#define AUXINPUT0_ANALOG_PORT       GPIOF   // T2
#define AUXINPUT0_ANALOG_PIN        6
#define AUXINPUT1_ANALOG_PORT       GPIOF   // T3
#define AUXINPUT1_ANALOG_PIN        7

// -------------------------------------------------------------------------------
// Control signal bindings (enable in my_machine.h if physically wired)
// -------------------------------------------------------------------------------
#if CONTROL_ENABLE & CONTROL_HALT
#define RESET_PORT                  AUXINPUT4_PORT
#define RESET_PIN                   AUXINPUT4_PIN
#endif
#if CONTROL_ENABLE & CONTROL_FEED_HOLD
#define FEED_HOLD_PORT              AUXINPUT5_PORT
#define FEED_HOLD_PIN               AUXINPUT5_PIN
#endif
#if CONTROL_ENABLE & CONTROL_CYCLE_START
#define CYCLE_START_PORT            AUXINPUT6_PORT
#define CYCLE_START_PIN             AUXINPUT6_PIN
#endif

#if SAFETY_DOOR_ENABLE
#define SAFETY_DOOR_PORT            AUXINPUT0_PORT
#define SAFETY_DOOR_PIN             AUXINPUT0_PIN
#endif

#if PROBE_ENABLE
#define PROBE_PORT                  AUXINPUT1_PORT
#define PROBE_PIN                   AUXINPUT1_PIN
#endif

// -------------------------------------------------------------------------------
// TMC2209 UART-mode pin mapping
// -------------------------------------------------------------------------------
// Each TMC2209 exposes a single-wire UART pin on the driver socket "MS3" row.
// The physical jumpers under each driver must be configured for UART mode as
// described in the BTT Octopus Pro manual (section 3.2).
//
// NOTE: upstream marks this path "Not tested, use with care". If you experience
// communication errors at startup ($338 / M122), refer to the Troubleshooting
// section in README.md. The SPI fallback paths are retained below.
// -------------------------------------------------------------------------------
#if TRINAMIC_UART_ENABLE

#define MOTOR_UARTX_PORT            GPIOC
#define MOTOR_UARTX_PIN             4
#define MOTOR_UARTY_PORT            GPIOD
#define MOTOR_UARTY_PIN             11
#define MOTOR_UARTZ_PORT            GPIOC
#define MOTOR_UARTZ_PIN             6

#ifdef  M3_AVAILABLE
#define MOTOR_UARTM3_PORT           GPIOC
#define MOTOR_UARTM3_PIN            7
#endif
#ifdef  M4_AVAILABLE
#define MOTOR_UARTM4_PORT           GPIOF
#define MOTOR_UARTM4_PIN            2
#endif
#ifdef  M5_AVAILABLE
#define MOTOR_UARTM5_PORT           GPIOE
#define MOTOR_UARTM5_PIN            4
#endif
#ifdef  M6_AVAILABLE
#define MOTOR_UARTM6_PORT           GPIOE
#define MOTOR_UARTM6_PIN            1
#endif
#ifdef  M7_AVAILABLE
#define MOTOR_UARTM7_PORT           GPIOD
#define MOTOR_UARTM7_PIN            3
#endif

#elif TRINAMIC_SPI_ENABLE

#ifdef TRINAMIC_SOFT_SPI
#define TRINAMIC_MOSI_PORT          GPIOA
#define TRINAMIC_MOSI_PIN           7
#define TRINAMIC_SCK_PORT           GPIOA
#define TRINAMIC_SCK_PIN            5
#define TRINAMIC_MISO_PORT          GPIOA
#define TRINAMIC_MISO_PIN           6
#endif

#define MOTOR_CSX_PORT              GPIOC
#define MOTOR_CSX_PIN               4
#define MOTOR_CSY_PORT              GPIOD
#define MOTOR_CSY_PIN               11
#define MOTOR_CSZ_PORT              GPIOC
#define MOTOR_CSZ_PIN               6

#ifdef  M3_AVAILABLE
#define MOTOR_CSM3_PORT             GPIOC
#define MOTOR_CSM3_PIN              7
#endif
#ifdef  M4_AVAILABLE
#define MOTOR_CSM4_PORT             GPIOF
#define MOTOR_CSM4_PIN              2
#endif
#ifdef  M5_AVAILABLE
#define MOTOR_CSM5_PORT             GPIOE
#define MOTOR_CSM5_PIN              4
#endif
#ifdef  M6_AVAILABLE
#define MOTOR_CSM6_PORT             GPIOE
#define MOTOR_CSM6_PIN              1
#endif
#ifdef  M7_AVAILABLE
#define MOTOR_CSM7_PORT             GPIOD
#define MOTOR_CSM7_PIN              3
#endif

#endif  // TRINAMIC_UART_ENABLE / TRINAMIC_SPI_ENABLE

// -------------------------------------------------------------------------------
// CAN bus (onboard RJ11 connector) — reserved for future toolhead expansion
// -------------------------------------------------------------------------------
#define CAN_PORT                    GPIOD
#define CAN_RX_PIN                  0
#define CAN_TX_PIN                  1

// -------------------------------------------------------------------------------
// Free pin: PB6 (PROBE connector, left pin)
// -------------------------------------------------------------------------------
// PB6 is the left signal pin of the dedicated PROBE/BLTouch connector (J43).
// It is NOT opto-isolated (unlike PG13 / AUXINPUT1).
// It supports hardware PWM via TIM4_CH1 on the STM32H723.
//
// Reserved use: 5 V TTL laser modulation input.
//
// To enable (see README "Adding a laser" for full procedure):
//   1. Add here:
//        #define AUXOUTPUT8_PORT   GPIOB
//        #define AUXOUTPUT8_PIN    6
//   2. Redirect the SPINDLE_PWM guard to AUXOUTPUT8
//   3. Set SPINDLE0_ENABLE=SPINDLE_PWM0 in my_machine.h
//   4. Rebuild and set $32=1 (laser mode) in ioSender
// -------------------------------------------------------------------------------

// EOF
