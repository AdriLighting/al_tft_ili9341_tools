
#include "adri_tft_ILI9341_ui.h"
#include "adri_tft_ILI9341_txt.h"
        #if defined(ESP8266)
          #include <LittleFS.h> 
        #elif defined(ESP32)
          #include <FS.h>
          #include <LITTLEFS.h>
        #endif
namespace {
	int explode(String s, char sep, String * list) {
	    // Serial.printf("\nexplode s: %s\n", s.c_str() );
	    String  t           = s + sep;
	    int     str_index   = 0;
	    int     list_index  = 0;
	    int     j;
	    String  sub;
	    int size = t.length();
	    while (str_index < size ) {
	        j=t.indexOf(sep,str_index);
	        if (j!=-1) {
	            sub=t.substring(str_index, j);
	            // Serial.printf("explode index:%d %s\n", str_index,sub.c_str() );
	            list[list_index]=sub;
	            list_index++;
	            str_index=j+1;
	        }
	    }
	    list[list_index]="";
	    return list_index;
	}	
}
// #include <adri_tools_v2.h>


/* 	adriTFTUI
	Originale 	: weather color from http://blog.squix.ch
	Author 		: Copyright (c) 2015 by Daniel Eichhorn

	12/04/2020 14:25:08
	Author 		: Adrilighting
	CREATE 		:
				ptr 		: adriTFTUI_button

				void 		: buttonFromText 					// private
				void 		: button_create 					// private

				void 		: button_int
				void 		: button_upd

				void 		: button_changePos
				void 		: button_changeSize
				void 		: button_changePS

				void 		: button_changeTextColor
				void 		: button_changeBgColor

				void 		: button_create_circle				// private
				void 		: buttonRandom_create
				void 		: buttonRandom_upd

				void 		: button_create_nextTriangle		// private
				void 		: button_create_prevTriangle		// private
				void 		: buttonNext_create
				void 		: buttonNext_upd
				void 		: buttonPrev_create
				void 		: buttonPrev_upd

				void 		: button_create_triangle			// private
				void 		: buttonPlay_create
				void 		: buttonPlay_upd

				void 		: button_create_DoubleRectangle 	// private
				void 		: buttonPause_create
				void 		: buttonPause_upd

				void 		: button_create_text 				// private
				void 		: buttonText_create

				void 		: buttonFromText_create

				int16_t 	: drawRectWithLabel
*/

/**
 * @brief      Constructs a new instance.
 *
 * @param      tft   The tft
 */
adriTFTUI::adriTFTUI(Adafruit_ILI9341 *tft) {
	_buttonCnt = 0;
	_buttonPos = 0;
	_tft = tft;
}

void adriTFTUI::u8g2_attach(U8G2_FOR_ADAFRUIT_GFX u8g2){
	_u8g2 = u8g2;
}


adriTFTUI_button * adriTFTUI::button_int(
		int16_t x, 
		int16_t y, 
		int16_t w, 
		int16_t h, 
		int16_t r, 
		int16_t s, 
		uint16_t bc, 
		uint16_t tc
	)

{
	button[_buttonCnt] 			= new adriTFTUI_button( x, y, w, h, r, s, bc, tc);
	button_manage[_buttonCnt] 	= new adriTFTUI_buttonID(button[_buttonCnt]);
	button[_buttonCnt]->_pos 	= _buttonCnt;
	button[_buttonCnt]->_tft 	= _tft;
	button[_buttonCnt]->_u8g2 	= _u8g2;
	_buttonCnt++;

	#ifdef DEBUG
		Serial.printf("[adriTFTUI button_int 0] cnt: %d\n", _buttonCnt);
	#endif
		
	return button[_buttonCnt-1];
}
adriTFTUI_button * adriTFTUI::button_int(
		int16_t x, 
		int16_t y, 
		int16_t w, 
		int16_t h, 
		int16_t r, 
		int16_t s, 
		uint16_t bc, 
		uint16_t tc, 
		button_mod mod,
		String id, 
		const char * screen
	)
{
	button[_buttonCnt] 			= new adriTFTUI_button( x, y, w, h, r, s, bc, tc);
	button[_buttonCnt]->_mod 	= mod;
	button[_buttonCnt]->_pos 	= _buttonCnt;
	button[_buttonCnt]->_tft 	= _tft;
	button[_buttonCnt]->_u8g2 	= _u8g2;
	
	button_manage[_buttonCnt] 	= new adriTFTUI_buttonID(button[_buttonCnt]);
	button_id(button[_buttonCnt], id, screen);

	_buttonCnt++;

	#ifdef DEBUG
		Serial.printf("[adriTFTUI button_int 1] cnt: %d - screen: %s\n", _buttonCnt, screen);
	#endif

	return button[_buttonCnt-1];
}

