# OI forum t=3743: "My Car tried to Kill me today" (runaway torque), summary

Source: https://openinverter.org/forum/viewtopic.php?t=3743 (118 posts, Jun 2023 to Feb 2025). Read 2026-10-03.

## The incidents
- **7yatna (Karim), 1973 Beetle, Tesla SDU, OI fw 5.11R, all analog I/O.** On cruise (push-button mode) at highway speed, braking did not cancel cruise. The motor added torque against the brakes until the fronts locked. Shifting to neutral stopped it. Back in Drive with the brake held hard, it did a standing burnout. Neutral, ignition off and on cleared it. No logs. The brake switch checked out fine afterwards. **Root cause never found.**
- **johu (Johannes, OI author), VW Touran, Leaf motor, all inputs over CAN from stm32-car VCU.** His wife's car accelerated hard every time she came off the brake in stop-and-go traffic, once. Months later he found a likely cause: on his single shared CAN bus a Mitsubishi charger message used the same ID (0x38A) as the car's cruise message, and some temperature values decoded as valid cruise commands.
- **Bigpie:** brake and throttle over CAN stopped being honoured; throttle stuck at the last received value. Cause was bad CAN termination (40 ohm instead of 60). Fixed by correcting the termination.
- **Romale:** with cruise input held high, regen accelerated the motor past fmax. Likely a bad syncofs on an EM57 motor; johu traced it to regen current not being bounded by the fmax limiter.

## Weaknesses found in the firmware
- Scheduler cleared all timer flags at once, so a task tick could be skipped (fixed).
- **CAN throttle had no per-message timeout:** any mapped CAN message kept the inverter "alive", so a dead VCU could leave throttle stuck.
- Dual-pot plausibility could be defeated when pot1 saturated at 4095.
- Brake input had no redundancy; cruise "Button" mode had only the brake to cancel it.

## Changes johu made (released after testing, Aug 2023)
- `cruisethrotlim` caps cruise throttle (default 50%; Pete9008 argued for 10-20%, especially on RWD where the driven-axle brakes are weaker).
- Cruise "Button" mode removed; switch mode required so turning the switch off also cancels.
- Losing `din_forward` cancels cruise. **Cruise won't work if you physically swap the forward/reverse inputs (e.g. Tesla units run reversed): set `din_forward` and `dirmode=SwitchReversed` instead.**
- Neutral forces zero torque (`dir == 0` → setpoint 0).
- Brake pedal must be seen at startup when cruise is enabled.
- In CAN cruise mode, `din_cruise` must also be set and times out after 500 ms.
- **Hardened CAN control message** (default ID 63 / 0x3F, spec on wiki "CAN_communication"): pot, pot2, canio, cruisespeed, regenpreset, plus two 2-bit counters and a CRC-8 from the STM32 hardware CRC. Several bad or missing frames cut throttle, cruise and regen until the inverter restarts. Counter and CRC checks can be disabled by a parameter for simple analog-to-CAN converters, but the timeout stays.

## Other ideas raised (not all adopted)
- Brake-overrides-throttle "panic" logic as in OEM ECUs (johu: OI brake has always overridden throttle).
- Two brake inputs (one active-high, one active-low), CRC over parameters, register locking, RAM canaries, a limp-home reason code, SD-card or flash freeze-frame logging.
- Write failure scenarios and list which failsafes cover each (celeron55).
- A min speed for cruise, and cross-checking driven vs. non-driven wheel speed.

## Latest (Feb 2025)
tom91 is reworking ZombieVerter's OI-board CAN comms to use the hardened message. In that mode ZV is a "dumb front end": current/voltage limits still need setting in the inverter's parameters. `potbrake` in the VCU maps to `regenpreset` in the inverter (a 0-100% scale applied after other regen calculations).

## What it means for the GMT900 build
- Use the hardened 0x3F message between ZV and the LDU's OI board, with counter and CRC checks left on. Check that the ZV firmware you run includes tom91's rework.
- Keep the ZV-to-inverter link on its own CAN bus, separate from the GM truck bus, to avoid ID clashes like johu's.
- Verify CAN termination is ~60 ohm on every bus.
- The LDU will run reversed: do it with `dirmode=SwitchReversed`, not by swapping wires.
- If cruise is used, set `cruisethrotlim` low (RWD truck) and use switch mode.
- Wire the brake switch so a broken wire reads as braking or as a fault, and keep neutral and the ignition as independent ways to kill torque.
- Plan some logging (CAN logger) from day one; the original incident was never solved for lack of data.
