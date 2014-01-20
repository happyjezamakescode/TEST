/***************************************************************************/
/*                                                                         */
/* THREADS                                                                 */
/*                                                                         */
/***************************************************************************/
#include "Boole.h"
#include "socklib.h"
#include <string.h>

char *serv_buffer=NULL;

pthread_t serv_tid, client_tid, mon_tid;             /* thread ID structure */
pthread_attr_t serv_attr, client_attr, mon_attr;     /* thread attributes */


//declare a function which is described in cristal.c
void InterpretCC(char *);

void* server_thread(pthread_mutex_t *vme_mutex)
{

  printf ("SERVER TREAD \n");


  SOCKET *sp; 
  int sd;      /* socket descriptor */

  int status, n;
  char str[80];
  time_t timenow; 

  printf ("SERVER server port %d\n",STInfo.portno);

  sleep(1);
  error_logger(__FILE__,ERINFO,"TCP Server thread launched");
  if(serv_buffer==NULL) serv_buffer=(char*)malloc(sizeof(char)*FBUFSIZ);
{
  error_logger(__FILE__,ERINFO,"Server thread: waiting for Central Control to connect");
       printf("waiting for connection\n");

}
  while((sp = sopen()) == NULL) {
    sprintf((char *)str,"Error connecting port number %ld. Retrying in 10 secs...",STInfo.portno);
    printf("Error connecting port number %ld. Retrying in 10 secs...",STInfo.portno);
    error_logger(__FILE__,ERWARN,(char *)str);
    STInfo.icomstatus=ST_COMSTATUS_NOTREADY;
    sleep(10);
  }

  while(!STInfo.exit) 
  {
    STInfo.server_conn=False;
    sprintf((char *)str,"Waiting for connection on port %ld...",STInfo.portno);
    error_logger(__FILE__,ERINFO,(char *)str);
    STInfo.icomstatus=ST_COMSTATUS_READY;
    if ((sd = sserver(sp, STInfo.portno, S_DELAY)) < 0) {
      error_logger(__FILE__,ERINFO,"server connection failed ...");
       printf("server connection failed\n");
      sleep(100);
    }
    else
    {
      error_logger(__FILE__,ERINFO,"Server connection established");
       printf("server connection established\n");
      STInfo.server_conn=True;
      /*
       * Command received: store time and check if there are errors
       * If so, discard the command but flag it as ignored.
       */
      n=0;
      *serv_buffer=0;
      while((n=sreadline(sd, serv_buffer, FBUFSIZ) ) > 0 && !STInfo.exit)
      {


        time(&STInfo.lastcmdtime);
    //   printf("DBG1 server thread on\n");
    //   printf("DBG1 server thread on %i \n",STInfo.istatus);
       printf("ST_STATUS %i \n",ST_STATUS_ERROR);
       if(STInfo.istatus!=ST_STATUS_ERROR){
     //  printf("DBG2 no status error\n");
          if(STInfo.lock_mode==ST_LOCK)
          {
            AllowInput=0;
            if(strncmp(serv_buffer,"CC-REPORT",9)==0)
            {
	    //  printf("DBG3 reading report\n");
  		TERM_BLUE_COLOR;printf("%s",serv_buffer);TERM_RESET_COLOR;
             
              remove_new_line( serv_buffer);
              InterpretCC(serv_buffer);
              error_logger(__FILE__,ERWARN,serv_buffer);
            }
            else
            {
              printf("%s",serv_buffer);
              remove_new_line( serv_buffer);
              if(STInfo.cmdbusy==0)
                simulate_input(serv_buffer);
              error_logger(__FILE__,ERINFO,serv_buffer);
            }
            AllowInput=1;
          }
          STInfo.icomstatus=ST_COMSTATUS_RECVD;
        }
        else {
	   //   printf("DBG4 ignoring report\n");
          error_logger(__FILE__,ERWARN,"Command Ignored:");
          error_logger(__FILE__,ERWARN,serv_buffer);
          STInfo.icomstatus=ST_COMSTATUS_IGNORED;
        }
      }
    }
    error_logger(__FILE__,ERINFO,"Server connection lost");
  }

  pthread_exit(0);
}

void* client_thread()
{
  int status, n;
  int iclient=0;
  Boolean timerstarted=FALSE;

  sleep(1);
  error_logger(__FILE__,ERINFO,"TCP Client thread launched");
  while(!STInfo.exit) 
  {
     //  printf("DBG con");   
      if(STInfo.client_conn[0]==False && STInfo.hostno[0]>0)
      {
      // printf("DBG connect");   
        client_select(0);
        if(client_connect(STInfo.hostname[0], STInfo.hostno[0]) < 0)
        {
          STInfo.client_conn[0]=False;
        }
        else
        {
          STInfo.client_conn[0]=True;
          char line[100];
          sprintf((char*)line,"Client connection %d established",0); 
          error_logger(__FILE__,ERINFO,line);
          if(timer_on==False && !timerstarted)
          {
            start_timer(imon_sec);
            timerstarted=TRUE;
          }
        }
      }
    sleep(10);
  }

  pthread_exit(0);
}