void adriTFTUI::button_id(adriTFTUI_button * button, String id, const char * screen){
	int pos = button->_pos;
	button_manage[pos]->init_btn(id, screen);
}
void adriTFTUI::button_id(adriTFTUI_button * button, String id, const char * screen, int eventPos){
	int pos = button->_pos;
	button_manage[pos]->init_btn(id, screen, eventPos);
}


/*
	creation d'un rectangle vide a angle droit et de son label situer  huat gauche 
	parm : x, y, w, h, color, label
	return position basse	
*/
int16_t adriTFTUI::drawRectWithLabel(
	int16_t x, int16_t y, int16_t w, int16_t h, int sep_1, int sep_2, 
	uint16_t tc, uint16_t bc, char *text, const GFXfont * font) {

	int xSep_2 = sep_2;
	int16_t xT, yT;
	uint16_t wT, hT;

	_tft->setTextWrap(false);
	_tft->setTextColor(tc);
	_tft->setFont(font);

	_tft->getTextBounds(text, 0, 0, &xT, &yT, &wT, &hT);
	int pos 	= ((w/2) - (xSep_2) - (wT/2));
	int xSep  	= ((pos*2)*sep_1)/100;
	int16_t x1 		= x;
	int16_t x2 		= x + xSep + wT + (xSep_2*2);
	int16_t w2 		= w - (xSep + wT + (xSep_2*2));

	_tft->startWrite();

	_tft->writeFastHLine(x, 	y, 		xSep , 	bc);

	_tft->writeFastHLine(x2, 	y, 		w2, 	bc);

	_tft->writeFastHLine(x, 	y+h-1, 	w, 		bc);

	_tft->writeFastVLine(x, 	y, 		h, 		bc);

	_tft->writeFastVLine(x+w-1, y, 		h, 		bc);

	_tft->endWrite();

	_tft->setCursor(x1+xSep+xSep_2, y + (hT/2));
	_tft->print(text);

	return (y + h);
}

uint16_t adriTFTUI::color565(uint8_t r, uint8_t g, uint8_t b){
	return _tft->color565(r, g, b);
}

