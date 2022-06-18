#include "adri_tft_ILI9341_button.h"

/**
 * @brief      Constructs a new instance.
 * 				
 * @param      btn   The button ptr from adriTFTUI_button class
 */
adriTFTUI_buttonID::adriTFTUI_buttonID(adriTFTUI_button * btn) {
	_btn 		= btn;
	_id 		= "";
	_screen 	= "";
	_eventPos 	= -1;

}
adriTFTUI_buttonID::adriTFTUI_buttonID() {

}	

/**
 * @brief      Initializes the button.
 *
 * @param[in]  id    String fro display info
 * @param[in]  mod   screen assign to object
 */
void adriTFTUI_buttonID::init_btn(String id, const char * screen){
	_id 		= id;
	_screen 	= screen;
	_eventPos 	= -1;
}
void adriTFTUI_buttonID::init_btn(String id, const char * screen, int eventPos){
	_id 		= id;
	_screen 	= screen;
	_eventPos 	= eventPos;
}







/**
 * @brief      Constructs a new instance.
 * @brief      create button object and initialize coords
 * @param[in]  x     { parameter_description }
 * @param[in]  y     { parameter_description }
 * @param[in]  w     { parameter_description }
 * @param[in]  h     { parameter_description }
 * @param[in]  r     { parameter_description }
 * @param[in]  s     { parameter_description }
 * @param[in]  bc    { parameter_description }
 * @param[in]  tc    { parameter_description }
 * 
 */
void noFunc(){
	// Serial.println("NOFUNC");
}
adriTFTUI_button::adriTFTUI_button(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, int16_t s, uint16_t bc, uint16_t tc) {
	_x 					= x;
	_y 					= y;
	_w 					= w;
	_h 					= h;
	_r 					= r;
	_s 					= s;
	_borderColor 		= ILI9341_LIGHTGREY;
	_backgroundColor 	= bc;
	_oldBackgroundColor = bc;
	_textColor 			= tc;
	_oldTextColor 		= tc;
	_u8g2_font	 		= NULL;
	_font	 			= NULL;
	_func	 			= noFunc;
	_modRelower 		= BTMR_BORDER;
}


void adriTFTUI_button::attachFunc(void (*func)(void)){
	_func = func;
}
void adriTFTUI_button::attachFunc(void (*func)(int pos)){
	_funParm = func;
}
void adriTFTUI_button::attachFunc(void (*func)(String value)){
	_funcParmStr = func;
}

/**************************************************************************/
/*!
	@brief    Helper to let us know if a coordinate is within the bounds of the
   button
	@param    x       The X coordinate to check
	@param    y       The Y coordinate to check
	@returns  True if within button graphics outline
*/
/**************************************************************************/
boolean adriTFTUI_button::contains(int16_t x, int16_t y) {
  return ((x >= _x) && (x < (int16_t)(_x + _w)) && (y >= _y) &&
		  (y < (int16_t)(_y + _h)));
}

/**************************************************************************/
/*!
   @brief    Query whether the button was pressed since we last checked state
   @returns  True if was not-pressed before, now is.
*/
/**************************************************************************/
boolean adriTFTUI_button::justPressed() { return (currstate && !laststate); }

/**************************************************************************/
/*!
   @brief    Query whether the button was released since we last checked state
   @returns  True if was pressed before, now is not.
*/
/**************************************************************************/
boolean adriTFTUI_button::justReleased() {
  return (!currstate && laststate);
}

void adriTFTUI_button::isPressed(uint16_t x, uint16_t y){
	
	if (contains(x, y)) {
		this->_func();
		if (_modRelower==BTMR_LOGO){
			if (this->button_changeTextColor(_relower_color)){
				this->button_upd();
				_relower = true;
			}
		}
		if (_modRelower==BTMR_BORDER){
			if (this->button_changeTextColor(_relower_color)){
				_relower = true;
				button_borderColor(true, ILI9341_GREEN);	
			}
		}		
	}

}
void adriTFTUI_button::relowerOff(){
	
	if (_relower) {
		_relower = false;
		if (_modRelower==BTMR_BORDER){
			button_borderColor(false, 0);
		}
		if (_modRelower==BTMR_LOGO){
			if (button_changeTextColor(_oldTextColor))button_upd();		
		}
	}

}
void adriTFTUI_button::button_relower_logo(){
	_modRelower = BTMR_LOGO;
}
void adriTFTUI_button::button_relower_border(){
	_modRelower = BTMR_BORDER;
}

