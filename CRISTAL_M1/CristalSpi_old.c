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
#include "intdefs.h"
#include "CristalSpi.h" 
uint16_t delay;
const char *device = "/dev/spidev1.0";
int fd;
static pthread_mutex_t SPIMutex;
int DBG=0;
uint8_t DEL = 0x01;
uint8_t CLP = 0x02;
uint8_t ATT = 0x03;
uint8_t LED = 0x04;
uint8_t SWC = 0x05;//fixdelay
uint8_t DIS = 0x06;
uint8_t TEM = 0x10;
uint8_t SN = 0x08;
uint8_t RTE = 0x0E;
uint8_t TEMP = 0x0F;
uint8_t VER = 0x06;


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
    pthread_mutex_lock(&SPIMutex);	
    int flag=0;
    int ret;
    int retset=0;
    int retread=0;
    int spi_error = 0;
    int spi_h_error = 0;
    uint8_t val1=0;
    uint8_t val2=0;

    uint8_t *ptr;
   
    uint8_t tx[] = {0,0,0,0,0};
    uint8_t con[] = {0,0,0,0,0};
    uint8_t dis[] = {0,0,0,0,0};
    uint8_t tx2[] = {0,0,0,0,0};
    uint8_t rx[5] = {0,0,0,0,0};
    uint8_t rx2[5] = {0,0,0,0,0};
    dis[0]=222;
    con[0]=221;
    
    struct spi_ioc_transfer tr = 
    {
            .tx_buf = (unsigned long)tx,
            .rx_buf = (unsigned long)rx2,
            .len =5,
            .delay_usecs = delay,
    };

    struct spi_ioc_transfer tr2 = 
    {
            .tx_buf = (unsigned long)tx2,
            .rx_buf = (unsigned long)rx,
            .len =5,
            .delay_usecs = delay,
    };

     struct spi_ioc_transfer trcon = 
    {
            .tx_buf = (unsigned long)con,
            .rx_buf = (unsigned long)rx2,
            .len =5,
            .delay_usecs = delay,
    };
    
    struct spi_ioc_transfer trdis = 
    {
            .tx_buf = (unsigned long)dis,
            .rx_buf = (unsigned long)rx2,
            .len =5,
            .delay_usecs = delay,
    };

    if (DBG==1)
    {
        printf("fd %i\n",fd);
        printf("function %i\n",Function);
        printf("board %i\n",Board);
        printf("channel %i\n",Channel);
        printf("value %i\n",data);
    }                                                                      
    

    tx[0]=Function;
    tx[1]=Board;
    tx[2]=Channel;
    
    ptr=(uint8_t *)&data;

    val1=ptr[0];
    val2=ptr[1];
    tx[3]=val2;
    tx[4]=val1;

    fd = open(device, O_RDWR);

//Connect if necesary
if(tx[0]==DEL|| tx[0]==CLP || tx[0]==ATT || tx[0]==LED || tx[0]==SWC || tx[0]==TEMP || tx[0]==SN|| tx[0]==RTE|| tx[0]==TEM|| tx[0]== VER)
{
        
	flag=1;
   ret= ioctl(fd, SPI_IOC_MESSAGE(1), &trcon);
}

    retset = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	usleep(20);  
    retread = ioctl(fd, SPI_IOC_MESSAGE(1), &tr2);

if (flag==1)
{
    ioctl(fd, SPI_IOC_MESSAGE(1), &trdis);
}