// Code mostly from 
// https://github.com/adafruit/Adafruit_ILI9341/blob/master/examples/spitftbitmap/spitftbitmap.ino
void adriTFTUI::drawBmp(String filename, uint8_t x, uint16_t y) {
  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  // boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0;

	if((x >= _tft->width()) || (y >= _tft->height())) return;
	#if defined(ESP8266)
		bmpFile = LittleFS.open(filename, "r");
	#elif defined(ESP32)
		bmpFile = SPIFFS.open(filename, "r");
	#else
	#endif    		
	

	if (!bmpFile) return;

  	if(read16(bmpFile) == 0x4D42) { 							// BMP signature
		read32(bmpFile); 										// "File size"
		(void)read32(bmpFile); 									// Read & ignore creator bytes
		bmpImageoffset = read32(bmpFile); 						// Start of image data "Image Offset"

		// Read DIB header
		read32(bmpFile);										// "Header size"
		bmpWidth  = read32(bmpFile);
		bmpHeight = read32(bmpFile);
		if(read16(bmpFile) == 1) { 								// # planes -- must be '1'
	 		bmpDepth = read16(bmpFile); 						// bits per pixel
	  		if((bmpDepth == 24) && (read32(bmpFile) == 0)) { 	// 0 = uncompressed
				// goodBmp = true; 								// Supported BMP format -- proceed!
				rowSize = (bmpWidth * 3 + 3) & ~3;

				// If bmpHeight is negative, image is in top-down order.
				// This is not canon but has been observed in the wild.				
				if(bmpHeight < 0) {
					bmpHeight = -bmpHeight;
					flip      = false;
				}

        		// Crop area to be loaded
				w = bmpWidth;
				h = bmpHeight;
				if((x+w-1) >= _tft->width())  w = _tft->width()  - x;
				if((y+h-1) >= _tft->height()) h = _tft->height() - y;
				_tft->setAddrWindow(x, y, x+w-1, y+h-1);
				for (row=0; row<h; row++) { 					// For each scanline...

				/* 	Seek to start of scan line.  It might seem labor-
					intensive to be doing this on every line, but this
					method covers a lot of gritty details like cropping
					and scanline padding.  Also, the seek only takes
					place if the file position actually needs to change
					(avoids a lot of cluster math in SD library). 	*/					
					if(flip)									// Bitmap is stored bottom-to-top order (normal BMP)
						pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
					else     									// Bitmap is stored top-to-bottom
						pos = bmpImageoffset + row * rowSize;
					if(bmpFile.position() != pos) { 			// Need seek?
						bmpFile.seek(pos, SeekSet);
						buffidx = sizeof(sdbuffer); 			// Force buffer reload
					}

					for (col=0; col<w; col++) { 				// For each pixel...
						if (buffidx >= sizeof(sdbuffer)) { 		// Indeed
							bmpFile.read(sdbuffer, sizeof(sdbuffer));
							buffidx = 0; 						// Set index to beginning
						}

            			// Convert pixel from BMP to TFT format, push to display
						b = sdbuffer[buffidx++];
						g = sdbuffer[buffidx++];
						r = sdbuffer[buffidx++];
						_tft->pushColor(_tft->color565(r,g,b));
						yield();
					} 											// end pixel
				} 												// end scanline
	  		} 													// end goodBmp
		}
  	}
 	bmpFile.close();
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t adriTFTUI::read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t adriTFTUI::read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}


adriTFTUI_lblValue::adriTFTUI_lblValue(String name, adriTFTUI * ui, Adafruit_ILI9341 * tft, U8G2_FOR_ADAFRUIT_GFX u8g2){
	_tft = tft;
	_u8g2 = u8g2;
	_ui = ui;
	_id = name;
}

void adriTFTUI_lblValue::lbl_get(){
	Serial.printf("\n[lbl_get] [%s]\n", _id.c_str());

	String split_1[2];

	for (int i = 0; i < _lines_cnt; ++i) {
		explode(_lines[i], _line_sep, split_1);
		Serial.printf("\t[%d] name: %-20s : %s\n", i, split_1[0].c_str(), split_1[1].c_str());
	}
}
void adriTFTUI_lblValue::lines_clear(){


	for (int i = 0; i < adriTFTUI_lblValue_max; ++i) {
		_lines[i]="";
	}
	_lines_cnt=0;
}

int adriTFTUI_lblValue::line_add(String str){
	if (str == "") return -1;

	for (int i = 0; i < adriTFTUI_lblValue_max; ++i)
	{
		if(_lines[i] == ""){
			_lines[i] = str;
			_lines_cnt++;
			return i;
		}
	}
	return -1;
}
int adriTFTUI_lblValue::line_add(String name, String value){
	if ((name == "") && (value = "")) return -1;
	return line_add(name+_line_sep+value);
}
int adriTFTUI_lblValue::line_add(int pos, String name, String value){
	if ((name == "") && (value = "")) 	return -1;
	if (pos >= adriTFTUI_lblValue_max)	return -1;
	if (pos >= _lines_cnt) 				return line_add(name+_line_sep+value);
	else {
		_lines[pos] = name+_line_sep+value;
		return pos;
	}
}
int adriTFTUI_lblValue::line_add(int pos, String str){
	if (str == "") 						return -1;
	if (pos >= adriTFTUI_lblValue_max)	return -1;
	if (pos >= _lines_cnt) 				return line_add(str);
	else {
		_lines[pos] = str;
		return pos;
	}
}

