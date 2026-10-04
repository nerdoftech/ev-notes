# Rear 4-link — mostly street, occasional drag racing

Compiled 2026-10-04. Geometry is target-driven, not from kit drawings. Recheck everything in a 4-link calculator with the final parts, ride height and tires.
**[inferred]** marks Claude's estimates.

Assumptions: wheel center 17.75 in, CG height 26 in **[inferred]**, wheelbase 143.6 in.

## Why this isn't a normal drag 4-link

- On a live axle, the housing reacts the driveshaft torque and the links turn it into "hit" on the tires. Here the LDU is bolted to the frame, so drive torque goes straight into the frame. The links only carry the push from the wheel centers.
- So for inboard drive, the **neutral (100% anti-squat) line runs from the rear wheel center** (not the contact patch) to the CG height above the front axle. The instant center (IC) has to sit above wheel-center height to get any anti-squat.
- With parallel links, the same line from the axle to the IC also sets roll steer. Every 1% of roll oversteer buys only about 5.5% anti-squat (L/h = 143.6/26). High anti-squat and calm street manners can't happen at the same setting, so the brackets are adjustable.
- Brake torque does go through the links, because the brakes are on the beam. Anti-lift still uses the contact-patch line (~34% in this layout, assuming 35% rear brake share).
- Launch traction on an EV is best handled with the torque ramp and limits in the ZombieVerter / OI board rather than suspension "hit".

## Layout

- **Type:** parallel 4-link plus Panhard bar, links outboard of the frame rails (Suspension Engineering 2007–2018 kit brackets as the base). Triangulated uppers won't fit because the LDU sits where they would meet.
- **Lower links:** ~32 in long, near level at ride height. Frame bracket with 3–4 vertical holes about 1 in apart, 15.0 to 18.0 in high.
- **Upper links:** ~21.6 in long. 9 in vertical separation from the lowers at the beam (more separation means lower link loads).
- **Panhard bar:** as long as fits (45 in or more), level at ride height. Its height sets the rear roll center; aim for about 12–14 in. A Watts link avoids lateral shift but is harder to package around the LDU; a Panhard bar is fine for ±4 in of travel.
- **Rear anti-roll bar:** fit one on the beam. Coilovers give none of the roll stiffness the leaf springs did, and less body roll also means less roll steer.
- **Joints:** bushed ends (urethane or Johnny-joint type) at least at the frame end, for street noise and harshness.
- **Coilovers:** QA1 double-adjustable, so the street and launch settings differ without changing parts. Spring rates come from the final corner weights (see [weight-and-cg.md](weight-and-cg.md)).
- **Travel:** ±4 in, with bump stops before the coilovers bottom. Check CV angle and plunge at full travel (~8° and ~0.25 in in this layout).

## Pivot points (bracket centers)

| Point | s (in) | z (in) | abs(y) (in) |
|---|---|---|---|
| Lower link frame bracket | 105.5 | 15.0–18.0 (holes) | 24.5 |
| Upper link frame bracket | 115.5 | 21.5 | 24.5 |
| Lower link beam bracket | 137.1 | 13.25 | 24.5 |
| Upper link beam bracket | 137.1 | 22.25 | 24.5 |

## Bracket settings **[inferred]**

| Setting | Lower / upper frame hole (in) | IC ahead of beam bracket | IC height | Anti-squat | Roll oversteer |
|---|---|---|---|---|---|
| Street | 15.0 / 21.5 | ~100 in | 18.8 in | ~6% | ~1% |
| Street, firmer | 16.0 / 21.5 | ~74 in | 19.7 in | ~14% | ~2.6% |
| Drag | 17.0 / 21.5 | ~59 in | 20.2 in | ~23% | ~4.2% |
| Drag, max | 18.0 / 21.5 | ~49 in | 20.6 in | ~32% | ~5.8% |

- Above about 5% roll oversteer a heavy truck gets darty on bumpy roads and in lane changes. Use the drag holes at the track and return to a street hole afterwards.
- Every hole change moves the beam fore-aft slightly. Recheck wheel centering and CV plunge, and keep the links the same length side to side.
- In the street setting the beam pitches −1.5°/+2.8° and moves ≤0.35 in fore-aft over ±4 in of travel.
- A 2 in error in CG height changes anti-squat by only about a tenth of its value (for example 23% vs 21%), so the 26 in estimate is good enough for choosing holes.

## Clearance (street setting, truck model)

- Links clear the kept frame and body through ±4 in of travel. The closest points (~1.7–1.9 in) are at the frame-bracket ends beside the rail outer face, where the brackets mount.
- The lower frame bracket sits ~2.5 in behind the between-rails battery box (ends ~103 in). Check the cab's rear body mount near s = 103–106.
