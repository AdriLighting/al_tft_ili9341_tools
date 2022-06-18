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
 * fillArc2 			https://forum.arduino.cc/index.php?topic=590677.0
 * ringMeter 			https://www.instructables.com/id/Arduino-analogue-ring-meter-on-colour-TFT-display/
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
#include <adri_timer.h>
#include <adri_tools.h>
#include <adri_tft_ILI9341_extrafunc.h>
#include "fonts/SansSerif_plain_10.h"
#include "fonts/SansSerif_plain_12.h"
	#include <adri_tft_ILI9341_ui.h>
	#include <adri_tft_ILI9341_txt.h>

#include <ILI9341_esp_controls.h>
#include <JPEGDecoder.h>
#include "ico.h"
Adafruit_ILI9341  		tft = Adafruit_ILI9341(15, 2);
XPT2046 				touch(/*cs=*/ 4, /*irq=*/ 5);
adriTFTUI 				ui 	= adriTFTUI(&tft);
U8G2_FOR_ADAFRUIT_GFX 	u8g2;
#define TURTLE_PI 3.14159265 // pi is used to set turtle heading and turn turtle
float m_x = 160;
float m_y = 220;
float m_heading = 35; // direction in radians   
boolean m_penDown = true;

uint8_t  Magnifier;
uint16_t First_Glyph;
void setup()
{
    Serial.begin(115200);
	while (!Serial) {
		yield(); 
	}
	delay(1000);
	Serial.println("---");
	Serial.println("");

	SPIFFS.begin();


	tft_setup(&tft, &touch);
	u8g2.begin(tft); 

	// int y 		= 95;
	// int x 		= 165;
	// int a 		= 0;
	tft.fillScreen(ILI9341_BLACK);
	// fillArc2(&tft, x, y, a, 120, 		90, 90, 10, C_YELLOW);
	// fillArc2(&tft, x, y, a, 30, 		90, 90, 10, C_WHITE);

	// float sx = 0, sy = 95, mx = 1, my = 0, hx = -1, hy = 0;    // Saved H, M, S x & y multipliers
	// uint16_t x0=0, x1=0, yy0=0, yy1=0;

	// 	int xA 	= 90;
	// 	int xB 	= 75; 
	// 	int	xC 	= 165; 
	// 	int	xD 	= 95; 
	// for(int i = 0; i<360; i+= 30) {
	// 	sx = cos((i-90)*0.0174532925);
	// 	sy = sin((i-90)*0.0174532925);
	// 	x0 = sx*xA+xC;
	// 	yy0 = sy*xA+xD;
	// 	x1 = sx*xB+xC;
	// 	yy1 = sy*xB+xD;

	// 	tft.drawLine(x0, yy0, x1, yy1, ILI9341_GREEN);
	// }
 //  // Draw 60 dots
 //  for(int i = 0; i<360; i+= 6) {
 //    sx = cos((i-90)*0.0174532925);
 //    sy = sin((i-90)*0.0174532925);
 //    x0 = sx*102+xC;
 //    yy0 = sy*102+xD;
 //    // Draw minute markers
 //    tft.drawPixel(x0, yy0, ILI9341_WHITE);
    
 //    // Draw main quadrant dots
 //    if(i==0 || i==180) tft.fillCircle(x0, yy0, 2, ILI9341_WHITE);
 //    if(i==90 || i==270) tft.fillCircle(x0, yy0, 2, ILI9341_WHITE);
 //  }
adriTFTUI_txtBox * _lbl_homeScreen;
	_lbl_homeScreen = new adriTFTUI_txtBox(&tft, u8g2);
	adriTFTUI_txtBox txt(&tft, u8g2);
	txt.u8g2_dir(0);
		txt.u8g2_draw(110, 110, "text", ui.color565(210, 210, 210), u8g2_font_helvR12_tf);
tft.invertDisplay(1);
	// int *pos = create_rect_3(10, 10, 20, 20, 30, "screenPos", "rectTitle", _lbl_homeScreen, &SansSerif_plain_12,  35, 35, 10);
	// ui.button_int	( pos[0], pos[1], 35, 35, 8, 10, C_WHITE, C_BLUE,	BTM_PREV,		"",	"cfg");
	// ui.button_int	( pos[2], pos[1], 35, 35, 8, 10, C_WHITE, C_BLUE,	BTM_PLAY,		"",	"cfg");
	// ui.button_int	( pos[3], pos[1], 35, 35, 8, 10, C_WHITE, C_BLUE,	BTM_NEXT,		"",	"cfg");
	// button_create("cfg");	

  // 60 segment pour secondes et 4 pr 0 6 12 18
	// for(int i = 0; i<360; i+= 6) {
	// 	sx = cos((i-90)*0.0174532925);
	// 	sy = sin((i-90)*0.0174532925);
	// 	x0 = sx*102+120;
	// 	yy0 = sy*102+120;
	// 	// Draw minute markers
	// 	tft.drawPixel(x0, yy0, ILI9341_WHITE);

	// 	// Draw main quadrant dots
	// 	if(i==0 || i==180) tft.fillCircle(x0, yy0, 2, ILI9341_WHITE);
	// 	if(i==90 || i==270) tft.fillCircle(x0, yy0, 2, ILI9341_WHITE);
	// }
  // int h = 80,w = 110, row, col, buffidx=0;
  // for (row=0; row<h; row++) { // For each scanline...
  //   for (col=0; col<w; col++) { // For each pixel...
  //     //To read from Flash Memory, pgm_read_XXX is required.
  //     //Since image is stored as uint16_t, pgm_read_word is used as it uses 16bit address
  //     tft.drawPixel(col, row, pgm_read_word(evive_in_hand + buffidx));
  //     buffidx++;
  //   } // end pixel
  // }
//   delay (1000);
//   Magnifier=1;
//   First_Glyph=0;
// Show() ;
}




