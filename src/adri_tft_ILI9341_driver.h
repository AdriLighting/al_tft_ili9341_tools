
#ifndef ADRI_TFT_ILI9341_DRIVER_H
   #define ADRI_TFT_ILI9341_DRIVER_H

		#include "SPI.h"
		#ifdef ESP32 
			#include <Adafruit_ILI9341.h>
		#else
			#include <Adafruit_ILI9341esp.h>
		#endif
		#include <Adafruit_GFX.h>
		#include <XPT2046.h>

		
		void tft_setup(Adafruit_ILI9341 * tft, XPT2046  * touch);
		void tft_setup(Adafruit_ILI9341 * tft);
#endif

