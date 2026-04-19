# grblHAL for BTT Octopus Pro v1.1 — Genmitsu 3020 Pro Max V2

> Custom grblHAL firmware for driving a Genmitsu 3020 Pro Max V2 CNC router
> with a BIGTREETECH Octopus Pro v1.1 (STM32H723ZET6), TMC2209 UART stepper
> drivers, 48 V spindle via SSR-40DD, opto-isolated probe input, WiFi WebUI
> via Wemos D1 Mini (ESP3D), and USB-C serial console for ioSender.
>
> Forked from [dresco/STM32H7xx](https://github.com/dresco/STM32H7xx).

---

## Table of Contents

1. [Setup overview](#setup-overview)
2. [Complete pin map](#complete-pin-map)
3. [Hardware wiring diagrams](#hardware-wiring-diagrams)
4. [Octopus Pro jumper settings](#octopus-pro-jumper-settings)
5. [Building the firmware](#building-the-firmware)
6. [Flashing the board](#flashing-the-board)
7. [ESP3D setup on the Wemos D1 Mini](#esp3d-setup-on-the-wemos-d1-mini)
8. [ioSender setup](#iosender-setup)
9. [Initial grblHAL settings](#initial-grblhal-settings)
10. [Adding a laser later](#adding-a-laser-later)
11. [Troubleshooting](#troubleshooting)

---

## Setup overview

| Component | Choice | Notes |
|---|---|---|
| Controller board | BTT Octopus Pro v1.1 H723 | STM32H723ZET6, 25 MHz XTAL, 128 KB BTT bootloader |
| Stepper drivers | TMC2209 UART × 3 | X/Y/Z on MOTOR0/1/2 — jumpers set to UART mode |
| Main power supply | 48 V DC PSU | Powers the spindle |
| Logic/motor supply | Buck 48 → 24 V DC | Powers the board and motor driver inputs |
| Spindle | 48 V DC brushed motor, ON/OFF | Switched by SSR-40DD (DC-DC solid state relay) from FAN4 |
| Limit switches | Genmitsu original mechanical microswitches | 2-wire (signal + GND), MCU internal pull-up enabled |
| Probe / tool setter | Continuity clip + metal reference plate | On STOP5 (PG13), opto-isolated, 12 V tolerant |
| Board cooling fan | 12 V direct wire | Connected to a 12 V breakout pin — always-on, no software control |
| PC console | USB-C → ioSender | Virtual COM port, 115200 baud |
| WiFi WebUI | Wemos D1 Mini + ESP3D | Wired to USART3 (PD8 / PD9) |
| Future | 5 V TTL laser module | PB6 (PROBE connector left pin) reserved — see [Adding a laser later](#adding-a-laser-later) |

### Intended use cases

- **CNC milling** — wood, aluminium, resins with DC spindle
- **PCB routing** — V-bit engraving with Z-probe for surface levelling and trace continuity
- **Laser engraving / cutting** (future, after adding a TTL 5 V laser module)

---

## Complete pin map

### Quick reference table

| Function | MCU pin | BTT connector | Notes |
|---|---|---|---|
| **X STEP / DIR / EN** | PF13 / PF12 / PF14 | MOTOR0 | |
| **Y STEP / DIR / EN** | PG0 / PG1 / PF15 | MOTOR1 | |
| **Z STEP / DIR / EN** | PF11 / PG3 / PG5 | MOTOR2_1 | MOTOR2_2 left empty |
| **X UART (TMC2209)** | PC4 | MOTOR0 MS3 pin | Single-wire; jumper in UART mode |
| **Y UART (TMC2209)** | PD11 | MOTOR1 MS3 pin | |
| **Z UART (TMC2209)** | PC6 | MOTOR2_1 MS3 pin | |
| **X LIMIT** | PG6 | STOP0 (J27) | Microswitch → GND |
| **Y LIMIT** | PG9 | STOP1 (J29) | Microswitch → GND |
| **Z LIMIT** | PG10 | STOP2 (J31) | Microswitch → GND |
| **PROBE** | PG13 | STOP5 | Opto-isolated, 12 V tolerant |
| **Spindle ENABLE (SSR)** | PD14 | FAN4 | Voltage-select jumper → 24 V |
| **Laser PWM** *(reserved)* | PB6 | PROBE left pin | TIM4_CH1 hardware PWM — unassigned |
| **Board cooling fan** | any 12 V pin | 12 V breakout | Hard-wired, NOT software controlled |
| **ESP3D TX / RX** (MCU side) | PD8 / PD9 | USART3 area | 115200 baud |
| **USB-C console** | PA11 / PA12 | USB-C | VCOM for ioSender |

### Block diagram

```
                     BIGTREETECH Octopus Pro v1.1
 ┌──────────────────────────────────────────────────────────────────┐
 │  MOTOR0 ─── TMC2209 ─── X axis  (STEP PF13, DIR PF12, EN PF14) │
 │  MOTOR1 ─── TMC2209 ─── Y axis  (STEP PG0,  DIR PG1,  EN PF15) │
 │  MOTOR2 ─── TMC2209 ─── Z axis  (STEP PF11, DIR PG3,  EN PG5)  │
 │  MOTOR3..7 ─ empty (reserved for future axes / modules)         │
 ├──────────────────────────────────────────────────────────────────┤
 │  STOP0 (PG6)  ─── X limit microswitch  [NO, common to GND]     │
 │  STOP1 (PG9)  ─── Y limit microswitch                          │
 │  STOP2 (PG10) ─── Z limit microswitch                          │
 │  STOP5 (PG13) ─── probe / tool-setter  (12 V tolerant)         │
 ├──────────────────────────────────────────────────────────────────┤
 │  FAN4  (PD14) ─── SSR-40DD ─── 48 V DC spindle                 │
 │  FAN0  (PA8)  ─── RESERVED  →  future laser PWM (TIM1_CH1)     │
 ├──────────────────────────────────────────────────────────────────┤
 │  USART3 PD8/PD9 ─── Wemos D1 Mini (ESP3D) ─── WiFi WebUI       │
 │  USB-C           ─── PC Windows ─── ioSender                   │
 └──────────────────────────────────────────────────────────────────┘
```

### Available expansion pins

| Pin | Connector | Suggested future use |
|---|---|---|
| PA8 (FAN0) | FAN0 | Laser PWM output (TIM1_CH1) |
| PE5 (FAN1) | FAN1 | Dust collector relay |
| PD12 (FAN2) | FAN2 | LED lighting |
| PD13 (FAN3) | FAN3 | Free |
| PD15 (FAN5) | FAN5 | Free |
| PA0 (HE0) | HE0 | Coolant pump (high-current MOSFET) |
| PA3 (HE1) | HE1 | Air-blast solenoid (high-current MOSFET) |
| PB6 | PROBE left pin | Laser TTL 5 V PWM (TIM4_CH1) |
| PB7 | PROBE right pin | Secondary probe or reference sensor |
| STOP3/4/6/7 | J33/J34/J46… | Additional sensors, BLTouch, etc. |

---

## Hardware wiring diagrams

### Power distribution

```
  AC mains ─── 48 V DC PSU ─┬──► Spindle 48 V  (through SSR-40DD)
                             │
                             └──► Buck 48→24 V ─┬──► Octopus MOTOR_POWER (+/-)
                                                │
                                                └──► Octopus POWER (+/-)

                                                     (BED_POWER not used)
```

**Key points:**
- **Common ground**: the 48 V PSU, buck converter output, and Octopus board GND
  must all be joined at a single point. Without a common GND the SSR control
  signal has no reference and the spindle will not switch.
- **Buck converter current rating**: size it for at least 5 A at 24 V (board
  draws ~1–2 A peak; each TMC2209 draws up to ~1 A under load).
- **Spindle cable gauge**: use at least AWG 16 / 1.5 mm² for the 48 V spindle
  wires if your motor draws 5–10 A.

### Spindle via SSR-40DD

```
  Octopus FAN4 connector:
       ┌─ VF4 (voltage-select jumper → 24 V) ─── (+) SSR control input  (3–32 V DC)
       └─ GND                                 ─── (−) SSR control input

  SSR-40DD load side:
       (+) 48 V ─────────────────────────────── (+) spindle motor
       (−) 48 V ── SSR internal switch ───────── (−) spindle motor
                   (open when SSR is OFF)
```

**G-code commands:**
- `M3 S1000` → spindle ON  (S-value is ignored; SSR is ON/OFF only)
- `M5` → spindle OFF

The SSR-40DD is a DC solid-state relay; it is **not designed for fast PWM
switching**. This is why the pin is configured as a pure digital enable with no
PWM in grblHAL. Do not attempt to use it as a PWM output.

### Mechanical limit switches

```
  STOP0 connector (J27):
       ┌─ 5 V    (leave unconnected)
       ├─ GND ───── common (COM) terminal of the microswitch
       └─ PG6 ───── NO (Normally Open) terminal of the microswitch

  Same wiring for STOP1 (Y / J29) and STOP2 (Z / J31).
```

grblHAL enables the **MCU internal pull-up** on each limit pin. At rest the
switch is open → pin is HIGH. When the switch is pressed it shorts to GND →
pin goes LOW → limit triggered.

If your Genmitsu microswitches are **NC** (Normally Closed, closed at rest),
invert the logic with `$5=7` in the ioSender console.

### Probe / tool setter

```
           ┌──── alligator clip on the tool shank (clipped to the collet)
           │
           ▼                              ┌── STOP5 connector:
   ┌──────────────┐                       │    5 V   (leave unconnected)
   │  end mill    │                       │    GND ── second alligator clip
   │     │        │                       │    PG13 ─ signal wire (second clip)
   └─────┼────────┘                       │
         │ touches                        │
   ──────▼────────── metal reference ─────┘
         plate on workpiece
```

When the spinning tool touches the metal plate, the circuit closes, PG13 is
pulled to GND, and grblHAL records a probe trigger.

The pin is **opto-isolated** via an onboard EL357C and tolerates up to 12 V
on the signal line without damage.

To activate the probe in firmware, uncomment `#define PROBE_ENABLE` (set to 1)
in `Inc/my_machine.h` and rebuild.

### Wemos D1 Mini (ESP3D WebUI)

```
   Wemos D1 Mini                    Octopus Pro
   ┌──────────────┐                 ┌──────────┐
   │  5 V ────────┼── 5 V ──────────┤ any 5 V breakout pin
   │  GND ────────┼── GND ──────────┤ GND
   │  TX  (GPIO1) ┼── wire ─────────┤ PD9 (USART3 RX)
   │  RX  (GPIO3) ┼── wire ─────────┤ PD8 (USART3 TX)
   └──────────────┘                 └──────────┘
```

Remember the **TX/RX crossover**: the transmit pin of one side connects to the
receive pin of the other.

The Wemos is powered from the board's 5 V rail, so it powers down together with
the Octopus — expected behaviour.

---

## Octopus Pro jumper settings

### Motor power voltage jumpers

Each motor slot (MOTOR0–MOTOR7) has a two-position jumper that selects whether
the driver is powered from **MOTOR_POWER** (high voltage, up to 60 V) or from
**POWER** (logic supply, typically 24 V).

> **For this build: set ALL motor slot jumpers to the RIGHT (POWER = 24 V).**

Do not use MOTOR_POWER unless you are intentionally running the motors above 24 V.
TMC2209 drivers support up to 29 V; running at 24 V gives better thermal headroom.

### FAN voltage-select jumpers

Each FAN0–FAN5 connector has a 3-position jumper: 5 V / 12 V / 24 V.

| Connector | Setting | Reason |
|---|---|---|
| FAN4 (spindle enable) | **24 V** | SSR-40DD control input accepts 3–32 V; 24 V gives the most reliable switching margin |
| FAN0 (future laser) | **5 V** | Set this when you wire a 5 V TTL laser module |
| All others | leave as-is or remove | Not wired in this build |

### PT100 / PT1000 DIP switch

Not relevant for CNC use. Leave in the factory default position.

### StallGuard / DIAG jumpers

These jumpers connect each TMC2209 DIAG pin to the corresponding limit-switch
input for sensorless homing. **We use physical microswitches, so leave all
DIAG jumpers unplugged.**

### MCU power jumper (USB-C)

A small jumper near the USB-C port allows the MCU to be powered from the USB
port alone. Useful **only** for DFU flashing without powering motors and spindle.

| Situation | Jumper state |
|---|---|
| Normal CNC operation | **Removed** — board powered from POWER input |
| DFU flash via USB-C only | **Inserted** — main power supply OFF |

---

## Building the firmware

Three options are available, in order of ease.

### Option A — Web builder (no installation required)

1. Open **http://svn.io-engineering.com:8080/**
2. Select processor: **STM32H7xx**
3. Select board: **BTT Octopus Pro (H723)**
4. Select driver: **TMC2209**
5. Enable plugins: **SD card**, **WebUI**, **EEPROM**
6. Under "Options" enable **Serial2** (for ESP3D on USART3)
7. Click **Generate** — download the `.zip` and extract `firmware.bin`

> **Limitation:** the web builder uses the upstream dresco board map, not your
> custom one (probe on STOP5, laser pin reserved on PB6). Use Option B for
> your personalised builds.

### Option B — PlatformIO (recommended for custom builds)

**Prerequisites** (install once on the PC):
1. [Visual Studio Code](https://code.visualstudio.com/)
2. **PlatformIO IDE** extension (from the VSCode marketplace)
3. [Git for Windows](https://git-scm.com/download/win) (includes Git Bash)

**Steps:**

```bash
# Clone the repo with all submodules (grblHAL core, plugins, etc.)
git clone --recursive https://github.com/zinoalex/grblHAL-OctopusPro-3020Max.git
cd grblHAL-OctopusPro-3020Max

# Open in VSCode
code .
```

Inside VSCode:
- Click the **PlatformIO ant icon** in the left sidebar
- Go to **Project Tasks → octopus_pro_3020max → General → Build**

Or from Git Bash / terminal:

```bash
pio run -e octopus_pro_3020max
```

Output binary:
```
.pio/build/octopus_pro_3020max/firmware.bin
```

### Option C — GitHub Actions (cloud build, no local tools needed)

Every push to `master` automatically triggers a PlatformIO build in the cloud.

To download the compiled firmware:
1. Go to the repo on GitHub
2. Click the **Actions** tab
3. Select the most recent run with a green checkmark
4. Scroll to the **Artifacts** section at the bottom of the run page
5. Download the `.zip` — it contains `firmware.bin` ready to flash

---

## Flashing the board

### Method 1 — SD card (recommended)

The BTT 128 KB bootloader auto-flashes `firmware.bin` from a microSD at startup.

```
1. Format a microSD card (≤32 GB) as FAT32 using the official SD formatter:
   https://www.sdcard.org/downloads/formatter/

2. Copy the compiled file to the SD root, named exactly:  firmware.bin
   (no subfolders, no uppercase, no extra characters)

3. Power off the Octopus Pro board.

4. Insert the microSD into the TF card slot on the board.

5. Power on. Wait ~10 seconds (the STATUS LED blinks during flashing).

6. Power off, remove the SD card, power on again.

7. Check: on the SD card firmware.bin will have been renamed to FIRMWARE.CUR.
   This confirms the flash was successful.
```

### Method 2 — DFU via USB-C

Use only if the SD card method is unavailable.

> ⚠️ DFU flashing overwrites the BTT bootloader. After a DFU flash you will
> need to re-flash the BTT 128 KB bootloader to restore SD-card update capability.

```
1. Power off the main supply completely.
2. Insert the MCU Power jumper (small 2-pin header near the USB-C port).
3. Connect the USB-C cable to the PC.
4. Hold the BOOT0 button, press and release RESET, then release BOOT0.
5. In Git Bash or terminal:
      pio run -e octopus_pro_3020max -t upload
6. After flashing completes, remove the MCU Power jumper before next use.
```

---

## ESP3D setup on the Wemos D1 Mini

### Flashing ESP3D onto the Wemos

1. Download the latest ESP3D 3.x release from:
   https://github.com/luc-github/ESP3D/releases
   Choose the ESP8266 binary.

2. Flash it with `esptool.py` (install via `pip install esptool`):
   ```bash
   esptool.py --chip esp8266 --port COMx --baud 460800 \
     write_flash --flash_size detect 0x0 ESP3D-3.x.x-ESP8266.bin
   ```
   Replace `COMx` with the COM port shown in Windows Device Manager when the
   Wemos is connected via micro-USB.

### First-time configuration

After flashing, the Wemos creates a WiFi access point named `ESP3D`
(default password: `12345678`).

1. Connect your PC or phone to the `ESP3D` WiFi network.
2. Open a browser and go to `http://192.168.0.1` (or the IP shown on serial).
3. In the WebUI, go to **Settings → Network**:
   - Mode: **STA** (client — connects to your home/workshop router)
   - SSID and password: your WiFi network
4. Go to **Settings → Serial**:
   - Baud rate: **115200** (must match grblHAL USART3 speed)
   - Flow control: **None**
5. Save and restart. The Wemos will now connect to your router on every boot.

### Using the WebUI

Once both the Octopus Pro and the Wemos are powered:
1. Find the Wemos IP from your router's admin page, or from the Wemos serial
   output at boot (it prints the assigned IP).
2. Open `http://<wemos_ip>` in a browser.
3. The **Machine** / **Console** section lets you:
   - Monitor grblHAL status and alarms in real time
   - Send G-code commands directly
   - Upload G-code files to the SD card over WiFi

---

## ioSender setup

### Connecting

1. With the Octopus Pro main supply powered on, connect a USB-C cable to the PC.
   (Do **not** insert the MCU Power jumper — the board is already powered.)
2. Windows installs the virtual COM driver automatically.
   Open **Device Manager → Ports (COM & LPT)** and note the COM number assigned
   to the "USB Serial Device".
3. Open ioSender.
4. Go to **Settings → Comms**:
   - Port: the `COMx` noted above
   - Baud rate: **115200**
   - Handshake: **None**
5. Click **Connect**. The grblHAL startup banner should appear.

### First connection

grblHAL starts in **alarm state** on the first connection:

```
GrblHAL 1.1f ['$' or '$HELP' for help]
[MSG:Axis count 3]
[MSG:'$H'|'$X' to unlock]
```

- Type `$X` to clear the alarm and enable manual jogging (no homing required).
- Type `$H` to run the homing cycle (requires limit switches wired and working).

---

## Initial grblHAL settings

Paste these settings one at a time into the ioSender console, or import them
as a settings file. They are calibrated for the Genmitsu 3020 Pro Max V2
mechanical specification.

> ⚠️ **Verify steps/mm with a real measurement before running any job.**
> Mechanical tolerances vary between machines. See the calibration note below.

```gcode
; Steps per mm
; 3020 Pro Max V2: T8x8 leadscrew (8 mm/rev), NEMA17 200 steps/rev, 1/16 microstep
; X/Y: 200 * 16 / 8 = 400 steps/mm
; Z:   T8x2 leadscrew (2 mm/rev) -> 200 * 16 / 2 = 1600 steps/mm
$100=400.000
$101=400.000
$102=1600.000

; Maximum feed rates (mm/min)
$110=2500.000
$111=2500.000
$112=600.000

; Accelerations (mm/sec²)
$120=250.000
$121=250.000
$122=100.000

; Maximum travel (soft limits, mm)
$130=300.000
$131=200.000
$132=60.000

; Limit switch logic
; $5=0  for NO microswitches (open at rest, close when pressed) <- most common
; $5=7  for NC microswitches (closed at rest, open when pressed)
$5=0

; Hard limits and soft limits
$21=1
$20=1

; Homing direction (bit 0=X, 1=Y, 2=Z; 1=home toward negative, 0=toward positive)
$23=3
$22=1
$25=500.000
$24=50.000
$27=5.000

; Spindle (ON/OFF only — PWM values are cosmetic for M3 S-word)
$30=1000
$31=0
$32=0

; TMC2209 motor current (mA RMS)
; Start conservative; increase if you get missed steps under load
$338=7
$200=800
$201=800
$202=800
```

**Calibration procedure for steps/mm:**

```gcode
G91 G1 X100 F500   ; command a 100 mm move
```

Measure the actual distance moved with a calliper. If the actual distance is
`D` mm instead of 100 mm, correct the setting:

```
New $100 = current $100 * (100 / D)
```

Repeat for Y and Z.

---

## Adding a laser later

When you are ready to add a 5 V TTL laser module, follow these steps.

### Hardware

Wire the laser TTL input to the **left signal pin of the PROBE/BLTouch connector
(J43)** on the Octopus Pro. This is **PB6** — it is a 3.3 V capable GPIO that
supports hardware PWM via TIM4_CH1 on the H723.

```
  Octopus PROBE connector (J43)   Laser module
  ┌─────────────────────┐
  │ GND ────────────────┼── GND
  │ 5 V ────────────────┼── VCC (if laser is 5 V powered; else use ext supply)
  │ PB6 ────────────────┼── TTL PWM input
  │ GND │               │
  │ PB7 │ (unused here) │
  └─────────────────────┘
```

The laser's **power supply** (12 V or higher for most diode modules) must be a
**separate PSU** with GND tied to the Octopus GND.

### Firmware changes

**In `boards/btt_octopus_pro_map.h`**, add after the `AUXOUTPUT7` block:

```c
#define AUXOUTPUT8_PORT             GPIOB   // PB6 — laser PWM (TIM4_CH1)
#define AUXOUTPUT8_PIN              6
```

Then change the `SPINDLE_PWM` guard to point to AUXOUTPUT8:

```c
#if DRIVER_SPINDLE_ENABLE & SPINDLE_PWM
#define SPINDLE_PWM_PORT            AUXOUTPUT8_PORT
#define SPINDLE_PWM_PIN             AUXOUTPUT8_PIN
#endif
```

**In `Inc/my_machine.h`**, enable the PWM spindle plugin:

```c
#define SPINDLE0_ENABLE         SPINDLE_PWM0
```

Rebuild (PlatformIO or GitHub Actions) and re-flash.

### grblHAL settings for laser mode

```gcode
$32=1       ; Enable laser mode (M4 variable power enabled)
$30=1000    ; Max laser power (maps to 100% duty cycle)
$31=0       ; Min laser power
```

Use `M3 S<power>` for constant power (e.g. engraving), `M4 S<power>` for
dynamic power scaling with feed rate (recommended for laser cutting paths).

---

## Troubleshooting

### Board not detected as COM port in Windows

- Verify the MCU Power jumper is **not** inserted during normal operation
- Try a different USB-C cable — many cheap cables are charge-only with no data wires
- Install the STM32 Virtual COM driver manually:
  https://www.st.com/en/development-tools/stsw-stm32102.html

### firmware.bin not loaded from SD card

- Format the SD with the [official SD Formatter](https://www.sdcard.org/downloads/formatter/)
  (Windows built-in format sometimes creates incompatible partition tables)
- The file must be named **exactly** `firmware.bin` — lowercase, no spaces
- Place it in the **root** of the SD, not inside any folder
- Remove all other files from the SD to rule out interference

### grblHAL reports TMC communication error at startup

- Verify that the jumpers under each TMC2209 are set to **UART mode** per the
  BTT Octopus Pro manual (section 3.2)
- `$338` should only include the drivers that are physically installed:
  `$338=7` for XYZ only
- Try increasing the startup delay: `$339=500`
- Run `M122` in ioSender to read back driver status registers and check for errors

### Homing does not trigger (axis crashes into the endstop)

- Test the microswitch independently: with the board powered but motors idle,
  manually press the microswitch. In ioSender the **Limit pins** indicator should
  change state.
- If the state is inverted (triggers at rest, releases when pressed), your
  microswitches are NC — change `$5=7`
- If nothing changes, check the wiring: the signal wire must go to the `PGx` pin
  on the STOP connector, and GND to the GND pin

### Wemos keeps disconnecting from WiFi

- Power instability: the ESP8266 can draw up to 300 mA during transmit.
  Use short, thick wires from the 5 V breakout and avoid long cable runs.
- Keep the Wemos away from metal enclosures — ESP8266 RF performance degrades
  significantly inside a metallic frame.

### WebUI loads but SD card files are not visible

- Check that both `SDCARD_ENABLE=1` and `FTP_ENABLE=1` are active at build time.
  `FTP_ENABLE` is auto-enabled by the `#if WEBUI_ENABLE` block in `my_machine.h`,
  so this usually indicates `OVERRIDE_MY_MACHINE` is accidentally defined in
  `platformio.ini`. Remove that flag if present.
- Rebuild and re-flash.

---

## License

This repository inherits the **GPL v3** license from the upstream grblHAL
project. See [COPYING](COPYING).

## Credits

- [grblHAL](https://github.com/grblHAL) — Terje Io and contributors
- [dresco/STM32H7xx](https://github.com/dresco/STM32H7xx) — H7 port this fork is based on
- [BIGTREETECH](https://github.com/bigtreetech) — Octopus Pro hardware
- [luc-github/ESP3D](https://github.com/luc-github/ESP3D) — Wemos firmware

## Useful links

- [BTT Octopus Pro schematic and pin map](https://github.com/bigtreetech/BIGTREETECH-OCTOPUS-Pro)
- [grblHAL wiki](https://github.com/grblHAL/core/wiki)
- [grblHAL $ settings reference](https://github.com/grblHAL/core/wiki/grblHAL-settings)
- [ioSender releases](https://github.com/terjeio/ioSender/releases)
- [ESP3D releases](https://github.com/luc-github/ESP3D/releases)
