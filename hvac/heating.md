# Cabin and battery heating

## Diesel coolant heater

A 5 kW coolant heater plugs straight into the stock GMT900 heater core hoses. The factory HVAC box, blend doors, vents and defrost stay unchanged. Examples: Webasto Thermo Top C/Evo 5, Eberspächer Hydronic S3, or the cheaper copies. Coolant heaters start around 4–5 kW, which suits a truck cab plus pack warming.

What it needs:
- A 12 V circulation pump, since the engine water pump is gone. Some Thermo Top models have one built in.
- A small fuel tank, since the stock tank is coming out.
- An exhaust outlet and a combustion air inlet.
- An enable signal from the ZV (cabin heat requested, or pack cold).

The upside is about 5 kW of heat that doesn't come out of the pack. The downside is that it burns fuel and makes exhaust, so it shouldn't run unattended in a closed garage.

## Warming the battery with it

Don't run heater coolant through the modules. It leaves the heater at about 70–85 °C, and the cells shouldn't see coolant much above 30–40 °C. Instead:
- Put a brazed plate heat exchanger between the heater loop and a separate battery loop.
- The ZV runs the battery pump only when module temps are low, stopping around 15–20 °C.
- A heater leak or fault can't push hot or dirty coolant into the modules.

For warming the pack while it's plugged in and charging, a small electric heater on the battery loop is the cleaner option. Many builds use both.

## Plate heat exchanger

It's a small stainless block of stacked plates with four ports: two for the heater loop and two for the battery loop. The fluids pass in alternating layers and never mix. They're sold for hydronic floor heating, wood boilers and solar water heating. They're 316L stainless brazed with copper, which is fine with normal glycol coolant.

**Size:** a 3" × 8" unit, 16–20 plates, 3/4" MPT ports.
- Heat isn't the limit. The packs weigh about 360 kg **[inferred]**, so warming them 20 °C in an hour takes about 2 kW **[inferred]**. Even a palm-sized unit passes that with 80 °C on one side.
- Flow restriction is the limit. Small units with 1/2" ports choke a 12 V pump. Use 3/4" ports.
- You'll also need four 3/4" FPT-to-hose-barb fittings.

**Where to buy** (prices seen 2026-10-04):

| Seller | Unit | Price |
|---|---|---|
| [PexUniverse](https://pexuniverse.com/brazed-plate-heat-exchangers) | 3×8, 16 / 20 plate | ~$68 / ~$74 |
| [Alfa Heating Supply](https://alfaheating.com/products/brazed-plate-heat-exchanger-3x8-3-4-mpt) | WiseWater BL14, 3×8 | from ~$105 |
| [AltHeatSupply](https://altheatsupply.com/collections/3-x-8-stainless-steel) | 3×8, 20 plate | ~$127 |
| [eBay](https://www.ebay.com/shop/brazed-plate-heat-exchanger?_nkw=brazed+plate+heat+exchanger) | Generic 3×8 | from ~$85 |
