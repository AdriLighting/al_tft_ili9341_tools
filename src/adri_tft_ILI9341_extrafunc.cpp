#include "adri_tft_ILI9341_extrafunc.h"
#include <JPEGDecoder.h>




void drawJPEGFromSpiffs(Adafruit_ILI9341 * _tft, String filename, int xpos, int ypos) {
  char buffer[filename.length() + 1];
  filename.toCharArray(buffer, filename.length() + 1);
  
  JpegDec.decodeFile(buffer);
  uint8_t zoomFactor = 2;
  uint16_t  *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  // uint32_t mcu_pixels = mcu_w * mcu_h; // total pixels
  // TODO immplmenet something to track drawtime performance
  // uint32_t drawTime = millis();

  while( JpegDec.read()){
	
	pImg = JpegDec.pImage;
	int mcu_x = (JpegDec.MCUx * mcu_w) / zoomFactor + xpos;
	int mcu_y = (JpegDec.MCUy * mcu_h) / zoomFactor + ypos;
	//if ( ( mcu_x + mcu_w) <= tft_->width() && ( mcu_y + mcu_h) <= tft_->height()){
	  
	  _tft->setAddrWindow(mcu_x, mcu_y, mcu_x + (mcu_w / zoomFactor) - 1, mcu_y + (mcu_h / zoomFactor) - 1);
	  // uint32_t count = mcu_pixels; // what was this for?
	  
	  for (uint8_t y = 0; y < mcu_h; y++) {
		for (uint8_t x = 0; x < mcu_w; x++) {
		  if (x % zoomFactor == 0 && y % zoomFactor == 0) {
			_tft->pushColor(*pImg);
		  }
		  *pImg++;
		}
	  }
	  // drawSongInfo();
  
  }

}




// #########################################################################
// Return a value in range -1 to +1 for a given phase angle in degrees
// #########################################################################
float sineWave(int phase) {
  return sin(phase * 0.0174532925);
}

// #########################################################################
//  Draw the meter on the screen, returns x coord of righthand side
// #########################################################################
int semiArc(Adafruit_ILI9341 * _tft, int value, int vmin, int vmax, int x, int y, int r, int incli, int angle, int w, uint16_t scheme)
{
  // Minimum value of r is about 52 before value text intrudes on ring
  // drawing the text first is an option
  x += r; y += r;   // Calculate coords of centre of ring

  // int w = r / 2;    // Width of outer ring is 1/4 of radius
  
  // int angle = 90;  // Half the sweep angle of meter (300 degrees)

  // int text_colour = 0; // To hold the text colour

  // int v = map(value, vmin, vmax, -angle, angle); // Map the value to an angle v

  byte seg = 5; // Segments are 5 degrees wide = 60 segments for 300 degrees
  byte inc = 5; // Draw segments every 5 degrees, increase to 10 for segmented ring
  // int incli = 10;

	// Draw colour blocks every inc degrees
  int ret = 0;
	for (int i = -angle; i < angle; i += inc) {

		// Calculate pair of coordinates for segment start
		float sx = cos((i - incli) * 0.0174532925);
		float sy = sin((i - incli) * 0.0174532925);
		uint16_t x0 = sx * (r - w) + x;
		uint16_t y0 = sy * (r - w) + y;
		uint16_t x1 = sx * r + x;
		uint16_t y1 = sy * r + y;

		// Calculate pair of coordinates for segment end
		float sx2 = cos((i + seg - incli) * 0.0174532925);
		float sy2 = sin((i + seg - incli) * 0.0174532925);
		int x2 = sx2 * (r - w) + x;
		int y2 = sy2 * (r - w) + y;
		int x3 = sx2 * r + x;
		int y3 = sy2 * r + y;

		_tft->fillTriangle(x0, y0, x1, y1, x2, y2, scheme);
		_tft->fillTriangle(x1, y1, x2, y2, x3, y3, scheme);
		ret = y2;
	}
  return ret;
}








