#ifndef ADRI_TFT_ILI9341_EXTRAFUNC_H
	#define ADRI_TFT_ILI9341_EXTRAFUNC_H
	#include <Arduino.h>

		#ifdef ESP32 
			#include <Adafruit_ILI9341.h>
		#else
			#include <Adafruit_ILI9341esp.h>
		#endif

		void draw7Number(Adafruit_ILI9341 * _tft, long n, unsigned int xLoc, unsigned int yLoc, char cS, unsigned int fC, unsigned int bC, char nD) ;
		void draw7Number180(Adafruit_ILI9341 * _tft, long n, unsigned int xLoc, unsigned int yLoc, char cS, unsigned int fC, unsigned int bC, char nD) ;

		int fillArc2(Adafruit_ILI9341 * _tft, int x, int y, int start_angle, int seg_count, int rx, int ry, int w, uint16_t colour);
		int semiArc(Adafruit_ILI9341 * _tft, int value, int vmin, int vmax, int x, int y, int r, int incli, int angle, int w, uint16_t scheme);	
#endif		