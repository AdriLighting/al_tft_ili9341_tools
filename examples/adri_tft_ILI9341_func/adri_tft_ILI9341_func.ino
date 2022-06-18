/**
 * @defgroup  TFTUI
 * 
 * @author     Adri
 * @date       2020
 * 
 * @brief      	Créations de l'environement TFT pour mon projet de reveill wifi/domo multifonction
 * 
 * 				PIC 	ESP8266 MODEMCU V1.0 AMICA 
 * 				TFT 	240x320 ILI9341+TOUCHPANEL 
 * 				
 * 				requis :
 * 					multiple ecran
 * 					bouton  avec mémorisation des coord pour le touch panel
 * 						- navigation ecran
 * 						- reglahe heure/alarm
 * 						- player mp3
 * 						- ...
 * 					label mis a jour régulierement 
 * 						- heure
 * 						- date
 * 						- player mp3
 * 						- ...
 * 					support du text UTF-8
 * 					BITMAP/JPEG
 * 					
 * AVEC DU COURRAGE PASSER L"ESSEMBLE DE L'UDF POUR L'ESP32 pour le CIRCUIT FINALE DU REVEILL					
 *		ESP32 PROBLEMME :
 *							TOUCHSCREEN POSITION.
 *							MES UDF SONT TOUTE BASER SUR LESP8266.
 *							
 * Librairie perso :
 * TFT :
 * 		<adri_tft_ILI9341_driver.h>
 * 		<adri_tft_ILI9341_ui.h>
 * 		<adri_tft_ILI9341_txt.h>
 * WEBSERVER
 * 		<adri_espwebserver.h>
 * 		<adri_webserver_parsecommand.h>
 * 		<adri_webserver_reponse.h>
 * 		<adri_webserver_reponselist.h> 	
 * ALARME
 * 		<calendar.h>
 * 		<calendar_add.h>
 * 		<calendar_date.h> 
 * PLAYER MP3
 * 		<adri_dfplayer.cpp>		
 * 		
 * <adri_timer.h>
 * <adri_tools.h>
 * <adri_wifi.h>
 * <adri_udp.h>
 * <adri_wifi.h>
 * 
 * MODULE SERVEUR MP3: ESP8266 WEMOSD1MINI, DFPLAY
 * 		<adri_dfplayer.h>			
 * 
 * RTC MODULE
 * 		voir si le cablage et passible sur le wemosd1mini + le tft-touch
 * 
 * 
 * 
 * Librairies importer :
 * SoftwareSerial  		https://github.com/PaulStoffregen/SoftwareSerial
 * DFRobotDFPlayerMini	https://github.com/DFRobot/DFRobotDFPlayerMini	 
 * AdarfruitGFX 		https://github.com/adafruit/Adafruit-GFX-Library
 * Adafruit_ILI9341esp
 * u8g2 utf8			https://github.com/olikraus/U8g2_for_Adafruit_GFX
 * XPT2046 				https://github.com/PaulStoffregen/XPT2046
 * JPEGDecoder 			https://github.com/Bodmer/JPEGDecoder
 * controls 			https://github.com/KrisKasprzak/ILI9341_t3_controls
 * ESP826WIFI
 * ESP826
 * 
 * fonstion importeer :
 * 		<adri_tft_ILI9341_extrafunc.h>
 * 			fillArc2 			https://forum.arduino.cc/index.php?topic=590677.0
 * 			ringMeter 			https://www.instructables.com/id/Arduino-analogue-ring-meter-on-colour-TFT-display/
 * 			7segment 			https://forum.arduino.cc/index.php?topic=307348.0
 * 
 * inspiration :
 * minigrafx 			https://github.com/ThingPulse/
 * hula hoop 			nri
 * tutrtle graphic 		https://www.instructables.com/id/Arduino-TFT-Shield-Turtle-Graphics/
 * 
 * 
 * 
 */

#include <adri_tft_ILI9341_driver.h>
#include <adri_tft_ILI9341_ui.h>
#include <adri_tft_ILI9341_txt.h>
#include <adri_timer.h>
#include <adri_tools_v2.h>

