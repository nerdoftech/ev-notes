# OI forum t=575: "Tesla Model 3 Rear Drive Unit Hacking", summary

Source: https://openinverter.org/forum/viewtopic.php?t=575 (OEM specific topics > Tesla). The thread has 1,276 posts on 52 pages, from Feb 2020 to Oct 2026. All 52 pages were read on 2026-10-05.

**Relevance to this build:** the plan uses a Tesla LDU with an OI board. This thread covers the main alternative: a Model 3 or Model Y drive unit (rear permanent-magnet, front induction) run by an open-source replacement logic board.

Names used below:
- **Jack Bauer** is Damien Maguire (EVBMW). He designs the boards.
- **davefiddes** writes the M3 firmware.
- **johu** is Johannes Hübner, the author of OI.

## Bottom line (Oct 2026)
- **The board works and is for sale.** It is the "Tesla Model 3 DU" V3.2, an OI logic PCB that replaces the Tesla board inside the drive unit's own inverter. Damien says it is the only open-source way to run a Model 3 or Model Y drive unit, but he still calls it beta.
  - Several people have them running: Damien's Volvo V50 test car since Nov 2025, plus beta testers.
  - Nathaniel's Model Y motor car reached 180 km/h.
- **It runs the standard OI firmware**, stm32-sine on an STM32F103VCT6, in davefiddes' M3_DU branch. The current release is `M3_DU_v5.40.0` (Mar 2026). It has FOC firmware for the rear PM motor and sine firmware for the front induction motor.
- **Links:**
  - Hardware: https://github.com/damienmaguire/Tesla-Model-3-Drive-Unit (V3.2 in `Design/M3DU_BoardV3_2`)
  - Firmware: https://github.com/davefiddes/stm32-sine (branch `M3_DU`, builds under Actions and Releases)
  - Shop: https://www.evbmw.com/index.php/evbmw-webshop/tesla-boards/tesla-model-3-du-32 (it also sells the full design source files)
  - Wiki: https://openinverter.org/wiki/Tesla_Model_3_Drive_Unit_PCB, plus the `_Install` and `_Parameters` pages
- **The board is a kit you fit yourself.** Since Mar 2026, boards ship with the gate drivers, transformer and current sensors already fitted. You still have to:
  - desolder the Tesla board;
  - solder in the temperature sensors (supplied) and the 30-pin header (harvested from the OEM board);
  - solder the board to the MOSFET/IGBT pins and the chassis.

  Damien won't sell "built and tested" units.
