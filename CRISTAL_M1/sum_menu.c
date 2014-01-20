#include "SumLib.h"                                                             
#include "CristalSpi.h"

void sum_menu()
{
  short board, value;	
  short address;	
  float tvalue = 0;
  float DesiredTiming = 0;//should be global?
  char *verb;
  char *tablepath;
  int4 iverb,eDelay,rate, patchnumber, ecount ;
  
  CPLD_disconect();
  
  while(True)
    {
      verb = action_menu( "SUM_BOARD_MENU", &iverb);
      
      if(strcmp(verb,"ENABLE_THR")==0)
	{
	  STInfo.threshold_control_flag=true;
	  printf("Threshold control ON \n");
	}
      
      if(strcmp(verb,"DISABLE_THR")==0)
	{
	  STInfo.threshold_control_flag=false;
	  printf("Threshold control OFF\n");
	}
      
      if(strcmp(verb,"LOAD_THRE_TABLE")==0)
	{
	  printf("LOADING..\n");
	}
      
      if(strcmp(verb,"SAVE_THRESHOLDS")==0)
	{   
	  FILE *Tfile;
	  int i;
	  Tfile= fopen ((char *)ASData.thr_outputname, "a" );     
	  fprintf(Tfile,"#Macrocell     #Threshold [V]\n");
	  for(i=0;i<55;i++)
	    {
	      fprintf(Tfile,"%i          %f\n",i+1,ASData.threshold[i]);
	    }
	  fclose ( Tfile );
	}
      
      if(strcmp(verb,"PRINT_THRESHOLDS")==0)
        {
          printf("#Macrocell       #Threshold [V]\n");
          int i;
          for(i=0;i<55;i++)
            {
              printf("%i          %f\n", i+1, ASData.threshold[i]);
            }
        }

      if(strcmp(verb,"PRINT_Rates")==0)
	{   
	  printf("#Macrocell:  Rates: \n");
	  int i;
	  for(i=0;i<55;i++)
            {
	      printf("%i          %f\n", i+1, (float)ASData.rate[i]);
            }
	  printf("Total rate: %f", (float)ASData.globalrate);
        }
      
      if(strcmp(verb,"CHANGE_ONE_THRE_DB")==0)
	{   
            int board=0;
            int addres=0;
            int macro=0;
            int data=0;
            float tvalue=0;
            float db=0;
            get_short("Macrocell",(int2*) &macro,1,55);
		    get_real("Set attenuation",&db, 0,0); 
            tvalue=ASData.threshold[macro-1]*pow(10.0,db/20.0);
            data=(tvalue +2.0)*thre_conv;
            board=(int)macro/3.;
            address=(int)macro%3;
            printf("board %i, address %i\n",(int)board,address);
            if (tvalue >-2 && tvalue < 2)
            {
                printf("Threshold %f, on Macrocell %i\n",tvalue,macro);
                printf(" DBG Threshold %i, on Macrocell %i\n",(unsigned short)data,macro);
                transfer(FUNC_SUM_THR ,board,address,(unsigned short)data);
                ASData.threshold[macro-1]=tvalue;
            }
            else printf("Threshold value outside limits\n");
        }
		
	if(strcmp(verb,"CHANGE_ALL_THRE_DB")==0)
	{   
            int board=0;
            int addres=0;
            int data=0;
            int i=0;
            float tvalue=0;
            float db=0;
		    get_real("Set attenuation",&db, 0,0); 
            for(i=0;i<55;i++)
            {
                tvalue=ASData.threshold[i]*pow(10.0,db/20.0);
                if (tvalue>2) tvalue=2;
                if (tvalue<0) tvalue=0;
                data=(tvalue +2.0)*thre_conv;
                board=(int)i/3.;
                address=(int)i%3;
                printf("Threshold %f, on Macrocell %i\n",tvalue,i+1);
                printf(" DBG Threshold %i, on Macrocell %i\n",(unsigned short)data,i+1);
                printf(" DBG Board %i,  Addres %i\n",board, address);
                transfer(FUNC_SUM_THR ,board,address,(unsigned short)data);
                ASData.threshold[i]=tvalue;
            }
        }
		
	if(strcmp(verb,"PRINT_AVERAGE_THR")==0)
	{   
	    int i;
            float average=0;
	    for(i=0;i<55;i++)
            {
                average+=ASData.threshold[i];
            }
                printf(" Average Threshold %f [V] \n", average/55.);
        }
		
        if(strcmp(verb,"OUTPUTNAME_THR")==0)
	{
			
            get_text("Enter Sum-Board Table name",(char *)ASData.thr_outputname);
	}
    
	if(strcmp(verb,"PATHSUMTABLES")==0)
	{
		get_text("Enter Sum-Board Table(s) path",(char *)tablepath);
	}
    
	if(strcmp(verb,"SET_ONE_MAX")==0)
	{
            get_short("Macrocell",(int2*) &board,1,55);
            get_short("Maximum rate",(int2*) &value,1,10000);
            
            ASData.rate_max[board-1]=value;
	}
    
	if(strcmp(verb,"SET_ONE_MIN")==0)
	{
            get_short("Macrocell",(int2*) &board,1,55);
            get_short("Minimum rate",(int2*) &value,1,10000);
            ASData.rate_min[board-1]=value;
	}
    
	if(strcmp(verb,"SET_ALL_MAX")==0)
	{
            get_short("Maximum rate", &value,1,10000);
            int i;
            for(i=0;i<55;i++)
            {    
                ASData.rate_max[i]=value;
            }
	}
    
	if(strcmp(verb,"SET_ALL_MIN")==0)
	{
            get_short("Minimum rate", &value,1,10000);
            int i;
            for(i=0;i<55;i++)
            {    
                ASData.rate_min[i]=value;
            }
	}


	if(strcmp(verb,"SET_ONE_T")==0)
	{
            float data;
            get_short("Board",(int2*) &board,1,19);
            get_short("Address",(int2*) &address,1,3);
	    get_real("Threshold Value (-2,2)",&tvalue, 0,0); 
            data=(tvalue +2.0)*thre_conv;
            transfer(FUNC_SUM_THR ,board-1,address-1,(unsigned short)data);
            printf("Set %f Threshold on address %i on board %i\n", tvalue, address,board);
            ASData.threshold[address-1+board*3]=tvalue;
	}
		
	if(strcmp(verb,"SET_BOARD_T")==0)
	{
            float data;
            get_short("Board",(int2*) &board,1,19);
		    get_real("Threshold Value (-2,2)",&tvalue, 0,0); 
            data=(tvalue +2.0)*thre_conv;
            int i;
            for (i=0;i<3;i++)
            {
                transfer(FUNC_SUM_THR ,board-1,i,(unsigned short)data);
                ASData.threshold[i+board*3]=tvalue;
            }
            printf("%f Threshold on all the address on board %i\n", tvalue,board);

	}
		
	if(strcmp(verb,"SET_CRATE_T")==0)
	{
            float data;
		    get_real("Threshold Value (-2,2)",&tvalue, 0,0); 
            data=(tvalue +2.0)*thre_conv;
            int i,j;
            for (j=0;j<19;j++)
            {
                for (i=0;i<3;i++)
                {
                    transfer(FUNC_SUM_THR ,j,i,(unsigned short)data);
                    ASData.threshold[i+j*3]=tvalue;
                }
            }
              
            printf("*********************************\n");
            printf("%f Threshold on all the crate\n", tvalue);
            printf("*********************************\n");
		
        }
    
		if(strcmp(verb,"RETURN")==0)
			return;
	}
}
