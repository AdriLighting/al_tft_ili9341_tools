#include "adri_tft_ILI9341_txt.h"


adriTFTUI_txtBox::adriTFTUI_txtBox(Adafruit_ILI9341 * tft, U8G2_FOR_ADAFRUIT_GFX u8g2){
	_tft 	= tft;
	_u8g2 	= u8g2;
}

void adriTFTUI_txtBox::textAlignment(TextAlignment alignment) {
	_alignment = alignment;
}
void adriTFTUI_txtBox::textColor(uint16_t c) {
	_tc = _bc = c;
}
void adriTFTUI_txtBox::textColor(uint16_t c, uint16_t bg) {
	_tc = c;
	_bc = bg;
}
void adriTFTUI_txtBox::textPos(int x, int y) {
	_text_x = x;
	_text_y = y;
}
void adriTFTUI_txtBox::textAlign(String text) {

	char buf[text.length()+2];
	text.toCharArray(buf, text.length() + 1);
	int16_t x1, y1;
	uint16_t w, h;	
	_tft->getTextBounds(buf, 0, 0, &x1, &y1, &w, &h);
	_text_x = (_canvas_w/2)-(w/2);
	// _text_y = (_canvas_h/2);
}
void adriTFTUI_txtBox::textPos(int x, int y, int w, int h){
	_text_x 	= x;
	_text_y 	= y;
	_text_w 	= w;
	_text_h 	= h;
}
void adriTFTUI_txtBox::text(String text) {
	_text = text;
}
void adriTFTUI_txtBox::textGet_xy(int & x, int & y) {
	x = _text_x;           		
	y = _text_y;           		
}

void adriTFTUI_txtBox::text_font(const GFXfont * font) {
	_font = font;
}


void adriTFTUI_txtBox::drawString_getWh(String text, uint16_t & w, uint16_t & h) {
	char buf[text.length()+2];
	text.toCharArray(buf, text.length() + 1);
	int16_t x1, y1;
	_tft->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
}

void adriTFTUI_txtBox::text_setup() {
	_tft->setTextColor(_tc, _bc);
	_tft->setFont(_font);                		
}

void adriTFTUI_txtBox::drawString(int x, int y, String text, uint16_t c, const GFXfont * font) {
	_tc 	= _bc = c;
	_font 	= font;
	_text 	= text;                     		
	_tft->setTextColor(_tc, _bc);
	_tft->setFont(_font);
	uint16_t w, h;
	drawString_getWh(text, w, h);	
	switch (_alignment) {
		case LEFT:
			x = x;
			break;
		case CENTER:
			x = x - w / 2;
			break;
		case RIGHT:
			x = x - w;
			break;
	}
	_text_x = x;
	_text_y = y;		
	_tft->setCursor(x, y);
	_tft->print(text);
}
void adriTFTUI_txtBox::drawString(int x, int y, String text, uint16_t c, uint16_t bc, const GFXfont * font) {
	_tc 	= c;
	_bc 	= bc;
	_font 	= font;
	_text 	= text;                     		
	_tft->setTextColor(_tc, _bc);
	_tft->setFont(_font);
	uint16_t w, h;
	drawString_getWh(text, w, h);
	switch (_alignment) {
		case LEFT:
			x = x;
			break;
		case CENTER:
			x = x - w / 2;
			break;
		case RIGHT:
			x = x - w;
			break;
	}	
	_text_x = x;
	_text_y = y;	
	_tft->fillRect(x, y - h -1, w + 1, h + 4, _bc);
	_tft->setCursor(x, y);
	_tft->print(text);
}
void adriTFTUI_txtBox::drawStringAling(String text, TextAlignment pos) {
	_tft->setFont(_font);

	uint16_t wo, ho;
	drawString_getWh(_text, wo, ho);
	
	int x = _text_x_ref;
	switch (pos) {
		case LEFT:
			x = x;
			break;
		case CENTER:
			x = x - wo / 2;
			break;
		case RIGHT:
			x = x - wo;
			break;
	}	
	_tft->fillRect(x, _text_y - ho -1, wo + 1, ho + 4, _bc);

	_text = text;

	textAlignment(pos);
	drawString(_text_x_ref, _text_y, text, _tc, _bc, _font);
	textAlignment(LEFT);

}