//Error handling
//handshaking

    if (rx[0] == tx[0] && rx[1] == tx[1] && rx[2] == tx[2])                  
    {                                                                        
        if (DBG==1)
        {
            printf("handshake ok\n");
        }                                                                      
        spi_h_error=0;
    }                                                                      
    else                                                                     
    {   
        if(tx[1]!=255 && tx[0]!=221)
        {
            printf("handshake wrong\n");                                        
            printf(" SENDING:\n");
            for (ret = 0; ret < 5; ret++) 
            {
                printf("%.2X ", tx[ret]);
            }
            printf("\n");
            printf(" READING:\n");
            for (ret = 0; ret < 5; ret++)
            {   
                printf("%.2X ", rx[ret]);
            }
            spi_h_error=4;
        }         
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

    if (DBG==1)
    {
        printf("*****DBG ON*****\n");
        printf(" SENDING:\n");
        for (ret = 0; ret < 5; ret++) 
        {
            printf("%.2X ", tx[ret]);
        }
        printf("\n");
        printf(" READING:\n");
        for (ret = 0; ret < 5; ret++)
        {
            printf("%.2X ", rx[ret]);
        }
	puts("");
    }

    close(fd);
pthread_mutex_unlock(&SPIMutex);
    return spi_error; 

}


/*
 *   Transfer function temp 
 */
