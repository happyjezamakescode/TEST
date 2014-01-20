/******************************************************
    Cristalspi.c
	* Program to control Sum-trigger SPI bus 
	* date: Feb 2013, modified Oc 2013	 
	* by: jrg
******************************************************/

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "CristalSpi.h" 
//#include "CristalSpi_var.h" 
uint16_t delay;
const char *device = "/dev/spidev1.0";
int fd;


/*
 *   Dummy program 
 */
    void getdummy(int *var)
    {
        (*var)++;
    }


/*
 *   Transfer function  
 */
int transfer(uint8_t Function, uint8_t Board, uint8_t Channel, unsigned short data)
{
	
    int ret;
    int retset=0;
    int retread=0;
    int spi_error = 0;
    int spi_h_error = 0;
    uint8_t val1=0;
    uint8_t val2=0;

    uint8_t *ptr;
   
    uint8_t tx[] = {0, 0, 0, 0, 0};
    uint8_t rx[5] = {0,0,0,0,0};

    struct spi_ioc_transfer tr = {
            .tx_buf = (unsigned long)tx,
            .rx_buf = (unsigned long)rx,
            .len =5,
            .delay_usecs = delay,
    };


    fd = open(device, O_RDWR);

//    printf("fd %i\n",fd);
//    printf("valores funcion %i\n",Function);
//    printf("valores board %i\n",Board);
//    printf("valores channel %i\n",Channel);
//    printf("valor %i\n",data);
    

    tx[0]=Function;
    tx[1]=Board;
    tx[2]=Channel;
    
    ptr=(uint8_t *)&data;

    val1=ptr[0];
    val2=ptr[1];
    tx[3]=val2;
    tx[4]=val1;

    retset = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    tx[] = {0, 0, 0, 0, 0};
    retread = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);

//Error handling
//handshaking

    if (rx[0] == tx[0] && rx[1] == tx[1] && rx[2] == tx[2])                  
    {                                                                        
        printf("handshake ok\n");
        spi_h_error=0;
    }                                                                      
    else                                                                     
    {                                                                        
                    printf("handshake wrong\n");                                        
        spi_h_error=4;
    }         

    if (fd<1) spi_error = 1; //Device not found 
    else
    {
        if (retset < 0)  spi_error = 2; // Wirte error
        else 
        {
            if (retread < 0)  spi_error = 3; //read error
            else 
            { 
                if (spi_h_error != 0) 
                {
                    spi_error = spi_h_error; // Wirte error
                    printf("handshake wrong\n");                                        
                }  
                else spi_error=0;

            }
        }
    }

   for (ret = 0; ret < 5; ret++) {
       printf("%.2X ", tx[ret]);
     }
   printf("\n");
 
   for (ret = 0; ret < 5; ret++) {
	    printf("%.2X ", rx[ret]);
	}
    puts("");



    close(fd);
    return spi_error; 
}


/*
 *   Transfer function temp 
 */
int transfer_t(uint8_t Function, uint8_t Board, uint8_t Channel, short *data)
{
	
    int ret;
    int retset=0;
    int retread=0;
    int spi_error = 0;
    int spi_h_error = 0;
    uint8_t val1=0;
    uint8_t val2=0;
    int counts=0;
    int time=0;

    uint8_t *ptr;
   
    uint8_t tx[] = {0, 0, 0, 0, 0};
    uint8_t rx[5] = {0,0,0,0,0};

    struct spi_ioc_transfer tr = {
            .tx_buf = (unsigned long)tx,
            .rx_buf = (unsigned long)rx,
            .len =5,
            .delay_usecs = delay,
    };


    fd = open(device, O_RDWR);

//    printf("fd %i\n",fd);
//    printf("valores funcion %i\n",Function);
//    printf("valores board %i\n",Board);
//    printf("valores channel %i\n",Channel);
//    printf("valor %i\n",*data);
    

    tx[0]=Function;
    tx[1]=Board;
    tx[2]=Channel;
    
    ptr=(uint8_t *)&(*data);

    val1=ptr[0];
    val2=ptr[1];
    tx[3]=val2;
    tx[4]=val1;
/*
////////////////DBG    
    (*data)=100;
*/
    retset = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    tx[] = {0, 0, 0, 0, 0};
    retread = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);