boolean adriTFTUI_button::button_changeTextColor(uint16_t newC){
	if (newC != _textColor) {_textColor = newC; return true;}
	return false;
}

boolean adriTFTUI_button::button_changeBgColor(uint16_t newC){
	if (newC != _backgroundColor) {_backgroundColor = newC; return true;}
	return false;
}

void adriTFTUI_button::button_invertColor(boolean state){
	if (state) {
		_backgroundColor 	= _oldTextColor;
		_textColor 			= _oldBackgroundColor;
	}else {
		_backgroundColor 	= _oldBackgroundColor;
		_textColor 			= _oldTextColor;
	}
}

void adriTFTUI_button::button_changeBorderColor( uint16_t c){
	_borderColor = c;
}
void adriTFTUI_button::button_borderColor(boolean state, uint16_t c){
	if (state) {
		_tft->drawRoundRect(
				_x, 
				_y, 
				_w, 
				_h, 
				_r, 
				c
			);
	}else {
		_tft->drawRoundRect(
				_x, 
				_y, 
				_w, 
				_h, 
				_r, 
				_borderColor
			);
	}
}

void adriTFTUI_button::button_changeTextUtf8Font(const uint8_t * font) {
	_u8g2_font = font;
}
void adriTFTUI_button::button_changeTextFont(const GFXfont * font) {
	_font = font;
}
void adriTFTUI_button::button_changeText(String txt) {
	_text = txt;
}
void adriTFTUI_button::button_changePos(int16_t x, int16_t y, int16_t s) {
	if (x != -1) _x = x;
	if (y != -1) _y = y;
	if (s != -1) _s = s;
}
void adriTFTUI_button::button_changeSize(int16_t w, int16_t h, int16_t r) {
	if (w != -1) _w = w;
	if (h != -1) _h = h;
	if (r != -1) _r = r;
}
void adriTFTUI_button::button_changePS(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, int16_t s) {
	if (x != -1) _x = x;
	if (y != -1) _y = y;	
	if (w != -1) _w = w;
	if (h != -1) _h = h;
	if (r != -1) _r = r;
	if (s != -1) _s = s;

}

void adriTFTUI_button::button_create_fromLoop(){
	button_mod mod = _mod;
	switch(mod) {
		case BTM_VOLUME:	buttonVolume_create(); 			break;
		case BTM_RND: 		buttonRandom_create(); 			break;
		case BTM_NEXT: 		buttonNext_create(); 			break;
		case BTM_PREV: 		buttonPrev_create(); 			break;
		case BTM_PAUSE: 	buttonPause_create(); 			break;
		case BTM_PLAY: 		buttonPlay_create(); 			break;
		case BTM_UP: 		buttonUp_create(); 				break;
		case BTM_DOWN: 		buttonDown_create(); 			break;
		case BTM_TEXT: 		buttonText_create(""); 			break;
		case BTM_FTEXT: 	buttonFromText_create(_text);	break;
		case BTM_TEXTUTF8:  buttonTextUtf8_create(_text);	break;
		case BTM_FTEXTUTF8: buttonFromTextUtf8_create(_text);	break;		
		case BTM_SIMPL: 	button_noBc_create();				break;		
		default:break;
	}
}
void adriTFTUI_button::button_upd(){
	button_mod mod = _mod;
	switch(mod) {
		case BTM_VOLUME:	buttonVolume_upd(); 		break;
		case BTM_RND: 		buttonRandom_upd(); 		break;
		case BTM_NEXT: 		buttonNext_upd(); 			break;
		case BTM_PREV: 		buttonPrev_upd(); 			break;
		case BTM_PAUSE: 	buttonPause_upd(); 			break;
		case BTM_PLAY: 		buttonPlay_upd(); 			break;
		case BTM_UP: 		buttonUp_upd(); 			break;
		case BTM_DOWN: 		buttonDown_upd(); 			break;
		case BTM_TEXT: 		buttonText_create(""); 		break;
		case BTM_FTEXT: 	buttonFromText_create(""); 	break;
		case BTM_TEXTUTF8:  buttonTextUtf8_create(_text);	break;
		case BTM_FTEXTUTF8: buttonFromTextUtf8_create(_text);	break;		
		case BTM_SIMPL: 	button_noBc_create(); 			break;	
		default:break;	
	}
}
void adriTFTUI_button::button_create(){
	_tft->fillRoundRect(
			_x, 
			_y, 
			_w, 
			_h, 
			_r, 
			_backgroundColor
		);
	_tft->drawRoundRect(
			_x, 
			_y, 
			_w, 
			_h, 
			_r, 
			_borderColor
		);	
}  
void adriTFTUI_button::button_clear(){
	_tft->fillRoundRect(
			_x, 
			_y, 
			_w, 
			_h, 
			_r, 
			ILI9341_BLACK
		);
} 
void adriTFTUI_button::button_noBc_create(){
	_tft->drawRoundRect(
			_x, 
			_y, 
			_w, 
			_h, 
			_r, 
			_borderColor
		);		
}  
void adriTFTUI_button::buttonSimple_noBc_create(){
	_mod = BTM_SIMPL;
	button_noBc_create();
} 

