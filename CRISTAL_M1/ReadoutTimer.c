//
// Readout Timer routines
//

#include <string.h>

int4 imon_sec=1;
static int timer_on=False;

static struct itimerval realt;
sigset_t block_mask;

#include "ReceiverReport.c"
#include "TriggerReport.c"
#include "PulsarReport.c"
#include "CalibReport.c"
#include "L3Report.c"

void sigalrm_handler()
{

//    printf("timer %d , ReceivInfo.istatus %d\n", timer_on, ReceivInfo.istatus );

    if(timer_on)
    {
//      printf("timer is on ...\n");
      
      timer_on=False;  

      ReceiverReport();

      TriggerReport();

      PulsarReport();
      
      CalibReport();

      L3Report();

      timer_on=True;  

    }
//    else
//      printf("timer is off ...\n");
}

void install_timer(int nsec)
{
    realt.it_interval.tv_sec = nsec;
    realt.it_interval.tv_usec = 0;
    realt.it_value.tv_sec = 1;
    realt.it_value.tv_usec = 0;
    if(setitimer(ITIMER_REAL, &realt, (struct itimerval *)0) == -1)
        perror("Real Timer Setting Error");
    signal(SIGALRM,(void *)sigalrm_handler);
    timer_on=False;
}
void start_timer(int nsec)
{
//      sigprocmask(SIG_UNBLOCK,&block_mask,NULL);
      pthread_sigmask(SIG_UNBLOCK,&block_mask,NULL);
      timer_on=True;
}

void stop_timer()
{
//      sigprocmask(SIG_BLOCK,&block_mask,NULL);
      pthread_sigmask(SIG_BLOCK,&block_mask,NULL);
      puts("stop_timer called");
      timer_on=False;
}