// fillArc2 			https://forum.arduino.cc/index.php?topic=590677.0
// #########################################################################
// Draw a circular or elliptical arc with a defined thickness
// #########################################################################

// x,y == coords of centre of arc
// start_angle = 0 - 359
// seg_count = number of 3 degree segments to draw (120 => 360 degree arc)
// rx = x axis radius
// yx = y axis radius
// w  = width (thickness) of arc in pixels
// colour = 16 bit colour value
// Note if rx and ry are the same then an arc of a circle is drawn
#define DEG2RAD 0.0174532925
int fillArc2(Adafruit_ILI9341 * _tft, int x, int y, int start_angle, int seg_count, int rx, int ry, int w, uint16_t colour)
{

  byte seg = 3; // Segments are 3 degrees wide = 120 segments for 360 degrees
  byte inc = 3; // Draw segments every 3 degrees, increase to 6 for segmented ring

    // Calculate first pair of coordinates for segment start
    float sx = cos((start_angle - 90) * DEG2RAD);
    float sy = sin((start_angle - 90) * DEG2RAD);
    uint16_t x0 = sx * (rx - w) + x;
    uint16_t y0 = sy * (ry - w) + y;
    uint16_t x1 = sx * rx + x;
    uint16_t y1 = sy * ry + y;

  // Draw colour blocks every inc degrees
  for (int i = start_angle; i < start_angle + seg * seg_count; i += inc) {

    // Calculate pair of coordinates for segment end
    float sx2 = cos((i + seg - 90) * DEG2RAD);
    float sy2 = sin((i + seg - 90) * DEG2RAD);
    int x2 = sx2 * (rx - w) + x;
    int y2 = sy2 * (ry - w) + y;
    int x3 = sx2 * rx + x;
    int y3 = sy2 * ry + y;

    _tft->fillTriangle(x0, y0, x1, y1, x2, y2, colour);
    _tft->fillTriangle(x1, y1, x2, y2, x3, y3, colour);

    // Copy segment end to sgement start for next segment
    x0 = x2;
    y0 = y2;
    x1 = x3;
    y1 = y3;
  }
  return 0;
}







