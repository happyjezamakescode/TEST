/**********************************************************/
/* Monitor thread                                         */
/* date: Septmeber 2013                                   */
/* by: jrg                                                */

/*
  Steps:
   1. Read Macrocell Rate (MR) and Global Rate (GR)

   2. Calc Average Global Rate (AGR) and Deviation (DGR) from Target Global Rate (TGR)

      AGR = (1-fG)*GR + fG*AGR  where fG=exp(-1/tauG) 
      DGR = (AGR - TGR) / sqrt(AGR * tauG) 

      If DGR > mDGR (maximum Deviation from Global Target Rate)
        - Calc index

	index = (AGR>threshold_index_global ) ? index_global_high : index_global_low
	ratio = TGR/AGR

        - Change Macrocell Target Rate (TMR) in epsilon steps

	TMR = (1 - epsilon*(1-ratio**(1/index)) * TMR

       
   3. Loop over macrocells
      Calc Average Macrocell Rate (AMR) and Deviation (DMR) from Target Macrocell Rate (TMR)
         
      AMR = (1-fM)*MR + fM*AMR  where fM=exp(-1/tauM)
      DMR = (AMR - TMR) / sqrt(AMR * tauM)
          
`     If DMR > mDMGR (maximum Deviation from Macrocell Target Rate)
         - Calc index

	 index = (AMR>threshold_index_macro) ? index_macro_high : index_macro_low
	 ratio = TMR/AMR

         - Change the Macrocell Threshold (Th)

	 Th = (1 + epsilon*(1-ratio**(1/index))) * Th   

 */

/**********************************************************/

#define sign(a) ((a>=0)?1:(-1));

pthread_t STmon_tid;             /* thread ID structure */
pthread_attr_t STmon_attr;     /* thread attributes */
#include "CristalSpi.h"
FILE *Tfile;