void adriTFTUI_button::buttonSimple_noBc_upd(){
	button_noBc_create();
}  


/*
	creation du logo random baser sur la postion et taille du bouton 
	parm : bouton object correspondant
	return void	
*/
void adriTFTUI_button::button_create_circle(){
	int sep 	= _s;
	int d;
	if (_h > _w) 	d = (_w-(sep*2));
	else 							d = (_h-(sep*2));
	int16_t x0 	= _x + (_w/2) ;
	int16_t y0 	= _y + (_h/2);
	_tft->fillCircle(x0, y0, d/2, _textColor);
	_tft->fillCircle(x0, y0, (d/2)/2, _backgroundColor);
} 
/*
	creation du bouton + logo random
	parm : ptr ver lobjet bouton
	return void	
*/
void adriTFTUI_button::buttonRandom_create(){
	_mod = BTM_RND;
	button_create();
	button_create_circle();
} 
/*
	update du logo random
	parm : ptr ver lobjet bouton
	return void	
*/ 
void adriTFTUI_button::buttonRandom_upd(){
	button_create_circle();
}  



/*
	creation du logo next/prev baser sur la postion et taille du bouton 
	parm : bouton object correspondant
	return void	
*/
void adriTFTUI_button::button_create_nextTriangle(){
	int sep 	= _s;
	int h 		= (_h-(sep*2));
	int wRect 	= (_w*10)/100;
	int16_t x0 	= _x + sep;
	int16_t y0 	= _y + sep;
	int16_t x1 	= x0;
	int16_t y1 	= y0 + h;
	int16_t x2 	= (x1 + _w)-(sep*2)-wRect;
	int16_t y2 	= y1 - (h/2);

	_tft->fillTriangle(
			x0, 
			y0, 
			x1, 
			y1, 
			x2, 
			y2, 
			_textColor
		);
	_tft->fillRoundRect(x2, y0, wRect, h, 3, _textColor);
} 
void adriTFTUI_button::button_create_prevTriangle(){
	int sep 	= _s;
	int h 		= (_h-(sep*2));
	int wRect 	= (_w*10)/100;
	int16_t x0 	= (_x + _w ) - (sep);
	int16_t y0 	= _y + sep;
	int16_t x1 	= x0;
	int16_t y1 	= y0 + h;
	int16_t x2 	= (_x)+(sep)+wRect;
	int16_t y2 	= y1 - (h/2);

	_tft->fillTriangle(
			x0, 
			y0, 
			x1, 
			y1, 
			x2, 
			y2, 
			_textColor
		);
	_tft->fillRoundRect(_x+sep, y0, wRect, h, 3, _textColor);
} 
/*
	creation du bouton + logo prev/next 
	parm : ptr ver lobjet bouton
	return void	
*/
void adriTFTUI_button::buttonNext_create(){
	_mod = BTM_NEXT;
	button_create();
	button_create_nextTriangle();
} 
void adriTFTUI_button::buttonPrev_create(){
	_mod = BTM_PREV;
	button_create();
	button_create_prevTriangle();
} 
/*
	update du logo prev/next
	parm : ptr ver lobjet bouton
	return void	
*/ 
void adriTFTUI_button::buttonPrev_upd(){
	button_create_prevTriangle();
}  
void adriTFTUI_button::buttonNext_upd(){
	button_create_nextTriangle();
}  