//Error handling
//handshaking

    if (rx[0] == tx[0] && rx[1] == tx[1] && rx[2] == tx[2])                  
{                                                                        
    printf("handshake ok\n");
        spi_h_error=0;
        (*data)=rx[4];
}                                                                      
else                                                                     
{                                                                        
    spi_h_error=4;
        (*data)=0;
            printf("handshake wrong\n");                                        
    }         

    if (fd<1) spi_error = 1; //Device not found 
	//	printf("can't open device");
    else
    {
        if (retset < 0)  spi_error = 2; // Wirte error
        else 
        {
            if (retread < 0)  spi_error = 3; //read error
            else 
            { 
                if (spi_h_error != 0) 
                {
                    spi_error = spi_h_error; // Wirte error
                    printf("handshake wrong\n");                                        
                }  
                else spi_error=0;

            }
        }
    }

   for (ret = 0; ret < 5; ret++) {
       printf("%.2X ", tx[ret]);
     }
   printf("\n");
 
   for (ret = 0; ret < 5; ret++) {
	    printf("%.2X ", rx[ret]);
	}
    puts("");



    close(fd);
    return spi_error; 
}

/*
 *   Transfer function counters 
 */
int transfer_c(uint8_t Function, uint8_t *counts, uint8_t *time, unsigned short *macro_rates)
{
	
    int ret;
    int retset = 0;
    int retread = 0;
    int spi_error = 0;
    int spi_h_error = 0;
    uint8_t val1 = 0;
    uint8_t val2 = 0;
    int ct = 0;
    int ti = 0;
    float rate = 0;
    int macro = 0;
    int E_compare = 0;
    int R_compare = 0;
    uint8_t *ptr;
   
    uint8_t tx[] = {0, 0, 0, 0, 0};
    uint8_t rx[5] = {1,1,1,1,1};

    struct spi_ioc_transfer tr = 
        {
            .tx_buf = (unsigned long)tx,
            .rx_buf = (unsigned long)rx,
            .len =5,
            .delay_usecs = delay,
        };


    fd = open(device, O_RDWR);

    printf("fd %i\n",fd);
    printf("valores funcion %i\n",Function);
    printf("valor %i\n",*macro_rates);
    
    
    tx[0]=Function;
//    printf("DBG2\n");
    tx[1]=(uint8_t *)*counts;
    tx[2]=(uint8_t *)*time;
//    printf("DBG2\n");
    
    ptr=(uint8_t *)macro_rates;
    val1=ptr[0];
    val2=ptr[1];
    tx[3]=val2;
    tx[4]=val1;
//    printf("DBG3\n");

    retset = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    tx[] = {0, 0, 0, 0, 0};
    retread = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    ptr[0]=rx[0];
    ptr[1]=rx[1];
    ct=*macro_rates;
    

//printf ("DBG2");
    ptr[0]=rx[2];
    ptr[1]=rx[3];
    ti=*macro_rates;
    rate=1000.0*(float)ct/(float)ti;


//printf ("DBG3");

//Error handling
//handshaking
        E_compare=(rx[4] & 0x40)?1:0; 
        R_compare=(rx[4] & 0x20)?1:0;
        
        macro = rx[4] & 0x3F;
if(E_compare==1){        
    if (macro == tx[4] )                  
    {                                                                        
        printf("handshake ok\n");
        spi_h_error=0;
        (*counts)=ct;
        (*time)=ti;
        (*macro_rates)=rate; 
    }                                                                      
    else                                                                     
    {                                                                        
        printf("handshake wrong\n");                                        
        spi_h_error=4;
        (*counts)=ct;
        (*time)=ti;
        (*macro_rates)=0; 
    }         

    if (fd<1) spi_error = 1; //Device not found 
	//	printf("can't open device");
    else
    {
        if (retset < 0)  spi_error = 2; // Wirte error
        else 
        {
            if (retread < 0)  spi_error = 3; //read error
            else 
            { 
                if (spi_h_error != 0) 
                {
                    spi_error = spi_h_error; // Wirte error
                    printf("handshake wrong\n");                                        
                }  
                else spi_error=0;

            }
        }
    }

   for (ret = 0; ret < 5; ret++) {
       printf("%.2X ", tx[ret]);
     }
   printf("\n");
 
   for (ret = 0; ret < 5; ret++) {
	    printf("%.2X ", rx[ret]);
	}
    puts("");
    }
    else printf("still counting...");

    close(fd);
    return spi_error; 
}


/*
 *   Comunication with the power control hardware 
 */
int POWER1_SET(int fdPower1,uint8_t Function,uint8_t Board,uint8_t Channel,uint8_t Value1,uint8_t Value2)
{
	int spi_error=0;

return spi_error;
}  
int POWER2_SET(uint8_t Function,uint8_t Board,uint8_t Channel,uint8_t Value1,uint8_t Value2)
{
	int spi_error=0;

return spi_error;
}  


/*
 *  CPLD conect and disconect 
 */
int CPLD_conect()
{
int conect_error;
conect_error=transfer(0xDD,0,0,0);
return conect_error;
}

int CPLD_disconect()
{
int disconect_error;
disconect_error=transfer(0xDE,0,0,0);
return disconect_error;
}

//////////////////////////////////////////////////////////////////////////////////////////////
void pabort(const char *s)
{
    perror(s);
    abort();
} 