#include <ILI9341_esp_controls.h>
#include <JPEGDecoder.h>


	#include "fonts/ArialRoundedMTBold_10.h"
	#include "fonts/ArialRoundedMTBold_12.h"
	#include "fonts/ArialRoundedMTBold_14.h"
	#include "fonts/ArialRoundedMTBold_15.h"
	#include "fonts/ArialRoundedMTBold_16.h"
	#include "fonts/SansSerif_plain_14.h"
	#include "fonts/SansSerif_plain_11.h"
	#include "fonts/SansSerif_plain_10.h"
	// select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall


Adafruit_ILI9341  		tft = Adafruit_ILI9341(15, 2);
XPT2046 				touch(/*cs=*/ 4, /*irq=*/ 5);
adriTFTUI 				ui 	= adriTFTUI(&tft);
U8G2_FOR_ADAFRUIT_GFX 	u8g2;


adri_timer * timer_1;
adri_timer * timer_2;
adri_timer * timer_3;
adri_timer * timer_4;
adri_timer * timer_5;
adri_timer * timer_6;
adri_timer * timer_7;

adriTFTUI_button * btn_upd;

adriTFTUI_txtBox * _txtBox_canvas;
adriTFTUI_txtBox * _txtBox_scroll;
adriTFTUI_txtBox * _txtBox_bg;
adriTFTUI_txtBox * _txtBox_bg_2;



SliderH Red(&tft, 	60, 140,	200, C_WHITE, C_BLACK, C_RED);
SliderV Blue(&tft, 	20, 40, 	100, C_WHITE, C_BLACK, C_BLUE);

static uint16_t touch_prev_x = 0xffff, touch_prev_y = 0xffff;
int 	touch_btn_interval = -1;
boolean touch_btn_resetBorder = false;

int 	screen_txt_counter = 0;
bool 	screen_txt_loop_mod = false;

void screen_footer_init();;

void setup()
{
    Serial.begin(115200);
	while (!Serial) {
		yield(); 
	}
	delay(1000);
	Serial.println("---");
	Serial.println("");

	LittleFS.begin();

    tft_setup(&tft, &touch);

	u8g2.begin(tft);    

	ui.u8g2_attach(u8g2);

	// myScreen.init();
	// myScreen.pos(1);
  	new adriTFTscreen("main", 	screen_main, 	screen_end);
  	new adriTFTscreen("txt", 	screen_txt, 	screen_txt_loop);
  	new adriTFTscreen("bar", 	screen_bar, 	screen_bar_loop);

	tft.fillScreen(ILI9341_BLACK);

	_txtBox_canvas = new adriTFTUI_txtBox(&tft, u8g2);
	_txtBox_canvas->canvas_create(10, 20, 200, 20);
	_txtBox_canvas->text_font(&SansSerif_plain_14);
	_txtBox_canvas->textColor(C_CYAN, 0);

	_txtBox_scroll = new adriTFTUI_txtBox(&tft, u8g2);
	_txtBox_scroll->textColor(ILI9341_GREEN, ui.color565(40, 40, 40));
	_txtBox_scroll->u8g2_mod(1);
	_txtBox_scroll->u8g2_font(u8g2_font_helvR14_tf);
	_txtBox_scroll->text("_txtBox_scroll");
	_txtBox_scroll->u8g2_setup();

	_txtBox_bg = new adriTFTUI_txtBox(&tft, u8g2);

	_txtBox_bg_2 = new adriTFTUI_txtBox(&tft, u8g2);

	timer_1 = new adri_timer(250, 	"touch_btn", 			true);
	timer_2 = new adri_timer(10, 	"bar", 					true);
	timer_3 = new adri_timer(1000, 	"canvas", 				true);
	timer_4 = new adri_timer(10000, "screen", 				true);
	timer_5 = new adri_timer(250, 	"touch_btn_intervall", 	true);
	timer_6 = new adri_timer(500, 	"txt", 					true);
	timer_7 = new adri_timer(1000, 	"txtarray", 			true);

	screen_footer_init();

	adriTFTscreen_set_pos(1);
	adriTFTscreen_display();
	screen_footer_display();
// screen_bar();
	Serial.println("EXEMPLE STARTING");
}




