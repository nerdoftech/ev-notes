# ZombieVerter: spoofing ECM/TCM frames on GMT900 high-speed GMLAN

Compiled 2026-10-03 from the "Main" chat (2026-10-01 and 2026-10-02), transcript at
`/mnt/project-files/history/main-chat.md`. Nothing here has been checked on the truck.
Confidence labels are the ones the chat gave; **[unverified]** marks anything that still
needs a capture from a stock 2012 Silverado.

## Why spoof at all

Pulling the ECM and the 6L80 TCM removes periodic heartbeat frames the rest of the truck
expects. Without them you get lost-communication DTCs (U0100 ECM, U0101 TCM), a dead tach and
gear display, and likely traction control/StabiliTrak warnings. The ZV has to recreate them.

## Bus layout

- **High-speed GMLAN:** 500 kbps dual-wire CAN, OBD pins 6 and 14. ECM, TCM, EBCM and BCM live here.
- **Low-speed GMLAN:** 33.3 kbps single-wire CAN, OBD pin 1. Cluster, radio and HVAC are generally here.
- **The BCM gateways between them.** A forum user on a related GM platform said the cluster is on
  the low-speed bus but all the data needed to drive it is on the high-speed bus. If that holds on
  the GMT900, spoofing only on the high-speed side is enough and the ZV never needs a single-wire
  transceiver. **[unverified on GMT900]**
- Put the truck's high-speed GMLAN on **its own ZV CAN channel**, separate from the LDU/OI board and
  the BMS, so bus load and IDs don't collide.

## Frames to spoof

IDs and rates come from an HP Tuners user's capture of a stock GM powertrain, **not a GMT900**:
0F9 (80 Hz), 1C3 (40 Hz), 1ED (80 Hz), 1EF (80 Hz), 1F5 (40 Hz), 2C3 (20 Hz), 3C1 (10 Hz),
3D1 (10 Hz), 3F9 (4 Hz), 3FB (4 Hz), 4C9 (no rate). Treat the rates as a starting point, not a spec.

| ID | Original sender | Contents | Rate | Confidence |
|---|---|---|---|---|
| 0x0C9 | ECM | RPM, engine-run status, brake bit | ~80 Hz (12.5 ms) typical | Contents solid, rate needs verifying |
| 0x0F9 | ECM/TCM | Undecoded | 80 Hz (12.5 ms) | Rate observed |
| 0x1A1 | ECM | Accelerator position | Likely 40–80 Hz | Verify |
| 0x1C3 | ECM | Torque / accelerator | 40 Hz (25 ms) | Rate observed |
| 0x1ED | ECM | Undecoded | 80 Hz (12.5 ms) | Rate observed |
| 0x1EF | ECM | RPM (alternate) | 80 Hz (12.5 ms) | Rate observed |
| 0x1F5 | TCM | PRNDL, tow/haul | 40 Hz (25 ms) | Solid |
| 0x2C3 | ECM | Undecoded | 20 Hz (50 ms) | Rate observed |
| 0x3C1 / 0x3D1 | ECM | Undecoded status | 10 Hz (100 ms) | Rate observed |
| 0x3F9 / 0x3FB | ECM/TCM | Undecoded | 4 Hz (250 ms) | Rate observed |
| 0x4C1 | ECM | Coolant, IAT, outside air temp | ~2 Hz (500 ms) | Contents solid, rate needs verifying |
| 0x4C9 | TCM? | Possibly trans temp | Slow | Verify |
| 0x4D1 | ECM | Oil temp/pressure, possibly fuel level | ~2 Hz (500 ms) | Partial |

### Known byte contents (from various GM vehicles)

- **0x0C9:** RPM in bytes 2–3, scale 0.25 rpm/bit. Brake on/off bit in the 6th byte. Run/crank
  status bits location **[unverified]**.
- **0x1A1 and 0x1C3:** accelerator position in byte 8, range 0–254 (seen on another GM vehicle).
  0x1C3 torque fields **[unverified]**.
- **0x1EF:** engine RPM in bytes 3–4 (another GM vehicle).
- **0x1F5:** byte 4 = PRNDL: 1 Park, 2 Reverse, 3 Neutral, 4 Drive (plus manual-gear codes).
  Byte 6 = tow/haul flag.
- **0x4C1:** bytes 2–4 = coolant, intake air and outside air temps, mostly `A − 40` (°C).
- **0x4D1:** byte 2 = engine oil temp. Oil pressure and fuel level bytes **[unverified]**.

## What each module cares about

- **Cluster / BCM, gear (0x1F5):** drives the gear display. On many GM trucks the BCM also uses it
  for reverse lamps and Park-dependent behaviour such as auto door unlock. The 6L80 range switch goes
  away, so the shifter becomes a ZV input and the ZV generates this frame.
- **BCM / HVAC, engine running (0x0C9):** they likely check the "engine running" state. If it says
  not running you may lose A/C compressor enable or get charging warnings. Send a fixed idle RPM, or
  scale LDU motor RPM onto the tach.
- **Cluster, coolant (0x4C1):** send a steady ~90 °C so the gauge and overheat warnings stay quiet,
  or map inverter/motor temperature onto it.
- **Cluster, fuel level:** on the GMT900 the ECM probably reads the fuel sender and broadcasts the
  level, likely in 0x4D1 or a nearby ID. If so, SOC can drive the fuel gauge. **[unverified]**