void loop()
{

}


// /*
//  * Use this function instead of display.drawChar to draw the symbol or to use
//  * the default font if it's not in the symbol range.
//  */
// void drawSymbol(uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t Size){
//   if( (c>=32) && (c<=126) ){ //If it's 33-126 then use standard mono 18 font
//       tft.setFont(&SansSerif_plain_10);
//   } else {
//     tft.setFont(&SymbolMono18pt7b);//Otherwise use special symbol font
//     if (c>126) {      //Remap anything above 126 to be in the range 32 and upwards
//       c-=(127-32);
//     }
//   }
//   tft.drawChar(x,y,c,color,bg,Size);
// }

// /*
//  * Display characters on the screen starting with First_Glyph until you hit
//  * last character of the font or run out of space on the screen.
//  */
// void Show(void)  {
//   /*
//    * Although our characters are at most 16 pixels wide, we're going to space them
//    * an arbitrary horizontal amount DELTA_C for readability. Similarly the vertical 
//    * spacing is set at DELTA_R for readability. The BASE_R is a vertical adjustment 
//    * that is the distance from the top of the cell to the baseline of the character.
//    */
//   #define DELTA_C 21
//   #define DELTA_R 30
//   #define BASE_R 22
//   #define DHEIGHT 320
//   #define DWIDTH 
//   uint8_t Max_C = DWIDTH / (DELTA_C * Magnifier);
//   uint8_t Max_R = DHEIGHT / (DELTA_R * Magnifier);
//   uint8_t NumChar = Max_C * Max_R;
//   uint16_t Last_Glyph = min(First_Glyph+NumChar-1,SymbolMono18pt7b.last+128-32);
//   // Serial.print("Displaying "); Serial.print(NumChar,DEC);
//   // Serial.print(" glyphs in "); Serial.print(Max_R,DEC);
//   // Serial.print(" rows by "); Serial.print(Max_C,DEC);
//   // Serial.println(" columns.");
//   // Serial.print("Magnifier:"); Serial.println(Magnifier,DEC);
//   // Serial.print("Displaying characters:"), 
//   // Serial.print(First_Glyph,DEC); 
//   // Serial.print(" through ");
//   // Serial.println(Last_Glyph,DEC);
//   // Serial.println();
//   tft.fillScreen(ILI9341_BLACK);  //Clear the screen
//   if(Magnifier>1){    //If magnified, draw a grid for reference
//     for (uint16_t C=0; C<DWIDTH; C=C+(DELTA_C*Magnifier)) {
//       tft.drawFastVLine(C,0,DHEIGHT,ILI9341_YELLOW); 
//     }
//     for (uint16_t R=0; R<DHEIGHT; R=R+(DELTA_R*Magnifier)) {
//       tft.drawFastHLine(0,R,DWIDTH,ILI9341_YELLOW); 
//     }
//   }
//   uint8_t I=0;
//   for (uint8_t Glyph = First_Glyph; Glyph <= Last_Glyph; Glyph++) {
//     drawSymbol((I % Max_C) * DELTA_C * Magnifier,
//         (BASE_R*Magnifier) + (I / Max_C) * DELTA_R * Magnifier,
//         Glyph,ILI9341_WHITE,ILI9341_BLACK,Magnifier);
//     I++;
//   }
// }