void adriTFTUI_button::button_create_volume(){
	float sep 		= _s;
	// float sepRect	= _s;
	float h 		= (_h-(sep*2));
	float hRect 	= (_h/2.5);
	float wRect 	= _w/4;
	float 	x0 	= (_x + (_w/2) ) + sep;
	int16_t y0 	= _y + sep;
	float 	x1 	= x0;
	int16_t y1 	= y0 + h;
	int16_t x2 	= (_x)+(sep);
	int16_t y2 	= y1 - (h/2);

	_tft->fillTriangle(
			x0, 
			y0, 
			x1, 
			y1, 
			x2, 
			y2, 
			_textColor
		);
	_tft->fillRoundRect(_x+sep, (y0 + (h/2)) - (hRect/2), wRect, hRect, 3, _textColor);
} 
/*
	creation du bouton + logo prev/next 
	parm : ptr ver lobjet bouton
	return void	
*/

void adriTFTUI_button::buttonVolume_create(){
	_mod = BTM_VOLUME;
	button_create();
	button_create_volume();
} 

void adriTFTUI_button::buttonVolume_upd(){
	button_create_volume();
}  


/*
	creation du logo play baser sur la postion et taille du bouton 
	parm : bouton object correspondant
	return void	
*/
void adriTFTUI_button::button_create_triangle(){
	float sep 	= _s;
	float h 		= (_h-(sep*2));
	float x0 	= _x + sep;
	float y0 	= _y + sep;
	float x1 	= x0;
	float y1 	= y0 + h;
	float x2 	= (x1 + _w)-(sep*2);
	float y2 	= y1 - (h/2);

	_tft->fillTriangle(
			x0, 
			y0, 
			x1, 
			y1, 
			x2, 
			y2, 
			_textColor
		);
} 
void adriTFTUI_button::button_create_triangle_top(){
	float sep 	= _s;
	// float h 	= (_h-(sep*2));
	float x0 	= _x + sep;
	float y0 	= _y+_h-(sep);
	float x1 	= _x + (_w/2);
	float y1 	= _y + (_h*35)/100;
	float x2 	= (_x + _w) - sep;
	float y2 	= (_y + _h) - sep;

	_tft->fillTriangle(
			x0, 
			y0, 
			x1, 
			y1, 
			x2, 
			y2, 
			_textColor
		);
} 
void adriTFTUI_button::button_create_triangle_bot(){

	float sep 	= _s;
	// float h 	= (_h-(sep*2));
	float x0 	= _x + (_w/2); ;
	float y0 	= ((_y + _h) ) - (_h*35)/100;;
	float x1 	= _x + sep;
	float y1 	= _y + sep;
	float x2 	= (_x + _w) - sep;
	float y2 	= y1;

	_tft->fillTriangle(
			x0, 
			y0, 
			x1, 
			y1, 
			x2, 
			y2, 
			_textColor
		);		
} 
/*
	creation du bouton + logo play 
	parm : ptr ver lobjet bouton
	return void	
*/
void adriTFTUI_button::buttonPlay_create(){
	_mod = BTM_PLAY;
	button_create();
	button_create_triangle();
} 
/*
	update du logo play
	parm : ptr ver lobjet bouton
	return void	
*/ 
void adriTFTUI_button::buttonPlay_upd(){
	button_create_triangle();
}  

void adriTFTUI_button::buttonUp_create(){
	_mod = BTM_UP;
	button_create();
	button_create_triangle_top();
} 
void adriTFTUI_button::buttonUp_upd(){
	button_create_triangle_top();
}  
void adriTFTUI_button::buttonDown_create(){
	_mod = BTM_DOWN;
	button_create();
	button_create_triangle_bot();
} 
void adriTFTUI_button::buttonDown_upd(){
	button_create_triangle_bot();
}  