void loop()
{
	// if (timer_4->loop()) {adriTFTscreen_next(1);	screen_footer_display();}

	// adriTFTscreen_loop();
	button_loop();
	
}


void screen_end(){

}


void screen_bar(bool init){
	tft.fillScreen(ILI9341_BLACK);
  	Red.init    (  0, 255, 127.5, 0);
  	Red.drawSliderColor(true);
  	Blue.init    (  0, 255, 127.5, 0);
  	Blue.drawSliderColor(true);  
  	adriTFTUI_progressBar drawProgressBar(&tft, 50, 40, 220, 20, 0, 0, C_WHITE, C_YELLOW);
  	drawProgressBar.draw();	
	Red.draw(0);
	Blue.draw(0);  	
}
void slider_loop(){
	uint16_t x, y;
	int value_1;
	int value_2;
	if (_isTouched(&touch)) {
		touch.getPosition(x, y);
		touch_prev_x = x;
		touch_prev_y = y;
		value_1 = Red.slide(touch_prev_x, touch_prev_y);
		value_2 = Blue.slide(touch_prev_x, touch_prev_y);
		if (Red.changed()) 	{adriTFTUI_progressBar drawProgressBar_1(&tft, 50, 40, 220, 20, 0, value_1, C_WHITE, C_YELLOW) ;drawProgressBar_1.draw();}
		if (Blue.changed()) {adriTFTUI_progressBar drawProgressBar_2(&tft, 50, 40, 220, 20, 0, value_2, C_WHITE, C_YELLOW) ;drawProgressBar_2.draw();}	
				

	} else touch_prev_x = touch_prev_y = 0xffff;		
}
void screen_bar_loop(){

	slider_loop();
/*
	static uint8_t value = 0;
	static boolean fw = true;
	if (timer_2->loop()) {
		if (fw) {
			value++;	
			if (value>=255) fw = false;
		} else {
			value--;	
			if (value<=0) fw = true;
		}
		
		slider_loop(value);
		progressr_loop(value);
	}
*/
}
void slider_loop(uint8_t value){
	Red.draw(value);
	Blue.draw(value);
}
void progressr_loop(uint8_t value){
}
int txtArray_pos=0;
String txtArray[5] = {
	"classique sans accent",
	"line 2",
	"(crac speciaux : line 3)",
	"4",
	"hello world how your ar to day, fine ?"
};
void screen_txt(bool init){

	screen_txt_loop_mod = !screen_txt_loop_mod;

	tft.fillScreen(ui.color565(35, 35, 35));

	drawJPEGFromSpiffs("/Mouse.jpg", 160, 10);

	adriTFTUI_txtBox temp_2(&tft, u8g2);
	temp_2.u8g2_draw 	(10, 60, 	"u8g2_font_helvR14_tf",	ui.color565(255, 0, 0), u8g2_font_helvR14_tf);

	_txtBox_bg->drawString 	(10, 85, 	"equalizer", 	ui.color565(255, 0, 0), 	ui.color565(35, 35, 35), &SansSerif_plain_10);
	_txtBox_bg_2->u8g2_draw (10, 110, 	"equalizer",	ui.color565(255, 0, 0), 	ui.color565(35, 35, 35), u8g2_font_helvR10_tf);

	_txtBox_scroll->textPos(120, 135);

	// adriTFTUI_txtBox temp_2(&tft, u8g2);
	// temp_2.text("_txtBox_3");
	// temp_2.text_font(&ArialRoundedMTBold_16);
	// temp_2.textColor(ui.color565(0, 200, 255), ui.color565(30, 80, 100));

	// temp_2.text_setup();
	// temp_2.textPos(10, 150);
	// temp_2.drawString();


  	// ui.u8g2_draw(10, 55, "u8g2_font_crox5tb_tf", 	ui.color565(0, 0, 255), 	u8g2_font_crox5tb_tf);

  	// ui.u8g2_draw(10, 80, "u8g2_font_logisoso16_tf", ui.color565(0, 255, 0), 	u8g2_font_logisoso16_tf);  

  	// ui.u8g2_draw(10, 105, "u8g2_font_inr16_mf", 	ui.color565(255, 0, 0), 	u8g2_font_inr16_mf);   

  	// ui.u8g2_draw(190, 130, "u8g2_font_helvR12_tf", 	ui.color565(255, 0, 0),	 	u8g2_font_helvR12_tf);  

  	// ui.u8g2_draw(10, 130, "inr16_mf", 				ui.color565(255, 25, 250), 	ui.color565(0, 80, 80), u8g2_font_inr16_mf);    	

  	// ui.u8g2_draw(10, 155, "u8g2_font_helvR12_tf", 	ui.color565(255, 25, 250), 	ui.color565(0, 80, 80), u8g2_font_helvR12_tf);    	


}
void screen_txt_loop(){
	int w1, h1;
	static int x 				= 0;
	static int y 				= 0;
	static boolean textwrap_dir = true;
	String str_1;

	if (timer_7->loop(100)){
		_txtBox_bg->drawString(txtArray[txtArray_pos]);
		_txtBox_bg_2->u8g2_drawRect(txtArray[txtArray_pos]);
		txtArray_pos++;
		if (txtArray_pos>4)txtArray_pos=0;



	}
	if (timer_6->loop(5) ) {
		_txtBox_scroll->u8g2_get_wh(w1, h1);
		y = _txtBox_scroll->_text_y;
		// x = ((tft.width() /2) - (w1/2));
		if (textwrap_dir){
			x++;	
			if ((x + w1) > tft.width()) {
				textwrap_dir = false;
				x = tft.width()-w1;
			}
		} else {
			if (x <= (1)) {
				textwrap_dir = true;
				x=0;
			}
			x--;				
		}

		str_1 = "x: " + String(x);
		_txtBox_scroll->u8g2_drawRect(str_1, x, y);			
		_txtBox_canvas->text(String(millis()/1000));
		_txtBox_canvas->canvas_display();		
	}

}