// draw7Number 			https://forum.arduino.cc/index.php?topic=307348.0
// #########################################################################
/* Routine to Draw Large 7-Segment formated number with Arduino TFT Library
Contributed by William Zaggle  (Uses TFT Library DrawLine or DrawFastLine functions).

long n - The number to be displayed
int xLoc = The x location of the upper left corner of the number
int yLoc = The y location of the upper left corner of the number
int cS = The size of the number.
fC is the foreground color of the number
bC is the background color of the number (prevents having to clear previous space)
nD is the number of digit spaces to occupy (must include space for minus sign for numbers < 0).


// Example to draw the number 37 in four directions in four corners of the display
draw7Number(37,10,10,4, ILI9341_WHITE , ILI9341_BLACK,2);       //LEFT2RIGHT
draw7Number90(37,10,310,4, ILI9341_WHITE , ILI9341_BLACK,2);    //DOWN2UP
draw7Number180(37,230,310,4, ILI9341_WHITE , ILI9341_BLACK,2);  //RIGHT2LEFT
draw7Number270(37,230,10,4, ILI9341_WHITE , ILI9341_BLACK,2);   //UP2DOWN
*/
void draw7Number(Adafruit_ILI9341 * _tft, long n, unsigned int xLoc, unsigned int yLoc, char cS, unsigned int fC, unsigned int bC, char nD) {
	unsigned int num = abs(n), i, t, w, col, h, x, y, j = 1, d = 0;
	unsigned int S2 = 5 * cS;                     // width of horizontal segments   5 times the cS
	unsigned int S3 = 2 * cS;                     // thickness of a segment 2 times the cs
	unsigned int S4 = 7 * cS;                     // height of vertical segments 7 times the cS
	unsigned int x1 = cS + 1;                     // starting x location of horizontal segments
	unsigned int x2 = S3 + S2 + 1;                // starting x location of right side segments
	unsigned int y1 = yLoc + x1;                  // starting y location of top side segments
	unsigned int y3 = yLoc + S3 + S4 + 1;         // starting y location of bottom side segments
	// actual x,y locations and direction of all 7 segments 
	unsigned int seg[7][2] = {{x1, yLoc}, {x2, y1}, {x2, y3 + x1}, {x1, (2 * y3) - yLoc}, {0, y3 + x1}, {0, y1}, {x1, y3}}; 
	// segment defintions for all 10 numbers plus blank and minus sign bit mapped as typical segment labels (*,g,f,e,d,c,b,a)  
	static const unsigned char nums[12] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x00, 0x40}; 
	unsigned char c, cnt;

	c = abs(cS);                                  // get character size between 1 and 10 ignoring sign
	if (c>10) c= 10;
	if (c<1) c = 1;

	cnt = abs(nD);                                // get number of digits between 1 and 10 ignoring sign
	if (cnt > 10) cnt = 10;
	if (cnt < 1) cnt = 1; 

	d = S2+(3*S3)+2;                              // width of one number

	xLoc += (cnt-1) * d;                          // set starting x at last digit location

	while( cnt > 0) {                             // for cnt number of places

		--cnt;

		if (num > 9) i = num%10;                    // get the last digit 
		else if (!cnt && n<0) i = 11;               // show minus sign if 1st position and negative number
		else if (nD < 0 && !num) i = 10;            // show blanks if remaining number is zero
		else i = num;

		num = num/10;                               // trim this digit from the number  

		for (j = 0; j < 7; ++j) {                   // draw all seven segments

			if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
			else col = bC;                            // else use background color

			if (j==0 || j==3 || j==6) {

				w = S2;                                 // Starting width of segment (side)
				x = xLoc + seg[j][0] + cS;              // starting x location
				y = seg[j][1];                          // starting y location
				t = y + S3;                             // maximum thickness of segment
				h = y + S3/2;                           // half way point thickness of segment


				while (y < h) {                         // until y location = half way
					_tft->drawFastHLine(x, y, w, col);      // Draw a horizontal segment top 
					++y;                                  // move the y position by 1
					--x;                                  // move the x position by -1           
					w += 2;                               // make the line wider by 2
				}

				while (y < t) {                         //finish drawing horizontal bottom  
					_tft->drawFastHLine(x, y, w, col);      // Draw Horizonal segment bottom
					++y;                                  // keep moving the y or y draw position until t 
					++x;                                  // move the length or height starting position back the other way.
					w -= 2;                               // move the length or height back the other way
				}

			} else {

				w = S4;                                 // Starting height of segment (side)
				x = xLoc + seg[j][0];                   // starting x location
				y = seg[j][1] + cS;                     // starting y location 
				t = x + S3;                             // maximum thickness of segment
				h = x + S3/2;                           // half way point thickness of segment

				while (x < h) {                         // until x location = half way
					_tft->drawFastVLine(x, y, w, col);      // Draw a vertical line right side
					++x;                                  // move the x position by 1
					--y;                                  // move the y position by -1          
					w += 2;                               // make the line wider by 2
				}
				while (x < t) {
					_tft->drawFastVLine(x, y, w, col);      // Draw a vertical line right side
					++x;                                  // move the x position by 1       
					++y;                                  // move the length or height starting position back the other way.
					w -= 2;                               // move the length or height back the other way
				}
			}       
		}

		xLoc -=d;                                   // move to next digit position
	}
}

