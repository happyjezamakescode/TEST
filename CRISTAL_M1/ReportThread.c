/***************************************************************************/
/*                                                                         */
/*                  Report thread                                          */
/*                                                                         */
/***************************************************************************/
#include <string.h>
#include <pthread.h>

int4 imon_sec=1;
static int timer_on=False;

pthread_t Report_tid;             /* thread ID structure */
pthread_attr_t Report_attr;     /* thread attributes */


//static struct itimerval realt;
//sigset_t block_mask;

#include "STReport.c"

void* Report_thread(pthread_mutex_t *vme_mutex)
{

  while(!STInfo.exit) 
  {

    sleep(imon_sec);
    error_logger(__FILE__,ERINFO," reports are starting ");

    if (timer_on) STReport();

  }
  pthread_exit(0);
}


void start_timer(int nsec)
{
      error_logger(__FILE__,ERINFO,"start_timer (i.e. start reports) called ...");
      imon_sec = nsec;
      timer_on=True;
}

void stop_timer()
{
      timer_on=False;
      error_logger(__FILE__,ERINFO,"stop_timer (i.e. stop reports) called ...");
}
