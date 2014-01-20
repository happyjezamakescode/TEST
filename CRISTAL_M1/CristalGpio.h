//////////////////////////////////
// file: ASTRO_GPIO.h		//
//////////////////////////////////
//Based on T.Saito program: Example.C
int gpioexport(int gpioid);

int gpiosetdir(int gpioid,char *mode);

int gpiogetbits(int gpioid);

int gpio1(int gpioid);

int gpio0(int gpioid);
