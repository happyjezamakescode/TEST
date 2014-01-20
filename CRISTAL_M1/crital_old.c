#include <pthread.h>   /* needs to be before stdio.h */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>
#include <math.h>
#include <sys/time.h>
#include <errno.h> 
#include "intdefs.h"
int fd;
bool AllowInput;
#include "logicals.h"

#include "termcolor.h"

#define __LOCALINFO_INTERNAL__
#include "Status.h"
#include "nsockets2.c"
#include "BBmenu.h"
//has to be after BBmenu.h

void flush_buffers(void);
#include "STControl.h"//Tiene que estar antes de STLogger
#include "Boole.h"
#include "ReportThread.c"// move here for pulsar_select (contiene dummyreport)
#include "STLogger.c"
#include "STCCconections.c"
//////////////////incluir_menus////////////////////////////
//the menus should be here becouse they need to be after 
//ReporThread.c 
#include "astro_menu.c" //menu to astoboard
#include "clip_menu.c"
#include "sum_menu.c"
#include "power_menu.c" 
#include "dummy_menu.c"
#include "load_mapping.c"
#include "spi_control_menu.c"
#include "tools_menu.c"
#include "calib_menu.c"

#include "STMonitor.c"
unsigned short *recboardar;

unsigned short *getrecboardar()
{
  return recboardar;
}


struct run_status_struct {                                                       
  unsigned opened:        1;                                                     
  unsigned output:        1;                                                     
  unsigned book:          1;                                                     
  unsigned hist:          1;                                                     
  unsigned debug:         1;                                                     
  unsigned message:       1;                                                     
  unsigned dummy:    10;                                                         
  char name[40];                                                                 
  char type[4];                                                                  
  char path[40];                                                                 
} rs;                                                                            
unsigned int2 nrun;                                                              
unsigned int4 usecs=100;                                                         
                        
static pthread_mutex_t Mutex;

struct timespec interval;
struct timespec mremainder; 

void mysleep(int );  //using dummy loop
void mymusleep(int ); //using nanosleep


pthread_mutex_t *getmutex();

pthread_mutex_t *getmutex()
{
  return &Mutex;
}

void initmutex()
{
  pthread_mutex_init (&Mutex,  NULL); 
}

void destroymutex()
{
  pthread_mutex_destroy (&Mutex);  
}

void log_error_clipboard (short iboard, unsigned int4 addr)
{
    char str[256];
    sprintf((char *)str,"Comunication error, board %d, Function %lu",iboard, addr);
    error_logger(__FILE__,ERWARN,(char *)str);
    TERM_RED_COLOR;
    printf("\n **********  Error  *********** \n"); 
    printf("%s\n",str);
    TERM_RESET_COLOR;
    fflush (stdout);
}

void log_error_astroboard(short Function)
{
    char str[256];
    sprintf((char *)str,"Communication error inside AstroBoard, function %d \n",Function);
    error_logger(__FILE__,ERWARN,(char *)str);
    TERM_RED_COLOR;
    printf("\n **********  Error  *********** \n"); 
    printf("%s\n",str);
    TERM_RESET_COLOR;
    fflush (stdout);
}

void log_error_sumboards(short Function)
{
    char str[256];
    sprintf((char *)str,"Communication error inside AstroBoard, crate %d \n",Function);
    error_logger(__FILE__,ERWARN,(char *)str);
    TERM_RED_COLOR;
    printf("\n **********  Error  *********** \n"); 
    printf("%s\n",str);
    TERM_RESET_COLOR;
    fflush (stdout);
}

int4 interrupted=0; 
void myinterrupt(int dd)
{
  interrupted = True;
  flush_buffers();
  set_async(False);
  STInfo.lock_mode=False;
}

void brokenpipe(int dd)
{
    STInfo.client_conn[0]=False;
}

void mybuserror(int dd)
{
  puts("\n***\t BUS ERROR detected\t***\n");
}


void myabort(int dd)
{
  error_logger(__FILE__,ERSEVR,"Aborting program");
}

