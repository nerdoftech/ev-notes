/*
 * This file is part of the stm32-vcu project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GM_GMT900.h"
#include "digio.h"
#include "params.h"

/////////////////////////////////////////////////////////////////////////////
// Tuning constants
/////////////////////////////////////////////////////////////////////////////
static const float kTachScale = 1.0f;    // motor rpm -> tach rpm
static const int kIdleRpm = 700;         // fake idle shown while READY
static const int kMaxTachRpm = 6500;
static const float kGaugeNormal = 90.0f; // coolant gauge "normal", degC
static const float kHeatsinkCool = 60.0f; // below this the needle sits at normal
static const float kGaugeHot = 120.0f;   // shown when heatsink hits tmphsmax

// Frames with known contents are on by default: 0x0C9, 0x1F5, 0x4C1, 0x4D1.
// Turn others on one at a time and watch which warnings go away.
static const uint32_t kDefaultEnableMask =
    (1 << 0) | (1 << 6) | (1 << 12) | (1 << 14);

/////////////////////////////////////////////////////////////////////////////
// Schedule. Periods and offsets are in 0.5 ms units so 12.5 ms frames
// average 12.5 ms on the 1 ms task. Rates come from a capture of a
// different GM vehicle: confirm every one on a stock GMT900.
/////////////////////////////////////////////////////////////////////////////
const GM_GMT900::Frame GM_GMT900::frames[] = {
    // id     period  offset
    {0x0C9, 25, 0},    //  0 ECM  RPM, run status, brake     80 Hz
    {0x0F9, 25, 3},    //  1 ECM/TCM undecoded              80 Hz
    {0x1A1, 50, 6},    //  2 ECM  accelerator position      ~40 Hz (guess)
    {0x1C3, 50, 9},    //  3 ECM  torque / accelerator      40 Hz
    {0x1ED, 25, 12},   //  4 ECM  undecoded                 80 Hz
    {0x1EF, 25, 15},   //  5 ECM  RPM (alternate)           80 Hz
    {0x1F5, 50, 18},   //  6 TCM  PRNDL, tow/haul           40 Hz
    {0x2C3, 100, 21},  //  7 ECM  undecoded                 20 Hz
    {0x3C1, 200, 24},  //  8 ECM  undecoded status          10 Hz
    {0x3D1, 200, 27},  //  9 ECM  undecoded status          10 Hz
    {0x3F9, 500, 30},  // 10 ECM/TCM undecoded               4 Hz
    {0x3FB, 500, 33},  // 11 ECM/TCM undecoded               4 Hz
    {0x4C1, 1000, 36}, // 12 ECM  coolant, IAT, OAT          2 Hz (verify)
    {0x4C9, 1000, 39}, // 13 TCM? possibly trans temp        slow (guess)
    {0x4D1, 1000, 42}, // 14 ECM  oil temp, fuel level?      2 Hz (verify)
};
const int GM_GMT900::numFrames = sizeof(frames) / sizeof(frames[0]);

GM_GMT900::GM_GMT900()
    : enableMask(kDefaultEnableMask), halfMs(0), rollingCtr(0), rpm(0),
      heatsinkTemp(20), soc(0), vehicleSpeed(0), powerMode(0) {
  for (int i = 0; i < numFrames; i++)
    nextSend[i] = frames[i].offset;
}

void GM_GMT900::SetCanInterface(CanHardware *c) {
  can = c;

  can->RegisterUserMessage(0x3E9); // EBCM wheel / vehicle speed
  can->RegisterUserMessage(0x1F1); // BCM power mode
}

bool GM_GMT900::Ready() { return DigIo::t15_digi.Get(); }

void GM_GMT900::Task1Ms() {
  // Only talk while the key is on so the BCM can sleep the bus at key-off
  if (!Ready()) {
    halfMs = 0;
    for (int i = 0; i < numFrames; i++)
      nextSend[i] = frames[i].offset;
    return;
  }

  halfMs += 2;

  for (int i = 0; i < numFrames; i++) {
    if ((int32_t)(halfMs - nextSend[i]) < 0)
      continue;

    nextSend[i] += frames[i].period;

    if (!(enableMask & (1UL << i)))
      continue;

    uint8_t data[8] = {0};

    if (BuildFrame(i, data))
      can->Send(frames[i].id, data, 8);
  }
}

bool GM_GMT900::BuildFrame(int index, uint8_t *data) {
  switch (frames[index].id) {
  case 0x0C9:
    Build0C9(data);
    return true;
  case 0x1A1:
    Build1A1(data);
    return true;
  case 0x1C3:
    Build1C3(data);
    return true;
  case 0x1F5:
    Build1F5(data);
    return true;
  case 0x4C1:
    Build4C1(data);
    return true;
  case 0x4D1:
    Build4D1(data);
    return true;
  default:
    return BuildTemplate(index, data);
  }
}

// Undecoded frames: paste in bytes captured from a stock truck at idle.
// Until then nothing is sent, so nothing goes on the bus blind.
bool GM_GMT900::BuildTemplate(int index, uint8_t *data) {
  (void)data;

  switch (frames[index].id) {
  // Example once captured:
  // case 0x0F9: {
  //   const uint8_t t[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  //   memcpy(data, t, 8);
  //   return true;
  // }
  default:
    return false;
  }
}

// 0x0C9 engine general status. RPM in bytes 2-3 (0.25 rpm/bit), brake bit
// in the 6th byte. TODO from capture: exact byte/bit positions, run/crank
// status bits, and whether the frame carries a rolling counter.
void GM_GMT900::Build0C9(uint8_t *data) {
  int opmode = Param::GetInt(Param::opmode);
  int tach = rpm * kTachScale;

  if (opmode == MOD_RUN && tach < kIdleRpm)
    tach = kIdleRpm;
  else if (opmode != MOD_RUN)
    tach = 0;
  if (tach > kMaxTachRpm)
    tach = kMaxTachRpm;

  uint16_t raw = tach * 4;
  data[1] = raw >> 8;
  data[2] = raw & 0xFF;

  // TODO: engine running / crank bits, assumed in byte 0
  if (opmode == MOD_RUN)
    data[0] |= 0x80;

  // TODO: confirm bit position
  if (Param::GetBool(Param::din_brake))
    data[5] |= 0x01;

  rollingCtr = (rollingCtr + 1) & 0x3;
}

// 0x1A1 accelerator position in byte 8, 0-254. Layout from another GM vehicle.
void GM_GMT900::Build1A1(uint8_t *data) { data[7] = ThrottleByte(); }

// 0x1C3 accelerator position in byte 8, 0-254. TODO: torque fields from a
// capture. The EBCM needs them or StabiliTrak/TCS will set faults.
void GM_GMT900::Build1C3(uint8_t *data) { data[7] = ThrottleByte(); }

// 0x1F5 transmission status. Byte 4 = PRNDL (1 P, 2 R, 3 N, 4 D),
// byte 6 = tow/haul. The ZV's dir must mean truck direction: run the LDU
// reversed with dirmode on the inverter, not on the ZV.
void GM_GMT900::Build1F5(uint8_t *data) {
  uint8_t prndl;

  switch (Param::GetInt(Param::dir)) {
  case -1:
    prndl = 2;
    break;
  case 0:
    prndl = 3;
    break;
  case 1:
    prndl = 4;
    break;
  default:
    prndl = 1;
    break;
  }

  if (Param::GetInt(Param::opmode) != MOD_RUN)
    prndl = 1; // Park unless the ZV is in run mode

  data[3] = prndl;
  data[5] = 0; // tow/haul off
}

// 0x4C1 temperatures, bytes 2-4 = coolant, intake air, outside air (A-40).
void GM_GMT900::Build4C1(uint8_t *data) {
  data[1] = GaugeCoolantTemp();
  data[2] = 25 + 40; // TODO: intake air, fixed 25 degC
  data[3] = 25 + 40; // TODO: outside air, should probably come from BCM
}

// 0x4D1 engine oil temp in byte 2 (A-40). TODO from capture: oil pressure
// byte and fuel level byte. The fuel byte is how SOC reaches the fuel gauge.
void GM_GMT900::Build4D1(uint8_t *data) {
  data[1] = GaugeCoolantTemp();
  // data[?] = soc * 255 / 100; // TODO: fuel level byte and scaling
}

// Needle sits at normal until the inverter heatsink gets warm, then climbs
// so tmphsmax shows as hot.
uint8_t GM_GMT900::GaugeCoolantTemp() {
  float hsMax = Param::GetFloat(Param::tmphsmax);
  float t = kGaugeNormal;

  if (heatsinkTemp > kHeatsinkCool && hsMax > kHeatsinkCool)
    t += (heatsinkTemp - kHeatsinkCool) * (kGaugeHot - kGaugeNormal) /
         (hsMax - kHeatsinkCool);
  if (t > 150)
    t = 150;

  return (uint8_t)(t + 40);
}

uint8_t GM_GMT900::ThrottleByte() {
  float pot = Param::GetFloat(Param::potnom);

  if (pot < 0)
    pot = 0;
  if (pot > 100)
    pot = 100;

  return (uint8_t)(pot * 254 / 100);
}

void GM_GMT900::DecodeCAN(int id, uint32_t *data) {
  uint8_t *bytes = (uint8_t *)data;

  switch (id) {
  case 0x3E9:
    // TODO: placeholder, confirm layout and scaling from a capture
    vehicleSpeed = ((bytes[0] << 8) | bytes[1]) * 0.01f;
    break;
  case 0x1F1:
    // TODO: placeholder, confirm which bits carry the power mode
    powerMode = bytes[0] & 0x0F;
    break;
  default:
    break;
  }
}