void adriTFTUI_lblValue::lineSplit_add(String str){
	// Serial.printf("\n[lineSplit_add]\n%s\n", str.c_str());

	String 	split_1[50];

	explode(str, _lines_sep, split_1);

	for (int i = 0; i < adriTFTUI_lblValue_max; ++i)
	{
		String line = split_1[i];
		if (line != ""){
			line_add(line);
			// Serial.printf("[%d] %s\n", pos, line.c_str());
		}
	}
}

void adriTFTUI_lblValue::xy(int x, int y) {
	_x = x;
	_y = y;
}
void adriTFTUI_lblValue::string_getWh(String text, uint16_t & w, uint16_t & h) {
	char buf[text.length()+2];
	text.toCharArray(buf, text.length() + 1);
	int16_t x1, y1;
	_tft->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
}
// void adriTFTUI_txtBox::u8g2_drawRect(){
// 	char buffer_text[100];
// 	sprintf(buffer_text, "%s", _text.c_str());
// 	int width = _u8g2.getUTF8Width(buffer_text);
// 	int height = _u8g2.getFontAscent() - _u8g2.getFontDescent();
// 	_text_w = width;
// 	_text_w = _text_h;
// 	_tft->fillRect(_text_x, _text_y-_u8g2.getFontAscent(), width, height, _bc);	
// }
void adriTFTUI_lblValue::rect_clear(uint16_t c){


	// int16_t x1, y1;
	// uint16_t w, h;

	String split_1[2];

	char buffer_text[200];

	if (_u8g2_text) {
		_u8g2.setFontMode(1);                		
		_u8g2.setFont(_u8g2Font);    
		_u8g2.setFontDirection(0);  		
	} else {
		_tft->setFont(_font);
	}
	
	uint16_t width, height;

	int y = _y;

	for (int i = 0; i < _lines_cnt; ++i) {
		int cnt = explode(_lines[i], _line_sep, split_1);
		if (cnt < 2) continue;

		if (_u8g2_text) {
			sprintf(buffer_text, "%s", split_1[1].c_str());

			width 	= _u8g2.getUTF8Width(buffer_text);
			height 	= _u8g2.getFontAscent() - _u8g2.getFontDescent();	

		} else {
			_tft->setCursor(_x, y);			
			string_getWh(split_1[1], width, height);
		}

			_tft->fillRect(_x-2, y-(_h_max)+3, _w_name_max+20+width, _h_max, c);

		y+= _h_max+_h_diff;
	}
}
void adriTFTUI_lblValue::rect_create(uint16_t c){


	String split_1[2];

	char buffer_text[200];

	if (_u8g2_text) {
		_u8g2.setFontMode(1);                		
		_u8g2.setFont(_u8g2Font);    
		_u8g2.setFontDirection(0);  		
	} else {
		_tft->setFont(_font);
	}
	
	uint16_t width, height;

	int y = _y;

	for (int i = 0; i < _lines_cnt; ++i) {
		int cnt = explode(_lines[i], _line_sep, split_1);
		if (cnt < 2) continue;

		if (_u8g2_text) {
			sprintf(buffer_text, "%s", split_1[1].c_str());

			width 	= _u8g2.getUTF8Width(buffer_text);
			height 	= _u8g2.getFontAscent() - _u8g2.getFontDescent();	
		} else {
			
			_tft->setCursor(_x, y);			
			string_getWh(split_1[1], width, height);
		}

		_tft->fillRect(_x-2, 				y-(_h_max)+3, 	_w_name_max+2, 	_h_max, c);
		_tft->fillRect(_x+_w_name_max+10, 	y-(_h_max)+3, 	width+3, 		_h_max, c);

		y+= _h_max+_h_diff;
	}
}
void adriTFTUI_lblValue::rect_create(int x, int y, uint16_t c){
	_x = x;
	_y = y;
	rect_create(c);
}

