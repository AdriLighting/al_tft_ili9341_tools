#include "adri_tft_ILI9341_screen.h"

// #define DEBUG
// adriTFTscreen::adriTFTscreen(int count, adriTFTscreen_list * list)
// {
// 	_pos 		= 0;
// 	_pos_max 	= count;
// 	_list 		= list;

	
// }

// void adriTFTscreen::init() {
// 	for (int i = 0; i < _pos_max; ++i) {
// 		Serial.printf("[%d] [adriTFTscreen] %s\n", i, _list[i]._id);	
// 		_isInitialised[i] = 1;
// 	}
// }
// void adriTFTscreen::loop() {
// 	_list[_pos]._loop();
// }
// void adriTFTscreen::display() {
// 	_id = _list[_pos]._id;
// 	_list[_pos]._display(_isInitialised[_pos]);
// 	if (_isInitialised[_pos] = 1) _isInitialised[_pos] = 0;
	
// }
// void adriTFTscreen::next(int delta){
// 	int cPos = _pos;
// 	if 		(cPos == 0 && delta < 0) 				cPos = _pos_max - 1;
// 	else if (cPos >= (_pos_max - 1) && delta > 0) 	cPos = 0;
// 	else 											cPos += delta;
// 	if 		(cPos >= _pos_max) 						cPos = 0;
// 	_pos = cPos;	
// }
// void adriTFTscreen::pos(int pos){
// 	_pos = pos;
// }



int 				adriTFTscreen_count = 0; 
int 				adriTFTscreen_pos 	= 0; 
adriTFTscreen * 	adriTFTscreen_array[SCREEN_SET_MAX];
const char * 		adriTFTscreen_current;

adriTFTscreen::adriTFTscreen(const char * id, void (*f_display)(boolean init), void (*f_loop)())
{
    if (adriTFTscreen_count < SCREEN_SET_MAX) {
    	_id 			= id;
    	_display 		= f_display;
    	_loop 			= f_loop;
    	_isInitialised 	= true;
        adriTFTscreen_array[adriTFTscreen_count++] = this;
        #ifdef DEBUG
        	Serial.printf("\n[adriTFTscreen] [%d] [%s]\n", adriTFTscreen_count, id);
        #endif
    } else {
    	#ifdef DEBUG
    		Serial.printf("\n[adriTFTscreen] FUUUUUUUUUUULLLLLLLL\n");
    	#endif
    }
}
void adriTFTscreen::display(){
	adriTFTscreen_current = _id;
	_display(_isInitialised);
	if(_isInitialised) _isInitialised = false;
	#ifdef DEBUG
		Serial.printf("\n[adriTFTscreen_display] id: %s - curr: %s - init:%d\n", _id, adriTFTscreen_current, _isInitialised);
	#endif
}
void adriTFTscreen_loop(){
	adriTFTscreen_array[adriTFTscreen_pos]->_loop();
}
void adriTFTscreen_display(int pos){
	// Serial.printf("\n[adriTFTscreen_display] %s\n", adriTFTscreen_array[pos]->_id);
	adriTFTscreen_pos = pos;
	adriTFTscreen_array[pos]->display();
}
void adriTFTscreen_display(){
	// Serial.printf("\n[adriTFTscreen_display] %s\n", adriTFTscreen_array[adriTFTscreen_pos]->_id);
	adriTFTscreen_array[adriTFTscreen_pos]->display();
}
void adriTFTscreen_next(int delta, int & ret){
	int cPos = adriTFTscreen_pos;
	if 		(cPos == 0 && delta < 0) 							cPos = adriTFTscreen_count - 1;
	else if (cPos >= (adriTFTscreen_count - 1) && delta > 0) 	cPos = 0;
	else 														cPos += delta;
	if 		(cPos >= adriTFTscreen_count) 						cPos = 0;
	adriTFTscreen_pos = cPos;
	ret = cPos;	
}
void adriTFTscreen_next(int delta){
		int pos;
		adriTFTscreen_next(delta, pos);
		adriTFTscreen_array[pos]->display();
}

void adriTFTscreen_get_pos(int & pos){
	pos = adriTFTscreen_pos;
}
void adriTFTscreen_set_pos(int pos){
	adriTFTscreen_pos = pos;
}
const char *  adriTFTscreen_get_name(){
	return  adriTFTscreen_current;
}