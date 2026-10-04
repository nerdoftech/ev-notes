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

#ifndef GM_GMT900_h
#define GM_GMT900_h

/*  GM GMT900 (2007-2014 Silverado/Sierra) high-speed GMLAN support.
    Replaces the periodic frames of the removed ECM and TCM so the BCM,
    cluster and EBCM keep working. Draft: byte layouts and rates are from
    other GM vehicles and must be confirmed with a capture of a stock truck.
*/

#include "vehicle.h"
#include <stdint.h>

class GM_GMT900 : public Vehicle {
public:
  GM_GMT900();
  void SetCanInterface(CanHardware *c);
  void Task1Ms();
  void DecodeCAN(int id, uint32_t *data);
  void SetRevCounter(int speed) { rpm = speed; }
  void SetTemperatureGauge(float temp) { heatsinkTemp = temp; }
  void SetFuelGauge(float level) { soc = level; }
  bool Ready();

  // Bit n enables frame n of the schedule table (see GM_GMT900.cpp)
  void SetEnableMask(uint32_t mask) { enableMask = mask; }

private:
  struct Frame {
    uint16_t id;
    uint16_t period; // in 0.5 ms units
    uint16_t offset; // first send, in 0.5 ms units, to stagger frames
  };

  bool BuildFrame(int index, uint8_t *data);
  bool BuildTemplate(int index, uint8_t *data);
  void Build0C9(uint8_t *data);
  void Build1A1(uint8_t *data);
  void Build1C3(uint8_t *data);
  void Build1F5(uint8_t *data);
  void Build4C1(uint8_t *data);
  void Build4D1(uint8_t *data);
  uint8_t GaugeCoolantTemp();
  uint8_t ThrottleByte();

  static const Frame frames[];
  static const int numFrames;

  uint32_t enableMask;
  uint32_t halfMs;
  uint32_t nextSend[16];
  uint8_t rollingCtr;
  int rpm;
  float heatsinkTemp;
  float soc;
  float vehicleSpeed;
  uint8_t powerMode;
};

#endif /* GM_GMT900_h */
