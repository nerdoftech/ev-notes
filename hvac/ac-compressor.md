# Electric A/C compressor (Tesla Model S/X)

Sources: openinverter wiki text that Eric pasted into the thread, [diyelectriccar.com forum posts](https://www.diyelectriccar.com/threads/tesla-a-c-compressor-questions.189978/page-3), and parts listings.

## Variants

| Gen | Maker / model | Tesla P/N | Control |
|---|---|---|---|
| 1 (2013–2014) | Denso ES34C | 6007380-00-D (-00-C also seen) | PWM, 35–400 Hz. 5 % duty = max speed, 85 % = min speed. 12 V and ground only |
| 2 (2015+) | HVCC ESC33 | 1028398-00-E / -F / -J | CAN |
| 2 | Hanon HES33 | 1063369-00-D / -E / -F / -G | CAN |

All of them run R134a, which matches the GMT900. Power draw is about 12 A at 360 V (~4.5 kW) at max load. That was measured on Gen 1, and Gen 2 is assumed similar.

**Pick:** Gen 2 (HVCC or Hanon). The CAN protocol is simple, it reports speed, faults, voltage and current, and these units are newer and easier to find. Gen 1 is the fallback if a cheap one turns up, since it needs only a PWM signal.

## Gen 2 LV pinout

| Pin | Function | OEM wire color (varies by year) |
|---|---|---|
| 1 | +12 V enable | White/DkBlue or Red/Green |
| 2 | CAN-H | White/Red or Red/White (twisted pair) |
| 3 | CAN-L | Red (twisted pair) |
| 4 | Ground | Black |

Startup order: apply ~340 V HV, ground pin 4, bond the compressor case to the frame, then put 12 V on pin 1. With 12 V only, it sends no CAN traffic at all.

The HV connector part number is unknown. A Lexus GS450h A/C compressor HV cable has been reported to fit.

## Gen 2 CAN

All multi-byte fields are little endian. The bitrate isn't documented (probably 500 kbps, to confirm on the bench).

**Command `0x28A`, sent every 50–100 ms**

| Byte | Meaning |
|---|---|
| 0–1 | Target duty, 0.1 % per bit (40.0 % = 400 = `90 01`) |
| 2–3 | Max power, W (3000 W = `B8 0B`) |
| 4 | 0 |
| 5 | 1 = enable |
| 6–7 | 0 |

Example (40 %, 3 kW max): `90 01 B8 0B 00 01 00 00`

**Status `0x223`**

| Byte | Meaning |
|---|---|
| 0–1 | Speed, RPM |
| 2–3 | Output duty, 0.1 % |
| 4 | Inverter temp, °C + 40 |
| 5, 6 | Fault bits (both should be 0) |
| 7 | Bit 7 = ready; bits 0–1 = status (0 none, 1 normal, 2 wait, 3 faulted) |

Example: `68 05 0E 00 41 00 00 81` = 1384 RPM, 1.4 % duty, 25 °C, no faults, ready, normal.

**HV status `0x233`**

| Byte | Meaning |
|---|---|
| 0–1 | HV, 0.1 V |
| 2 | 12 V input (reads 0xFF in testing) |
| 3–4 | Current, 0.1 A |
| 5–6 | Power, W |

Example: `65 0E FF 0B 00 94 01 00` = 368.5 V, 1.1 A, 404 W.

## ZV integration

- Put the compressor on the ZV CAN bus shared with the LDU board, not GMLAN, so 0x223, 0x233 and 0x28A can't collide with truck traffic.
- Duty comes from the cabin A/C request (read off GMLAN from the factory head unit) plus chiller demand, if a chiller is added.
- Before buying, confirm the unit runs down near 280 V.
