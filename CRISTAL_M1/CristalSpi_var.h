const char *device = "/dev/spidev1.0";
uint8_t mode;
uint8_t Function = 0x00;
uint8_t Board = 0x00;
uint8_t Channel = 0x00;
uint8_t Value1 = 0x00;
uint8_t Value2 = 0x00;
uint16_t delay;
float  Value, max;
int fd,ii,boardnumber,channelnumber;
int op0=0, op1=0, op2=0,op3=0,op4=0,op=5;
float data=0,max_number=0;