void draw7Number90(Adafruit_ILI9341 * _tft, long n, unsigned int xLoc, unsigned int yLoc, char cS, unsigned int fC, unsigned int bC, char nD) {
	unsigned int num = abs(n), i, t, w, col, h, x, y, j = 1, d = 0;
	unsigned int S2 = 5 * cS;                     // width of horizontal segments   5 times the cS
	unsigned int S3 = 2 * cS;                     // thickness of a segment 2 times the cs
	unsigned int S4 = 7 * cS;                     // height of vertical segments 7 times the cS
	unsigned int x1 = cS + 1;                     // starting x location of horizontal segments
	unsigned int x2 = S3 + S2 + 1;                // starting x location of right side segments
	unsigned int y1 = xLoc + x1;                  // starting y location of top side segments
	unsigned int y3 = xLoc + S3 + S4 + 1;         // starting y location of bottom side segments
	// actual x,y locations and direction of all 7 segments 
	unsigned int seg[7][2] = {{x1, xLoc}, {x2, y1}, {x2, y3 + x1}, {x1, (2 * y3) - xLoc}, {0, y3 + x1}, {0, y1}, {x1, y3}}; 
	// segment defintions for all 10 numbers plus blank and minus sign bit mapped as typical segment labels (bits 7-0)=(*,g,f,e,d,c,b,a)  
	static const unsigned char nums[12] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x00, 0x40}; 

	unsigned char c, cnt;

	c = abs(cS);                                  // get character size between 1 and 10 ignoring sign
	if (c>10) c= 10;
	if (c<1) c = 1;

	cnt = abs(nD);                                // get number of digits between 1 and 10 ignoring sign
	if (cnt > 10) cnt = 10;
	if (cnt < 1) cnt = 1; 

	d = S2+(3*S3)+2;                              // width of one number

	yLoc -= (cnt-1) * d;                          // set starting x at last digit location

	while( cnt > 0) {                             // for cnt number of places

		--cnt;

		if (num > 9) i = num%10;                    // get the last digit 
		else if (!cnt && n<0) i = 11;               // show minus sign if 1st position and negative number
		else if (nD < 0 && !num) i = 10;            // show blanks if remaining number is zero
		else i = num;

		num = num/10;                               // trim this digit from the number  

		for (j = 0; j < 7; ++j) {                   // draw all seven segments

			if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
			else col = bC;                            // else use background color

			if (j==0 || j==3 || j==6) {               // only three segments are horizontal

				w = S2;                                 // Starting width of segment (side)
				y = yLoc - (seg[j][0] + cS + S2 - 1);              // starting x location
				x = seg[j][1];                          // starting y location
				t = x + S3;                             // maximum thickness of segment
				h = x + S3/2;                           // half way point thickness of segment


				while (x < h) {                         // until y location = half way
					_tft->drawFastVLine(x, y, w, col);      // Draw a horizontal segment top 
					++x;                                  // move the y position by 1
					--y;                                  // move the x position by -1           
					w += 2;                               // make the line wider by 2
				}

				while (x < t) {                         // finish drawing horizontal bottom  
					_tft->drawFastVLine(x, y, w, col);      // Draw Horizonal segment bottom
					++x;                                  // keep moving the x draw position until t 
					++y;                                  // move the length or height starting position back the other way.
					w -= 2;                               // move the length or height back the other way
				}

			} else {

				w = S4;                                 // Starting height of segment (side)
				y = yLoc - seg[j][0];                   // starting y location
				x = seg[j][1] + cS;                     // starting x location 
				t = y - S3;                             // maximum thickness of segment
				h = y - S3/2;                           // half way point thickness of segment

				while (y > h) {                         // until y location = half way
					_tft->drawFastHLine(x, y, w, col);      // Draw a vertical line right side
					--x;                                  // move the x position by 1
					--y;                                  // move the y position by 1          
					w += 2;                               // make the line wider by 2
				}
				while (y > t) {
					_tft->drawFastHLine(x, y, w, col);      // Draw a vertical line right side
					++x;                                  // move the x position back the other way        
					--y;                                  // move the y position by 1.
					w -= 2;                               // make line narrower by 2
				}
			}       
		}

		yLoc +=d;                                   // move to next digit position
	}
}

