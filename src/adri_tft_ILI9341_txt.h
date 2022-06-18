#ifndef ADRI_TFT_ILI9341_TXT_H
#define ADRI_TFT_ILI9341_TXT_H 

#include "adri_tft_ILI9341_ui.h"

enum TextAlignment {
	LEFT, CENTER, RIGHT
};

class adriTFTUI_txtBox
{
	uint16_t 	_tc;
	uint16_t 	_bc;


	const GFXfont * _font;

	uint8_t 			_u8g2_mod 	= 0;
	uint8_t 			_u8g2_dir 	= 0;
	const uint8_t  * 	_u8g2_font = NULL;

public:
	String 		_text;
	int			_text_x_ref = 1;
	int			_text_x = 1;
	int 		_canvas_x;
	int 		_canvas_y;		
	int 		_canvas_w;
	int 		_canvas_h;	
	int 		_text_y;
	int			_text_w;
	int 		_text_h;	
	TextAlignment _alignment = LEFT;

	U8G2_FOR_ADAFRUIT_GFX	 _u8g2;
	Adafruit_ILI9341 *	 	_tft;
	GFXcanvas1 * 			_canvas;

	adriTFTUI_txtBox(Adafruit_ILI9341 * tft, U8G2_FOR_ADAFRUIT_GFX u8g2);
	~adriTFTUI_txtBox(){};

	void canvas_create(int x, int y, int w, int h);
	void canvas_pos(int x, int y, int w, int h);
	void canvas_display();
	void canvas_update();

	void textColor(uint16_t c);
	void textAlign(String text);
	void textAlignment(TextAlignment alignment);

	void textColor(uint16_t c, uint16_t bg) ;
	void textPos(int x, int y) ;
	void textPos(int x, int y, int w, int h);
	void textGet_xy(int & x, int & y);

	void text_font(const GFXfont * font) ;
	void text_setup();
	void text(String s);


		void drawString_getWh(String text, uint16_t & w, uint16_t & h);
		void drawString(int x, int y, String text, uint16_t c, uint16_t bc, const GFXfont * font);
		void drawString(int x, int y, String text, uint16_t c, const GFXfont * font);
		void drawString(String text);		
		void drawString();		
		void drawString(String text, int w, int h);
		void drawStringAling(String text, TextAlignment p);

	void u8g2_setup();
	void u8g2_draw(int x, int y, String str);
	void u8g2_draw(int x, int y, String str, uint16_t tc, const uint8_t * f);
	void u8g2_draw(int x, int y, String str, uint16_t tc, uint16_t bc, const uint8_t * f);

	void u8g2_draw(String v);
	void u8g2_draw();
	void u8g2_drawRect();
	void u8g2_drawRect(String v);
	void u8g2_drawRect(String v, int x, int y)	;
	void u8g2_drawRect(int x, int y)	;
	void u8g2_get_wh(int & w, int & h);
	

	void u8g2_mod(uint8_t c); // transparent = 1
	void u8g2_dir(uint8_t c);
	void u8g2_font(const uint8_t * f);
	// void u8g2_mod(uint8_t c);
	// void u8g2_dir(uint8_t c);
	// void u8g2_font(const uint8_t * f);
};

#endif