int * create_rect_3(
	int xRect, int yRect, int lMarge, int tMarge, int vMarge,
	String value, String rectTitle , adriTFTUI_txtBox * txt, const GFXfont * font,
	int wBt_1, int hBt_1, int sBt_1){

	uint16_t h_rectTitle, w_rectTitle;
	uint16_t wT, hT;

	txt->text_font(font);
	txt->text(rectTitle);
	txt->text_setup();
	txt->drawString_getWh(rectTitle, w_rectTitle, h_rectTitle);
	txt->text_font(font);
	txt->text(value);
	txt->text_setup();
	txt->drawString_getWh(value, wT, hT);

	int yT_1;
	int xBt_1;
	int ybT_1;
	int wRect 	= (w_rectTitle) + (lMarge*2);
	int wTxt 	= (wT) + (lMarge*2);
	int wBt 	= (wBt_1*3) + ((sBt_1*2));
	int hRect 	= hT + hBt_1 + (tMarge*2) + vMarge;

	if (wRect < wTxt) 					wRect = wTxt;
	if (wRect < (wBt + (lMarge*2)))		wRect = (wBt + (lMarge*2));
	 

		xBt_1 	= xRect + (wRect/2) - (wBt/2);
	int xT_1 	= xRect + (wRect/2) ;
		ybT_1   = yRect + tMarge + hT + vMarge;
		yT_1   	= yRect + vMarge ;
	int xBt_2 	= xBt_1 + sBt_1 + wBt_1;
	int xBt_3 	= xBt_1 + (sBt_1*2) + (wBt_1*2);

	txt->_text_x_ref = xT_1;	
	
	txt->text_font(font);
	txt->textPos(xT_1, yT_1);	
	txt->textColor(C_WHITE, 0);
	txt->drawStringAling(value, CENTER);

	char buf[80];
	sprintf(buf , "%s", rectTitle.c_str());
	ui.drawRectWithLabel(xRect, yRect, wRect, hRect, 50, 5, C_YELLOW, C_WHITE, buf, font);	

    int     *array          = new int[4];
            array[0]        = xBt_1;
            array[1]        = ybT_1;
            array[2]        = xBt_2;
            array[3]        = xBt_3;
    return array;   	
}

void button_create(const char * screen){
	for (int i = 0; i < ui._buttonCnt; ++i) {
		if (ui.button_manage[i]->_screen == screen) ui.button[i]->button_create_fromLoop();
	}		
}