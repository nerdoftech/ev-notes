# Running the Tesla LDU reversed with an OI board

The GMT900 build mounts the Tesla Model S/X large drive unit (LDU) turned 180°, with the motor behind the axle, so the motor has to spin in Tesla's reverse direction to drive the truck forward. See [../rear-suspension/de-dion-and-ldu.md](../rear-suspension/de-dion-and-ldu.md) for why it's mounted that way.

Short version: the electrical side is a single parameter on the OI board. The real concerns are mechanical: oil pump, gear thrust and seals.

## Motor direction (control)

- Set `dirmode` on the LDU's **OI inverter board** to `SwitchReversed` (3), or `ButtonReversed` (2) for a button selector.
  - In stm32-sine `VehicleControl::SelectDirection()`, the Reversed flag multiplies the forward/reverse inputs by -1.
  - The ZombieVerter sends forward/reverse to the inverter as CAN bits (`Can_OI.cpp`: forward = bit 8, reverse = bit 16), so ZV forward makes the motor spin reversed.
- **Keep the ZV's own `dirmode` normal.** The ZV's Reversed flag would flip its `dir` value. That value drives the GMT900 PRNDL frame (0x1F5) and the reverse lamps, so the cluster would show R while driving forward.
- **Don't reverse by swapping the forward/reverse wires.** Cruise control won't work that way. Use `din_forward` with `dirmode = SwitchReversed` instead (OI forum [t3743](https://openinverter.org/forum/viewtopic.php?t=3743)).
- Verify the direction on the bench at low torque before driving.

### Why OI makes this easy

With Tesla's own logic board, reverse is limited by Tesla's firmware. People who run a reversed LDU on the OEM board (for example under an EV Controls T-2C or Dynam Labs VCU+) have to cut and swap two of the U/V/W phase busbars and swap the encoder wires, so the board thinks it's going forward. One builder confirmed on the bench that swapping the small white phase-board harnesses works instead of cutting busbars, as long as the two inner encoder pins are swapped too ([t6396](https://openinverter.org/forum/viewtopic.php?t=6396), nubster, Dec 2025). None of that is needed with an OI board.

## Lubrication: fit a reverse-drive oil pump

The factory LDU oil pump only circulates oil when the motor turns in Tesla's forward direction. Neither Tesla LDU of that era pumps oil when the car drives in reverse ([t2242 p42874](https://openinverter.org/forum/viewtopic.php?p=42874#p42874)). For sustained reversed driving, replace it with a reverse-drive pump:

- [Westside EV LDU Reverse Drive Oil Pump](https://www.westside-ev.com/store/p/tesla-ldu-reverse-drive-oil-pump-replacement) ($275; reuses the two OEM pump gears in a new housing)
- Also sold by [Fellten](https://shop.fellten.com/shop/ldurdop-tesla-ldu-reverse-drive-oil-pump-replacement-12881), [Stealth EV](https://stealthev.com/product/tesla-ldu-reverse-drive-oil-pump/) and [ZeroEV](https://zero-ev.co.uk/product/tesla-large-drive-unit-replacement-reverse-drive-oil-pump/) (the one the OI wiki LDU page links)

## Gears, bearings and seals

- **Helical gear thrust reverses.** Tesla's gears are helical, so running reversed pushes axial loads onto the bearings in the opposite direction. Toyota's RAV4 EV uses a Tesla-based unit mounted reversed, and Toyota fitted reverse-cut helical gears and a different oil pump to keep the loads the same as the Model S ([t2242 p42874](https://openinverter.org/forum/viewtopic.php?p=42874#p42874), asavage; midway in [t6396](https://openinverter.org/forum/viewtopic.php?t=6396)).
- **Rotor coolant seal.** Some seals have a spiral pattern that pumps coolant back toward the wet side in one direction only. Run backwards, the same pattern can pump coolant toward the dry side, a known leak risk for reversed units. Seals with patterns for both directions are available ([t2242 p42356](https://openinverter.org/forum/viewtopic.php?p=42356#p42356), muehlpower and follow-ups).
- **Field experience.** "Boxster EV" has run a reversed LDU for several years with a reverse-drive oil pump, a Quaife diff and stock Tesla gear sets, with no problems ([t2242 p42899](https://openinverter.org/forum/viewtopic.php?p=42899#p42899)).

## Sources

- OI wiki: [Tesla Model S/X Large Drive Unit (LDU)](https://openinverter.org/wiki/Tesla_Model_S_X_Large_Drive_Unit_LDU): "The motor itself will run as well in the reverse direction as in the forward direction," but fit a reverse oil pump.
- OI forum [t2242](https://openinverter.org/forum/viewtopic.php?t=2242): Tesla LDU motor teardown and maintenance.
- OI forum [t6396](https://openinverter.org/forum/viewtopic.php?t=6396): "Could this work instead of swapping busbars?"
- OI forum [t3743](https://openinverter.org/forum/viewtopic.php?t=3743): runaway torque and CAN safety; see [../oi-forum/t3743-runaway-torque.md](../oi-forum/t3743-runaway-torque.md).
- stm32-sine (`VehicleControl::SelectDirection()`) and the ZombieVerter firmware (`Can_OI.cpp`).