void InterpretCC(char *ccreport)
{
char temp[100];
float PI=3.14159265;
float Az, Zd, dis;
int AltAzPos[2];  //Posicion de las variables que quiero compara
  AltAzPos[0]=30; //41;
  AltAzPos[1]=31; //42;
                      
//  printf(" we print :%s \n",ccreport);                                         
  strcpy(temp,"");
                 
  int i, k, n;
  n=0; 
  for (i=0;i<1000;i++)
  {                                                                              
    k=0;                                                                         
    while (strncmp(ccreport+i," ",1)!=0 && strncmp(temp,"OVER",5)!=0)            
    {                
      strncpy(temp+k,ccreport+i,1);                                              
//     printf("tempor: %s \n", temp );                                          
      k++;          
      i++;                                                                      
    }                                                                            
    n++;                                                                         
    strcpy(temp+k, "\0");                                                        
                                                                                 
    if (n==AltAzPos[0]) 
    {                                                         
    	printf("Alt: %s \n", temp ); 
     	STInfo.SumL3_Zd=atof(temp);
     }	
     
     else if (n==AltAzPos[1])                                                     
     {
     	printf("Az : %s \n", temp );                                          
     	STInfo.SumL3_Az=atof(temp);
     }
if (strncmp(temp,"OVER",5)==0)                                               
      break;                                                                     
  }                                                                              
    Zd=STInfo.SumL3_Zd*PI/180;
    Az=STInfo.SumL3_Az*PI/180;
    dis=sin(Zd)*(52.33*sin(Az)+66.89*cos(Az));//meters
    STInfo.SumL3_delay= dis/0.299;//ns
    printf("Delay that should be set:%f \n",STInfo.SumL3_delay);	
}