void draw7Number180(Adafruit_ILI9341 * _tft, long n, unsigned int xLoc, unsigned int yLoc, char cS, unsigned int fC, unsigned int bC, char nD) {
	unsigned int num = abs(n), i, t, w, col, h, x, y, j = 1, d = 0;
	unsigned int S2 = 5 * cS;                     // width of horizontal segments   5 times the cS
	unsigned int S3 = 2 * cS;                     // thickness of a segment 2 times the cs
	unsigned int S4 = 7 * cS;                     // height of vertical segments 7 times the cS
	unsigned int x1 = cS;                         // starting x location of horizontal segments
	unsigned int x2 = S3 + S2 + 1;                // starting x location of right side segments
	unsigned int y1 = yLoc - x1;                  // starting y location of top side segments
	unsigned int y3 = yLoc - S3 - S4 - 1;         // starting y location of bottom side segments
	// actual x,y locations and direction of all 7 segments 
	unsigned int seg[7][2] = {{x1, yLoc}, {x2, y1}, {x2, y3 - x1}, {x1, (2 * y3) - yLoc}, {0, y3 - x1}, {0, y1}, {x1, y3}}; 
	// segment defintions for all 10 numbers plus blank and minus sign bit mapped as typical segment labels (bits 7-0)=(*,g,f,e,d,c,b,a)  
	static const unsigned char nums[12] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x00, 0x40}; 
	unsigned char c, cnt;

	c = abs(cS);                                  // get character size between 1 and 10 ignoring sign
	if (c>10) c= 10;
	if (c<1) c = 1;

	cnt = abs(nD);                                // get number of digits between 1 and 10 ignoring sign
	if (cnt > 10) cnt = 10;
	if (cnt < 1) cnt = 1; 

	d = S2+(3*S3)+2;                              // width of one number

	xLoc -= (cnt-1) * d;                          // set starting x at last digit location

	while( cnt > 0) {                             // for cnt number of places

		--cnt;

		if (num > 9) i = num%10;                    // get the last digit 
		else if (!cnt && n<0) i = 11;               // show minus sign if 1st position and negative number
		else if (nD < 0 && !num) i = 10;            // show blanks if remaining number is zero
		else i = num;

		num = num/10;                               // trim this digit from the number  

		for (j = 0; j < 7; ++j) {                   // draw all seven segments

			if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
			else col = bC;                            // else use background color

			if (j==0 || j==3 || j==6) {               // only three segments are horizontal

				w = S2;                                 // Starting width of segment (side)
				x = xLoc - (seg[j][0] + cS + S2);       // starting x location
				y = seg[j][1];                          // starting y location
				t = y - S3;                             // maximum thickness of segment
				h = y - S3/2;                           // half way point thickness of segment


				while (y > h) {                         // until y location = half way
					_tft->drawFastHLine(x, y, w, col);      // Draw a horizontal segment top 
					--y;                                  // move the y position by 1
					--x;                                  // move the x position by -1           
					w += 2;                               // make the line wider by 2
				}

				while (y > t) {                         // finish drawing horizontal bottom  
					_tft->drawFastHLine(x, y, w, col);      // Draw Horizonal segment bottom
					--y;                                  // keep moving the y draw position until t 
					++x;                                  // move the length or height starting position back the other way.
					w -= 2;                               // move the length or height back the other way
				}

			} else {

				w = S4;                                 // Starting height of segment (side)
				y = seg[j][1] - cS - S4;                // starting y location
				x = xLoc - seg[j][0];                   // starting x location 
				t = x - S3;                             // maximum thickness of segment
				h = x - S3/2;                           // half way point thickness of segment

				while (x > h) {                         // until y location = half way
					_tft->drawFastVLine(x, y, w, col);      // Draw a vertical line right side
					--x;                                  // move the x position by 1
					--y;                                  // move the y position by 1          
					w += 2;                               // make the line wider by 2
				}
				while (x > t) {
					_tft->drawFastVLine(x, y, w, col);      // Draw a vertical line right side
					--x;                                  // move the x position back the other way        
					++y;                                  // move the y position by 1.
					w -= 2;                               // make the line narrower by 2
				}
			}       
		}

		xLoc +=d;                                   // move to next digit position
	}
}

