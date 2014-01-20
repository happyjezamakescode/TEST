extern void exit_program();


int4 erlevl=EREROR, errglb=NOERR;
char *errstr[5]={NULL,"-I-","-W-","-E-","-F-"};
#define ERROR (errglb>=erlevl)

FILE *LOGF;

/*
 * This is the error report routine.
 * If the error is above the set error level, then print the diagnostic string
 * and set the global error to this value.
 */
void error_logger( char *from, int4 err, char *string)
{
  int4 locerr;
  struct tm *writetime;
  time_t timenow; 
  char *str=NULL;
  char *asctime=NULL;

  locerr = err;
  if(locerr>ERSEVR) locerr=ERSEVR;
  errglb=locerr;
  if(locerr>NOERR) {
    str=(char*)malloc(sizeof(char)*FBUFSIZ);
    time(&timenow);
    asctime=ctime(&timenow);
    *(asctime+strlen(asctime)-1)='\0';
    sprintf(str,"%s %s %s - %s", errstr[locerr], asctime, from, string);

    //if(STInfo.debug) puts(str);
    if(STInfo.uselog) {
      fprintf(LOGF,"%s\n", str);
      fflush(LOGF);
	}
    //free(str);
    if(locerr==ERSEVR) exit_program();
    if(locerr>=EREROR) {
      STInfo.istatus=ST_STATUS_ERROR;
      STInfo.status=ST_status[STInfo.istatus];
      flush_buffers();
    }
    if(locerr>=erlevl) 
      errglb=locerr;
    else
      errglb=NOERR;
  }
}