/*
	creation du logo pause baser sur la postion et taille du bouton 
	parm : ptr ver lobjet bouton
	return void
*/
void adriTFTUI_button::button_create_DoubleRectangle(){
	int16_t w1 				= _w/2;
	int16_t w2 				= w1/2;
	// int16_t btCenter_sep 	= 10;
	int16_t sep 			= _s;
	int 	hB 				= (_h-(sep*2));
	int 	wB 				= _w/4;
	int16_t x0 				= _x + w2;
			x0 				= x0 - (wB/2);
	int16_t x1 				= _x + w1 + w2;
			x1 				= x1 - (wB/2);
	int16_t y0 				= _y + sep;
	int16_t w 				= wB;
	int16_t h 				= hB;
	int16_t r 				= _r-3;

	uint16_t c 				= _textColor;

	_tft->fillRoundRect(x0, y0, w, h, r, c);
	_tft->fillRoundRect(x1, y0, w, h, r, c);	
} 
/*
	creation du bouton + logo pause 
	parm : ptr ver lobjet bouton
	return void	
*/
void adriTFTUI_button::buttonPause_create(){
	_mod = BTM_PAUSE;
	button_create();
	button_create_DoubleRectangle();
}  
/*
	update du logo pause
	parm : ptr ver lobjet bouton
	return void	
*/
void adriTFTUI_button::buttonPause_upd(){
	button_create_DoubleRectangle();
}  

/*
	creation du text a ajouter au bouton
	parm : 	ptr ver lobjet bouton,	
			String text || si String == "" alor sa update le text
	return void	
*/
void adriTFTUI_button::button_create_text(String text) {

	String getText = text;
	if (getText == "") getText = _text;
	if (getText == "") return;

	_tft->setFont(_font); 	

	_text	= getText;
	_mod 	= BTM_TEXT;

	int16_t x1, y1;
	uint16_t w, h;
	_tft->setTextWrap(false);
	_tft->getTextBounds(getText, 0, 0, &x1, &y1, &w, &h);	

	int16_t x0 = _x;
			x0 = x0 + (_w/2);
			x0 = x0 - (w / 2);
	int16_t y0 = _y;
			y0 = y0 + (_h/2);
			y0 = y0 + (h / 2);

	_tft->setTextColor(_textColor, ILI9341_BLACK); 		
	_tft->setCursor(x0, y0);
	_tft->print(getText);	
}
/*
	creation du bouton + texte
	parm : 	ptr ver lobjet bouton,	
			String text || si String == "" alor sa update le text
			police				
	return void	
*/
void adriTFTUI_button::buttonText_create(String text, const GFXfont * font){
	_mod = BTM_TEXT;
	_font = font;
	button_create();
	button_create_text(text);
}  
void adriTFTUI_button::buttonText_create(String text){
	_mod = BTM_TEXT;
	button_create();
	button_create_text(text);
}  

/*
	creation du text et de son bouton baser sur la taille du texte
	parm : 	ptr ver lobjet bouton,	
			String text || si String == "" alor sa update le text
			postion x, y du texte
	return void	
*/
void adriTFTUI_button::buttonFromText(String text, int16_t btX, int16_t btY){

	String getText = text;
	if (getText == "") getText = _text;
	if (getText == "") return;

	_tft->setFont(_font); 	

	_text 	= getText;
	_mod 	= BTM_FTEXT;

	int16_t x1, y1;
	uint16_t w, h;
	_tft->setTextWrap(false);
	_tft->getTextBounds(getText, 0, 0, &x1, &y1, &w, &h);	

	_x = btX;
	_y = btY;
	_w = w + _s;
	_h = h + _s;

	button_create();

	int16_t x0 = _x;
			x0 = x0 + (_w/2);
			x0 = x0 - (w / 2);
	int16_t y0 = _y;
	 		y0 = y0 + (_h/2);
	 		y0 = (y0 + (h / 2)) - 1;

		
	_tft->setTextColor(_textColor, _backgroundColor); 		
	_tft->setCursor(x0, y0);
	_tft->print(getText);		
} 
void adriTFTUI_button::buttonFromText_create(String text, int16_t btX, int16_t btY){
	buttonFromText(text, btX, btY);
} 
void adriTFTUI_button::buttonFromText_create(String text, const GFXfont * font) {
	_font = font;
	buttonFromText(text, _x, _y);
} 
void adriTFTUI_button::buttonFromText_create(String text) {
	buttonFromText(text, _x, _y);
} 





