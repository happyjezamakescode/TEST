/********
* file: CRISTAL_SPI.h      
*      Prototypes for the foxg20 SPI-BUS to comunicate with Astro, and Clip boards  *
* by: jrg                                                                           *
* date: September 2013                                                              *
                                                                                 ****/
//#include <stdint.h>
void getdummy(int *var);
void pabort(const char *s);
int POWER1_SET(int fdPower1,uint8_t Function,uint8_t Board,uint8_t Channel,uint8_t Value1,uint8_t Value2);
int POWER2_SET(uint8_t Function,uint8_t Board,uint8_t Channel,uint8_t Value1,uint8_t Value2);
int CPLD_conect();
int CPLD_disconect();
int transfer_t(uint8_t Function, uint8_t Board, uint8_t Channel, short *data);
int transfer(uint8_t Function, uint8_t Board, uint8_t Channel, unsigned short data);
int transfer_c(uint8_t Function, int *counts, int *time, int *macro_rates);

