#ifndef ADRI_TFT_ILI9341_BUTTON_H
#define ADRI_TFT_ILI9341_BUTTON_H 

#ifdef ESP32 
	#include <Adafruit_ILI9341.h>
#else
	#include <Adafruit_ILI9341esp.h>
#endif
#include <U8g2_for_Adafruit_GFX.h>

#include "adri_tft_ILI9341_screen.h"


enum button_mod {
	BTM_PAUSE, 
	BTM_UP, 
	BTM_DOWN, 
	BTM_PLAY, 
	BTM_TEXT,
	BTM_FTEXT, 
	BTM_TEXTUTF8, 
	BTM_FTEXTUTF8, 
	BTM_NEXT, 
	BTM_PREV, 
	BTM_RND, 
	BTM_VOLUME, 
	BTM_UNK,
	BTM_SIMPL
};

enum button_relower_mod {
	BTMR_BORDER, BTMR_LOGO, BTMR_BORLO
};




/**
 * @brief      This class describes a graphics user interface button.
 */
class adriTFTUI_button {
	private:
  		boolean currstate, laststate;
		void button_create();
		
		void buttonFromText(String text, int16_t btX, int16_t btY);
		void button_create_text(String text);

		void buttonFromTextUtf8(String text, int16_t btX, int16_t btY);
		void button_create_textUtf8(String text);		

		void button_create_DoubleRectangle();
		void button_create_triangle();
		void button_create_triangle_top();
		void button_create_triangle_bot();
		void button_create_nextTriangle();
		void button_create_prevTriangle();
		void button_create_circle();
		void button_create_volume();

		boolean 	_relower 		= false;
		uint16_t 	_relower_color 	= ILI9341_WHITE;
	public:
		void 			(*_func)(void);
		void 			(*_funParm)(int pos);
		void 			(*_funcParmStr)(String value);
		void attachFunc(void (*func)(void));
		void attachFunc(void (*func)(int pos));
		void attachFunc(void (*func)(String value));

		Adafruit_ILI9341 		* _tft;
		U8G2_FOR_ADAFRUIT_GFX 	_u8g2;

		int 		_pos;
		int16_t 	_x;
		int16_t 	_y;
		int16_t 	_w;
		int16_t 	_h;
		int16_t 	_r;
		int16_t 	_s;
		uint16_t 	_borderColor;
		uint16_t 	_backgroundColor;
		uint16_t 	_oldBackgroundColor;
		uint16_t 	_textColor;
		uint16_t 	_oldTextColor;
		String 		_text;
		button_mod 			_mod;
		button_relower_mod 	_modRelower;
		const uint8_t *	_u8g2_font;
		const GFXfont *	_font;

		adriTFTUI_button(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, int16_t s, uint16_t bc, uint16_t tc);
  		~adriTFTUI_button(){delete this;};

  		boolean contains(int16_t x, int16_t y);

		void press(boolean p) {
			laststate = currstate;
			currstate = p;
		}

		boolean justPressed();
		boolean justReleased();
		void 	isPressed(uint16_t x, uint16_t y);
		
		void button_upd();
		
		void relowerOff();
		void button_relower_border();
		void button_relower_logo();

		void button_changePos(int16_t x, int16_t y, int16_t s);
		void button_changeSize(int16_t w, int16_t h, int16_t r);
		void button_changePS(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, int16_t s);
		
		void 	button_changeText(String txt);
		void 	button_changeTextUtf8Font(const uint8_t * font);
		void 	button_changeTextFont(const GFXfont * font);

		boolean button_changeTextColor(uint16_t newC);
		boolean button_changeBgColor(uint16_t newC);
		void 	button_invertColor(boolean state);
		void 	button_borderColor(boolean state, uint16_t c);
		void 	button_changeBorderColor( uint16_t c);




		void button_noBc_create();
		void buttonSimple_noBc_create();
		void buttonSimple_noBc_upd();

		void buttonRandom_create();
		void buttonRandom_upd();

		void buttonNext_create();
		void buttonNext_upd();

		void buttonPrev_create();
		void buttonPrev_upd();

		void buttonPlay_create();
		void buttonPlay_upd();

		void buttonUp_create();
		void buttonUp_upd();

		void buttonDown_create();
		void buttonDown_upd();

		void buttonPause_create();
		void buttonPause_upd();

		void buttonVolume_create();
		void buttonVolume_upd();
		

		void buttonText_create(String text, const GFXfont * font);
		void buttonText_create(String text);

		void buttonFromText_create(String text, int16_t btX, int16_t btY);
		void buttonFromText_create(String text, const GFXfont * font);
		void buttonFromText_create(String text);


		void buttonTextUtf8_create(String text, const uint8_t * font);
		void buttonTextUtf8_create(String text);

		void buttonFromTextUtf8_create(String text, int16_t btX, int16_t btY);
		void buttonFromTextUtf8_create(String text, const uint8_t * font);
		void buttonFromTextUtf8_create(String text);

		void button_clear();

		void button_create_fromLoop();
};

 
/**
 * @brief      This class describes a graphics user interface management.
 */
class adriTFTUI_buttonID {

	adriTFTUI_button * _btn;

	public:
		String 			_id 		= "test";
		const char *	_screen		= "main";
		int 			_eventPos 	= -1;
		String 			_enventStr 	= ""; 

	public:
		adriTFTUI_buttonID(adriTFTUI_button * btn);
		adriTFTUI_buttonID();
		~adriTFTUI_buttonID(){delete this;};

		void init_btn(String id, const char * screen);
		void init_btn(String id, const char * screen, int eventPos);
};


#endif