void adriTFTUI_lblValue::lines_create(int x, int y){
	String split_1[10];
	_x = x;
	_y = y;
	int y2 = _y;

	if (_u8g2_text) {
		_u8g2.setFontMode(1);                		
		_u8g2.setFont(_u8g2Font);    
		_u8g2.setFontDirection(0);  		
	} else {
		_tft->setFont(_font);
	}
	
	adriTFTUI_txtBox temp(_tft, _u8g2);

	for (int i = 0; i < _lines_cnt; ++i) {
		int cnt = explode(_lines[i], _line_sep, split_1);
		if (cnt < 2) continue;

		if (_u8g2_text) {
			temp.u8g2_draw(	_x, 				y2, split_1[0], 	_c_name, 	_u8g2Font);
			temp.u8g2_draw(	_x+_w_name_max+2, 	y2, _sep, 			_c_sep, 	_u8g2Font);		
			temp.u8g2_draw(	_x+_w_name_max+12,	y2, split_1[1], 	_c_value, 	_u8g2Font);	
		} else {
			temp.drawString(	_x, 				y2, split_1[0], 	_c_name, 	_font);
			temp.drawString(	_x+_w_name_max+2, 	y2, _sep, 			_c_sep, 	_font);		
			temp.drawString(	_x+_w_name_max+12,	y2, split_1[1], 	_c_value, 	_font);	
		}

		y2+= _h_max+_h_diff;
	}
}



