/********************************************************************/
/* Clip-Board contol menu                                           */
/* date: Septmeber 2013                                             */
/* by: jrg                                                          */
/********************************************************************/

#include "ClipLib.h" 
#include "CristalSpi.h" 
void Delay2Bias(float Delay, int *fixed, float *Bias, float *Attenuation);
void Bias2Delay(float Bias, int fixed, float *Delay, float *Attenuation);
void clip_menu(void)
{
  short clip_onoff= 1;
  short clip_fixD= 1;
  short pixel= 1;
  short clip_board= 1;
  short clip_channel = 3;
  short value=255;
  float clip_val= 0.0;
  float att_val= 0.0;
  float del_val= 0.0;
  char *verb;
  char*tablepath,*atablename, *dtablename, *ftablename, *ctablename; 
  int status;
  int4 iverb,eDelay,rate, patchnumber, ecount ;
  
  //Adeed by Daisuke 
  float biasvoltage = 0;    
  int fixeddelay = 0;
  float delayvalue=0; 
  float biasvalue=0;
  float attenuationvalue=0;
  
  CPLD_conect();
  
  while(True)
    {
      verb = action_menu( "CLIP_BOARD_MENU", &iverb);
      /*
       * Connect and Disconnect to the ClipBoard
       */
      if(strcmp(verb,"DISCONNECT")==0)
	{
	  CPLD_disconect();
	}
      
      if(strcmp(verb,"CONNECT")==0)
	{
	  CPLD_conect();
	}
      
      /*
       * Board information
       */
      if(strcmp(verb,"VERSION")==0)
	{
	  get_short("Board",(int2*) &clip_board,1,18);
	  status = transfer(FUNC_CLIP_VER ,ALL_BOARDS,ALL_C,0);
	  printf("Current version:\n");
	}
      
      if(strcmp(verb,"SERIALN")==0)
	{
	  get_short("Board",(int2*) &clip_board,1,18);
	  printf("READ\n");
	  status = transfer(FUNC_CLIP_SN ,clip_board,0,0);
	}

      /*
       * Initialize
       */
      if(strcmp(verb,"INIBOARD")==0)
	{
	  get_short("Board",(int2*) &clip_board,1,18);
	  int i;
	  for (i=0;i<32;i++)
	    {
	      transfer(FUNC_CLIP_CLP,clip_board,i,0 );
	      transfer(FUNC_CLIP_DEL,clip_board,i,0);
	      transfer(FUNC_CLIP_ATT,clip_board,i,0);
	    }
	  printf("");
	}
      
      if(strcmp(verb,"INICRATE")==0)
	{
          int i,j;
          for (j=1;j<19;j++)
	    {
              for (i=0;i<32;i++)
		{
                  transfer(FUNC_CLIP_CLP,j,i,0 );
                  transfer(FUNC_CLIP_DEL,j,i,0);
                  transfer(FUNC_CLIP_ATT,j,i,0);
		}
	    }
          printf("");
	}
      
      if(strcmp(verb,"LEDON")==0)
	{
	  
	  get_short("Board",(int2*) &clip_board,1,255);
	  status = transfer(FUNC_CLIP_LED,clip_board,clip_channel, 0xFF);
	}
      
      if(strcmp(verb,"LEDOFF")==0)
	{
	  get_short("Board",(int2*) &clip_board,1,255);
	  status = transfer(FUNC_CLIP_LED,clip_board,clip_channel, 0x00);
	}
      
      if(strcmp(verb,"CTABLEPATH")==0)
	{
	  get_text("Enter Clip-Board Table(s) path",(char *)tablepath);
	}

      /*
       * Attenuation 
       */
      if(strcmp(verb,"SATT")==0)
	{
	  printf("Seting all CLIP-Board attenuation\n");
	  char str[1024]; 
	  str[0]='\0';
	  strcat((char *)str,"CHATT @");   //setdt
	  strcat((char *)str,"dummy.uic"); 
	  printf("executing : \n %s \n", str);
	  simulate_input((char *)str);
	}


      //added by Thomas set delay in ns                                                                                   
      if(strcmp(verb,"SETDELPIX")==0)
	{
	  float data=0;
	  int i,j;
	  int dummy;

	  // comment: careful ! This routine only works if the delay was 
	  // calibrated when using flatfielded attenuations at 30V and fix
	  // delay off.

	  for (i=0;i<529;i++)
	    {
	      //	    printf("%d --> /n", i);
	      //pixel=1;
	      //delayvalue=3.2;
	      get_short("Position (dummy)",(int2*) &dummy,1,800);
	      get_short("Pixel",(int2*) &pixel,1,800);
	      get_real("Delay [ns]",&delayvalue,0.,7.);

	      //convert                                                                                                // move to 6.5ns - measured value
	      Delay2Bias(delayvalue, &fixeddelay, &biasvalue, &attenuationvalue);
	    
	    
	      // invert polarity
	    
	      fixeddelay-=1;
	      fixeddelay=abs(fixeddelay);
	    

	      for(j=0;j<559;j++)
		{
		  if (pixel==MAP[j].PIXEL)
		    {
		      clip_board=MAP[j].CLIP_SLOT;
		      clip_channel=(MAP[j].S_CLIP_CONNECTOR[0]-1)*8+MAP[j].S_CLIP_CHANNEL[0];

		      att_val =ClipData[clip_board-1].att[clip_channel-1] + 
			attenuationvalue - 9.07; //6.9 ? or 9.06 ?;                  
		      ClipData[clip_board-1].att[clip_channel-1] = att_val;
		      ClipData[clip_board-1].delay[clip_channel-1] = biasvalue;
		      ClipData[clip_board-1].fixd[clip_channel-1] = fixeddelay;

		      // change the delay voltage, the fix delay, the attenuation accordingly
		      status = transfer(FUNC_CLIP_ATT ,clip_board,
					clip_channel-1, (unsigned short)(att_val*att_conv));
		      status = transfer(FUNC_CLIP_DEL ,clip_board,
					clip_channel-1 ,(unsigned short)(biasvalue*del_conv));
		      status = transfer(FUNC_CLIP_SWC ,clip_board,
					clip_channel-1, fixeddelay);

	

		    }

		}

	    }
	}



                                                                                                           
      if(strcmp(verb,"SETDEL_ONEPIX")==0)
	{
	  float data=0;
	  int i,j;
	  int dummy;

	  // comment: careful ! This routine only works if the delay was                                     
	  // calibrated when using flatfielded attenuations at 30V and fix                                   
	  // delay off.                                                                                      

	  // get_short("Position (dummy)",(int2*) &dummy,1,800);
	  get_short("Pixel",(int2*) &pixel,1,800);
	  get_real("Delay [ns]",&delayvalue,0.,7.);
	
	  //convert				\
	  // move to 6.5ns - measured value                                                               
	
	  Delay2Bias(delayvalue, &fixeddelay, &biasvalue, &attenuationvalue);
	
	  // invert polarity                                                                             
	
	  fixeddelay-=1;
	  fixeddelay=abs(fixeddelay);
	

	  for(j=0;j<559;j++)
	    {
	      if (pixel==MAP[j].PIXEL)
		{
		  clip_board=MAP[j].CLIP_SLOT;
		  clip_channel=(MAP[j].S_CLIP_CONNECTOR[0]-1)*8+MAP[j].S_CLIP_CHANNEL[0];
		
		  att_val =ClipData[clip_board-1].att[clip_channel-1] +
		    attenuationvalue - 9.07; //6.9 ? or 9.06 ?;                                          
		  ClipData[clip_board-1].att[clip_channel-1] = att_val;
		  ClipData[clip_board-1].delay[clip_channel-1] = biasvalue;
		  ClipData[clip_board-1].fixd[clip_channel-1] = fixeddelay;
		
		  // change the delay voltage, the fix delay, the attenuation accordingly                
		  status = transfer(FUNC_CLIP_ATT ,clip_board,
				    clip_channel-1, (unsigned short)(att_val*att_conv));
		  status = transfer(FUNC_CLIP_DEL ,clip_board,
				    clip_channel-1 ,(unsigned short)(biasvalue*del_conv));
		  status = transfer(FUNC_CLIP_SWC ,clip_board,
				    clip_channel-1, fixeddelay);		
		
		}

	    }
	}



      if(strcmp(verb,"LABSDEL")==0)
	{
	  char Name[100];
	
	  get_text("Enter flatfielded delay table name",(char *)Name);

	  char str[1024];
	  str[0]='\0';
	  strcat((char *)str,"SETDELPIX @");   //set delay                                                                    
	  strcat((char *)str, Name);
	  printf("executing : \n %s \n", str);
	  simulate_input((char *)str);
	}



      // end added by Thomas      
      if(strcmp(verb,"SETATPIX")==0)
	{
	  float data=0;
	  int i,j;	
	  int dummy;
	  for (i=0;i<529;i++)
	    {
	      get_short("Pos",(int2*) &dummy,1,800);
	      get_short("Pixel",(int2*) &pixel,1,800);
	      get_real("Amplitud [V]",&data,0.,0);
	      get_real("Attenuation [dB]",&att_val,0.,31.5);
	    
	      for(j=0;j<559;j++)
		{
		  if (pixel==MAP[j].PIXEL)
		    {  
		      clip_board=MAP[j].CLIP_SLOT;
		      clip_channel=(MAP[j].S_CLIP_CONNECTOR[0]-1)*8+MAP[j].S_CLIP_CHANNEL[0];			        
		      ClipData[clip_board-1].att[clip_channel-1]=att_val;
		    }
	        
		}

	      transfer(FUNC_CLIP_ATT ,clip_board,clip_channel-1, (unsigned short)(att_val*att_conv));
	    }
	}
      
      if(strcmp(verb,"LATT")==0)
	{
	  char Name[100];
	  
	  //printf("Load Attenuation table for all the Pixels\n");
	  get_text("Enter flatfielded attenuation table name",(char *)Name);
	  
	  
	  char str[1024]; 
	  str[0]='\0';
	  strcat((char *)str,"SETATPIX @");   //setdt
	  strcat((char *)str, Name); 
	  printf("executing : \n %s \n", str);
	  simulate_input((char *)str);
	}
      

      if(strcmp(verb,"CHATT")==0)
	{
	  get_short("Board",(int2*) &clip_board,1,18);
	  get_short("Channel",(int2*) &clip_channel,1,32);
	  get_real("Attenuation [dB]",&att_val,0.,31.5);
	  // printf("DBG status real %i\n", status);
	  ClipData[clip_board-1].att[clip_channel-1]=att_val;
	  status = transfer(FUNC_CLIP_ATT ,clip_board,clip_channel-1, (unsigned short)(att_val*att_conv));
	  printf("DBG status transfer %i\n", status);
	  printf("%f Attenuation on channel %i on board %i\n", ClipData[clip_board-1].att[clip_channel-1], clip_channel,clip_board);
	  printf("Conversion factor %f", att_conv);
	  printf("");
	}
    
      if(strcmp(verb,"BATT")==0)
	{
	  get_short("Board",(int2*) &clip_board,1,18);
	  get_real("Attenuation [db]",&att_val,0.,31.5);
	  int i;
	  for (i=0;i<32;i++)
	    {
	      status = transfer(FUNC_CLIP_ATT ,clip_board,i, (unsigned short)(att_val*att_conv));
	      ClipData[clip_board-1].att[i]=att_val;
	    }
	  printf("*********************************\n");
	  printf("%f db attenuation on all channeles on board %i\n", att_val, clip_board);
	  printf("*********************************\n");
	  printf("");
	}
    
      if(strcmp(verb,"CATT")==0)
	{
	  get_real("Attenuation [db]",&att_val,0.,31.5);
	  int i,j;
	  for (j=0;j<18;j++)
	    {
	      for (i=0;i<32;i++)
		{
		  status = transfer(FUNC_CLIP_ATT ,j+1,i, (unsigned short)(att_val*att_conv));
		  ClipData[j].att[i]=att_val;
		}
	    }
            
	  printf("*********************************\n");
	  printf("%f db attenuation on all channeles of the crate\n", att_val);
	  printf("*********************************\n");
	  printf("");
	}
    
      /*
       * CLIP 
       */
      if(strcmp(verb,"LCLIP")==0)
	{
	  printf("Seting all CLIP-Board clipping\n");
	  char str[1024]; 
	  str[0]='\0';
	  strcat((char *)str,"CHCLIP @");   //setdt
	  strcat((char *)str,"dummy.uic"); 
	  printf("executing : \n %s \n", str);
	  simulate_input((char *)str);
	}
    
      if(strcmp(verb,"SCLIP")==0)
	{
	  get_text("Enter Clip-Board clipping Table name",(char *)ctablename);
	  printf("Save all CLIP-Board\n");
	}
    
      if(strcmp(verb,"CHCLIP")==0)
	{
	  get_short("Board",(int2*) &clip_board,1,18);
	  get_short("Channel",(int2*) &clip_channel,1,32);
	  get_real("Clipping [db]",(float*) &clip_val,0.,2.5);
	  ClipData[clip_board-1].clip[clip_channel-1]=clip_val;
	  status = transfer(FUNC_CLIP_CLP,clip_board,clip_channel-1,(unsigned short)(clip_val*clip_conv));
	  printf("%f Clipping on channel %i on board %i\n", ClipData[clip_board-1].clip[clip_channel-1], clip_channel,clip_board);
	  printf("");
	}
    
      if(strcmp(verb,"BCLIP")==0)
	{
	  get_short("Board",(int2*) &clip_board,1,18);
	  get_real("Clipping [V]",(float*) &clip_val,0.,2.5);
            
	  int i;
	  for (i=0;i<32;i++)
	    {
	      status = transfer(FUNC_CLIP_CLP,clip_board,i,(unsigned short)(clip_val*clip_conv));
	      ClipData[clip_board-1].clip[i]=clip_val;
	    }
	  printf("%f clipping voltage on all channeles on board %i\n", clip_val,clip_board);
	  printf("");
	}
    
      if(strcmp(verb,"CCLIP")==0)
	{
	  get_real("Clipping [V]",(float*) &clip_val,0.,2.5);
            
	  int i,j;
	  for (j=0;j<18;j++)
	    {
	      for (i=0;i<32;i++)
		{
		  status = transfer(FUNC_CLIP_CLP,j+1,i,(unsigned short)(clip_val*clip_conv));
		  ClipData[j].clip[i]=clip_val;
		}
	    }
	  printf("*********************************\n");
	  printf("%f clipping voltage on all the crate \n", clip_val);
	  printf("*********************************\n");
	  printf("");
	}
    
      /*
       * Delay
       */    
      if(strcmp(verb,"SETDELAYPIX")==0)
	{
	  float data=0;
	  float att=0;
	  int data_delay=0;
	  int i,j;		
	  // for (i=0;i<2;i++)
	  for (i=0;i<613;i++)
	    {
	      get_short("Pixel",(int2*) &pixel,1,800);
	      get_real("Delay [ns]",&data,0.,0);
	      Delay2Bias(data,&fixeddelay, &biasvalue,&attenuationvalue);
	      data_delay=(int)(biasvalue *del_conv);
	      fixeddelay=abs(fixeddelay-1);
	      for(j=0;j<559;j++)
	        {
		  if (pixel==MAP[j].PIXEL)
		    {  
		      clip_board=MAP[j].CLIP_SLOT;
		      clip_channel=(MAP[j].S_CLIP_CONNECTOR[0]-1)*8+MAP[j].S_CLIP_CHANNEL[0];
		      att=attenuationvalue+ClipData[clip_board-1].att[clip_channel-1]-8.0;
		      ClipData[clip_board-1].att[clip_channel-1]=att;
		      
		      transfer(FUNC_CLIP_DEL ,clip_board,clip_channel-1,data_delay);
		      transfer(FUNC_CLIP_SWC ,clip_board,clip_channel-1,fixeddelay);
		      transfer(FUNC_CLIP_ATT ,clip_board,clip_channel-1,(int)(att*att_conv));
		    }
		  
		}
	      
	    }
	}
    
      if(strcmp(verb,"LDELAY")==0)
	{
	  printf("Load Delay table for all the Pixels\n");

	  char str[1024]; 
	  str[0]='\0';
	  strcat((char *)str,"SETDELPIX @");   //setdt
	  strcat((char *)str,"DelayFlatM2.uic"); 
	  printf("executing : \n %s \n", str);
	  simulate_input((char *)str);
	}
    
      if(strcmp(verb,"SDEL")==0)
	{
	  get_text("Enter Clip-Board delay Table name",(char *)dtablename);
	  printf("Save all CLIP-Board\n");
	}
    
      if(strcmp(verb,"CHDEL")==0)
	{
	  get_short("Board",(int2*) &clip_board,1,18);
	  get_short("Channel",(int2*) &clip_channel,1,32);
	  get_real("Delay [V]",(float*) &del_val,0.,30.);
	  status = transfer(FUNC_CLIP_DEL ,clip_board,clip_channel-1,
			    (unsigned short)(del_val*del_conv));
	  ClipData[clip_board-1].delay[clip_channel-1]=del_val;
	  printf("%f Delay on channel %i on board %i\n", 
		 ClipData[clip_board-1].delay[clip_channel-1], clip_channel,clip_board);
	  printf("");
	}
    
      if(strcmp(verb,"LCHDEL")==0)
	{
	  int i;
	  for (i=0;i<608;i++)
	    {
	      get_short("Board",(int2*) &clip_board,1,18);
	      get_short("Channel",(int2*) &clip_channel,1,32);
	      get_real("Delay [V]",(float*) &del_val,0.,30.);
	      status = transfer(FUNC_CLIP_DEL ,clip_board,clip_channel-1,(unsigned short)(del_val*del_conv));
	      ClipData[clip_board-1].delay[clip_channel-1]=del_val;
	      printf("%f Delay on channel %i on board %i\n", ClipData[clip_board-1].delay[clip_channel-1], clip_channel,clip_board);
	      printf("");
	    }
	}
    
      if(strcmp(verb,"BDEL")==0)
	{
	  get_short("Board",(int2*) &clip_board,1,18);
	  get_real("Delay [V]",(float*) &del_val,0.,30.);
	  int i;
	  for (i=0;i<32;i++)
	    {
	      status = transfer(FUNC_CLIP_DEL ,clip_board,i ,(unsigned short)(del_val*del_conv));
	      ClipData[clip_board-1].delay[i]=del_val;
	    }
	  printf("%f Delay voltage on all channeles on board %i\n", del_val, clip_board);
	}
    
      if(strcmp(verb,"CDEL")==0)
	{
	  get_real("Delay [V]",(float*) &del_val,0.,30.);
            
	  int i,j;
	  for (j=0;j<18;j++)
	    {
	      for (i=0;i<32;i++)
		{
		  status = transfer(FUNC_CLIP_DEL ,j+1,i ,(unsigned short)(del_val*del_conv));
		  ClipData[j].delay[i]=del_val;
		}
	    }
	  printf("*********************************\n");
	  printf("%f Delay voltage on all the crate \n", del_val);
	  printf("*********************************\n");
	}


      /*
       * FixDelay
       */    
    
      if(strcmp(verb,"LFIXD")==0)
	{
	  printf("Load all CLIP-Board\n");
	  printf("Seting all CLIP-Board fixdelay on or off\n");
	  char str[1024]; 
	  str[0]='\0';
	  strcat((char *)str,"CHFIXD @");   //setdt
	  strcat((char *)str,"dummy_fixdelay.uic"); 
	  printf("executing : \n %s \n", str);
	  simulate_input((char *)str);
	}
    
      if(strcmp(verb,"SFIXD")==0)
	{
	  get_text("Enter Clip-Board fix delay Table name",(char *)ftablename);
	  printf("Save all CLIP-Board\n");
	}
    
      if(strcmp(verb,"BFIXD")==0)
	{
	  get_short("Board",(int2*) &clip_board,1,18);
	  get_short("0 = on, 1 = off",(int2*) &clip_fixD,0,1);
	  int i;
	  status = transfer(FUNC_CLIP_SWC ,clip_board,ALL_C,clip_fixD);
	  for (i=0;i<32;i++)
	    {
	      ClipData[clip_board-1].fixd[i]=clip_fixD;
	    }
	  printf("");
	  printf("%f  Fix Delay  on all channeles on board %i\n ", del_val, clip_board);
	}
    
      if(strcmp(verb,"CFIXD")==0)
	{
	  get_short("0 = on, 1 = off",(int2*) &clip_fixD,0,1);
	  int i,j;
	  for (j=0;j<18;j++)
	    {
	      status = transfer(FUNC_CLIP_SWC ,j+1,ALL_C,clip_fixD);
	      for (i=0;i<32;i++)
		{
		  ClipData[j].fixd[i]=clip_fixD;
		}
	    }
	  printf("*********************************\n");
	  printf("The Crate have fix delay %i\n",clip_fixD);
	  printf("*********************************\n");
	  printf("");
	}
    
      if(strcmp(verb,"CHFIXD")==0)
	{
	  get_short("Board",(int2*) &clip_board,1,18);
	  get_short("Channel",(int2*) &clip_channel,1,32);
	  get_short("0 = on, 1 = off",(int2*) &clip_fixD,0,1);
	  ClipData[clip_board-1].fixd[clip_channel-1]=clip_fixD;
	  status = transfer(FUNC_CLIP_SWC ,clip_board,clip_channel-1,clip_fixD);
	  printf("%i  on channel %i on board %i\n", 
		 ClipData[clip_board-1].fixd[clip_channel-1], clip_channel,clip_board);
	  printf("");
	}
    
      /*
       * On-Off functions
       */
      if(strcmp(verb,"BOFF")==0)
	{
	  get_short("Board",(int2*) &clip_board,1,18); 
	  status = transfer(FUNC_CLIP_DIS ,clip_board,ALL_C,0);
	  printf("All channels  on board %i are OFF\n",clip_board);
	  int i;
	  for(i=0;i<32;i++)
	    {
	      ClipData[clip_board-1].onoff[i]=0;
	    }
	}
    
    
      if(strcmp(verb,"COFF")==0)
	{
	  status = transfer(FUNC_CLIP_DIS ,ALL_BOARDS,ALL_C,0);
	  printf("*********************************\n");
	  printf("All channels are OFF\n");
	  printf("*********************************\n");
	  int j,i; 
	  for(j=1;j<19;j++)
	    {
	      for(i=0;i<32;i++)
		{
		  ClipData[j-1].onoff[i]=0;
		}
	    }
	}
    
      if(strcmp(verb,"CHOFF")==0)
	{
	  get_short("Board",(int2*) &clip_board,1,18);
	  get_short("Channel",(int2*) &clip_channel,1,32);
	  ClipData[clip_board-1].onoff[clip_channel-1]=0;
	  status = transfer(FUNC_CLIP_DIS ,clip_board,clip_channel-1,0);
	  printf("%i  on channel %i on board %i\n",ClipData[clip_board-1].onoff[clip_channel-1], clip_channel,0);
	  printf("");
	  printf("OFF\n");
	}
    
      if(strcmp(verb,"OFF_NN")==0)
	{
	  printf("Not programed yet\n");
	  printf("OFF\n");
	}
    
      if(strcmp(verb,"CRON")==0)
	{
	  status = transfer(FUNC_CLIP_DIS ,ALL_BOARDS,ALL_C,1);
	  printf("*********************************\n");
	  printf("All channels are ON \n");
	  printf("*********************************\n");
	  int i,j; 
	  for(j=1;j<19;j++)
	    {
	      for(i=0;i<32;i++)
		{
		  ClipData[j-1].onoff[i]=1;
		}
               
	    }
	}
    
      if(strcmp(verb,"BON")==0)
	{
	  get_short("Board",(int2*) &clip_board,1,18);
	  status = transfer(FUNC_CLIP_DIS ,clip_board,ALL_C,1);
	  printf("%i  on channel %i on board %i\n", 1, clip_channel,clip_board);
	  printf("");
	  printf("ON\n");
	  int i; 
	  for(i=0;i<32;i++)
	    {
	      ClipData[clip_board-1].onoff[i]=1;
	    }
	}
    
      if(strcmp(verb,"CHON")==0)
	{
	  get_short("Board",(int2*) &clip_board,1,18);
	  get_short("Channel",(int2*) &clip_channel,1,32);
	  status = transfer(FUNC_CLIP_DIS ,clip_board,clip_channel-1,1);
	  ClipData[clip_board-1].onoff[clip_channel-1]=1;
	  printf("%i  on channel %i on board %i\n", 1, clip_channel,clip_board);
	  printf("");
	  printf("ON\n");
	}


      /*
       * Temperature
       */
 
      if(strcmp(verb,"TEMPCPLD")==0)
	{
	  short data=0;
	  get_short("Board",(int2*) &clip_board,1,18);
	  status = transfer(FUNC_CHANGE_TEM ,clip_board,0,1);
	  status = transfer_t(FUNC_CLIP_TEM ,clip_board,0,&data);
	  ClipData[clip_board-1].TCPLD= data ;
	  printf("%i C on board %i\n", ClipData[clip_board-1].TCPLD,clip_board);
	}
		
      if(strcmp(verb,"TEMPAMP")==0)
	{
	  short data=0;
	  get_short("Board",(int2*) &clip_board,1,18);
	  status = transfer(FUNC_CHANGE_TEM ,clip_board,0,3);
	  status = transfer_t(FUNC_CLIP_TEM ,clip_board,0,&data);
	  ClipData[clip_board-1].TAMP= data ;
	  printf("%i C on board %i\n", ClipData[clip_board-1].TAMP,clip_board);
	}
		
      if(strcmp(verb,"ALLTEMP")==0)
	{
	  int j;
	  short data=0;
           
	  for(j=1;j<19;j++)
	    {
	      status = transfer(FUNC_CHANGE_TEM ,j,0,1);
	    }
		  
	  for(j=1;j<19;j++)
	    {
	      status = transfer_t(FUNC_CLIP_TEM ,j,0,&data);
	      ClipData[j-1].TCPLD= data ;
	      status = transfer(FUNC_CHANGE_TEM ,j,0,3);
	      printf("%i C on board %i\n", ClipData[j-1].TCPLD,j);
	    }
            
	  for(j=1;j<19;j++)
	    {
	      status = transfer_t(FUNC_CLIP_TEM ,j,0,&data);
	      ClipData[j-1].TAMP= data ;
	      printf("%i C next to amplifiers on board %i\n", ClipData[j-1].TAMP,j);
	    }
	  if(CalSet.TempFlag==1 )
	    {
	      FILE *Tfile;
	      Tfile= fopen ((char *)CalSet.outputname, "a" );     
	      fprintf(Tfile,"#BOARD    #TEMP [C]\n");
	      for(j=1;j<19;j++)
		{
		  status = transfer_t(FUNC_CLIP_TEM ,j,0,&data);
		  ClipData[j-1].TAMP= data ;
		  fprintf(Tfile,"%i     %i\n",j,ClipData[j-1].TAMP);
		}
	
	      fprintf(Tfile,"\n");
	      fclose ( Tfile );
	    }
	}



      /*
       * LUT
       */

      if(strcmp(verb,"LOADLUT")==0)
	{
	  printf("Load Attenuation-Delay LUT\n");
	  char str[1024]; 
	  str[0]='\0';
	  strcat((char *)str,"SETLUT @");   //setdt
	  strcat((char *)str,"LUT.uic"); 
	  printf("executing : \n %s \n", str);
	  simulate_input((char *)str);
	}

      //Added by Daisuke 
      if(strcmp(verb,"SETLUT")==0)
	{
	  int i;
	  for(i=0;i<N_LUT;i++)
	    {
	      DelayLUT[i].Bias = 0;
	      DelayLUT[i].Fixed = 0;
	      DelayLUT[i].Delay = 0;
	      DelayLUT[i].Attenuation = 0;
	    }
	  for(i=0;i<N_LUT;i++)
	    {
	      get_real("Bias", &DelayLUT[i].Bias,0.001,30);
	      get_short("fixed Delay", &DelayLUT[i].Fixed,0,1);
	      get_real("Delay",&DelayLUT[i].Delay,0,10);
	      get_real("Attenuation",&DelayLUT[i].Attenuation,0,32);
	      LUTLoaded = true;
	    }
	}

      //Added by Daisuke 
      if(strcmp(verb,"BIASTODELAY")==0)
	{
	  if(!LUTLoaded)
	    {
	      printf("Load LUT first!!!\n");
	    }
	  else
	    {
	      get_real("Bias Voltage [V]", &biasvoltage,0,30);
	      get_integer("Fixed Delay", &fixeddelay,0,1);

	      Bias2Delay(biasvoltage, fixeddelay, &delayvalue,&attenuationvalue);
	      printf("Delay = %f Attenuation= %f\n",delayvalue,attenuationvalue);
	    }
	}
    
      if(strcmp(verb,"DELAYTOBIAS")==0)
	{
	  if(!LUTLoaded)
	    {
	      printf("Load LUT first!!!\n");
	    }
	  else
	    {
	      get_real("Delay  [ns]", &delayvalue,0,10);
	      //get_short("Fixed Delay", &fixeddelay,0,1);

	      Delay2Bias(delayvalue, &fixeddelay, &biasvalue,&attenuationvalue);
	      if(fixeddelay >0)
		printf("Bias = %f fixed ON Attenuation= %f\n ",biasvalue,attenuationvalue);
	      else
		printf("Bias = %f fixed OFF Attenuation= %f\n ",biasvalue,attenuationvalue);

	    }
	}
      // till here added by daisuke

      if(strcmp(verb,"RETURN")==0)
	{       
	  CPLD_disconect();
	  return;
	}
    }
}
//added by Daisuke
void Delay2Bias(float Delay, int *fixed, float *Bias, float *Attenuation)
{

  int i = 0;
  bool Found = false;
  for(i=1;i<N_LUT;i++)
    {
      
      if(Delay <DelayLUT[i-1].Delay && Delay>=DelayLUT[i].Delay &&
	 DelayLUT[i-1].Fixed == DelayLUT[i].Fixed)
	{
	  float slope_bias = (DelayLUT[i].Bias - DelayLUT[i-1].Bias)/(DelayLUT[i].Delay - DelayLUT[i-1].Delay);
	  *Bias = slope_bias * (Delay  - DelayLUT[i-1].Delay)+DelayLUT[i-1].Bias;
	  float slope_att = (DelayLUT[i].Attenuation - DelayLUT[i-1].Attenuation)/(DelayLUT[i].Delay - DelayLUT[i-1].Delay);
	  *Attenuation = slope_att * (Delay  - DelayLUT[i-1].Delay)+DelayLUT[i-1].Attenuation;
	  *fixed = DelayLUT[i-1].Fixed;
	  Found = true;
	}
      //if(Found)break; //If the delay is in the overlapped reagion,  Always a value without fixed-delay is taken

    }

  if(!Found )
    {
      printf("Proper value not found : check something \n");
      printf("Delay %f Fixex %i \n",Delay, fixed);
    }

}
void Bias2Delay(float Bias, int fixed, float *Delay, float *Attenuation)
{

  int i = 0;
  bool Found = false;
  for(i=1;i<N_LUT;i++)
    {
      
      if(Bias >=DelayLUT[i-1].Bias && Bias<DelayLUT[i].Bias &&
	 fixed == DelayLUT[i-1].Fixed && fixed == DelayLUT[i].Fixed)
	{
	  float slope_delay = (DelayLUT[i].Delay - DelayLUT[i-1].Delay)/(DelayLUT[i].Bias - DelayLUT[i-1].Bias);
	  *Delay = slope_delay * (Bias  - DelayLUT[i-1].Bias)+DelayLUT[i-1].Delay;
	  float slope_att = (DelayLUT[i].Attenuation - DelayLUT[i-1].Attenuation)/(DelayLUT[i].Bias - DelayLUT[i-1].Bias);
	  *Attenuation = slope_att * (Bias  - DelayLUT[i-1].Bias)+DelayLUT[i-1].Attenuation;
	  Found = true;
	}
      if(Found)break;
    }

  if(!Found )
    {
      printf("Proper value not found : check something\n");
      printf("Delay %f Fixex %i \n",Bias, fixed);
    }

}
