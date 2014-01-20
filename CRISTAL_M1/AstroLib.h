/*****
*   File:
*     Function Code, use on AstroBoards                 *
*   by: jrg                                             *
*   date: September 2013                                *
*                                                   *****/

//Functions
uint8_t FUNC_ASTRO_RES = 0xFD;  //Reset every MCP
uint8_t FUNC_ASTRO_TOUT = 0xFB; // Set MPCs timeout
uint8_t FUNC_ASTRO_MCNT = 0xEB; //Set MPCs max counts
uint8_t FUNC_ASTRO_TIM = 0xFC;  //Read  Calib Timing counters
uint8_t FUNC_ASTRO_AMP = 0xFA;  //Reset Calib Amplitud counters
uint8_t FUNC_ASTRO_RATES = 0x00;  //Read Counters (cheeck function code)
uint8_t FUNC_ASTRO_GLOBAL = 0xA7;  //Read Global counter 
uint8_t FUNC_ASTRO_SUML3 = 0xAA;  //Read Sum-L3 counter 
uint8_t FUNC_ASTRO_ED = 0xA0;  //Set external delay 
uint8_t FUNC_ASTRO_UPC = 0x87;  //Set compartor threshold
uint8_t FUNC_ASTRO_DEL = 0xFF;  //Set oneshot delay
uint8_t FUNC_ASTRO_ADEL = 0xFE;  //set oneshot delay analog
uint8_t FUNC_ASTRO_VER = 0xF1;  // read Firmware version
uint8_t FUNC_ASTRO_TEM = 0xE0;  //read Temperature
uint8_t FUNC_ASTRO_ERR = 0xF0;  //read Error
uint8_t FUNC_ASTRO_RAT = 0xA0;  //set delay line delay
uint8_t FUNC_ASTRO_GEN = 0xA1;  //enable gate
uint8_t FUNC_ASTRO_GDIS = 0xA2;  //disable gate
uint8_t FUNC_ASTRO_LASER = 0xA9;  //laser control

//coversion
float comp_conv =4095./2.5; 
float ddel_conv =511./5.6; 
float edel_conv1 =255./510.; 
float edel_conv2 =255./64.; 

