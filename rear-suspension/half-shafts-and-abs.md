# Half-shafts, hubs and ABS

Compiled 2026-10-04. Items marked VERIFY still need confirming on the truck or parts.

## Tesla Model S / LDU rear half-shaft (2012–2021 rear, GSP NCV99029 / TrakMotive TS-8003)

Sources: [finditparts listing for GSP NCV99029](https://www.finditparts.com/products/13092213/gsp-auto-parts-north-america-inc-ncv99029); length also from [TrakMotive TS-8003 listing](https://partshawk.com/tesla-s-rear-left-cv-axle-shaft-trakmotive-ts-8003.html)

| Item | Value |
|---|---|
| Outboard (wheel hub) splines | 30 |
| Inboard (LDU) splines | 29 |
| Axle nut thread | M24 x 1.5 (32 mm socket) |
| Compressed length | 34.17 in (868 mm) |
| Extended length | 36.18 in |
| Outboard spline major diameter / pitch | Not published — VERIFY with calipers / spline gauge |

## GMT900 1500 4WD front hub (MOOG 515096 and equivalents) — used on the beam ends

Source: [MOOG 515096 specifications](https://afa-motors.com/products/moog-515096-wheel-bearing-hub-assembly)

| Item | Value |
|---|---|
| Splines | 33 |
| Wheel studs / bolt circle | 6 on 5.5 in (139.7 mm) |
| Knuckle mount | 3-bolt triangular flange |
| Hub pilot | 3.087 in (78.4 mm) |
| ABS | Integrated wheel speed sensor with pigtail |

## Hybrid half-shaft

- Tesla inner joint (29 splines into the LDU) + GMT900 4WD outer CV joint (33 splines into the GM hub) on a custom-length bar. Custom axle shops build these.
- Keeps the truck's 6 x 5.5 wheels and GM brake hardware, no wheel adapters.
- Geometry: LDU output faces ±3.84 in from centerline, hub faces ~±34–35 in, so ~28 in CV center span per side. ±4 in of travel is about 8° of joint angle and ~0.25 in of plunge.
- VERIFY with the axle builder: a common bar diameter for both joints, and CV torque capacity for LDU launch torque on a loaded truck.
- Fallback if the hybrid shaft doesn't work out: Model S rear hub bearing/knuckle on the beam ends (Tesla shafts fit as designed), but that brings 5 x 120 mm wheels, adapters, or a custom 6 x 5.5 hub flange.

## ABS wheel speed sensors

The stock rear sensors and tone rings leave with the axle. The EBCM still needs rear wheel speeds for ABS, StabiliTrak and likely the speedometer.

### Tooth counts

| Location | Model years | Tooth count | Source |
|---|---|---|---|
| Front hub, 1500 4WD (GMT900) | 2007–2013 trucks (2007–2014 SUVs) | 55 | [irate4x4 swap thread](https://irate4x4.com/threads/2011-silverado-1500-keeping-abs-happy-after-sas.407815/) (single source — VERIFY) |
| Rear axle ring, GM 8.6 in (GMT900) | 2007–2015 Silverado/Sierra 1500, Tahoe, Suburban | 32 (GM 40030759) | [CM Gearworks](https://cmgearworks.com/product/2007-gm-8-6-10-bolt-rear-axle-abs-exciter-tone-ring-chevy-tahoe-suburban-1-2-ton/); Yukon [YSPABS-021](https://www.americantrucks.com/yukon-gear-silverado1500-abs-wheel-speed-sensor-tone-ring-yspabs-021.html) |

- A 2012 Silverado's EBCM expects 55T at the front and 32T at the rear.
- GMT900 4WD front hubs (55T) at the rear would read ~1.72x fast, giving wheel-speed mismatch, ABS/TC faults and possible speedo error.

### Plan

- Keep a 32T reluctor and the stock GMT900 rear wheel speed sensor at each rear hub. Press a 32T ring (GM 40030759 or Yukon YSPABS-021) onto a machined sleeve on the hub or outer CV, bracket the stock rear sensor to the upright at the correct air gap, and leave the hub's built-in sensor unplugged.
- VERIFY ring ID/OD against the sleeve, and the sensor air gap.
- VERIFY sensor type (passive 2-wire vs active) for both the front-hub and the rear sensors.

### How to check the truck

- Rear: lift a wheel, remove the sensor, mark a tooth through the sensor hole with a paint pen, rotate and count; or scope the sensor and count pulses over one wheel revolution.
- Front hub: the tone ring is sealed inside the hub bearing unit. Count electrically (scope or logic analyzer across the sensor pins over one marked revolution) or cut a scrap hub in half.
