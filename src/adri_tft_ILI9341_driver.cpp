#include "adri_tft_ILI9341_driver.h"


static void calibratePoint(Adafruit_ILI9341 * tft, XPT2046 * touch, uint16_t x, uint16_t y, uint16_t &vi, uint16_t &vj) {
	// Draw cross
	
	
	tft->drawFastHLine(x - 8, y, 16,0xff);
	tft->drawFastVLine(x, y - 8, 16,0xff);
	while (!touch->isTouching()) {
		delay(10);
	}

	touch->getRaw(vi, vj);
	char buf[80];

	sprintf(buf,  "calibratePoint: %d,%d", (int)vi, (int)vj);
	Serial.println(buf);

	// Erase by overwriting with black
	tft->drawFastHLine(x - 8, y, 16, 0);
	tft->drawFastVLine(x, y - 8, 16, 0);
}

void calibrate(Adafruit_ILI9341 * tft, XPT2046 * touch) {
	uint16_t x1, y1, x2, y2;
	uint16_t vi1, vj1, vi2, vj2;
	touch->getCalibrationPoints(x1, y1, x2, y2);
	calibratePoint(tft, touch, x1, y1, vi1, vj1);
	delay(1000);
	calibratePoint(tft, touch, x2, y2, vi2, vj2);
	touch->setCalibration(vi1, vj1, vi2, vj2);

	char buf[80];
	sprintf(buf,  "%d,%d,%d,%d", (int)vi1, (int)vj1, (int)vi2, (int)vj2);
	Serial.println(buf);
}

void tft_setup(Adafruit_ILI9341 * tft, XPT2046 * touch){
		#if defined(ESP8266)
			SPI.setFrequency(ESP_SPI_FREQ);
		#elif defined(ESP32)
		#else
		#endif    	

		tft->begin();		


		tft->fillScreen(ILI9341_BLACK);

		touch->begin(tft->width(), tft->height());  // Must be done before setting rotation
		// Serial.print("tftx ="); Serial.print(tft->width()); Serial.print(" tfty ="); Serial.println(tft->height());
		// calibrate( tft,  touch);

  		touch->setCalibration(156,1760,1736,267); // x2 1
  		// touch->setCalibration(1839,268,312,1784); // x2 1
  		// touch->setCalibration(177,1712,1824,241); // x2 1
  		// touch->setCalibration(1887,1799,352,288); // x1 1

		tft->setRotation(3);
		touch->setRotation(touch->ROT270);


}
void tft_setup(Adafruit_ILI9341 * tft){
		#if defined(ESP8266)
			SPI.setFrequency(ESP_SPI_FREQ);
		#elif defined(ESP32)
		#else
		#endif  

		tft->begin();		

		tft->fillScreen(ILI9341_BLACK);

		tft->setRotation(3);
}
