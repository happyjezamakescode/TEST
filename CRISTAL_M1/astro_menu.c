/*********************************************************/
/*   File: Astroboard Menu                               */
/*   by: jrg                                             */
/*   date: September 2013                                */
/*   modified: November 2013                             */
/*********************************************************/
#include "AstroLib.h"
#include "CristalSpi.h"
void astro_menu(void)
{
	
	float DesiredPatch ;
	float DesiredDelay = 0;
	float DesiredDelay1 = 0;
	float DesiredDelay2 = 0;
	float DesiredDelay3 = 0;
	float DesiredRateCounts = 0;
	float DesiredRateTime = 0;
	float DesiredRate = 0;
	float DesiredTiming = 0;
	short data = 0;
	float datat = 0;
	int Macrocell = 0;
	short Refchannel = 0;
	char *verb;
	char *tablepath;
    int status;
    int4 iverb,eDelay,Delay,rate, patchnumber, ecount;
     int data1;
     int data2;
     int data3;
     data1=1;
     data2=2;

    CPLD_disconect();
    while(True)
	{
		verb = action_menu( "ASTRO_BOARD_MENU", &iverb);
		if(strcmp(verb,"TMWAIT")==0) 
		{                
			  printf("Maximum time,%i ",ASData.Tmax*1000);
			  usleep(ASData.Tmax*1000);
		}   	

		if(strcmp(verb,"RESET_START")==0)
		{
		    status = transfer(FUNC_ASTRO_RES,0,0,0);   
			printf("Reset every MCPs and start counting\n");
	        	
		}
    
		if(strcmp(verb,"TMAX")==0)
		{
			printf("Default 20 ms\n");
			get_real("Set desired maximum time [ms]",&DesiredRateTime,1,65000);
            status = transfer(FUNC_ASTRO_TOUT,0,0,DesiredRateTime);   
            ASData.Tmax=DesiredRateTime;
		}
    
		if(strcmp(verb,"CMAX")==0)
		{
		    printf("Default 65000 counts\n");
		    get_real("set desired maximum number of counts",&DesiredRateCounts,10.,65000);
		    status = transfer(FUNC_ASTRO_MCNT,0,0,DesiredRateCounts);   
		    ASData.Cmax=DesiredRateCounts;
		}
    
  		if(strcmp(verb,"GLOBALREAD")==0)
                {
                    int Macrocell;
                    status = transfer_c(0xA7,&data1,&data2,&Macrocell);
		    ASData.globalrate=Macrocell;
		    printf("counts %i, elapsed time %i [ms],rate %i[Hz]\n ",data1,data2,Macrocell);
        	}

		if(strcmp(verb,"TIM_ONEREAD")==0)
		  {
		    int j=0;
		    get_short("Macrocell",(int2*) &Macrocell,1,55);
		    j=Macrocell;
		    status = transfer_c(FUNC_ASTRO_TIM,&data1,&data2,&Macrocell);   
		    ASData.tim[j-1]=Macrocell;
		    printf("counts %i, elapsed time %i [ms], "
                   "rate %i[Hz]\n ",data1,data2,Macrocell);
		}
    
		if(strcmp(verb,"TIM_ALLREAD")==0)
		{
			int i;

			printf("Reading all Astro-Board Macrocells\n");
			for ( i=0;i<55;i++)
			  { 
			    data3=i;
			    status = transfer_c(FUNC_ASTRO_TIM,&data1,&data2,&data3);   
			    printf("counts %i, elapsed time %i [ms], "
				   "rate %i[Hz] \n",data1,data2,data3);
			    ASData.tim[i]=data3;
			  }
		}
		
		if(strcmp(verb,"AMP_ONEREAD")==0)
		  {
		    int j=0;
		    get_short("Macrocell",(int2*) &Macrocell,1,55);
		    j=Macrocell;
		    status = transfer_c(FUNC_ASTRO_AMP,&data1,&data2,&Macrocell);   
		    ASData.amp[j-1]=Macrocell;
		    printf("counts %i, elapsed time %i [ms],rate %i[Hz]\n ",data1,data2,Macrocell);
		}
		
		if(strcmp(verb,"ALLREAD")==0)
		  {
		    int i;
		    printf(" DBG Reading all Astro-Board Macrocells\n");
		    for ( i=0;i<55;i++)
		      { 
			data3=i;
			status = transfer_c(FUNC_ASTRO_AMP,&data1,&data2,&data3);   
			ASData.rate[i]=data3;
			printf("macrocell %d  counts %d   elapsed time %d [ms], "
			       "rate %i[Hz] \n",i+1, data1,data2,data3);
		      }


		    status = transfer_c(0xA7,&data1,&data2,&data3);
		    ASData.globalrate=data3;
		    printf("Total rate:  counts %d   elapsed time %d [ms], "
			   "total rate %i[Hz] \n", data1,data2,data3);



		  }
		
		if(strcmp(verb,"ONEREAD")==0)
		  {
		    int j=0;
		    get_short("Macrocell",(int2*) &Macrocell,1,55);
		    j=Macrocell;
		    status = transfer_c(FUNC_ASTRO_RATES,&data1,&data2,&Macrocell);   
		    
		    ASData.rate[j-1]=Macrocell;
		    printf("counts %i, elapsed time %i [ms],rate %i[Hz]\n ",data1,data2,Macrocell);
		  }
    
/*	
	if(strcmp(verb,"ALLREAD")==0)
		{
			int i;
            printf("Reading all Astro-Board Macrocells\n");
            for ( i=0;i<55;i++)
            { 
                data3=i;
                status = transfer_c(FUNC_ASTRO_RATES,&data1,&data2,&data3);   
                ASData.rate[i]=data3;
                printf("counts %i, elapsed time %i [ms],rate %i[Hz]\n ",data1,data2,data3);
           }
        }
  */  

		if(strcmp(verb,"SAVE_RATES")==0)
		{   
		  FILE *Tfile;
		  int i;
		  printf("Saving all Astro-Board Macrocells\n");
		  Tfile= fopen ((char *)ASData.rates_outputname, "a" );     
		  fprintf(Tfile,"#Macrocell     #Rates [Hz]\n");
		  for(i=0;i<55;i++)
		    {
		      fprintf(Tfile,"%i          %f\n",i+1,ASData.rate[i]);
		    }
		  
		  fprintf(Tfile,"Total rate:          %d\n" , (int)ASData.globalrate);
		  fclose ( Tfile );
		}
    
        if(strcmp(verb,"OUTPUTNAME_RATES")==0)
		{
			
            get_text("Enter Sum-Board Table name",(char *)ASData.rates_outputname);
		}
		if(strcmp(verb,"COMPARATOR")==0)
		{
            int channel=0;
            float datat=0;
            get_short("Channel",(int2*) &channel,1,2);
            get_real("Threshold value [0,2.5]", &datat,0,2.5);
            
			printf("Comparator threshold on channel %i is %f\n",channel,datat);
            ASData.cthreshold[channel-1]=datat;
		}
    
		if(strcmp(verb,"ALLDDEL")==0)
		{
		    int i;
		    get_real("Set desired delay (ns)",&DesiredDelay,0.0,5.6);
         	    
		   for (i=0;i<55;i++)
		   {
		        ASData.ddelay[Macrocell-1]=DesiredDelay;
			    status = transfer(FUNC_ASTRO_DEL,i,0,(unsigned short)(DesiredDelay*ddel_conv));
		   	    printf("%d ns delay on Macrocell %d\n", (int)(DesiredDelay*ddel_conv),i);
		   }
		}
    
		if(strcmp(verb,"ONEDDEL")==0)
		{
		    get_short("Macrocell",(int2*) &Macrocell,1,55);
		    get_real("Set desired delay (ns)",&DesiredDelay,0.0,5.6);
          
		    status = transfer(FUNC_ASTRO_DEL,Macrocell-1,0,(int)(DesiredDelay*ddel_conv));
		    ASData.ddelay[Macrocell-1]=DesiredDelay;
		    printf("%i ns delay on Macrocell %i\n", DesiredDelay,Macrocell);
		}
    
		if(strcmp(verb,"ONEDREF")==0)
		{
            get_short("Macrocell",(int2*) &Refchannel,0,1);
            get_real("set desired delay (ns)",&DesiredDelay,0.0,5.6);
		    status = transfer(FUNC_ASTRO_ADEL, Refchannel,0,(int)(DesiredDelay*ddel_conv));
            ASData.dref[Refchannel]=DesiredDelay;
            printf("%f ns delay on analog signal on anlaog input %i\n", DesiredDelay,Refchannel);
        
		}
    
		if(strcmp(verb,"LDDEL")==0)
		{
			printf("Loading Delay tables...\n");
			//load_tables(astro_Delay);
		}
    
		if(strcmp(verb,"SDDEL")==0)
		{
	
			//load_tables(astro_Delay);
		}
    
    
		if(strcmp(verb,"ATABLEPATH")==0)
		{
			//get_text("Enter AStro-Board Table(s) path:",(char *)tablepath);
		//	printf("The astrotables path is: ");
		}
    
		if(strcmp(verb,"WAIT")==0)
		{
			int4 secs=5;
			get_integer("How many secs?",&secs,0,0);
			usleep(secs);
		}
		if(strcmp(verb,"EDEL")==0)
		{
            int line=0;
            float edel_conv=0;
            get_short("Select delay line",(int2*) &line,1,6);
            if (line < 4)
            {
			    get_real("Set desired delay (ns)",&DesiredDelay1,0.,510.);
                edel_conv=edel_conv1;
            }
            if (line > 3 && line < 7)
            {
                get_real("Set desired delay (ns)",&DesiredDelay1,0.,64);
                edel_conv=edel_conv2;
            }
		    
            status = transfer(FUNC_ASTRO_ED,line,0,(int)(DesiredDelay1*edel_conv));
            ASData.edel[line-1] = DesiredDelay1; 
        }
    
		if(strcmp(verb,"ALLEDEL")==0)
		{
            int i;
            get_real("Set desired delay (ns)",&DesiredDelay1,0.,64.);
        
		    status = transfer(FUNC_ASTRO_ED,0x00,7,DesiredDelay1);
            for (i=0;i<7;i++)
            ASData.edel[i] = DesiredDelay1; 
        }
    
		if(strcmp(verb,"ATEMP")==0)
		{
            short data=0;
            status = transfer_t(FUNC_ASTRO_TEM,0 ,0,&data);
			printf("AStro-Board Temperature %i\n",data);
            ASData.TAst=data;
		}
    
		if(strcmp(verb,"STATUS")==0)
		{
            short data=0;
            transfer_t(FUNC_ASTRO_ERR,0 ,0,&data);
            ASData.Astatus= data;
			switch (data)
            {
                case 0:
                    printf("AStro-Board status OK\n");
                    break;  
                case 1:
                    printf(" Error on temperature device\n");
                    break;  
                case 2:
                    printf(" Error on set-read threshold\n");
                    break;  
                case 3:
                    printf(" Error on temperature device & on set-read threshold\n");
                    break;  
           //     case 4:
           //         printf(" STATUS not checked\n");
           //         break;  
            }
		}
    
		if(strcmp(verb,"VERSION")==0)
		{
            status = transfer(FUNC_ASTRO_VER,0,0,0);   
			printf("Reading AStro-Version\n");
		}
    
		if(strcmp(verb,"ENGATE")==0)
		{
            status = transfer(FUNC_ASTRO_GEN,0,0,0);   
            ASData.Gate=1;
			printf("Gate Enable\n");
		}
    
		if(strcmp(verb,"DISGATE")==0)
		{
            status = transfer(FUNC_ASTRO_GDIS,0,0,0);   
            ASData.Gate=0;
			printf("Gate disable\n");
		}
    
		if(strcmp(verb,"PRINT")==0)
		{
		    int i;
            printf("******************************************* \n");
            printf(" One shot delays \n");
			printf(" Macrocell   Delay [ns]\n");
		    for (i=0;i<55;i++)
            {    
                printf("   %i          %f   \n",i+1,ASData.ddelay[i]);
            }
            printf("\n");
            printf("******************************************* \n");
		
            printf(" One shot  anlogue delays \n");
			printf("Analogue   Delay [ns]\n");
		    for (i=0;i<2;i++)
            {    
                printf("   %i          %f   \n",i,ASData.dref[i]);
            }
            printf("\n");
            printf("******************************************* \n");
            
            printf(" Last rates value \n");
			printf(" Macrocell   Rate [counts/ms]\n");
		    for (i=0;i<55;i++)
            {    
                printf("   %i          %f   \n",i+1,ASData.rate[i]);
            }
            printf("\n");
            printf("******************************************* \n");
            
            printf(" Thresholds \n");
			printf(" Macrocell   Threshold [V]\n");
		    for (i=0;i<55;i++)
            {    
                printf("   %i          %f   \n",i+1,ASData.threshold[i]);
            }
            printf("\n");
            printf("******************************************* \n");
            printf(" One shot anlogue delays \n");
            
			printf("Analogue   Delay [ns]\n");
		    for (i=0;i<2;i++)
            {    
                printf("   %i          %f   \n",i,ASData.dref[i]);
            }
            printf("\n");
            printf("******************************************* \n");
            
		
            printf(" One shot anlogue delays \n");
			printf("Analogue   Threshold [V]\n");
		    for (i=0;i<2;i++)
            {    
                printf("   %i          %f   \n",i,ASData.cthreshold[i]);
            }
            printf("\n");
            printf("******************************************* \n");
		
            printf(" External delays \n");
			printf("Analogue   Delay [ns]\n");
		    for (i=0;i<6;i++)
            {    
                printf("   %i          %f   \n",i,ASData.edel[i]);
            }
            printf("\n");
            printf("******************************************* \n");
		
            printf(" STATUS \n");
            data= ASData.Astatus;
			switch (data)
            {
                case 0:
                    printf("AStro-Board status OK\n");
                    break;  
                case 1:
                    printf(" Error on temperature device\n");
                    break;  
                case 2:
                    printf(" Error on set-read threshold\n");
                    break;  
                case 3:
                    printf(" Error on temperature device & on set-read threshold\n");
                    break;  
            }
            printf("\n");
            printf("******************************************* \n");
        }
    
		if(strcmp(verb,"RETURN")==0)
			return;
	}
}
