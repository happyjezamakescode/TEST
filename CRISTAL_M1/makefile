CFLAGS := -g -c
LFLAGS := -lm -lpthread -W -Wall #to show more warnings 

SRC=socklib.c cristal.c CristalSpi.c BBmenu.c 

cristal: cristal.o BBmenu.o socklib.o CristalSpi.o
	gcc -o cristal socklib.o cristal.o BBmenu.o CristalSpi.o $(LFLAGS)


socklib.o: socklib.c
	gcc $(CFLAGS) socklib.c

cristal.o: cristal.c  CristalSpi.o
	gcc $(CFLAGS) cristal.c

BBmenu.o: BBmenu.c 
	gcc $(CFLAGS) BBmenu.c

CristalSpi.o: CristalSpi.c 
	gcc $(CFLAGS) CristalSpi.c

#
# clean label has no rules, only actions
#

clean:
	rm *.o

archive:
	tar cvfz cristal_archive.tar.gz *.c *.h \
	       
	tar cvfz macro_archive.tar vmetest_define.uic vmetest_init.uic makefile



cristal.o: cristal.c CristalSpi.c astro_menu.c STMonitor.c clip_menu.c sum_menu.c power_menu.c dummy_menu.c load_mapping.c spi_control_menu.c tools_menu.c calib_menu.c STControl.h
