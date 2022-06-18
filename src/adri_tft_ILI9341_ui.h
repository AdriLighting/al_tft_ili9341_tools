
#ifndef ADRI_TFT_ILI9341_UI_H
#define ADRI_TFT_ILI9341_UI_H

#ifdef ESP32 
	#include <SPIFFS.h>
	#include <Adafruit_ILI9341.h>
#else
	#include <Adafruit_ILI9341esp.h>
	#include <FS.h>
#endif
#include <U8g2_for_Adafruit_GFX.h>

#include "adri_tft_ILI9341_screen.h"
#include "adri_tft_ILI9341_button.h"

#define GFUI_BUTTON_MAX 70
#define BUFFPIXEL 20

	#define C_BLACK       0x0000
	#define C_BLUE        0x001F
	#define C_RED         0xF800
	#define C_GREEN       0x07E0
	#define C_CYAN        0x07FF
	#define C_MAGENTA     0xF81F
	#define C_YELLOW      0xFFE0
	#define C_WHITE       0xFFFF
	#define C_LTBLUE      0xB6DF
	#define C_LTTEAL      0xBF5F
	#define C_LTGREEN     0xBFF7
	#define C_LTCYAN      0xC7FF
	#define C_LTRED       0xFD34
	#define C_LTMAGENTA   0xFD5F
	#define C_LTYELLOW    0xFFF8
	#define C_LTORANGE    0xFE73
	#define C_LTPINK      0xFDDF
	#define C_LTPURPLE    0xCCFF
	#define C_LTGREY      0xE71C
	#define C_TEAL        0x0438
	#define C_ORANGE      0xFD20
	#define C_PINK        0xF81F
	#define C_PURPLE      0x801F
	#define C_GREY        0xC618
	#define C_DKBLUE      0x0069
	#define C_DKTEAL      0x020C
	#define C_DKGREEN     0x03E0
	#define C_DKCYAN      0x03EF
	#define C_DKRED       0x6000
	#define C_DKMAGENTA   0x8008
	#define C_DKYELLOW    0x8400
	#define C_DKORANGE    0x8200
	#define C_DKPINK      0x9009
	#define C_DKPURPLE    0x4010
	#define C_DKGREY      0x4A49



/**
 * @brief      This class describes a graphics user interface.
 */
class adriTFTUI {
	public:
		int _buttonCnt;
		int _buttonPos;
		adriTFTUI_button * button[GFUI_BUTTON_MAX];
		adriTFTUI_buttonID * button_manage[GFUI_BUTTON_MAX];

		adriTFTUI(Adafruit_ILI9341 * tft);
		~adriTFTUI(){
			delete this;
		}
    	void u8g2_attach(U8G2_FOR_ADAFRUIT_GFX u8g2);
		U8G2_FOR_ADAFRUIT_GFX _u8g2;

		adriTFTUI_button * button_int(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, int16_t s, uint16_t bc, uint16_t tc);
		adriTFTUI_button * button_int(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, int16_t s, uint16_t bc, uint16_t tc, button_mod mod, String id, const char * screen);

		void button_id(adriTFTUI_button * button, String id, const char * screen);
		void button_id(adriTFTUI_button * button, String id, const char * screen, int eventPos);

		// void drawProgressBar(uint16_t x, uint16_t y, uint16_t w, uint16_t h, float percentage, uint16_t frameColor, uint16_t barColor);
				
		int16_t drawRectWithLabel(
			int16_t x, 
			int16_t y,
			int16_t w, 
			int16_t h, 
			int sep_1, 
			int sep_2, 
			uint16_t tc, 
			uint16_t bc, 
			char *text, 
			const GFXfont * font);

    	void drawBmp(String filename, uint8_t x, uint16_t y);


		uint16_t color565(uint8_t r, uint8_t g, uint8_t b);


	private:

		Adafruit_ILI9341 * _tft;

		uint16_t 	_min = 0;
		uint16_t 	_max = 255;

	    uint16_t read16(File &f);
	    uint32_t read32(File &f);
};
 
#define adriTFTUI_lblValue_max 9
class adriTFTUI_lblValue
{
	String 	_lines[9];
	String 	_id;	
	int 	_lines_cnt = 0;

	// int 	_
	int _x = 0;
	int _y = 0;

	uint16_t _bc_name;	
	uint16_t _bc_value;

	String _sep = ":";

	Adafruit_ILI9341 	* 	_tft;
	adriTFTUI 			* 	_ui;
	U8G2_FOR_ADAFRUIT_GFX	_u8g2;

	void 	string_getWh(String text, uint16_t & w, uint16_t & h);

public:

	boolean 		_u8g2_text = true;

	uint16_t 		_c_value;
	uint16_t 		_c_name;
	uint16_t 		_c_sep;

	const uint8_t * _u8g2Font;
	const GFXfont * _font;

	int 			_w_value_max 	= 100;
	int 			_w_name_max 	= 125;
	int 			_h_max 			= 15;
	int  			_h_diff 		= 3;

	char 			_lines_sep 		= '\n';
	char 			_line_sep 		= ';';

	adriTFTUI_lblValue(String name, adriTFTUI * ui, Adafruit_ILI9341 * tft, U8G2_FOR_ADAFRUIT_GFX u8g2);
	void 	lbl_get();
	void 	lines_clear();
	int 	line_add(String str);
	int 	line_add(String name, String value);
	int 	line_add(int pos, String name, String value);
	int 	line_add(int pos, String str);
	void 	lineSplit_add(String str);

	void 	xy(int x, int y);
	void 	lines_create(int x, int y);
	void 	rect_create(int x, int y, uint16_t c);
	void 	rect_create(uint16_t c);
	void 	rect_clear(uint16_t c);
};


class adriTFTUI_progressBar
{
	uint16_t 	_r 			= 10;
	uint16_t 	_x 			= 0;
	uint16_t 	_y 			= 0;
	uint16_t 	_w 			= 0;
	uint16_t 	_h 			= 0;
	uint16_t	 _bc 		= 0;
	uint16_t	 _c 		= 0;

	Adafruit_ILI9341 * 	_tft;
public:
	float 		_percentage = 0;
	uint8_t 	_min 		= 0;
	uint8_t 	_max 		= 255;	
	adriTFTUI_progressBar(
		Adafruit_ILI9341 * tft, 
		uint16_t x0, 
		uint16_t y0, 
		uint16_t w, 
		uint16_t h, 
		uint16_t r, 
		float percentage, 
		uint16_t frameColor, 
		uint16_t barColor);
	~adriTFTUI_progressBar(){
	};
	void drawRound();
	void draw();
	void drawUpd();
	void percentage(float v);
	void percentage_get(float & v);
	void minMax(uint8_t min, uint8_t max);
	void minMax_get(uint8_t & min, uint8_t & max);
};
#endif