- **Speedometer / odometer:** with the trans output speed sensor gone, check whether they use EBCM
  wheel speeds (0x3E9) or a powertrain message. If powertrain, the ZV must compute speed from LDU RPM
  and the axle ratio. **[unverified]**
- **EBCM (ABS / TCS / StabiliTrak):** expect it to want ECM torque frames (0x1C3 and others).
  Without them, expect a traction control light. ABS will probably still work, but plan on
  StabiliTrak and TCS being disabled unless the torque interface is fully emulated.
  **[unverified]**
- **EBCM rear wheel speeds:** separate from the ECM frames. The de Dion rear needs real rear wheel
  speed sensors (32-tooth ring on 2007+ GMT900). Computing wheel speeds in the ZV and sending them
  over CAN was mentioned only as a riskier fallback. See `knowledge/claude/rear-axle.md`.

## Draft `GM_GMT900` ZV vehicle class

Written 2026-10-01 against upstream ZombieVerter firmware (`damienmaguire/Stm32-vcu`, Sept 2026),
modelled on the existing Subaru and VAG classes. It passed a syntax check with no warnings but was
**never built for the board or run on hardware**. It lived on local branch `gmt900-vehicle`
(commit 5875a5c) in a container that is now gone, and **was never pushed**, so the code itself is lost.
The chat only described it; the summary below is everything the transcript shows.

**Registration (three small edits plus the new class):**
- Vehicle enum: new entry, shown in the ZV's Vehicle parameter as `9=GM_GMT900`.
- Main loop: selects the class when Vehicle = 9.
- Makefile: adds `GM_GMT900.cpp`.

**Scheduler:**
- Runs on the ZV's 1 ms task.
- Each frame has its own period stored in 0.5 ms units, so 12.5 ms frames average 12.5 ms.
- Frames are staggered so they don't all fire on the same tick.
- All IDs and rates from the table above are in the schedule.

**Bring-up mask:**
- Every frame has an enable bit.
- Default on: only the four frames with known contents, 0x0C9, 0x1F5, 0x4C1 and 0x4D1.
- Turn others on one at a time and watch which warnings clear.

**Live data mapped into frames:**
- 0x0C9 tach: motor RPM, scaled, with a fake idle while READY. Also the brake bit.
- 0x1F5 PRNDL: from the ZV's `dir` value, forced to Park when the ZV isn't in run mode.
- 0x4C1 coolant: inverter heatsink temperature, mapped so the needle sits at normal until things get hot.
- 0x4D1: oil temperature.
- 0x1A1 and 0x1C3: accelerator position.

**Undecoded frames:** `BuildTemplate()` returns false (nothing sent) until bytes captured from a
stock truck are pasted in, so nothing goes on the bus blind.

**Receive side:** listens for vehicle speed on 0x3E9 and BCM power mode on 0x1F1. Both decoders are
placeholders. **[unverified]**

**Ignition:** transmits only while the T15 ignition input is on, so the BCM can sleep the bus at key-off.

**Tuning constants** at the top of `GM_GMT900.cpp`: tach scale, idle RPM, temp gauge mapping.

**Rolling counters:** a `rollingCtr` variable was in place for frames that turn out to need an
alive counter or checksum.

### Still to fill from a capture

- Run/crank bits in 0x0C9.
- Torque fields in 0x1C3 (EBCM needs them or StabiliTrak complains).
- Oil pressure and fuel level bytes in 0x4D1 (fuel byte is the SOC-to-fuel-gauge path).
- Speed scaling on 0x3E9.
- Which frames need rolling counters or checksums.
- Real GMT900 periods for every frame.

## Motor direction and the gear frame (2026-10-02)

The LDU is mounted turned 180° and runs reversed. Do the reversal **on the OI inverter board**
(`dirmode` = SwitchReversed, 3), **not** with the ZV's Reversed option. Reversing on the ZV flips
its `dir` value, which feeds 0x1F5, so the cluster would show R while driving forward and the
reverse lamps would come on. With the inverter reversed, the ZV's `dir` keeps meaning truck direction.

## How to pin it down on the truck

1. **Capture a stock truck.** Ideally a 2012 Silverado with the same engine and transmission. Log
   high-speed GMLAN at 500k (SavvyCAN with a CANable, or the ZV itself) through key-on, idle,
   shifting through PRNDL, and driving. Record every ID, payload and period.
2. **Find the ECM/TCM IDs.** Unplug the ECM and see which IDs disappear, then the TCM. Whatever
   disappears is the spoof list for this exact truck.
3. **Replay first, then go dynamic.** Have the ZV replay captured idle frames at the measured rates,
   check which DTCs (U0100, U0101) and cluster messages go away, then switch to live data only for
   the signals that matter: PRNDL, RPM, temperatures, fuel/SOC.
4. **Check for rolling counters and checksums.** If a module rejects replayed frames, that's the
   usual cause.
5. **References:** GM Global A DBC files in comma.ai's opendbc repo, and GM standard GMW8762, which
   covers many of the 11-bit IDs (many remain platform-specific).

## Related

- `knowledge/claude/rear-axle.md`: rear ABS tone rings, LDU orientation, `dirmode`.
- `notes/oi-forum-t3743-runaway-summary.md`: runaway / CAN safety notes.