void adriTFTUI_txtBox::drawString(String text) {
	_tft->setFont(_font);
	_tft->setCursor(_text_x, _text_y);

	uint16_t wo, ho;
	drawString_getWh(_text, wo, ho);

	uint16_t w, h;
	drawString_getWh(text, w, h);

	if (_tc != _bc) {  
		if (wo>w) 	_tft->fillRect(_text_x, _text_y - ho -1, 	wo + 1, ho + 4, _bc);
		else  		_tft->fillRect(_text_x, _text_y - h -1, 	w + 1, 	h + 4, 	_bc);
	} 

	_text 	= text; 

	_tft->print(text);
}

void adriTFTUI_txtBox::drawString() {
	_tft->setFont(_font);
	_tft->setCursor(_text_x, _text_y);

	uint16_t w, h;
	drawString_getWh(_text, w, h);

	if (_tc != _bc) _tft->fillRect(_text_x, _text_y - h -1, w + 1, 	h + 4, 	_bc);

	_tft->print(_text);
}
void adriTFTUI_txtBox::drawString(String text, int w2, int h2) {
	_text 	= text;    
	_tft->fillRect(_text_x, _text_y - (h2/2)-4 , w2, h2, _bc);
	_tft->setFont(_font);
	_tft->setCursor(_text_x, _text_y);
	_tft->print(_text);
}


void adriTFTUI_txtBox::canvas_create(int x, int y, int w, int h){
	_canvas_x 	= x;
	_canvas_y 	= y;
	_canvas_w 	= w;
	_canvas_h 	= h;
	_text_y 	= _canvas_h-2;
	_canvas 	= new GFXcanvas1(_canvas_w, _canvas_h);
}
void adriTFTUI_txtBox::canvas_pos(int x, int y, int w, int h){
	_canvas_x 	= x;
	_canvas_y 	= y;
	_canvas_w 	= w;
	_canvas_h 	= h;
}
void adriTFTUI_txtBox::canvas_display() {
	_tft->drawBitmap(_canvas_x, _canvas_y, _canvas->getBuffer(), _canvas_w, _canvas_h, _tc , _bc);	
	_canvas->fillScreen(ILI9341_BLACK);
	_canvas->setFont(_font);
	_canvas->setCursor(_text_x,  _text_y);
	_canvas->print(_text);
}
void adriTFTUI_txtBox::canvas_update() {
	this->_canvas->fillScreen(ILI9341_BLACK);
	this->_canvas->setCursor(_canvas_x,  _canvas_h-2);
	this->_canvas->print(_text);
}

void adriTFTUI_txtBox::u8g2_setup() {
	_u8g2.setFontMode(_u8g2_mod);                		
	_u8g2.setFontDirection(_u8g2_dir);                		
	_u8g2.setFont(_u8g2_font);                		
	if(_u8g2_mod==0)_u8g2.setBackgroundColor(_bc);                		
	_u8g2.setForegroundColor(_tc);                		
}
	
	
void adriTFTUI_txtBox::u8g2_mod(uint8_t v) {
	_u8g2_mod = v;
}
void adriTFTUI_txtBox::u8g2_dir(uint8_t v) {
	_u8g2_dir = v;
}
void adriTFTUI_txtBox::u8g2_font(const uint8_t * f) {
	_u8g2_font = f;
}


