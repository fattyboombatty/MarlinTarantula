/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 *
 */
#pragma once

/**
 * printer_event_leds.h - LED color changing based on printer status
 */

#include "leds.h"
#include "../../inc/MarlinConfig.h"

class PrinterEventLEDs {
private:
  static uint8_t old_intensity;

  #if HAS_LEDS_OFF_FLAG
    static bool leds_off_after_print;
  #endif

public:
  #if HAS_TEMP_HOTEND
    FORCE_INLINE static void onHotendHeatingStart() { old_intensity = 0; }
    static void onHotendHeating(const float &start, const float &current, const float &target);
  #endif

  #if HAS_HEATED_BED
    FORCE_INLINE static void onBedHeatingStart() { old_intensity = 127; }
    static void onBedHeating(const float &start, const float &current, const float &target);
  #endif

  #if HAS_TEMP_HOTEND || HAS_HEATED_BED
    FORCE_INLINE static void onHeated()     { leds.set_white(); }
    FORCE_INLINE static void onHeatersOff() { leds.set_off(); }
  #endif

  #if ENABLED(SDSUPPORT)

    FORCE_INLINE static void onPrintCompleted() {
      leds.set_green();
      #if HAS_LEDS_OFF_FLAG
        leds_off_after_print = true;
      #else
        safe_delay(2000);
        leds.set_off();
      #endif
    }

    FORCE_INLINE static void onResumeAfterWait() {
      #if HAS_LEDS_OFF_FLAG
        if (leds_off_after_print) {
          leds.set_off();
          leds_off_after_print = false;
        }
      #endif
    }

  #endif // SDSUPPORT
};

extern PrinterEventLEDs printerEventLEDs;