main()
{
   const char *device = "/dev/spidev1.0"; 
    fd = open(device, O_RDWR);
    int4 i;
  int4 status;
  float f;
  char *verb, *qual;
  int4 iverb;
  char host_name[80];
  int4 port;
  char table_name[80];
  char *theString;

      printf  (" \n");
      TERM_BLUE_COLOR;
      printf  (" ***************************************************** \n");
      printf  (" ***************************************************** \n");
      printf  (" **                                                 ** \n");
      printf  (" **            Welcome to CRISTAL:                  ** \n");
      printf  (" **       ContRol In Sum-Trigger ALgorithm          ** \n");
      printf  (" **                                                 ** \n");
      printf  (" **          by: Jezabel R. Garcia                  ** \n");
      printf  (" **                                                 ** \n");
      printf  (" ***************************************************** \n");
      printf  (" ***************************************************** \n");
      TERM_RESET_COLOR;
      printf  (" \n");

  //  signal(SIGINT, myinterrupt);
  signal(SIGPIPE,brokenpipe);
  signal(SIGTERM,myabort);
  signal(SIGBUS,mybuserror);

  theString=(char *)malloc(sizeof(char)*256);
  STInfo.hostname[0]=(char*)malloc(sizeof(char)*80);
  STInfo.hostname[1]=(char*)malloc(sizeof(char)*80);
  
  /*
   * Set the initial values for status/times
   */
  time(&STInfo.startime);// mirar los usos del tiempo
  time(&STInfo.writetime);//
  STInfo.istatus=ST_STATUS_BUSY; 
  STInfo.threshold_control_flag=0;  //by default off
  STInfo.Global_Target_rate = 500; //Hz
  STInfo.telescope=1;
  STInfo.temp_control=0;
  STInfo.total_time=3600;
  STInfo.time_interval=900;   
  
  //initializes the  BBmenu package
  menu_init();
  
  //allow simulate_input()
  AllowInput=1;
  
  define_file("cristal_define.uic");
  
  simulate_input("@cristal_init");
  
  AllowInput=0;
  /*
   * Launch threads for service programs
   */
 // usleep(100);//DBG
  STInfo.exit = False;
  printf("init threads \n");
  STInfo.monexit=true; //monitoring thread not running

  //SERVER THREAD (para oir a cc)
    pthread_attr_init(&serv_attr);
    pthread_create(&serv_tid, &serv_attr, (void * (*)(void *)) server_thread, NULL);
  printf("server_thread initiated \n");
  
  //CLIENT THREAD ( para hablar a cc)
    pthread_attr_init(&client_attr);
    pthread_create(&client_tid, &client_attr,  (void * (*)(void *)) client_thread, NULL);
  printf("client_thread initiated \n");
 //Add a Thread for L3 
  
  
  //Report thread 
  pthread_attr_init(&Report_attr);
  pthread_create(&Report_tid, &Report_attr, (void * (*)(void *)) Report_thread, NULL);
  printf("Report_thread initiated \n");
  


  while(True)
    {
      
      verb=action_menu( "ST_Main_Menu", &iverb);
      if(interrupted==True)
	{
	  interrupted=False;
	  flush_buffers();
	}
      
      //////////////////////
      if(strcmp(verb,"STOP!")==0) 
	{             
	  transfer(0xA4, 0, 0, 0);
	  printf("Stopping gloabl triggers.");
	}
      
      //////////////////////
      if(strcmp(verb,"START")==0)
	{                        
	  transfer(0xA3, 0, 0, 0);
          printf("Starting gloabl triggers.");
	}

      //////////////////////                                                                  
      if(strcmp(verb,"SETTARGETRATE")==0)
        {
	  get_integer("Enter Global Target Trigger Rate", 
		      &STInfo.Global_Target_rate, 0, 0);
        }



      //////////////////////                                                                          
      if(strcmp(verb,"MONSTOP!")==0)  //monitoring stop
        {
	  STInfo.monexit=true; //stop monitpring thread
        }

      //////////////////////                                                                          
      if(strcmp(verb,"MONSTART")==0)  //monitoring start
        {
	  pthread_attr_init(&STmon_attr);
	  pthread_create(&STmon_tid, &STmon_attr, (void * (*)(void *)) STmon_thread, NULL);
	  //STInfo.monexit=0; // keep monitoring threat running
        }

      
      //////////////////////
      if(strcmp(verb,"REPORT")==0) 
	{                                                
	  error_logger(__FILE__,ERINFO,"Reports ON!");                               
	  if(!timer_on)start_timer(imon_sec);                                        
	}
      
      //////////////////////
      if(strcmp(verb,"REP!")==0) 
	{                                                
	  stop_timer();                                                              
	  error_logger(__FILE__,ERINFO,"Reports OFF");                               
	}                           
      
      //////////////////////
      if(strcmp(verb,"LOCK!")==0) 
	{
	//switch to remote control
	  error_logger(__FILE__,ERINFO,"Readout ready to be engaged!");
	  set_async(True);//aqui es cuando cierra la conexion, usando getsring
	  STInfo.lock_mode=True;
	}
    
      //////////////////////
      if(strcmp(verb,"ULOCK")==0) 
	{
	  //	if(client_conn) {
	  //	    client_close(); client_conn=False; }
	  error_logger(__FILE__,ERINFO,"Readout disengaged from CC");
	  set_async(False);
	  STInfo.lock_mode=False;
	}
      
      //////////////////////
      if(strcmp(verb,"HOSTNAME")==0) 
	{
	  int ihost;
	  for(ihost=0;ihost<2;ihost++)
	    {
	      get_text("Enter Host Name",(char *)STInfo.hostname[ihost]);
	     printf(" DBG HOSTNAME, %s", STInfo.hostname[ihost]);
	      //if(STInfo.debug==True)
	      // printf("Host Name defined: %s\n", STInfo.hostname[ihost]);
	    }
	}
      
      //////////////////////
      if(strcmp(verb,"HOSTNO")==0) 
	{
	  int ihost;
	  for(ihost=0;ihost<2;ihost++)
	    {
	      get_integer("Enter Host Port Number",&STInfo.hostno[ihost],0,0);
	      //  if(STInfo.debug==True)
	          printf("Host Port Number defined: %ld\n", STInfo.hostno[ihost]);
	    }
	}
      
      //////////////////////
      	if(strcmp(verb,"CRISTAL")==0) 
	{
	  printf("\n");
	  TERM_PURPLE_COLOR;
	  printf("CIRSTAL:Control in Sum-Trigger Algorithm\n");
	  printf("Author: Jezabel R. Garcia\n");
    	  TERM_RESET_COLOR;
	  printf("This progam  was developed following the concept of MIR .\n");
	  printf("(by Riccardo Paoeletti and Daniel Mazin)\n");
	  printf("The Mir space station was built by connecting\n");
	  printf("several modules, each launched into orbit separately\n");
	  printf("One of the main module of the Mir was Cristal\n");
	  printf("that performed the astronomical observations and \n");
	  printf("extended the scientific possibilities of Mir.\n");
	  printf  (" \n");
	}
      
      //////////////////////
      	if(strcmp(verb,"TELESCOPE")==0) 
	{
	  get_integer("Who I am?",&STInfo.telescope,1,2);
	  printf("This cristal version is running on M%i\n",
	  STInfo.telescope);
	}
      
      //////////////////////
      if(strcmp(verb,"PORTNO")==0) 
	{
	  get_integer("Enter Local Port Number",&STInfo.portno,0,0);
	  // if(STInfo.debug==True)
	     printf("Local Port Number defined: %ld\n", STInfo.portno);
	}
      
      //////////////////////
      if(strcmp(verb,"TOOLS")==0) 	tools_menu();
      
      //////////////////////
      if(strcmp(verb,"DUMMY")==0) 	dummy_menu();
      
      //////////////////////
      if(strcmp(verb,"ASTRO")==0) 	astro_menu();
      
      //////////////////////
      if(strcmp(verb,"CLIP")==0) 	clip_menu();
      
      //////////////////////
      if(strcmp(verb,"SUM")==0) 	sum_menu();
      
      //////////////////////
      if(strcmp(verb,"POWER")==0) power_menu();
      
      //////////////////////
      if(strcmp(verb,"MESSAGE")==0) {
	get_text("Enter command", theString);
	puts(theString);
	error_logger(__FILE__,ERINFO, theString);
      }
      
      //////////////////////
      if(strcmp(verb,"TIME_READ")==0)
	{
	  int data4=0;
	  get_hexadecimal("time? ",(long unsigned int*) &data4,0,0);
	  
	  int secs=15-data4&0xF;
	  int sect=15-(data4>>4)&0x7;
	  int mins=15-(data4>>7)&0xF;
	  int mint=15-(data4>>11)&0x7;
	  int hout=15-(data4>>14)&0x2;
	  
	  int hour=15-(data4>>16)&0xF;
        int tickWrong=(data4>>24)&0xFF;
        int tickRight = 0;
        int tickc = 0;
        for (tickc=0;tickc<8;tickc++)
        {
            tickRight += ((tickWrong>>(7-tickc))&0x1)*pow(2,tickc);
            //				  printf(" right %x wrong  %x \n ",tickRight, (tickWrong>>(7-tickc))&0x1);
        }
        int tick = tickRight;

        //super second:
        //				printf("R %X ",data4&0x0000FFFF);
        //super sub second:
        printf(" %X ",(data4&0xFFFF0000)>>16);

        //subsecond:
        get_hexadecimal("time? ",(long unsigned int*)&data4,0,0);

        printf("time: %d.%d h %d%d m %d%d %lf %d\n ",hout,hour,mint,mins,sect,secs,1.677721600-(data4&0x00007FFF)*51200e-9, tickRight);

    }
    
    //////////////////////
    if(strcmp(verb,"SYSTEM")==0)
    {
      get_text("Enter command", theString);
      system(theString);
    }
    
    /////////////////////:
    if(strcmp(verb,"READY")==0)
    {
	  STInfo.istatus=ST_STATUS_BUSY; 
      TERM_BLUE_COLOR;
      printf  (" ***************************************************** \n");
      printf  (" ***************************************************** \n");
      printf  (" **                                                 ** \n");
      printf  (" **         CRISTAL IS NOW READY TO OPERATE         ** \n");
      printf  (" **                                                 ** \n");
      printf  (" **                 ENJOY IT!                       ** \n");
      printf  (" **                                                 ** \n");
      printf  (" ***************************************************** \n");
      printf  (" ***************************************************** \n");
      TERM_RESET_COLOR;
      printf  (" \n");
      printf  ("Press ? for more info. \n");
    }
    
    if(strcmp(verb,"EXIT")==0)
    {
        stop_timer();
      break;
    }
    usleep(250);
  }

  
    if(STInfo.client_conn[0]) { 
      client_select(0); 
      client_close(); 
      error_logger(__FILE__,ERINFO,"Closing client connection");
    }
  if(STInfo.server_conn) {
    error_logger(__FILE__,ERINFO,"Closing server connection");
    server_close();
  }

  destroymutex ();


  exit_program();  
}