int transfer_t(uint8_t Function, uint8_t Board, uint8_t Channel, short *data)
{
pthread_mutex_lock(&SPIMutex);	
	
    int flag=0;
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
   
   
    uint8_t tx[] = {0,0,0,0,0};
    uint8_t con[] = {0,0,0,0,0};
    uint8_t dis[] = {0,0,0,0,0};
    uint8_t tx2[] = {0,0,0,0,0};
    uint8_t rx[5] = {0,0,0,0,0};
    uint8_t rx2[5] = {0,0,0,0,0};
    dis[0]=222;
    con[0]=221;

    struct spi_ioc_transfer tr = {
            .tx_buf = (unsigned long)tx,
            .rx_buf = (unsigned long)rx,
            .len =5,
            .delay_usecs = delay,
    };


    struct spi_ioc_transfer tr2 = 
    {
            .tx_buf = (unsigned long)tx2,
            .rx_buf = (unsigned long)rx,
            .len =5,
            .delay_usecs = delay,
    };
     struct spi_ioc_transfer trcon = 
    {
            .tx_buf = (unsigned long)con,
            .rx_buf = (unsigned long)rx2,
            .len =5,
            .delay_usecs = delay,
    };
    
    struct spi_ioc_transfer trdis = 
    {
            .tx_buf = (unsigned long)dis,
            .rx_buf = (unsigned long)rx2,
            .len =5,
            .delay_usecs = delay,
    };


    fd = open(device, O_RDWR);

    if (DBG==1)
    {
        printf("*****DBG ON*****\n");
        printf("fd %i\n",fd);
        printf("function %i\n",Function);
        printf("board %i\n",Board);
        printf("channel %i\n",Channel);
        printf("value %i\n",data);
    }                                                                      
    

    tx[0]=Function;
    tx[1]=Board;
    tx[2]=Channel;
    
    ptr=(uint8_t *)&(*data);

    val1=ptr[0];
    val2=ptr[1];
    tx[3]=val2;
    tx[4]=val1;
if(tx[0]==DEL|| tx[0]==CLP || tx[0]==ATT || tx[0]==LED || tx[0]==SWC || tx[0]==TEMP || tx[0]==SN|| tx[0]==RTE|| tx[0]==TEM|| tx[0]== VER)
{
        
	flag=1;
   ret= ioctl(fd, SPI_IOC_MESSAGE(1), &trcon);
}
    
    retset = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	usleep(20);  
    retread = ioctl(fd, SPI_IOC_MESSAGE(1), &tr2);

if (flag==1)
{
    ioctl(fd, SPI_IOC_MESSAGE(1), &trdis);
}

//Error handling
//handshaking

    if (rx[0] == tx[0] && rx[1] == tx[1] && rx[2] == tx[2])                  
    {                                                                        
        if (DBG==1)
        {
            printf("*****DBG ON*****\n");
            printf("handshake ok\n");
        }                                                                      
        spi_h_error=0;
        (*data)=rx[4];
    }                                                                      
    else                                                                     
    {                                                                        
        spi_h_error=4;
        (*data)=0;
        
        printf("handshake wrong\n");                                        
        printf(" SENDING:\n");
        for (ret = 0; ret < 5; ret++) 
        {
            printf("%.2X ", tx[ret]);
        }
        printf("\n");
        printf(" READING:\n");
        for (ret = 0; ret < 5; ret++)
        {
            printf("%.2X ", rx[ret]);
        }
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

    if (DBG==1)
    {
        printf("*****DBG ON*****\n");
        printf(" SENDING:\n");
        for (ret = 0; ret < 5; ret++) 
        {
            printf("%.2X ", tx[ret]);
        }
        printf("\n");
        printf(" READING:\n");
        for (ret = 0; ret < 5; ret++)
        {
            printf("%.2X ", rx[ret]);
        }
	    puts("");
	}


    close(fd);
pthread_mutex_unlock(&SPIMutex);
    return spi_error; 
}

/*
 *   Transfer function counters 
 */
int transfer_c(uint8_t Function, int *counts, int *time, int *macro_rates)
{
pthread_mutex_lock(&SPIMutex);
	
    int ret;
    int retset = 0;
    int retread = 0;
    int spi_error = 0;
    int spi_h_error = 0;
    uint8_t val1 = 0;
    uint8_t val2 = 0;
    int ct = 0;
    int ti = 0;
    int rate = 0;
    int macro = 0;
    int E_compare = 0;
    int R_compare = 0;
    uint8_t *ptr;
   
    uint8_t tx2[] = {0, 0, 0, 0, 0};
    uint8_t tx[] = {0, 0, 0, 0, 0};
    uint8_t rx[5] = {1,1,1,1,1};
    uint8_t rx2[5] = {1,1,1,1,1};

    struct spi_ioc_transfer tr = 
        {
            .tx_buf = (unsigned long)tx,
            .rx_buf = (unsigned long)rx,
            .len =5,
            .delay_usecs = delay,
        };


    struct spi_ioc_transfer tr2 = 
    {
            .tx_buf = (unsigned long)tx2,
            .rx_buf = (unsigned long)rx2,
            .len =5,
            .delay_usecs = delay,
    };

    fd = open(device, O_RDWR);

    if (DBG==1)
    {
        printf("fd %i\n",fd);
        printf("function %i\n",Function);
        printf("Macrocell %i\n",macro_rates);
    }                                                                      
    
    
    tx[0]=Function;
    tx[1]=(uint8_t )*counts;
    tx[2]=(uint8_t )*time;
    ptr=(uint8_t *)macro_rates;
    val1=ptr[0];
    val2=ptr[1];
    tx[3]=val2;
    tx[4]=val1;

    retset = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	usleep(20);  
    retread = ioctl(fd, SPI_IOC_MESSAGE(1), &tr2);

    val1=rx2[0];
    val2=rx2[1];
    ct=256*val1+val2;
    val1=rx2[2];
    val2=rx2[3];
    ti=256*val1+val2;
    rate=(int)(1000.0*ct/ti);


//Error handling
//handshaking
        E_compare=(rx2[4] & 0x40)?1:0; 
        R_compare=(rx2[4] & 0x20)?1:0;
        
        macro = rx2[4] & 0x3F;
//if(E_compare==1){        
 //   if (macro == tx[4] )                  
 //   {                                                                        
    //    printf("handshake ok\n");
      //  spi_h_error=0;
        (*counts)=ct;
        (*time)=ti;
        (*macro_rates)=rate; 
  //  }                                                                      
 /*   else                                                                     
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
	    printf("%.2X ", rx2[ret]);
	}
   printf("\n");
   // }
   // else printf("still counting...");
*/
    close(fd);
pthread_mutex_unlock(&SPIMutex);	
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
    int conect_error=0;
   // conect_error=transfer(0xDD,0,0,0);
    return conect_error;
}

int CPLD_disconect()
{
    int disconect_error=0;
  //  disconect_error=transfer(0xDE,0,0,0);
    return disconect_error;
}

//////////////////////////////////////////////////////////////////////////////////////////////
void pabort(const char *s)
{
    perror(s);
    abort();
} 
