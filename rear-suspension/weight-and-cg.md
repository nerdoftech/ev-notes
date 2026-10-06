# Weight distribution and CG height

Compiled 2026-10-04. All weights are estimates **[inferred]**; weighing the truck replaces them.

## Battery layout

16 Tesla Model S modules (~55 lb each), 96s like a stock Tesla pack (~403 V full, ~355 V nominal, ~85 kWh), about 1,050 lb with enclosures and BMS:
- 6 modules where the engine was, 3 rows two high, centered ~5 in behind the front axle
- 10 modules between the frame rails, two layers of 5, each module turned across the truck, in a box ~62 in long (~670 lb with its share of enclosure and BMS)

Changed from 14 modules (6 + 8) on 2026-10-06 to get the full 96s pack voltage the LDU and 400 V accessories are built for.

The 4-link links run outboard of the rails, so neither box interferes with them.

## Axle loads **[inferred]**

Baseline: 2012 Silverado 1500 2WD crew cab, ~5,200 lb, ~57/43, with the engine, transmission, fuel tank and exhaust removed and the LDU and accessories added.

| 10-module box position | Front axle | Rear axle | Split |
|---|---|---|---|
| Stock | 2,964 lb | 2,236 lb | 57/43 |
| Forward, right behind the front crossmember (center ~53 in) | 3,082 lb (+118) | 2,577 lb (+341) | 54.5/45.5 |
| Back, ending near ~103 in (center ~72 in) | 2,994 lb (+30) | 2,665 lb (+429) | 53/47 |

Positions are inches behind the front axle (143.6 in wheelbase). Total ~5,660 lb, about 135 lb more than the 14-module layout.

- **Front:** 30–120 lb over stock. The back position keeps the stock front springs and ride height; the forward position may sit slightly low.
- **Rear:** 340–430 lb heavier before cargo. The coilover spring rates are picked for this.
- Removing the solid axle for the de Dion saves roughly 100 lb net at the rear, moving the split about one more point forward.
- The engine-bay modules sit higher than a frame-rail pack, so the CG rises slightly compared with putting everything between the rails.

## CG height

- Used in the 4-link numbers: 26 in **[inferred]**.
- Quick check without measuring: NHTSA's rollover rating gives a Static Stability Factor (SSF). CG height = track width / (2 x SSF). With a ~68 in track and an SSF around 1.2, the stock truck comes out near 28 in **[inferred]**; look up the exact cab/drive on safercar.gov. The conversion should lower it a little.

### Measuring it (axle-lift method)

1. **Lock the suspension at ride height, all four corners.** Tilting moves ~140 lb to the rear; spring movement of ~0.5 in shifts the body enough to change the reading by ~20 lb, worth ~3 in of CG height **[inferred]**.
   - Rear: replace the shocks with solid links of the same eye-to-eye length.
   - Front: hardwood block between each lower control arm and its bump stop, plus a chain or ratchet strap from the arm to the frame.
   - Measure hub-center heights before and after locking.
2. **Weigh level:** total weight W and rear axle weight. With one set of axle scales, weigh the rear, roll forward, weigh the front, and add.
3. **Raise the front:** rear tires stay on the scales, parking brake on, rear chocked. Lift on cribbing, ramps, or jack stands under the lower control arms near the ball joints (not under the frame). H is how far the front **hub center** rises; 20 in stands under the control arms give roughly 10–12 in.
4. **Weigh the rear again.** ΔW = rear raised − rear level.
5. **Calculate:** CG height = r + (L x ΔW) / (W x tan θ), where sin θ = H / L and r is the wheel-center height.

- Sensitivity: a 24 in lift moves only ~6–7 lb per inch of CG height **[inferred]**, so use scales that read to 5 lb or better and average three or four lifts.
- Do it once on the stock truck before teardown. The converted CG can then be calculated from each removed and added part's weight and height.