void adriTFTUI_txtBox::u8g2_draw(int x, int y, String v, uint16_t tc, uint16_t bc, const uint8_t * f) {
	_u8g2_font = f;
	_u8g2.setFont(f);	

	_u8g2_mod = 0;
	_u8g2.setFontMode(_u8g2_mod);

	_bc = bc;
	_u8g2.setBackgroundColor(bc);	

	_tc = tc;
	_u8g2.setForegroundColor(tc);

	_u8g2.setFontDirection(_u8g2_dir);                		

	_text_x = x;
	_text_y = y;
	_u8g2.setCursor(x,y);  

	_text = v;                     		
	_u8g2.print(v);	
}
void adriTFTUI_txtBox::u8g2_draw(int x, int y, String v, uint16_t tc, const uint8_t * f) {
	_u8g2_font = f;
	_u8g2.setFont(f);

	_u8g2_mod = 1;
	_u8g2.setFontMode(_u8g2_mod);

	_tc = tc;
	_u8g2.setForegroundColor(tc);	
	
	_u8g2.setFontDirection(_u8g2_dir);                		

	_text_x = x;
	_text_y = y;	
	_u8g2.setCursor(x,y);   

	_text = v;             		
	_u8g2.print(v);	
}	
void adriTFTUI_txtBox::u8g2_draw(int x, int y, String v) {
	_text 		= v;             		
	_text_x 	= x;
	_text_y 	= y;	
	_u8g2.setCursor(x,y);                		
	_u8g2.print(v);	
}
void adriTFTUI_txtBox::u8g2_draw(String v) {
	_u8g2.setCursor(_text_x, _text_y);   
	_text = v;             		
	_u8g2.print(_text);	
}
void adriTFTUI_txtBox::u8g2_draw() {
	_u8g2.setCursor(_text_x, _text_y);                		
	_u8g2.print(_text);	
}
void adriTFTUI_txtBox::u8g2_drawRect(){
	char buffer_text[100];
	sprintf(buffer_text, "%s", _text.c_str());
	int width = _u8g2.getUTF8Width(buffer_text);
	int height = _u8g2.getFontAscent() - _u8g2.getFontDescent();
	_text_w = width;
	_text_h = height;
	_tft->fillRect(_text_x, _text_y-_u8g2.getFontAscent()-1, width, height+2, _bc);	
}
void adriTFTUI_txtBox::u8g2_drawRect(int h, int w){
	u8g2_setup();
	char buffer_text[100];
	sprintf(buffer_text, "%s", _text.c_str());
	int width 	= _u8g2.getUTF8Width(buffer_text);
	int height 	= _u8g2.getFontAscent() - _u8g2.getFontDescent();
	if (w<width)w = width;
	_tft->fillRect(_text_x, _text_y-_u8g2.getFontAscent()-3, w+5, height+6, _bc);	
	u8g2_draw(_text);
}
void adriTFTUI_txtBox::u8g2_drawRect(String v){
	// char buffer_text[100];
	u8g2_drawRect();
	_text = v;
	u8g2_drawRect();
	u8g2_draw(v);
}
void adriTFTUI_txtBox::u8g2_drawRect(String v, int x, int y){
	char buffer_text[100];
	sprintf(buffer_text, "%s", v.c_str());
	int width 	= _u8g2.getUTF8Width(buffer_text);
	int height 	= _u8g2.getFontAscent() - _u8g2.getFontDescent();

	if ((x != _text_x) || (y > _text_y)) {
		u8g2_drawRect();   
		_text_x = x;    
		_text_y = y; 		
		u8g2_setup();         		
	}	
	if ((width > _text_w) || (height > _text_h)) {
		_text = v;    
		u8g2_setup();         		
	}

	u8g2_drawRect();
	u8g2_draw(v);
}
void adriTFTUI_txtBox::u8g2_get_wh(int & w, int & h){
	char buffer_text[100];
	sprintf(buffer_text, "%s", _text.c_str());
	int width 	= _u8g2.getUTF8Width(buffer_text);
	int height 	= _u8g2.getFontAscent() - _u8g2.getFontDescent();
	w = width;
	h = height;
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

 */