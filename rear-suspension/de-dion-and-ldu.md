# De Dion rear end and LDU placement

Compiled 2026-10-04. Nothing here has been checked on the truck.

## Stock GMT900 1500 rear axle (2007–2013) — what comes out

- Solid live axle, semi-floating, C-clip retained shafts
- Housing: GM 8.6 in (10-bolt) on most 1500s; some 6.2L / heavy-towing trucks got the 9.76 in (14-bolt) semi-float. Check the glovebox RPO codes.
- Leaf springs outboard of the frame rails (Hotchkiss); the springs carry and locate the axle
- Wheel bolt pattern: 6 x 5.5 in (6 x 139.7 mm)

## De Dion layout

- LDU on the frame. A rigid tube ties the two hubs together; half-shafts with plunging CV joints carry drive.
- Keeps the track width and constant camber. The axle and leaf springs are replaced by the 4-link and coilovers (see [four-link.md](four-link.md)).
- Needs hub uprights on the tube ends and a Panhard bar (or Watts link) for lateral location.
- The tube center section routes **ahead** of the LDU at about s = 130 in, in the open space between the battery box (ends ~103 in) and the LDU.
- Removing the solid axle saves roughly 100 lb net at the rear, most of it unsprung **[inferred]**.

## LDU placement

From the LDU scan (05_18_17_Tesla_Motor_1.stl) placed in the truck model.

- LDU size ~24.3 x 33.6 x 12.9 in (617 x 853 x 327 mm). Output bores on the narrow center gearbox section, ~7.7 in apart, common axis.
- **Turned 180°** from Tesla's orientation so the motor sits **behind** the axle line. This keeps the half-shafts clear of the 4-link.
- Output axis on the rear axle line (s = 143.6) at wheel-center height, output midpoint on the truck centerline.
- LDU spans s = 135.6–159.9 in; approximate center of mass ~5.7 in behind the axle.
- Clearance to kept structure (frame rails, bed) more than 5.3 in once the stock axle, driveshaft and leaf springs are gone.
- Height 11.1–24.0 in off the ground with the model's ~35.4 in tires; about 2 in lower (bottom ~9 in) with stock ~31.5 in tires.
- Placement transform (LDU STL mm → truck model mm, +X forward, Z up): rotation diag(-1,1,-1), translation (-4765.1, -347.5, 437.85).

## Running the LDU reversed (required by the 180° orientation)

Full notes, including gear, bearing and seal concerns: [../drive-unit/tesla-ldu-reversed.md](../drive-unit/tesla-ldu-reversed.md).

- **Motor direction:** set `dirmode` on the openinverter LDU board to SwitchReversed (3) or ButtonReversed (2). In stm32-sine `VehicleControl::SelectDirection()`, the Reversed flag multiplies the forward/reverse inputs by -1. The ZombieVerter sends forward/reverse as CAN bits (Can_OI.cpp: forward = bit 8, reverse = bit 16), so ZV forward makes the motor spin reversed.
- Keep ZV `dirmode` normal so ZV `dir` (and the GMT900 PRNDL frame 0x1F5 and reverse lamps) still means truck direction. Do **not** reverse on the ZV side: its Reversed flag only applies to din_forward/din_reverse and would flip `dir`.
- **Verify on the bench at low torque before driving.**
- **Lubrication:** the factory LDU oil pump only circulates properly in Tesla's forward direction. Fit a reverse-drive oil pump:
  - [Westside EV LDU Reverse Drive Oil Pump](https://www.westside-ev.com/store/p/tesla-ldu-reverse-drive-oil-pump-replacement) ($275, reuses the two OEM pump gears in a new housing)
  - Also sold by [Fellten](https://shop.fellten.com/shop/ldurdop-tesla-ldu-reverse-drive-oil-pump-replacement-12881) and [Stealth EV](https://stealthev.com/product/tesla-ldu-reverse-drive-oil-pump/)

## Fabrication materials — starting points, not engineered values

| Part | Material | Thickness | Source |
|---|---|---|---|
| De Dion tube | DOM mild steel or 4130 round (~3.0 in OD) or 3 x 3 in square | 0.250 in wall | Steel supplier (SendCutSend tube tops out at 1.75 x 0.095 in, 60 in long) |
| Hub mounting plates (GM 3-bolt flange) | A36 | 0.500 in | SendCutSend |
| Upright side plates / gussets | A36 | 0.375 in (or 0.250 in boxed) | SendCutSend |
| Coilover and link brackets on the beam | A36 | 0.250 in | SendCutSend |
| Panhard / Watts brackets | A36 or 4130 | 0.250 in | SendCutSend |
| LDU frame mounts | A36 0.375 in or 4130 0.250 in | — | SendCutSend |

- SendCutSend stock: mild steel 0.030–0.500 in, 4130 sheet 0.050–0.250 in ([materials](https://sendcutsend.com/materials/))
- Prefer mild steel: MIG-weldable without preheat; 4130 weight savings are small here
- Have the tube and hub mounts checked (FEA or an experienced builder) before cutting
