/*****
*   File:
*     Function Code, use on ClipBoards                  *
*   by: jrg                                             *
*   date: September 2013                                *
*                                                   *****/

//Functions
uint8_t FUNC_CLIP_DEL = 0x01;
uint8_t FUNC_CLIP_CLP = 0x02;
uint8_t FUNC_CLIP_ATT = 0x03;
uint8_t FUNC_CLIP_LED = 0x04;
uint8_t FUNC_CLIP_SWC = 0x05;//fixdelay
uint8_t FUNC_CLIP_DIS = 0x06;
uint8_t FUNC_CHANGE_TEM = 0x10;
uint8_t FUNC_CLIP_SN = 0x08;
uint8_t FUNC_CLIP_RTE = 0x0E;
uint8_t FUNC_CLIP_TEM = 0x0F;
uint8_t FUNC_CLIP_VER = 0x06;
//Others:
uint8_t TEMP_CLIP_CPLD = 0x48; //channel
uint8_t TEMP_CLIP_RES = 0x49; //channel
uint8_t ALL_BOARDS = 0xFF; //channel
uint8_t ALL_C = 0xFF; //channel
//coversion
float att_conv =63./31.5; 
float clip_conv =4095./2.5; 
float del_conv =4095/30.0; 