- **Damien nearly stopped development.** In Apr 2026 someone published a fork that runs the Tesla TI chip directly (https://github.com/techn0zahrt-ctrl/c2000-inverter-M3-Y), and Damien said he would stop developing the board. He kept shipping batches through May 2026 anyway and is working on a V3.3 and a "universal" M3 inverter for other motors. davefiddes said he will keep supporting the M3_DU firmware.

## Why a replacement board instead of CAN or the Tesla chip
- **The OEM inverter is locked to the car.** It is cryptographically paired with the car's VCSEC module, so it can't simply be driven over CAN. CAN replay attempts in the thread failed.
- **Commercial controllers keep Tesla's firmware.** Ingenext, EV Controls T2C and ZeroEV (a rebadged Ingenext) work around the pairing but keep Tesla's limits, such as reverse speed. johu moved closed-source CAN hacking to t=6497.
- **Erasing the Tesla chip bricks the inverter for Tesla use.** The chip is a TI TMS320F28377D.
  - It can be erased over JTAG, but the Tesla firmware can't be read back. davefiddes bricked a £1,500 unit this way.
  - His 2021-2023 port of OI to that chip (`c2000-inverter`) never became a usable inverter, so Damien switched to designing a full replacement PCB from Oct 2022.

## Which drive unit to buy
| Part / code | What it is | Notes |
|---|---|---|
| 1120980-xx (3D1, "980") | Model 3 rear, performance inverter | 4 MOSFET pairs per phase. Reddit claims about 800 A vs 600 A. The OI board can't add the missing FETs to a 990. |
| 1120990-xx (3D5, "990") | Model 3 rear, standard inverter | 3 MOSFET pairs per phase. Wire-wound stator. |
| 3D6 / 3D7 | Later Model 3 / Model Y rear | Hairpin (copper bar) stator. Many Model Y units have **no motor temp sensor**, so the firmware can use oil temperature instead. |
| Front DU (e.g. 3D3, 960) | Induction motor, IGBT inverter | Runs on the same V3.2 board with sine firmware after a few changes (see below). |

- **Pyrofuse:** rear inverters from about 2022 (3D5 and later, plus Plaid) have a pyro "crowbar" that cuts the phase busbars in a crash. It comes off with the OEM PCB, but a fired one leaves the busbars broken. Check before buying a crash-damaged unit.
- **Model Y oil pump:** pump `1108202-00-M` (Model Y and Highland) ignores the speed command and runs at full speed in limp-home mode. It still reports RPM, temperature and pressure, but its voltage reading is wrong. The Model 3 pump `1108202-00-F` follows speed commands, including 0.
- **Gearbox and dimensions:**
  - The ratio is about 9:1.
  - davefiddes measured about 675 mm across the mounting lugs and 565 mm from the back to the front edge of a lug.
  - A 3D scan is at https://grabcad.com/library/tesla-model-3-rear-drive-unit-evshop-fr-1
- **Price data point:** 600-800 EUR for damaged-car units in 2023-24.

## Fitting the board (install wiki has the full steps)
- **Removing the OEM board:**
  - Unsolder 51 joints and the HVIL loop, unclip the 30-pin connector clamp, and remove 11 T20 bolts. The current sensor block uses 3 T10.
  - A heated desoldering gun works well.
  - Heat the current sensors before prying up the block, or their heads snap off. The glue is heat sensitive.
- **Conformal coating:** MG Chemicals 8310A stripper softens it. Acetone does nothing.
- **Parts:**
  - Current sensors: Melexis MLX91209LVA-CAA-002-SP (7.3 mV/mT, Mouser). Use this exact part. Damien warns that a less sensitive variant can push high-current readings into ferrite-ring saturation, which makes them inaccurate even after recalibration.
  - Heatsink thermistors: EPCOS B57164K0473J000 (47k). V3.3 will use 0402 Panasonic ERT-J0ET473J on small PCBs from jrbe.
  - Gate drivers: STGAP1AS (OEM) or STGAP1BS (new) both work.
- **Connectors:**
  - 30-pin signal connector: Toyota 90980-12712 (Sumitomo 6189-6987). Also sold by BMotorsports and on AliExpress (Jorch).
  - HV DC connector: TE HC-STAK 2840900-1 (printed 2-2840440-1). Its contacts spread after many plug cycles, and that caused an arc in Damien's car.
  - Coolant: VDA NW18 quick connectors, Tesla hoses marked `FIP-NW18-90°-3` and `FIP-NW18-180°-1` (PA66 GF30). AliExpress copies fit.
  - Oil pump: red = +12 V, blue = LIN, black = GND. It can draw 10 A.
- **Oil:**
  - Tesla specifies "ATF-9" (SK ZIC). The thread used Dexron VI, Fuchs Titan ATF 6009, or Fuchs BluEV EDF 7005 for units with oil in the motor (Fellten's advice).
  - Avoid ATF not meant for hybrids, because the oil touches HV windings.
- **First power-up:** check the gate supply test points before soldering to the power stage. Expect +12.7 V / -5.1 V per phase (TP6-TP14). A lifted diode or a solder blob was the cause of most beta-tester faults.

## Wiring and control
- **Two 12 V feeds:** V3 boards need permanent 12 V (T30) and switched ignition (T15). With only T30 the board stays dark. Damien suggests tying the two together for now.
- **No precharge output:** the VCU or BMS must precharge and manage the HV bus. This differs from older OI boards.
- **Pin 23 (ACT_DISCH) must be at 12 V whenever HV is live,** or the active discharge resistors heat up. The thread discussed the rule that the bus must drop below 60 V within 5 s.
- **Control:**
  - Start, direction, brake and throttle can come over CAN, and Damien suggests a ZombieVerter.
  - The OEM harness has no wires on some OI pins (start and forward). One tester moved the wires from the second CAN pins (20, 30).
  - Six pins on the 30-pin connector are unused.
  - The pin map is on GitHub, but pin 1 is Start on the OI board, not Proximity.
- **Bench testing:**
  - Set `udcmin` and `udcsw` to 0.
  - With a single throttle channel, use `potmode` = `SingleRegen`.
  - No coolant is needed, and 12-50 V on the HV side is enough to turn the motor.
- **Logging and config:**
  - OpenInverter CAN Tool (`oic`), or the esp32/esp8266 web interface.
  - Example log command: `oic log VALUES file.csv`.
  - For fast logging, export a DBC and use SavvyCAN.

## Oil pump (LIN)
- **Protocol:** the inverter sends one command frame (ID 0x0A) and polls status IDs 0x2A, 0x30, 0x31 and 0x32. Newer Tesla firmware first runs a LIN diagnostic exchange (0x3C/0x3D).
  - Decoded values include fluid and pump temperatures, supply voltage and current, RPM, and pressure.
  - davefiddes' ESP32 test sketch and analyzer are in his fork of Damien's repo.
- **Without valid LIN commands the pump runs at full speed,** so put it on switched 12 V. Measured pressure was 40-60 psi.
- **Firmware parameters:** `pumpspeed`, `pumpspeedidle`, `tmpoillow` and `tmpoilhigh` ramp the pump with oil temperature. The firmware also derates torque on high oil temperature.

## Tuning numbers from the thread
- **Basics:** `polepairs` 3, `respolepairs` 3, `encmode` HFResolver (an 8.8 kHz exciter; Tesla uses 10 kHz).
- **`syncofs` 34968 on the rear motor:** that is 2200 + 32768. Damien's early 2200 had the field running backwards, which made the motor weak and slow to stop.
- **Temperature sensors:** set `snsm` and `snshs` to `TeslaM3`.
- **Dead time:** don't set `deadtime` below about 28 (800 ns). Below that the gate drivers fault.
- **Nathaniel's Model Y 3D7 motor:** 108s battery, about 420-440 V.
  - `ILGAIN` about -1.15, calibrated against BMS current. `lqminusld` 0.3 and `fluxlinkage` 50.
  - `throtcur` 10 (1000 A) gave about 260 Nm, and 11.5 gave about 305 Nm. He estimates rated 340 Nm needs about 1300 A.
  - Stock firmware caps `throtcur` at 10. Damien posted a build with a cap of 20.
  - DESAT trips at about 1150-1200 A phase current.
  - His parameter files `params M3 260Nm.json` and `params M3 305Nm.json` are attached to the thread.
- **Base speed:** about 4,500 rpm at 350 V (Damien). One quoted figure is 5,400 rpm at 320 V.
- **Field weakening:**
  - The problem: at 180 km/h (about 13,500 rpm, 690 Hz electrical) with `fwcurmax` -200, lifting off gave strong regen that ignored the pedal.
  - johu's advice: `fwcurmax` is probably far too small. Find the motor's critical current by coasting with regen off and stepping `manualid` negative until `uq` reaches 0. Never set `fwcurmax` beyond that, and lowering `vlimmargin` may help.
  - Shifting to neutral at speed is safe. PWM keeps running and commands zero torque.
- **Robustness:** Damien desaturated his inverter more than 50 times without damage. A dropped spanner across the phases and zero dead time only tripped it.

## Reverse
- With OI firmware, reverse has no Tesla speed limit, and the electric oil pump doesn't care which way the unit turns.
- One person cut and swapped two phase busbars to run a Tesla-controlled unit backwards.
- A Quaife ATB differential reportedly does not work in reverse. That came up for a front unit.

## Front drive unit on the same board
- Swap the gate resistors to 6R2, remove the steering diodes, and set R45 to 5K6 so the firmware detects the front unit (`TeslaM3FDU`). The rear unit uses 3K3.
- The front unit uses different gate driver settings and switches at -8 V / +16 V.
- Damien had a front unit running on the board in Mar 2026.

## Open issues (Oct 2026)
- Model Y / Highland oil pump speed control over LIN.
- Field-weakening tuning at high rpm.
- Which CAN messages the board sends at power-up, which a new builder asked about.
- V3.3 changes:
  - better hole alignment (H10 was off on V3.2);
  - OEM-style temperature sensor PCBs;
  - the two 10K resistor fixes added to the schematic. davefiddes found in Feb 2026 that the V3.2 design was missing them (p89663):
    - a pull-down from MAIN_PSU_EN (base of Q16) to ground. Without it the board powers on but never powers off if the MCU fails to initialise or boot loops;
    - a pull-up from GATE_DIAG to 3.3 V. Without it the fault LED glows and the inverter risks tripping.
    - Damien hand-fitted both resistors to the first V3.2 batch (Feb 2026) and said he would add them to the next board revision.
- JLCPCB placed the STM32 rotated on 2 of 10 boards. Power up and program each board before snapping off the panel rails that carry its ID.

_Corrected 2026-10-07 after Jack Bauer's review of this summary (openinverter.org/forum/viewtopic.php?p=94797): current sensor substitution and the V3.2 resistor fixes._