void exit_program()
{
    printf  (" %%%% Cristal is shutting down ......  %%%% \n");
  error_logger(__FILE__,ERINFO,"SHUTTING DOWN MIR ... ");
  STInfo.exit = True;
	if(STInfo.uselog) fclose(LOGF);
  if(pthread_join(Report_tid, NULL))
    printf  (" %%%% Error while joining of REPORT THREAD %%%% \n");
  else
    printf  (" %%%% joined of REPORT THREAD successful %%%% \n");
  if(pthread_join(client_tid, NULL))
    printf  (" %%%% Error while joining of CLIENT THREAD %%%% \n");
  else
    printf  (" %%%% joined of CLIENT THREAD successful %%%% \n");
  if(pthread_kill(serv_tid, NULL))
    printf  (" %%%% Error while joining of SERVER THREAD %%%% \n");
  else
    printf  (" %%%% joined of SERVER THREAD successful %%%% \n");
 
  if(STInfo.monexit!=true)
 { 
 if(pthread_join(STmon_tid, NULL))                                          
    printf  (" %%%% Error while joining of STMonitor THREAD %%%% \n");              
   else                                                                           
    printf  (" %%%% joined of STMonitor THREAD successful %%%% \n");  
}
  printf  (" %%%% joining of threads was successful ... detaching  %%%% \n");
  exit(0);
}