bool _isTouched(XPT2046 * touchScreen) {
 	return touchScreen->isTouching();
}
void button_loop(){
	const char * curr_screen = adriTFTscreen_get_name();
	// char buffer[80];
	uint16_t x, y;
	if ( timer_1->loop_wait()) {
		if (_isTouched(&touch)) {
			touch.getPosition(x, y);
			touch_prev_x = x;
			touch_prev_y = y;

		} else touch_prev_x = touch_prev_y = 0xffff;	
			
		for (int i = 0; i < ui._buttonCnt; ++i) {
			if (ui.button_manage[i]->_screen == "footer") {}		
			else {
				if (curr_screen != ui.button_manage[i]->_screen) continue;
			}
			ui.button[i]->press(ui.button[i]->contains(touch_prev_x, touch_prev_y));
			if (ui.button[i]->justPressed()) {
				timer_5->set_duration(millis());
				touch_btn_interval = i;
				ui.button[i]->button_borderColor(true, ILI9341_GREEN);	
			}	
			if (ui.button[i]->justReleased()) {

				if (touch_btn_interval >= 0 ){

					if (timer_5->loop()) {

						// sprintf(buffer, "[%d] - %s - %s", i, ui.button_manage[i]->_screen, curr_screen);
						// Serial.println(buffer);

						ui.button[i]->button_borderColor(false, 0);

						ui.button[i]->_func();

					}else {
						touch_btn_resetBorder = true;
					}

					touch_btn_interval = -1;

				}

				timer_1->activate();
				break;
			}	
		}			
	}	
	if (touch_btn_resetBorder) {
		for (int i = 0; i < ui._buttonCnt; ++i)	{
			touch_btn_resetBorder = false;
			if (ui.button_manage[i]->_screen == "footer") {}		
			else {
				if (curr_screen != ui.button_manage[i]->_screen) continue;
			}			
			ui.button[i]->button_borderColor(false, 0);
		}
	}
}

