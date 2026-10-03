# Dodge/Mopar electro-hydraulic power steering pump for the GMT900 EV

Compiled 2026-10-03 from the "Power Steering" thread. Nothing here has been checked on the truck.
**[inferred]** marks Claude's estimates, not published or measured figures.

## Why this pump

The 2012 Silverado 1500 has a hydraulic rack fed by an engine-driven pump. With the engine gone,
an electro-hydraulic (EHPS) pump can feed the stock rack. The front axle still carries close to
3,000 lb, so flow and pressure matter. The Volvo/TRW and Mazda pumps common in EV builds are often
reported as too weak. The Dodge pump is the strongest common option.

## The part

- **Part numbers:** Mopar 5154662AC (also 5154662AB). Aftermarket listings also use 68059524xx.
- **Donor vehicles:** 2011–15 Dodge Charger, Challenger and Chrysler 300, and 2011–15 Jeep
  Grand Cherokee and Dodge Durango, mostly with the 3.6L V6.
- **Pump year matters.** Reform Motorsports sells separate controllers for 2011–13 and 2014–18
  pumps, which suggests the CAN protocol or firmware changed. Stick to one year range.
- **Fluid:** Mopar ATF+4 / Power Steering Fluid +4 (PN 05013457AA).
- **Fittings:** Dodge-specific high-pressure fittings (straight and 90°) are sold by Hang Tight and Reform.

## Specs compared with other EHPS pumps

From Hang Tight's write-up:

| Pump | Max pressure | Max flow | Peak current | Runs with no CAN? |
|---|---|---|---|---|
| Dodge | 1,725 psi | 13.7 L/min unlocked (4.3 L/min stock firmware) | 87–97 A | No |
| Volvo/TRW | 1,650 psi | 3.2 L/min | 64 A | Yes (limp mode) |
| Mazda 3/5 | 1,570 psi | 1.9 L/min | 57 A | No |

Unlocked, the Dodge pump's flow should be in the range of the stock GM belt pump **[inferred]**.

## Electrical

- **Connector:** 3 pins: 1 = CAN-H, 2 = +12 V, 3 = CAN-L. The pinout has no ground pin, so check the pump for a separate ground stud or lead before wiring **[inferred]**.
- **Fuse and wire:** 100 A fuse and 8 AWG wire (Hang Tight). Hang Tight's write-up says an 80 A
  fuse is ideal.
- **Current draw** (Hang Tight measurements across the pumps they support):
  - Driving straight or stopped without steering: about 7–10 A
  - Normal street driving: rarely above 40 A, and only briefly
  - Turning the wheel while stopped: up to about 80 A, for about a second
  - Current follows hydraulic pressure (steering demand), and pump speed has little effect.
- **Estimated average on the road:** 10–15 A, or about 150–200 W **[inferred]**.

## Control

The pump has **no limp mode**. It stays off unless it sees the right CAN messages. In the donor
cars it runs on the high-speed CAN bus and uses vehicle speed and steering angle to set assist.

**CAN protocol: not found published** (searched 2026-10-03). Hang Tight (PSC) and Reform Motorsports
both sell controllers that drive it, but they keep the messages to themselves. The diyelectriccar
thread "Dodge/Chrysler/Jeep EPS Pump" (threads/206150) is the most likely public source but
couldn't be read from the cloud session. Openinverter threads would also have to be pasted in by hand.

### Control from the ZombieVerter

1. **Hang Tight PSC (recommended to start).** It sends the CAN messages to the pump and takes a
   potentiometer or PWM input. A ZV PWM output can set the assist level, for example from vehicle speed.
2. **Have the ZV send the messages directly.** First get the protocol by logging:
   - a PSC's output on the pump wires at several assist settings, or
   - a 2011–15 donor car's high-speed CAN at the pump connector while idling, turning and driving.

   Check the logs for rolling counters or checksums. Those would mean the ZV has to generate the
   frames, not just replay them.

## 12 V system / DC-DC sizing

A 1 kW DC-DC (about 72 A at 13.8 V) easily covers the pump's average. A normal car battery supplies
the brief 80 A bursts, and the DC-DC recharges it afterward.

Whole-truck 12 V estimate with LED lighting and a small EV radiator **[inferred]**:

| Load | Amps |
|---|---|
| Steering pump (average) | 10–15 |
| Brake vacuum pump | 5–15 |
| Coolant pumps (LDU, pack) | 10–15 |
| Radiator fan (small, often off) | 8–15 |
| LED lights | 3–5 |
| HVAC blower (high) | 15–20 |
| Wipers, rear defrost, seat heaters | 10–25 |
| ZV, OI board, BMS, contactors, truck modules | 5–10 |

- **Normal driving:** about 35–50 A, so 1 kW has plenty of margin.
- **Worst case** (cold, rainy night with everything on): about 70–110 A. That's marginal for 1 kW,
  and the battery makes up the short-term difference.
- **Recommendation:** 1 kW is workable. About 1.5 kW leaves room for things added later. Have the
  ZV watch 12 V and warn if it drops below about 12.4 V while driving.
- **A/C:** an electric A/C compressor still needs a condenser fan at low speed.

## Open items

- Confirm the truck has a vacuum brake booster and not hydroboost. Hydroboost would change the
  pump requirements.
- Get the CAN protocol, either from the diyelectriccar thread or by logging.
- Choose the pump year range (2011–13 or 2014–15) before buying the pump and controller.
- Pump mounting and hose routing to the GMT900 rack.

## Sources

- Hang Tight, "Everything I know about Electric Power Steering":
  https://hangtight.io/blogs/resources/everything-i-know-about-electric-power-steering
- Hang Tight PSC 2.0 instructions (Dodge pinout, fuse, wire):
  https://hangtight.io/blogs/resources/hang-tight-psc-2-0-instructions
- Hang Tight Dodge EHPS parts list:
  https://hangtight.io/blogs/resources/upgrading-to-electric-hydraulic-power-steering-parts-list
- Reform Motorsports 2011–13 Dodge/Jeep pump controller:
  https://reformmotorsports.com/product/dodge-electric-power-steering-pump-controller-ehps-micro/
- "Inside Chrysler EHPS Systems":
  https://www.tomorrowstechnician.com/inside-chrysler-ehps-systems/
- diyelectriccar, "Dodge/Chrysler/Jeep EPS Pump" (not read):
  https://www.diyelectriccar.com/threads/dodge-chrysler-jeep-eps-pump.206150/
