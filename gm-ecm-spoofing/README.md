# GM GMT900 ECM/TCM spoofing for the ZombieVerter

Draft `GM_GMT900` vehicle class for the ZombieVerter firmware
([damienmaguire/Stm32-vcu](https://github.com/damienmaguire/Stm32-vcu)). It recreates the periodic
frames the removed ECM and TCM sent on the truck's high-speed GMLAN bus. Background, frame table
and open questions are in [zv-gmt900-ecm-spoofing.md](zv-gmt900-ecm-spoofing.md).

**Status:** written 2026-10-04 against upstream Stm32-vcu commit `b061f84` (Sept 10, 2026).
It passes a host `g++ -fsyntax-only -Wall -Wextra` check, along with the patched main loop,
but has **not** been built with the ARM toolchain or run on hardware. Byte layouts and rates come
from other GM vehicles. Confirm them with a capture of a stock truck before putting it on the bus.

## Files

| File | Goes to | What it is |
|---|---|---|
| `GM_GMT900.h` | `include/` | Class declaration |
| `GM_GMT900.cpp` | `src/` | Scheduler, frame builders, receive side |
| `registration.patch` | repo root | Adds `9=GM_GMT900` to the Vehicle parameter, selects the class in `stm32_vcu.cpp`, adds the object to the Makefile |

To apply: copy the two source files into a Stm32-vcu checkout, then `git apply registration.patch`.
Set `Vehicle = 9` and point `VehicleCan` at the CAN port wired to the truck's high-speed GMLAN
(keep it separate from the OI inverter and BMS bus).

## What it does

- **Scheduler on the 1 ms task.** Each frame has its own period in 0.5 ms units, so 12.5 ms frames
  average 12.5 ms. Frames are staggered so they don't all fire on the same tick.
- **Bring-up mask.** Every frame has an enable bit. By default only 0x0C9, 0x1F5, 0x4C1 and 0x4D1
  are on. Change `kDefaultEnableMask` (or call `SetEnableMask()`) to add frames one at a time.
- **Live data:**
  - 0x0C9 tach: motor rpm × `kTachScale`, with a fake `kIdleRpm` idle while in run mode; brake bit.
  - 0x1F5 PRNDL: from the ZV `dir` value, forced to Park outside run mode.
  - 0x4C1 coolant: inverter heatsink temp, needle at 90 °C until the heatsink passes 60 °C, then
    climbing so `tmphsmax` reads 120 °C.
  - 0x4D1 oil temp: same mapping as coolant.
  - 0x1A1 / 0x1C3 accelerator: `potnom` scaled to 0–254 (off by default).
- **Undecoded frames send nothing.** `BuildTemplate()` returns false until captured bytes are pasted in.
- **Receive side.** Registers 0x3E9 (speed) and 0x1F1 (BCM power mode). Both decoders are placeholders.
- **Follows ignition.** Transmits only while T15 is on, so the BCM can sleep the bus at key-off.

## Still TODO from a capture

- Run/crank bits and the exact brake bit in 0x0C9 (currently guessed: byte 0 bit 7, byte 5 bit 0).
- Torque fields in 0x1C3. The EBCM needs them or StabiliTrak/TCS will fault.
- Oil pressure and fuel level bytes in 0x4D1. The fuel byte is the SOC-to-fuel-gauge path
  (`soc` is already stored from `SetFuelGauge`).
- Layout and scaling of 0x3E9 and 0x1F1.
- Which frames need a rolling counter or checksum (`rollingCtr` is in place).
- Real GMT900 periods for every frame.

## Important

Run the reversed LDU with `dirmode = SwitchReversed` on the OI inverter, not the ZV's own Reversed
option. The ZV's `dir` feeds 0x1F5, so reversing on the ZV would show R on the cluster while
driving forward and turn on the reverse lamps.