void draw7Number270(Adafruit_ILI9341 * _tft, long n, unsigned int xLoc, unsigned int yLoc, char cS, unsigned int fC, unsigned int bC, char nD) {
	unsigned int num = abs(n), i, t, w, col, h, x, y, j = 1, d = 0;
	unsigned int S2 = 5 * cS;                     // width of horizontal segments   5 times the cS
	unsigned int S3 = 2 * cS;                     // thickness of a segment 2 times the cs
	unsigned int S4 = 7 * cS;                     // height of vertical segments 7 times the cS
	unsigned int x1 = cS + 1;                     // starting x location of horizontal segments
	unsigned int x2 = S3 + S2 + 1;                // starting x location of right side segments
	unsigned int y1 = xLoc - x1;                  // starting y location of top side segments
	unsigned int y3 = xLoc - S3 - S4 - 1;         // starting y location of bottom side segments
	// actual x,y locations and direction of all 7 segments 
	unsigned int seg[7][2] = {{x1, xLoc}, {x2, y1}, {x2, y3 - x1}, {x1, (2 * y3) - xLoc}, {0, y3 - x1}, {0, y1}, {x1, y3}}; 
	// segment defintions for all 10 numbers plus blank and minus sign bit mapped as typical segment labels (bits 7-0)=(*,g,f,e,d,c,b,a)  
	static const unsigned char nums[12] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x00, 0x40}; 

	unsigned char c, cnt;

	c = abs(cS);                                  // get character size between 1 and 10 ignoring sign
	if (c>10) c= 10;
	if (c<1) c = 1;

	cnt = abs(nD);                                // get number of digits between 1 and 10 ignoring sign
	if (cnt > 10) cnt = 10;
	if (cnt < 1) cnt = 1; 

	d = S2+(3*S3)+2;                              // width of one number

	yLoc += (cnt-1) * d;                          // set starting x at last digit location

	while( cnt > 0) {                             // for cnt number of places

		--cnt;

		if (num > 9) i = num%10;                    // get the last digit 
		else if (!cnt && n<0) i = 11;               // show minus sign if 1st position and negative number
		else if (nD < 0 && !num) i = 10;            // show blanks if remaining number is zero
		else i = num;

		num = num/10;                               // trim this digit from the number  

		for (j = 0; j < 7; ++j) {                   // draw all seven segments

			if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
			else col = bC;                            // else use background color

			if (j==0 || j==3 || j==6) {               // only three segments are horizontal

				w = S2;                                 // Starting width of segment (side)
				y = yLoc + seg[j][0] + cS;              // starting x location
				x = seg[j][1];                          // starting y location
				t = x - S3;                             // maximum thickness of segment
				h = x - S3/2;                           // half way point thickness of segment


				while (x > h) {                         // until y location = half way
					_tft->drawFastVLine(x, y, w, col);      // Draw a horizontal segment top 
					--x;                                  // move the y position by 1
					--y;                                  // move the x position by -1           
					w += 2;                               // make the line wider by 2
				}

				while (x > t) {                         // finish drawing horizontal bottom  
					_tft->drawFastVLine(x, y, w, col);      // Draw Horizonal segment bottom
					--x;                                  // keep moving the x draw position until t 
					++y;                                  // move the length or height starting position back the other way.
					w -= 2;                               // move the length or height back the other way
				}

			} else {

				w = S4;                                 // Starting height of segment (side)
				y = yLoc + seg[j][0];                   // starting y location
				x = seg[j][1] - cS - S4 + 1;            // starting x location 
				t = y + S3;                             // maximum thickness of segment
				h = y + S3/2;                           // half way point thickness of segment

				while (y < h) {                         // until y location = half way
					_tft->drawFastHLine(x, y, w, col);      // Draw a vertical line right side
					--x;                                  // move the x position by 1
					++y;                                  // move the y position by 1          
					w += 2;                               // make the line wider by 2
				}
				while (y < t) {
					_tft->drawFastHLine(x, y, w, col);      // Draw a vertical line right side
					++x;                                  // move the x position back the other way        
					++y;                                  // move the y position by 1.
					w -= 2;                               // make line narrower by 2
				}
			}       
		}

		yLoc -=d;                                   // move to next digit position
	}
}