void *STmon_thread(void)
{
  float diff_target_macro[55];
  float diff_target_global;
  float av_macrocell_rate[55];
  float av_global_rate=0;
  float time_const_macro=3; // 5 seconds integration time for macrocell rate
  float time_const_global=4; // 15 seconds integration time for the global rate
  float keep_fraction_macro;
  float keep_fraction_global;
  float max_diff_global=0.5; // if rate difference is larger 
                             //than max_diff_global sigma then change threshold
  float max_diff_macro=0.5; // if rate difference is larger than max_diff_macro 
                            // sigma then change threshold
  float index_macro_high=5;  // slope of NSB background
  float index_macro_low=1.5; // slope of showers
  float threshold_index_macro=80; // index changes above threshold_index Hz.
  float index_global_high=1.0; // rate changes for macrocells are 'proportional' 
                               // to global rate changes 
  float index_global_low=1.0;
  float threshold_index_global=500; // index changes above threshold_index Hz.
  float avthres=0; 
  int i;					
  int data;
  unsigned short tdata;
  short laser;
  char s[100];
  float index, ratio, epsilon, del;
  static int count=0;
  static int count2=0;

  char outfname[500] = "/home/operator/ana10/STCalibration/summon_M2.txt"; //MLM
  //FILE* fout = fopen(outfname,"w"); //MLM

  
  printf("STmon_thread initiated \n");

  if(!STInfo.monexit)
    {
      printf("Monitoring thread already running. Stopping\n");
      pthread_exit(0);
    }

  STInfo.monexit=false; //thread running

  // initialize some variables
  for(i=0; i<56; i++) 
    {
      av_macrocell_rate[i]=0;
    }
  
  keep_fraction_macro=exp(-1./time_const_macro);
  keep_fraction_global=exp(-1./time_const_global);

  av_global_rate=0;

  // start monitoring loop

  while(!STInfo.monexit)
    { 
      if(!(count%30)) //every 30 sec
	{
	  count=0;
	  printf("Info: Monitoring thread activ.\n");
	}
      count++;
      /*
       *  Correct Delay betwen Telescopes 
      if(!(count%10)) //every 10 sec
	{
	  count2=0;
	  if (STInfo.telescope==1 && STInfo.SumL3_delay>0)
	    {
	      del= STInfo.SumL3_delay;
	      char str[1024];
	      str[0]='\0';
	      strcat((char *)str,"ast edel 2  ");   // external delay
	      sprintf(str+strlen(str), "%f ", del);
	      sprintf(str+strlen(str), "ret");
	      simulate_input((char *)str);
  	      TERM_GREEN_COLOR;
	      printf("********************************************************************\n");
	      printf("********************************************************************\n");
	     // printf("executing : \n %s \n", str);
	      printf("Correcting %fns delay on M%i\n", STInfo.SumL3_delay, STInfo.telescope);
	      printf("********************************************************************\n");
	      TERM_RESET_COLOR;
	    }
    
	  if (STInfo.telescope==2 && STInfo.SumL3_delay<0)
	    {
	      del= (-1)*STInfo.SumL3_delay;
	      char str[1024];
	      str[0]='\0';
	      strcat((char *)str,"ast edel 2 ");   // external delay
	      sprintf(str+strlen(str), "%f ", del);
	      sprintf(str+strlen(str), "ret"); 
  	      TERM_GREEN_COLOR;
	      printf("********************************************************************\n");
	     // printf("executing : \n %s \n", str);
	      simulate_input((char *)str);
	      printf("Correcting %fns delay on M%i\n", STInfo.SumL3_delay, STInfo.telescope);
	      printf("********************************************************************\n");
	      TERM_RESET_COLOR;
	    }
	}
      count2++;
 
  
       */
      
      // read the rates

      transfer(FUNC_ASTRO_RES,0,0,0); //start counting 
      
      // wait one second
      usleep(1000*ASData.Tmax);                                                                     
     
      // read all rates
      int data1=0,data2=0, data3=0;
      for (i=0;i<55;i++)
        {
	  data3=i+1;
	  transfer_c(FUNC_ASTRO_AMP,&data1,&data2,&data3);   
	  ASData.rate[i]=(unsigned int)data3;
	  //printf("%d  ", data3);
        }
      //printf("\n");

      //read the global rate
      transfer_c(FUNC_ASTRO_GLOBAL, &data1, &data2, &data);
      ASData.globalrate=data;
      if(av_global_rate==0) av_global_rate=data;
	  
      //Define the Status of the System related with the rate	   
      if(data == 0 && STInfo.istatus!=ST_STATUS_STOPPED) STInfo.istatus=ST_STATUS_ZERORATE;
      if(data != 0 && STInfo.istatus!=ST_STATUS_STOPPED)  STInfo.istatus=ST_STATUS_TRIGGERING;
     
       //If it is M2 read the Sum-L3 rate:
      ASData.L3Rate=0;
      if (STInfo.telescope==2)
      {
      	transfer_c(FUNC_ASTRO_SUML3, &data1, &data2, &data);
	ASData.L3Rate=data;
	}
	
       /*
       	* Laser control
       	*/
        transfer_t(FUNC_ASTRO_LASER,0,0,&laser); //Is the LASER on? 
        CalSet.Laser=laser; 
        printf("DBG LASER, %i \n", laser);
        printf("DBG LASER, %i \n", laser);
        printf("DBG LASER, %i \n", laser);

       /*
       	* Temperature control
       	*/
      	int i;
      	int j;
      	short data=0;
      
      for(j=1;j<19;j++)
	{
	  transfer(FUNC_CHANGE_TEM ,j,0,1);
	}
      
      for(j=1;j<19;j++)
	{
	  transfer_t(FUNC_CLIP_TEM ,j,0,&data);
	  ClipData[j-1].TCPLD= data ;
	  transfer(FUNC_CHANGE_TEM ,j,0,3);
	}
      
  
      for(j=1;j<19;j++)
	{
	  transfer_t(FUNC_CLIP_TEM ,j,0,&data);
	  ClipData[j-1].TAMP= data ;
	}
      
      transfer_t(FUNC_ASTRO_TEM,0 ,0,&data);
      ASData.TAst=data;
      
      /*
       * Automatic Threshold Control by Thomas
       */
      
      if(STInfo.threshold_control_flag) // threshold control activated ?
        {

	  //print rates on screen when ATC activ
//	  printf("Macrocell thresholds and rates [Hz]: \n");
//	  for(i=0; i<55; i++)
//	    printf("%d: %5.3f V:  %d Hz; \n  ", i+1, 
//		   ASData.threshold[i], (unsigned int)ASData.rate[i]);
//	  printf("\n");
	  
//	  printf("Global rate: %d\n", ASData.globalrate); 
	  
	  // two control loops, one within another
	  //--------------------------------------
	  
	  // average of global rate
	  av_global_rate = ASData.globalrate*(1-keep_fraction_global) + 
	    keep_fraction_global*av_global_rate;
	  
	  if(av_global_rate)
	    diff_target_global=((av_global_rate-(float)STInfo.Global_Target_rate)/
				sqrt(av_global_rate*time_const_global)); 
	  else
	    diff_target_global=-10;

	  if(diff_target_global>200) diff_target_global=200;
	  if(diff_target_global<-200) diff_target_global=-200;

//	  printf("Average global rate: %5.1f, deviation in sigma: %5.1f , target: %5.1f\n", 
	  //av_global_rate, diff_target_global, (float)STInfo.Global_Target_rate);
	  // deviation in units of sigma
	  
          // deviation bigger than max_diff_global ?
	  if(fabs(diff_target_global) >  max_diff_global)
	    {
	      if(STInfo.Macrocell_Target_rate == 0) // initialize value if zero
		STInfo.Macrocell_Target_rate=60;  // initialize to a meaningful value
	      
	      index=(av_global_rate>threshold_index_global)?index_global_high:index_global_low;
	      
	      if(av_global_rate)
		ratio=(float)STInfo.Global_Target_rate/av_global_rate;
	      else
		ratio=10;

	      // then change the wish target rate for the macrocells
	      epsilon=0.03; //change target rate in small steps
	      STInfo.Macrocell_Target_rate =(1 - epsilon*(1-pow(ratio, 1./index)))*
		(float)STInfo.Macrocell_Target_rate;

	      if(STInfo.Macrocell_Target_rate > 2000)
		STInfo.Macrocell_Target_rate=2000;
	      if(STInfo.Macrocell_Target_rate <1)
		STInfo.Macrocell_Target_rate=1;

	      //printf("New Macrocell target rate: %d\n", 
	        //     (int)STInfo.Macrocell_Target_rate);
	    }

	  avthres=0;

	  for(i=0; i<55; i++)
	    {
	      // average of each macrocell trigger rates
	      av_macrocell_rate[i] =(float) (ASData.rate[i]*(1-keep_fraction_macro) + 
		keep_fraction_macro*av_macrocell_rate[i]);
	      
//	      printf("----> %5.1f  ", av_macrocell_rate[i]);
	      // deviation in units of sigma                                                   
	      
	      if(av_macrocell_rate[i])
		diff_target_macro[i]=((av_macrocell_rate[i]-
				       (float)STInfo.Macrocell_Target_rate)/
				      sqrt(av_macrocell_rate[i]*time_const_macro));
	      else
	        {
		   diff_target_macro[i]=10;
                   av_macrocell_rate[i] = ASData.rate[i];
                }

	      // devation bigger than max_diff_global ?                                             
	      if(fabs(diff_target_macro[i]) >  max_diff_macro) 
		{     
		  index=(av_macrocell_rate[i] > threshold_index_macro)?
		    index_macro_high:index_macro_low;
		  
		  //change the macrocell thresholds
		  if(av_macrocell_rate[i])
		    ratio=(float)STInfo.Macrocell_Target_rate/av_macrocell_rate[i];
		  else
		    ratio=10;


		  epsilon=0.15; //change thresholds in small steps                            
		  
			//Add by jrg
		  
		    //if(ASData.rate[i]>5*av_macrocell_rate[i])
		    if(ASData.rate[i]>5*STInfo.Macrocell_Target_rate)
		   // if(diff_target_macro < 10)
                    { 
			ASData.threshold[i] +=0.2;
                    }
                    //else if(diff_target_macro > -10)
		/*    else if(ASData.rate[i]< 0.3*STInfo.Macrocell_Target_rate)
		    {
			ASData.threshold[i] -=0.2;

		    } 
                  */
		    else
                    {
		    	ASData.threshold[i] =  (1 + epsilon*(1 - pow(ratio, 1./index)))*
		    	(float)ASData.threshold[i];
		    }
 
		  
		  if (ASData.threshold[i]>2) ASData.threshold[i]=2; //max 2V
		  if (ASData.threshold[i]<0.005) ASData.threshold[i]=0.005; //min 5mV
		  //end of add by jrg
 		}


	      avthres+=ASData.threshold[i];	
	      //set new threshold
	      int i,j;
	      for (j=0;j<19;j++)
		{
		  for (i=0;i<3;i++)
		    {
		      tdata = (unsigned short)
			((ASData.threshold[i+j*3]+2.0)*thre_conv);
		      transfer(FUNC_SUM_THR ,j,i, tdata);
		      
		    }
		}

	    }

              printf("Macrocell thresholds and rates [Hz]: \n");
              for(i=0; i<55; i++)
                {
                   printf("%d: %5.3f V:  %d Hz -->  ", i+1, 
                      ASData.threshold[i], (unsigned int)ASData.rate[i]);
		    //if(ASData.rate[i]< 0.5*STInfo.Macrocell_Target_rate|| ASData.rate[i]> 3*STInfo.Macrocell_Target_rate) printf ("  Here!!  ");
		    //if(diff_target_macro > 10 || diff_target_macro < -10) printf ("  Here!!  ");
		      if(ASData.rate[i]>5*av_macrocell_rate[i]) printf ("  Here!!  ");
                   printf("%5.1f diff: %5.1f\n ", av_macrocell_rate[i], diff_target_macro[i]);
                }
              printf("\n");
              printf("New Macrocell target rate: %5.1f\n", 
                       STInfo.Macrocell_Target_rate);
              printf("Average global rate: %5.1f, deviation in sigma: %5.1f , target: %5.1f\n", 
                 av_global_rate, diff_target_global, (float)STInfo.Global_Target_rate);
              printf("Global rate: %d\n", ASData.globalrate); 


	  avthres/=55.0;
	  printf("Average Threshold: %f\n", avthres);
	  
	} // end loop over macrocells

/*
      // Write to file to plot with ROOT
      printf("Writing to file %s\n",outfname);

      //fprintf(fout, "Time: %f \n",  (float)writetime);
      fprintf(fout, "Target rates: %f %f \n",  (float)STInfo.Global_Target_rate, (float)STInfo.Macrocell_Target_rate);

      fprintf(fout, "Rates: %d  ", ASData.globalrate);
      for(i=0; i<55; i++)
	fprintf(fout,"%d ", (unsigned int)ASData.rate[i]);
      fprintf(fout, "\n");      

      fprintf(fout, "Average rates: %f  ", (float)  av_global_rate);
      for(i=0; i<55; i++)
	fprintf(fout,"%f ", av_macrocell_rate[i] );
      fprintf(fout, "\n");

      fprintf(fout, "Thresholds: %f  ", avthres);
      for(i=0; i<55; i++)
	fprintf(fout,"%f ", (float)ASData.threshold[i] );
      fprintf(fout, "\n");
*/



    } // end infinite loop

 // fclose(fout);
  printf("Exit monitoring thread.\n");
  
  pthread_exit(0);
}

