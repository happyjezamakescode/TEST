/*******************************************************************/
/* Sum-Trigger Report to SA++                                      */
/* date: Septmeber 2013                                            */
/* by: jrg                                                         */
/********************************************************************/

#include <string.h>


void STReport()
{
  struct tm *thetimenow, *thetimeprev ; //estrucutra
//  char *str;
  float L3Rate=0.0;
//  str=(char*)malloc(sizeof(char)*280000);
char str[10240];   
  printf("Starting Sum-Trigger-II Report() ...\n"); fflush(stdout);
  
  time(&STInfo.writetime);//salvo el tiempo en  writetime, la ultima vez q hable con SA
  thetimenow=gmtime(&STInfo.writetime);//lo paso a una forma mas leible y lo guardp en str
  

if (STInfo.telescope==1 )
{
      sprintf(str,"SUM-REPORT M1 %04d %02d %02d %02d %02d %02d %03d",
	  STInfo.istatus,
	  thetimenow->tm_year+1900, thetimenow->tm_mon+1, thetimenow->tm_mday,
	  thetimenow->tm_hour,thetimenow->tm_min,thetimenow->tm_sec,0);
    L3Rate=0.0;
}
  
if (STInfo.telescope==2 )
{
      sprintf(str,"SUM-REPORT M2 %04d %02d %02d %02d %02d %02d %03d",
	  STInfo.istatus,
	  thetimenow->tm_year+1900, thetimenow->tm_mon+1, thetimenow->tm_mday,
	  thetimenow->tm_hour,thetimenow->tm_min,thetimenow->tm_sec,0);
    L3Rate=ASData.L3Rate;
//     L3Rate=0.0;
}
  
  // thetimeprev=gmtime(&STInfo.writetime);
  thetimeprev=gmtime(&STInfo.lastcmdtime);
  if (STInfo.writetime - STInfo.lastcmdtime > 25)  //changed from 15 sec to 25seconds //ums pero donde se llena STInfo?
    STInfo.icomstatus = ST_COMSTATUS_NOTRECVD; 
  else
    STInfo.icomstatus = ST_COMSTATUS_READY;
  sprintf(str+strlen(str)," %02d ",STInfo.icomstatus);
 
  sprintf(str+strlen(str)," %02d %04d %02d %02d %02d %02d %02d %03d",
	  STInfo.icomstatus,
	  thetimeprev->tm_year+1900, thetimeprev->tm_mon+1, thetimeprev->tm_mday,
	  thetimeprev->tm_hour,thetimeprev->tm_min,thetimeprev->tm_sec,0);
  
  /* 
   *Info to send to SA++
   */   
  
  
  sprintf(str+strlen(str)," %s","attenuationtable.uic"); //thresholds cahnnge name for STInfo.atttable
  sprintf(str+strlen(str)," %s","cliptable.uic"); //thresholds
  sprintf(str+strlen(str)," %s","delaytable.uic"); //thresholds
  sprintf(str+strlen(str)," %s","LUTlaytable.uic"); //thresholds
  sprintf(str+strlen(str)," %s","initial_thr_table.uic"); //thresholds
  sprintf(str+strlen(str)," %5.2f %5.2f %5.2f", 0.0,0.0,0.0);
  int ii;

  sprintf(str+strlen(str)," %s", "RATES"); 

  for(ii=0;ii<55;ii++)
    {
      sprintf(str+strlen(str)," %06.1f",(float)ASData.rate[ii]);
    }                                  
      sprintf(str+strlen(str)," %06.1f",(float)ASData.globalrate);
      sprintf(str+strlen(str)," %06.1f",(float)L3Rate);
  
/* for(ii=0;ii<55;ii++)
     {
        sprintf(str+strlen(str)," %06.1f",(float)(25*ii));
      } 
*/
  sprintf(str+strlen(str)," %s", "THRE"); 
  for(ii=0;ii<55;ii++)
    {
      sprintf(str+strlen(str)," %06.1f",ASData.threshold[ii]*1000);
    }                                  
  
  sprintf(str+strlen(str)," %s", "TEMP"); 
  for(ii=0;ii<18;ii++)
    {
      sprintf(str+strlen(str)," %02d",ClipData[ii].TAMP);
    }                                  
  
  for(ii=0;ii<18;ii++)
    {
      sprintf(str+strlen(str)," %02d",ClipData[ii].TCPLD);
    }                                  
  
  sprintf(str+strlen(str)," %02d",ASData.TAst);
  
  int flag=1;  
  if (STInfo.threshold_control_flag==True ) flag=2;
  sprintf(str+strlen(str)," %s %d", "AUTO", flag); 

  //END OF THE REPORT
  sprintf(str+strlen(str)," OVER\n");
  
  /*
 *print on screen:
 */
  TERM_PURPLE_COLOR;printf("%s\n",str);TERM_RESET_COLOR;
  
//write to server
  if(STInfo.client_conn[Socket_1])
    {
      //    client_select(Socket_1);
      if(client_write2(str,Socket_1)==-1)
	{
	  STInfo.client_conn[Socket_1]=False;
	  client_close2(Socket_1);
	}
    }
  if(STInfo.client_conn[Socket_2])
    {
      //    client_select(Socket_2);
      if(client_write2(str,Socket_2)==-1)
	{
	  STInfo.client_conn[Socket_2]=False;
	  client_close2(Socket_2);
	}
    }
//  free(str);
  printf(" Report() finished...\n"); fflush(stdout);
  
}