float mapF(float val, float fromLow, float fromHigh, float toLow, float toHigh) {
  return (val - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;

}

// void adriTFTUI::drawProgressBar(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, float percentage, uint16_t frameColor, uint16_t barColor) {
// 	uint8_t 	margin 		= 2;
// 	uint16_t 	barHeight 	= h - (2 * margin);
// 	uint16_t 	barWidth 	= w - margin;

// 	_tft->drawRect(x0, y0, w, h, frameColor);

// 	int pos = mapF(percentage, _min, _max, (float) x0, (float) x0 + (float) barWidth);
// 	_tft->fillRect(x0, 	y0 + margin, 	pos - x0, 		barHeight, barColor);
// 	_tft->fillRect(pos, y0 + margin, 	barWidth - pos + x0, 	barHeight, 0); 
// }
adriTFTUI_progressBar::adriTFTUI_progressBar(Adafruit_ILI9341 * tft, 
	uint16_t x0, 
	uint16_t y0, 
	uint16_t w, 
	uint16_t h, 
	uint16_t r, float percentage, uint16_t frameColor, uint16_t barColor) {
	_r 			= r;
	_x 			= x0;
	_y 			= y0;
	_w 			= w;
	_h 			= h;
	_percentage = percentage;
	_bc 		= frameColor;
	_c 			= barColor;
	_tft 		= tft;
}

void adriTFTUI_progressBar::draw(){
	uint8_t 	margin 		= 2;
	uint16_t 	barHeight 	= _h - (2 * margin);
	uint16_t 	barWidth 	= _w - margin;

	int pos = mapF(_percentage, _min, _max, (float) _x , (float) _x + (float) barWidth);
	_tft->fillRect(_x, 	_y + margin, 	pos - _x, 				barHeight, _c);
	_tft->fillRect(pos, _y + margin, 	barWidth - pos + _x, 	barHeight, 0); 

	_tft->drawRect(_x, _y, _w, _h, _bc);
		
}
void adriTFTUI_progressBar::drawUpd(){
	uint8_t 	margin 		= 2;
	uint16_t 	barHeight 	= _h - (2 * margin);
	uint16_t 	barWidth 	= _w - margin;
	int pos = mapF(_percentage, _min, _max, (float) _x, (float) _x + (float) barWidth);
	_tft->fillRect(_x, 	_y + margin, 	pos - _x, 				barHeight, _c);
	_tft->fillRect(pos, _y + margin, 	barWidth - pos + _x, 	barHeight, 0); 	
}
void adriTFTUI_progressBar::drawRound(){
	int pos = mapF(_percentage, _min, _max, (float) _x, (float) _x + (float) _w);
	_tft->fillRoundRect(_x, 	_y, pos - _x,		_h, _r, _c);
	_tft->fillRoundRect(pos, 	_y, _w - pos + _x,	_h, _r, 0); 	
}

void adriTFTUI_progressBar::percentage(float v){
	_percentage = v;
}
void adriTFTUI_progressBar::percentage_get(float & v){
	v = _percentage;
}
void adriTFTUI_progressBar::minMax(uint8_t min, uint8_t max){
	_min = min;	
	_max = max;	
}
void adriTFTUI_progressBar::minMax_get(uint8_t & min, uint8_t & max){
	min = _min;	
	max = _max;	
}





























/*
		ui._u8g2.setFontMode(0);                 		
		ui._u8g2.setFontDirection(0);            		
		ui._u8g2.setBackgroundColor(0);      			
		ui._u8g2.setForegroundColor(ILI9341_GREEN);     
		ui._u8g2.setFont(u8g2_font_helvR14_tf); 	 	
		sprintf(screen_txt_buffer, "%s", str_1.c_str());
		int width = ui._u8g2.getUTF8Width(screen_txt_buffer);
		int height = ui._u8g2.getFontAscent() - ui._u8g2.getFontDescent();
		tft.fillRect(10, 60-ui._u8g2.getFontAscent(), width, height, ILI9341_BLACK);	
		ui._u8g2.setCursor(10,60);                		
		ui._u8g2.print(str_1);	
	}

		// int16_t startX = 25;
		// int16_t starty = 10;

		// ui.button[0] = new adriTFTUI_button(startX, starty, 120, 80, 8, 10, ILI9341_DARKGREY, ILI9341_RED);
		// adriTFTUI_button * bt_fromTxt = ui.button[0];
		// ui.buttonFromText_create(bt_fromTxt, "bt from text", &SansSerif_bold_22);
		// delay(1000);

		// startX = bt_fromTxt->_x + bt_fromTxt->_w + 10;
		// ui.button[1] = new adriTFTUI_button( startX, starty, 100, 20, 8, 10, ILI9341_DARKGREY, ILI9341_RED);
		// adriTFTUI_button * bt_txt = ui.button[1];
		// ui.buttonText_create(bt_txt, "bt add text", &SansSerif_bold_12);
		// delay(1000);

		// startX = 25;
		// starty = bt_fromTxt->_y + bt_fromTxt->_h + 10;
		// ui.button[2] = new adriTFTUI_button( startX, starty, 80, 80, 8, 10, ILI9341_DARKGREY, ILI9341_RED);
		// adriTFTUI_button * bt_play = ui.button[2];
		// ui.buttonPlay_create(bt_play);
		// delay(1000);

		// startX = bt_play->_x + bt_play->_w + 10;
		// starty = bt_play->_y;
		// ui.button[3] = new adriTFTUI_button( startX, starty, 80, 80, 8, 10, ILI9341_DARKGREY, ILI9341_RED);
		// adriTFTUI_button * bt_pause = ui.button[3];
		// ui.buttonPause_create(bt_pause);
		// delay(1000);

		// delay(1000);
		// ui.button_changeTextColor(bt_fromTxt, ILI9341_GREEN);
		// ui.button_upd(bt_fromTxt);

		// delay(1000);
		// ui.button_changeTextColor(bt_txt, ILI9341_GREEN);
		// ui.button_upd(bt_txt);

		// delay(1000);
		// ui.button_changeTextColor(bt_play, ILI9341_GREEN);
		// ui.button_upd(bt_play);

		// delay(1000);
		// ui.button_changeTextColor(bt_pause, ILI9341_GREEN);
		// ui.button_upd(bt_pause);

		// delay(1000);
		// tft.fillScreen(ILI9341_BLACK);		
		// ui.buttonFromText_create(bt_fromTxt, "", &ArialRoundedMTBold_36);
		// delay(1000);

		// startX = 25;
		// starty = bt_fromTxt->_y + bt_fromTxt->_h + 10;
		// ui.button_changeSize(bt_play, 120, 120, -1);
		// ui.button_changePos(bt_play, (tft.width()/2) - 50, starty, 10);
		// ui.buttonPlay_create(bt_play);
		// delay(1000);	

*/