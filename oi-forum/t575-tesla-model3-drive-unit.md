# OI forum t=575: "Tesla Model 3 Rear Drive Unit Hacking", summary

Source: https://openinverter.org/forum/viewtopic.php?t=575 (OEM specific topics > Tesla; about 1,276 posts on 52 pages, Feb 2020 to Oct 2026). Read 2026-10-05.

**Scope:** this covers page 1 (the project start, plus the March 2026 status update in the first post) and pages 50 to 52 (the current state). Pages 2 to 49 were not read, so early bring-up detail and older parameter discussions are missing.

**Relevance to this build:** the plan uses a Tesla LDU with an OI board. This thread covers the alternative: a Model 3 or Model Y rear drive unit (permanent magnet, SiC inverter) with an open-source replacement logic board.

## Where the project stands (first post, updated March 2026)
- Jack Bauer (Damien Maguire, EVBMW) built a complete replacement logic PCB for the Model 3 drive unit inverter and bench tested it. He calls it the only open-source way to run a Model 3 or Model Y drive unit.
- Hardware: https://github.com/damienmaguire/Tesla-Model-3-Drive-Unit (the OEM pinout is `M3_RearDU_OEM_Pinout.pdf` in that repo).
- Firmware: stm32-sine fork at https://github.com/davefiddes/stm32-sine/
- Boards are sold at the EVBMW webshop (Tesla boards > "Tesla Model 3 DU", V3.2 is current). Some people build their own from the repo through JLCPCB.

## OEM inverter facts (2020 teardown)
- 24 ST SiC MOSFETs (GK026), 6 STGAP1AS gate drivers, TI TMS320F28377D DSP, two CAN buses (SN65HVD1040A), one LIN (TJA1021).
- Holding the DSP reset pin (XRS, pin 124) low disables the OEM processor and leaves the power stage working. That is how a replacement logic board can take control.
- The low-side gate supplies come from a VIPer16 supply fed from HV, not from a separate multi-output DC-DC.

## Model 3 vs Model Y units
- **Oil pump:** the Model Y unit (and the Highland front unit) has a LIN-controlled oil pump. With the current firmware the pump runs at full speed and ignores speed commands. It does report RPM, temperature and pressure, but its voltage reading is wrong.
- **Motor temperature:** the Model Y unit has no motor temperature sensor. Oil temperature was suggested as a substitute.
- **Stators:** the 3D7 motor has hairpin (thick copper bar) windings, while the 3D5 is wire-wound. That affects how much current it handles.
- **Performance inverter:** it has 4 MOSFETs per phase instead of 3.

## Real-world results (pages 50-51, Nathaniel, Model Y motor in a Volvo V50)
- **Battery:** 108 cells, about 442 V nominal and 420-430 V under load.
- **First run:** it felt weak, about 55 kW peak and roughly 190 Nm.
  - He corrected the current sensor gain (`ILGAIN` of about -1.15, calculated by comparing against BMS current) and set `lqminusld` to 0.3.
  - Raising phase current from 800 A to 1000 A helped a lot. The board handled 1000 A without thermal trouble.
- **Tuned:** 305 Nm with `throtcur` 11.5, against about 260 Nm before. He reached 180 km/h with a 550 A battery current peak on the performance inverter, and the inverter stayed around 37 °C.
- **Raising throtcur:** Jack Bauer posted firmware that raises the `throtcur` limit from 10 to 20, which would allow up to 2000 A phase current in theory. DESAT warnings appeared at 1200 A phase current, so that is the practical ceiling seen so far.
- **Other parameters he changed:** `fluxlinkage` from 90 to 50 with no visible downside, `fwcurmax` -200 and `vlimmargin` 2500. His parameter files `params M3 305Nm.json` and `params M3 260Nm.json` are attached in the thread.
- **Field weakening:**
  - The problem: above about 13,500 rpm (690 Hz electrical) the motor regenerated hard even though regen was set to 10%.
  - The cause: johu traced it to too little field-weakening current. The fix is to raise `fwcurmax` (make it more negative) and adjust `vlimmargin`.
- **Neutral at speed:** Jack Bauer confirmed that shifting to neutral while field weakening is safe. PWM keeps running and commands zero torque, so the back-EMF does not spike past the 650 V MOSFET rating.

## Building or buying boards: known problems (pages 50-52)
- **`m3_phase_lo UVLOH` error:**
  - Seen on a 48 V bench test. It means the +15 V high-side supply for the phase C low side is out of range.
  - On a self-built board, JLC had left out U5. Fitting that IC fixed it.
- **Open solder joint on Q25:** gave a phase B low fault and was fixed by reflowing it.
- **Rotated STM32 (U16):** JLCPCB placed it 90° off on 2 of 10 boards. Power up and program each board before peeling off any ID labels. JLC support was reported as unhelpful.
- **joeflickers (Oct 2026), self-built board, probably a Model Y motor:**
  - After fixing phase C drive parts, the motor runs up to 60 km/h so far.
  - The oil pump runs and reports RPM and pressure, but its speed can't be controlled.
  - He is looking for how to raise the RPM limits and for documentation of the CAN messages the V3.2 board sends.
  - He plans a separate thread on fitting the whole rear subframe into a conventional car.

## Open questions in the thread (as of Oct 2026)
- Oil pump speed control over LIN (Model Y and Highland units).
- Documentation of the CAN messages the V3.2 board sends at power up.
- Using the board's spare I/O: 6 unused GPIO pins on the 30-pin connector. Driving other motors such as a Leaf was mentioned as possible.