/*
	creation du text a ajouter au bouton
	parm : 	ptr ver lobjet bouton,	
			String text || si String == "" alor sa update le text
	return void	
*/
void adriTFTUI_button::button_create_textUtf8(String text) {
	
	String getText = text;

	if (getText == "") getText = _text;
	if (getText == "") return;

	_text	= getText;
	_mod 	= BTM_TEXT;

	char buf[120];
	sprintf(buf, "%s", getText.c_str());
	uint16_t w, h;

	_u8g2.setFont(_u8g2_font); 	
	_u8g2.setFontMode(0);                
	_u8g2.setFontDirection(0);  
  	_u8g2.setBackgroundColor(_backgroundColor);
  	_u8g2.setForegroundColor(_textColor);   

	// ha = _u8g2.getFontAscent();
	h = _u8g2.getFontAscent() - _u8g2.getFontDescent();
	w = _u8g2.getUTF8Width(buf);

	int16_t x0 = _x;
			x0 = x0 + (_w/2);
			x0 = x0 - (w / 2);
	int16_t y0 = _y;
			y0 = y0 + ((_h/2) + (h/2));
			y0 = (y0 - 2);

	_u8g2.setCursor(x0,y0);   
	_u8g2.print(getText);	
}

/*
	creation du bouton + texte
	parm : 	ptr ver lobjet bouton,	
			String text || si String == "" alor sa update le text
			police				
	return void	
*/
void adriTFTUI_button::buttonTextUtf8_create(String text, const uint8_t * font){
	_mod = BTM_TEXTUTF8;
	_u8g2_font = font;
	button_create();
	button_create_textUtf8(text);
}  
void adriTFTUI_button::buttonTextUtf8_create(String text){
	_mod = BTM_TEXTUTF8;
	button_create();
	button_create_textUtf8(text);
}  

/*
	creation du text et de son bouton baser sur la taille du texte
	parm : 	ptr ver lobjet bouton,	
			String text || si String == "" alor sa update le text
			postion x, y du texte
	return void	
*/
void adriTFTUI_button::buttonFromTextUtf8(String text, int16_t btX, int16_t btY){

	String getText = text;
	if (getText == "") getText = _text;
	if (getText == "") return;


	_text 	= getText;
	_mod 	= BTM_FTEXTUTF8;

	char buf[120];
	sprintf(buf, "%s", getText.c_str());
	uint16_t w, h;

	_u8g2.setFont(_u8g2_font); 	
	_u8g2.setFontMode(1);                
	_u8g2.setFontDirection(0);  
  	_u8g2.setBackgroundColor(_backgroundColor);
  	_u8g2.setForegroundColor(_textColor);    

	// ha = _u8g2.getFontAscent();
	h = _u8g2.getFontAscent() ;
	w = _u8g2.getUTF8Width(buf);

	_x = btX;
	_y = btY;
	_w = w + _s;
	_h = h + _s;

	button_create();
	int h2 = h/2;
	int16_t x0 = _x;
			x0 = x0 + (_w/2);
			x0 = x0 - (w / 2);
	int16_t y0 = _y;
			y0 = y0 + (_h/2);
	 		y0 = y0 + (h2);
	 		// y0 = y0 - ha;

	// _tft->drawFastHLine(_x, y0, _w, _textColor);

	_u8g2.setCursor(x0,y0);         
	_u8g2.print(getText);		
} 
void adriTFTUI_button::buttonFromTextUtf8_create(String text, int16_t btX, int16_t btY){
	buttonFromTextUtf8(text, btX, btY);
} 
void adriTFTUI_button::buttonFromTextUtf8_create(String text, const uint8_t * font) {
	_u8g2_font = font;
	buttonFromTextUtf8(text, _x, _y);
} 
void adriTFTUI_button::buttonFromTextUtf8_create(String text) {
	buttonFromTextUtf8(text, _x, _y);
} 