/*******************************************************************
 * Tools menu
 * date: october 2013
 * by: jrg
 *
 ********************************************************************/
//#include "CristalSpi.h"

void tools_menu()
{
    short board;	
    short address;	
	float tvalue = 0;
	float DesiredTiming = 0;//should be global?
	char *verb;
	char tablepath[100], macroname[50];
	int4 iverb,eDelay,rate, patchnumber, ecount ;


	while(True)
	{
		verb = action_menu( "TOOLS_MENU", &iverb);
        if(strcmp(verb,"TEMP")==0) 
        {
          get_integer("Set the duration of the test",&STInfo.total_time,0,3600000);
          get_integer("Set the interval between measures",&STInfo.time_interval,1,36000);
          get_integer("Enable (1) or disable(0)Temperature Control) ",&STInfo.temp_control,0,0);
          if(STInfo.temp_control==1)
            printf("Monitoring the temperature of the crate\n");
        }
        if(strcmp(verb,"MAP")==0) mapping_menu();
   
        if(strcmp(verb,"CALIB")==0) calib_menu();
   
        if(strcmp(verb,"SPITEST")==0) SPI_test_menu();
    
        if(strcmp(verb,"RUNMACRO")==0) 
        {                                                
			get_text("Enter macro name",(char*)macroname);
            char str[1024]; 
            str[0]='\0';
            strcat((char *)str,"@");   //setdt
            strcat((char *)str,macroname); 
            printf("executing : \n %s \n", str);
            AllowInput=1;
            simulate_input((char *)str);
		}
    /* 
        if(strcmp(verb,"THRESHOLDSCAN")==0) 
        {  
            float threshold[50];
            int rate[50][57];//thresold-macrocell
            float gstep=0.1;
            float fstep=0.01;
            float tvalue;
            int data=0,data1=0,data2=0,Macrocell=0;
            int i,j,k;
            for (k=0;k<41;k++)
            {
                tvalue=gstep*k-2;
                threshold[k]=tvalue;
                data=(int)((tvalue +2.0)*thre_conv);
                for (j=0;j<19;j++)
                {
                    for (i=0;i<3;i++)
                    {
                        transfer(FUNC_SUM_THR ,j-1,i,data);//set thresold
                     //   transfer(FUNC_ASTRO_RES,0,0,0); //start counting  
                    }
                }
                    usleep(1000*ASData.Tmax);
                   
                    printf("%f Threshold ", tvalue);
                    
                for (j=1;j<55;j++)
                {
                    Macrocell=j;
                   // transfer_c(FUNC_ASTRO_AMP,&data1,&data2,&Macrocell);   
                    rate[k][j]=Macrocell;
                }
                    
                for (j=1;j<55;j++)
                {
                    printf("Macrocel %i, rate %i", j,rate[k][j]);
                }
                    
            }
		}
	*/
        if(strcmp(verb,"RETURN")==0)
			return;
	}