void sreen_next(){
	adriTFTscreen_next(1);	
	screen_footer_display();
}
void sreen_prev(){
	adriTFTscreen_next(-1);	
	screen_footer_display();
}
void screen_footer_init() {
	int wBt = 80;
	int hBt = 30;	
	int starty = (tft.height() ) - (hBt+10);
	adriTFTUI_button * myBtn;
	myBtn = ui.button_int( 10, 		starty, wBt, hBt, 	8, 10, C_DKGREY, ILI9341_NAVY, BTM_TEXT, 	"",	"footer");
	myBtn->button_changeText("précédent");
	myBtn->button_changeTextFont(&SansSerif_plain_11);
	myBtn->attachFunc(sreen_prev);

	int startX = (tft.width() - (wBt + 10));
	myBtn = ui.button_int( startX,	starty, wBt, hBt, 	8, 10, C_DKGREY, ILI9341_NAVY, BTM_TEXT, 	"",	"footer");
	myBtn->button_changeText("suivant");
	myBtn->button_changeTextFont(&SansSerif_plain_11);
	myBtn->attachFunc(sreen_next);

}
void screen_footer_display() {
	for (int i = 0; i < ui._buttonCnt; ++i) {
		if (ui.button_manage[i]->_screen == "footer") ui.button[i]->button_create_fromLoop();
	}		
}

void screen_main(bool init) {

	tft.fillScreen(ILI9341_BLACK);

	int wBt = (tft.width()-50);
	int hBt = 45;
	int startX = (tft.width()/2) - (tft.width()-50)/2;
	int starty = 20;	

	if (init) {

		adriTFTUI_button * myBtn;

		btn_upd = ui.button_int( startX, starty, wBt,hBt, 	8, 10, C_DKGREY, 		C_GREY, 		BTM_TEXT, 	"upd",	"main");
		btn_upd->button_changeText("touch me");
		btn_upd->button_changeTextFont(&SansSerif_plain_11);
		starty += 65;
		startX = 35;
		wBt = 50;
		myBtn = ui.button_int( startX,	starty, wBt, hBt, 	8, 10, ILI9341_DARKGREY, ILI9341_NAVY, BTM_PREV, 	"",		"main");
		startX = myBtn->_x + myBtn->_w + 15;
		myBtn = ui.button_int( startX, 	starty, wBt, hBt, 	8, 10, ILI9341_DARKGREY, ILI9341_NAVY, BTM_PLAY, 	"",		"main");
		startX = myBtn->_x + myBtn->_w + 15;
		myBtn = ui.button_int( startX, 	starty, wBt, hBt, 	8, 10, ILI9341_DARKGREY, ILI9341_NAVY, BTM_NEXT, 	"",		"main");
		startX = myBtn->_x + myBtn->_w + 15;
		myBtn = ui.button_int( startX, 	starty, wBt, hBt, 	8, 10, ILI9341_DARKGREY, ILI9341_NAVY, BTM_RND, 	"",		"main");
		starty += myBtn->_h+10;
		myBtn = ui.button_int( 35,		starty, 50, 50, 	8, 10, ILI9341_DARKGREY, ILI9341_NAVY, BTM_FTEXT,	"",		"main");
		myBtn->buttonFromText_create("Bienvenue sue ili9341", &SansSerif_plain_11);
		// myBtn = ui.button_int( 35,	starty, 50, 50, 	8, 10, ILI9341_DARKGREY, ILI9341_NAVY, BTM_FTEXTUTF8, 		"",	"main");
		// myBtn->buttonFromTextUtf8_create("Bienvenue sue ili9341", u8g2_font_helvR12_tf);
	} 
	for (int i = 0; i < ui._buttonCnt; ++i) {
		if (ui.button_manage[i]->_screen == "main") ui.button[i]->button_create_fromLoop();
	}		

}

void drawJPEGFromSpiffs(String filename, int xpos, int ypos) {
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
      
      tft.setAddrWindow(mcu_x, mcu_y, mcu_x + (mcu_w / zoomFactor) - 1, mcu_y + (mcu_h / zoomFactor) - 1);
      // uint32_t count = mcu_pixels; // what was this for?
      
      for (uint8_t y = 0; y < mcu_h; y++) {
        for (uint8_t x = 0; x < mcu_w; x++) {
          if (x % zoomFactor == 0 && y % zoomFactor == 0) {
            tft.pushColor(*pImg);
          }
          *pImg++;
        }
      }
      // drawSongInfo();
  
  }

}
