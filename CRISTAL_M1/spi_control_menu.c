/*******************************************************************
 * SPI testing utility (using spidev driver)
 * date: october 2013
 * by: jrg
 * Based on spi_test.c
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 ********************************************************************/

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

void SPI_test_menu(void)
{
    static const char *device = "/dev/spidev1.1";
    static uint8_t mode;
    static uint8_t bits = 8;
    static uint32_t speed = 100000;
    static uint16_t delay;
	char *verb;
    int4 iverb;
	int fdtest, ret;

    printf("Remember that all the values that you set are set only until you restart CRISTAL\n");
    fdtest = open(device, O_RDWR);
	if (fd < 0)
		printf("can't open device\n");

	while(True)
	{
		verb = action_menu( "SPI_TEST_MENU", &iverb);

    /*
     * spi mode
     */
        if(strcmp(verb,"MODE_SET")==0)
		{
			get_short("Set SPI mode: ",(int2*) &mode,0,3);
            ret = ioctl(fdtest, SPI_IOC_WR_MODE, &mode);
            if (ret == -1)
	    	printf("can't set spi mode\n");

            ret = ioctl(fdtest, SPI_IOC_RD_MODE, &mode);
            if (ret == -1)
            printf("can't get spi mode\n");
            printf("spi mode: %d\n", mode);
        }

        if(strcmp(verb,"MODE_READ")==0)
		{
            ret = ioctl(fdtest, SPI_IOC_RD_MODE, &mode);
            if (ret == -1)
            printf("can't get spi mode\n");
            printf("spi mode: %d\n", mode);
        }

	/*
	 * bits per word
	 */
        if(strcmp(verb,"BIT_SET")==0)
		{
			get_short("Set bits per word: ",(int2*) &bits,1,8);
            ret = ioctl(fdtest, SPI_IOC_WR_BITS_PER_WORD, &bits);
            if (ret == -1)
            printf("can't set bits per word\n");

            ret = ioctl(fdtest, SPI_IOC_RD_BITS_PER_WORD, &bits);
            if (ret == -1)
            printf("can't get bits per word\n");
            printf("bits per word: %d\n", bits);
        }

        if(strcmp(verb,"BIT_READ")==0)
		{
            ret = ioctl(fdtest, SPI_IOC_RD_BITS_PER_WORD, &bits);
            if (ret == -1)
            printf("can't get bits per word\n");
            printf("bits per word: %d\n", bits);
        }

	/*
	 * max speed hz
	 */
        if(strcmp(verb,"SPEED_SET")==0)
		{
			printf("Warning!! recomended value 1 KHz \n");
			get_short("Set the SPI bus speed (KHz): ",(int2*) &speed,0,10);
            speed=speed*1000;
            ret = ioctl(fdtest, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
            if (ret == -1)
            printf("can't set max speed hz\n");

            ret = ioctl(fdtest, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
            if (ret == -1)
            printf("can't get max speed hz\n");
            printf("max speed: %d Hz (%d KHz)", speed, speed/1000);
        }

        if(strcmp(verb,"SPEED_SET")==0)
		{
            ret = ioctl(fdtest, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
            if (ret == -1)
            printf("can't get max speed hz\n");
            printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
        }

	close(fdtest);

		if(strcmp(verb,"RETURN")==0)
        {    
			return;
        }
	}
}
