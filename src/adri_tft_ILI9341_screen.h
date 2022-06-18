#ifndef ADRI_TFT_ILI9341_SCREEN_H
	#define ADRI_TFT_ILI9341_SCREEN_H

	#include <arduino.h>

	#define SCREEN_SET_MAX 10
	#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

	class adriTFTscreen {
		int 					_isInitialised;
		const char * 			_id;

		public:	
			adriTFTscreen(const char * id, void (*f_display)(boolean init), void (*f_loop)());

			void 			(*_display)(boolean init);
			void 			(*_loop)(void);
			void 			(*_end)(void);

			// void next(int);
			// void pos(int);
			void loop();
			void display();
			// void init();

	};

void adriTFTscreen_loop();
void adriTFTscreen_display(int pos);
void adriTFTscreen_display();
void adriTFTscreen_next(int delta, int & ret);
void adriTFTscreen_next(int delta);
void adriTFTscreen_get_pos(int & pos);
const char *  adriTFTscreen_get_name();
void adriTFTscreen_set_pos(int pos);
extern int adriTFTscreen_count;
#endif