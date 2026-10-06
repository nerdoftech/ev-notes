# Battery cooling

The Model S modules have built-in coolant channels, so liquid cooling is the natural fit. Keep the battery loop separate from the LDU loop, because the drive unit's coolant runs hotter than the cells should see.

## Start simple: radiator only

Plenty of conversions cool the pack with just a radiator.
- The battery gets its own small radiator and 12 V pump.
- A bypass valve goes around the radiator, so it isn't chilling the pack in winter while the heater warms it.
- The loop is pump → plate heat exchanger (winter heat) → front and rear packs in parallel → radiator, with no refrigerant work.
- Leave a pair of capped tees in the line, so a chiller can drop in later.
- Run the packs in parallel, not in series. Series is simpler to plumb, but the rear pack would always run warmer.

The limit is that a radiator can only bring the pack down to a bit above outside air temperature. On a 100 °F day the pack sits around 105–115 °F **[inferred]**, close to the 45 °C (113 °F) where you want the cells to stop climbing. At street loads, 16 modules make only a few hundred watts of heat **[inferred]**. The pack gets hot from sustained hard use: back-to-back drag passes, towing up grades, or summer DC fast charging. Whether radiator-only is enough depends on summer climate and fast charging plans.

## Adding a chiller later

A chiller is a refrigerant-to-coolant heat exchanger on the A/C circuit, in parallel with the cabin evaporator.

| Type | Valve block | Heat exchanger | Notes |
|---|---|---|---|
| TXV + solenoid (2012–2016 Model S) | 6007362-00-C / -D | Tesla 1007476-00-x (Modine 1E006773) | **Pick this one.** 12 V solenoid the ZV switches; R134a |
| TXV, no solenoid | 1019541-00-B | Tesla 1019540-00-C (Modine 1E006836) | Needs a separate inline A/C solenoid valve |
| EXV (2016+ S/X) | n/a | 1037357-00-x, 1037764-00-C | Electronic expansion valve needs its own controller. Avoid |

Sources: [Autobahn Parts (1007476-00-C)](http://www.autobahnparts.com/part/tesla-model-s-2012-2016-oem-hvb-chiller-valve-txv-solenoid-part-1007476-00-c), [Stealth EV (6007362-00-D)](https://stealthev.com/product/6007362-00-d-tesla-chiller-valve/), [TCar Service](https://tcarservice.com/en/product/tesla-model-s-2016-2021-x-2015-2021-hv-battery-chiller-with-exv-electronic-expansion-valve-oem-pre-owned-1037357-00-g). The alternative is a generic brazed plate refrigerant chiller with a universal TXV and solenoid. Volt and Bolt chillers exist, but they're hard to find as standalone parts.

## Does the chiller hurt cabin cooling?

Not in practice. Tesla runs this compressor on both the cabin and the battery in the Model S and the larger Model X.
- The compressor is variable speed with up to about 4.5 kW draw, roughly the cooling output of the stock belt-driven unit **[inferred]**. A cab at steady temperature needs only a fraction of that.
- The ZV gives the cabin priority. The chiller solenoid stays closed during cab pull-down. It opens only when the pack is above about 35 °C and the cab is comfortable, or the truck is parked and charging.
- The one time you'd notice is a heat-soaked truck right after drag runs. The ZV briefly favors the pack, so the cab cools a little slower for a few minutes.
