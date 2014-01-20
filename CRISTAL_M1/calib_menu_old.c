/*******************************************************************
 * Calibration  menu
 * date: November 2013
 * by: jrg
 ********************************************************************/

#include "CristalSpi.h" 
void BiastoDelay(float Bias, int fixed, float *Delay, float *Attenuation);
void DelaytoBias(float Delay, int *fixed, float *Bias, float *Attenuation);
void calib_menu(void)
{
  char *verb;
  int cal_rate=25;
  int4 iverb;
  float biasvoltage = 0;    
  int fixeddelay = 0;
  float delayvalue=0; 
  float biasvalue=0;
  float attenuationvalue=0;
  
  short channel, layer;
  //    sprintf((char *)CalSet.outputname,"DEFAULT"); 
  while(True)
    {
      verb = action_menu( "CALIBRATION_MENU", &iverb);
      /* 
       * Threshold Scan     
       */	
      
      if(strcmp(verb,"THRESHOLDSCAN")==0) 
        {  
	  FILE *Tfile;
	  float threshold[57]={-999};
	  float fthreshold[101][55]={{-999}};
	  int rate[101][55]={{-999}};//thresold-macrocell
	  int frate[101][55]={{-999}};//thresold-macrocell
	  int kchange[57]={-999};//thresold-macrocell
	  int kchange2[57]={-999};//thresold-macrocell
	  float gstep=0.2;
	  float fstep= 0.03;
	  float tvalue;
	  float ftvalue[57]={-999};
	  int data=0,data1=0,data2=0,Macrocell=0;
	  int i,j,k,m;
	  int changeflag1=0;
	  int changeflag2=0;
	  int chanflag1[55]={0};
	  int chanflag2[55]={0};
	  // for (k=0;k<41;k++)//start at 0
	  for(i=0;i<57;i++)
            {
	      kchange2[i]=-999;
	      kchange[i]=-999;
	      ftvalue[i]=-2;//initialice the thresold in a value out of scan, but accepted by Sum-Board
	      threshold[i]=-2;//
            }
	  
	  for(j=0;j<55;j++)
	    {
	      for(i=0;i<101;i++)
		{
		  rate[i][j]=0;//thresold-macrocell
		  frate[i][j]=0;//thresold-macrocell
		}
	    }
	  
	  for (k=0;k<9;k++)
            {
	      if (changeflag1!=55)
                {
		  tvalue=gstep*k+0.2;//start at 0
		  // tvalue=gstep*k-2;
		  threshold[k]=tvalue;
		  data=(int)((tvalue +2.0)*thre_conv);
		  for (j=0;j<19;j++)
                    {
		      for (i=0;i<3;i++)
                        {
			  transfer(FUNC_SUM_THR ,j,i,data);//set thresold
                        }
                    }
		  transfer(FUNC_ASTRO_RES,0,0,0); //start counting  
		  usleep(1000*ASData.Tmax);
                  
		  for (j=1;j<56;j++)
                    {
		      Macrocell=j;
		      transfer_c(FUNC_ASTRO_AMP,&data1,&data2,&Macrocell);   
		      rate[k][j-1]=Macrocell;
		      if (rate[k][j-1]<cal_rate && kchange[j-1]<-99) 
			{
			  kchange[j-1]=k-1;
			  if(kchange[j-1]<0) kchange[j-1]=0;
			}
                    }
		  
		  Tfile= fopen ((char *)CalSet.outputname, "a" );     
		  fprintf(Tfile,"#Threshold ");    
		  fprintf(Tfile,"%f    ", tvalue);    
		  printf("%f [V] Threshold \n", tvalue);
		  for (j=1;j<56;j++)
                    {
		      printf("Macrocel %i, rate %i [Hz]\n", j,rate[k][j-1]);
		      //fprintf(Tfile, "Macrocel %i, rate %i [Hz]\n", j,rate[k][j-1]);
		      //fprintf(Tfile, "1st loop kchange %i \n", kchange[j-1]);
		      fprintf(Tfile,"%i    ", rate[k][j-1]);
                    }
		  fprintf(Tfile,"\n");
		  fclose ( Tfile );
		  changeflag1=0;

		  for (j=1;j<56;j++)
                    {
		      if(rate[k][j-1]<10) chanflag1[j-1]=1;
		      changeflag1+=chanflag1[j-1];
                    }
		  
                }   
            }   
	  
	  for (k=0;k<22;k++)
            {
	      // if (changeflag2!=55)
	      // {
	      for (j=0;j<19;j++)
		{
		  for (i=0;i<3;i++)
		    {
		      m=j*3+i;
		      //ftvalue[m]=gstep*kchange[m]-2+k*fstep;
		      ftvalue[m]=gstep*kchange[m]+0.2+k*fstep;
		      
		      fthreshold[k][m]=ftvalue[m];
		      data=(int)((ftvalue[m] +2.0)*thre_conv);
		      transfer(FUNC_SUM_THR ,j,i,data);//set thresold
		    }
		}
	      transfer(FUNC_ASTRO_RES,0,0,0); //start counting  
	      usleep(1000*ASData.Tmax);
	      changeflag2=0;
              
	      for (j=1;j<56;j++)
		{
		  Macrocell=j;
		  transfer_c(FUNC_ASTRO_AMP,&data1,&data2,&Macrocell);   
		  frate[k][j-1]=Macrocell;
		  if(frate[k][j-1]<1) chanflag2[j-1]=1;
		  if(frate[k][j-1]<1) kchange2[j-1]=1;
		  changeflag2+=chanflag2[j-1];
		}   
	      
	      Tfile= fopen ((char *)CalSet.outputname, "a" );     
	      fprintf(Tfile,"#Macrocel    #Threshold    #Rate[Hz]\n");
	      for (j=1;j<56;j++)
		{
		  printf("%f [V] Threshold   ", ftvalue[j-1]);
		  printf("Macrocel %i, rate %i [Hz]\n", j,frate[k][j-1]);
		  //fprintf(Tfile, "2nd loop kchange %i \n", kchange[j-1]);
		  fprintf(Tfile,"%i    ", j);    
		  fprintf(Tfile,"%f    ", ftvalue[j-1]);    
		  fprintf(Tfile,"%i    \n", frate[k][j-1]);
		}
	      fprintf(Tfile,"\n");
	      fclose ( Tfile );
              
	      
	      // }	
            }
	  Tfile= fopen ((char *)CalSet.outputname, "a" );     
          
	  fprintf(Tfile,"#Macrocel     #Threshold \n");
	  for (j=1;j<56;j++)
	    {
	      fprintf(Tfile,"%i    %f \n", j,gstep*kchange[j-1]+0.2+kchange2[j-1]*fstep);
	    }
	  fprintf(Tfile,"\n");
	  fclose ( Tfile );
        }
      
      /* 
       * Attenuation Scan     
       */	
      if(strcmp(verb,"ATTENUATIONSCAN")==0) 
        {  
	  FILE *Tfile;
	  float att_val=0;
	  float attenuation[65]={-999};
	  int rate[65][55]={{-999}};//att-macrocell
	  float step=0.5;//dB
	  int data=0,data1=0,data2=0,Macrocell=0;
	  int i,j,k,m;
	  for (k=0;k<64;k++)
            {
	      att_val=step*k;
	      attenuation[k]=att_val;
              
	      CPLD_conect();
	      for (i=0;i<32;i++)
		{
		  transfer(FUNC_CLIP_ATT ,ALL_BOARDS,i, (unsigned short)(att_val*att_conv));
		}
	      CPLD_disconect();
	      transfer(FUNC_ASTRO_RES,0,0,0); //start counting  
	      usleep(1000*ASData.Tmax);
              
	      for (j=1;j<56;j++)
                {
		  Macrocell=j;
		  transfer_c(FUNC_ASTRO_AMP,&data1,&data2,&Macrocell);   
		  rate[k][j-1]=Macrocell;
                }
	      Tfile= fopen ((char *)CalSet.outputname, "a" );     
	      printf("%f [dB] Attenuation \n", attenuation[k]);
	      fprintf(Tfile,"%f    ", attenuation[k]);    
	      for (j=1;j<56;j++)
                {
		  printf("Macrocel %i, rate %i [Hz]\n", j,rate[k][j-1]);
		  fprintf(Tfile,"%i    ", rate[k][j-1]);
                }
	      fprintf(Tfile,"\n");
	      fclose ( Tfile );
            }
        }
      
      
      /* 
       * Delay Scan     
       */	
      
      
      if(strcmp(verb,"FINEDELAYSCAN")==0)
	{
	  FILE *Tfile;
	  int data_delay=0;
	  float initialatt[57];
	  float att[57];
	  float delay[57]={-999};
	  float fdelay[101][55]={{-999}};
	  int rate[101][55]={{-999}};//thresold-macrocell                                             
	  int frate[101][55]={{-999}};//thresold-macrocell                                            
	  int kchange[57]={-999};//thresold-macrocell                                                 
	  int kchange2[57]={-999};//thresold-macrocell                                                
	  float gstep=0.5;  // 0.5ns steps over 6.5ns
	  float fstep= 0.1; // 100ps steps over 1.5ns
	  float dvalue;
	  float fdvalue[57]={-999};
	  int data=0,data1=0,data2=0,Macrocell=0;
	  int i,j,k,m;
	  int changeflag1=0;
	  int changeflag2=0;
	  int chanflag1[55]={0};
	  int chanflag2[55]={0};
	  
	  for(i=0;i<57;i++)
	    {
	      kchange2[i]=-999;
	      kchange[i]=-999;
	      fdvalue[i]=-999;
	      delay[i]=-999;//                                                                        
	      initialatt[57]=0;
	    }
	  
	  for(j=0;j<55;j++)
	    {
	      for(i=0;i<101;i++)
		{
		  rate[i][j]=0;//delay-macrocell                                                      
		  frate[i][j]=0;//delay-macrocell                                         
		}
	      
	    }
	  
	  //Check the inital gain values                                                          
	  for(j=0;j<MC_N;j++)
	    {
	      for(i=0;i<PX_N;i++)
		{
		  if(CalSet.Pixel[j]== MAP[i].PIXEL)
		    initialatt[j]=ClipData[CalSet.Lboard[j]-1].att[CalSet.Lchannel[j]-1];
		}
	    }
	  
	  
	  //coarse step

	  for (k=0;k<14;k++)
	    {
	      if (changeflag1!=55)
		{
		  dvalue=gstep*k+0.01;//start at 0         
		  delay[k]=dvalue;
		  DelaytoBias(dvalue,&fixeddelay, &biasvalue,&attenuationvalue);
		  data_delay=(int)(biasvalue *del_conv);
		  fixeddelay=abs(fixeddelay-1);
		  printf("***************DBG**********************\n");
		  printf("Data value %f\n",dvalue);
		  printf("Data bias %f\n",biasvalue);
		  printf("Data delay %i\n",data_delay);
		  printf("***************DBG**********************\n");
		 
		  
		  for(j=0;j<MC_N;j++)
		    {
		      if (CalSet.Pixel[j]!=-999)
			{
			  att[j]=(initialatt[j]-8+attenuationvalue);
			  
			  transfer(FUNC_CLIP_DEL ,CalSet.Lboard[j],
				   CalSet.Lchannel[j]-1,data_delay);
			  transfer(FUNC_CLIP_SWC ,CalSet.Lboard[j],
				   CalSet.Lchannel[j]-1,fixeddelay);
			  transfer(FUNC_CLIP_ATT ,CalSet.Lboard[j],
				   CalSet.Lchannel[j]-1,(int)(att[j]*att_conv));
			  
			  printf("***************DBG1**********************\n");
			  printf("Pixel %i\n",CalSet.Pixel[j]);
			  printf("board %i\n",CalSet.Lboard[j]);
			  printf("channel %i\n",CalSet.Lchannel[j]);
			  printf("***************DBG1**********************\n");
			}
		    }
		  
		 
		  transfer(FUNC_ASTRO_RES,0,0,0); //start counting                                    
		  usleep(1000*ASData.Tmax);
		  
		  for (j=1;j<56;j++)
		    {
		      Macrocell=j;
		      transfer_c(FUNC_ASTRO_TIM, &data1, &data2, &Macrocell);
		      rate[k][j-1]=Macrocell;
		      // find point of 50% rate and go 1 ns to the left
		      if (rate[k][j-1]<0.5*cal_rate && kchange[j-1]<-99) 
			{
			  kchange[j-1]=k-2;
			  if(kchange[j-1]<0) kchange[j-1]=0;
			}
		      
		    }
		  
		  Tfile= fopen ((char *)CalSet.outputname, "a" );
		  fprintf(Tfile,"#Delay ");
		  fprintf(Tfile,"%f    ", dvalue);
		  printf("%f [ns] Delay \n", dvalue);
		  for (j=1;j<56;j++)
		    {
		      printf("Macrocel %i, rate %i [Hz]\n", j,rate[k][j-1]);
		      //fprintf(Tfile, "Macrocel %i, rate %i [Hz]\n", j,rate[k][j-1]);                
		      //fprintf(Tfile, "1st loop kchange %i \n", kchange[j-1]);                       
		      fprintf(Tfile,"%i    ", rate[k][j-1]);
		    }
		  fprintf(Tfile,"\n");
		  fclose ( Tfile );
		  changeflag1=0;
		  
                  for (j=1;j<56;j++)
                    {
                      if(rate[k][j-1]<10) chanflag1[j-1]=1;
                      changeflag1+=chanflag1[j-1];
                    }
  
		}
	      
	    }

	  /*2nd loop*/
	  
	  for (k=0;k<15;k++)                                                               
            {                                                                          
	      
	      for (j=0;j<55;j++)                    
		{                                                                       
		  if (CalSet.Pixel[j]!=-999)                                    
		    {                   
		      // if 50% outside window set maximum
		      if(kchange[j]<0) kchange[j]=11;
		      fdvalue[j]=gstep*kchange[j]+k*fstep;                             
          
		      printf("Delay: %f\n", fdvalue[j]);
		    }
		  else 
		    fdvalue[j]=k*fstep;
		      
		  if(fdvalue[j]<0) fdvalue[j]=0;
		  if(fdvalue[j]>6.9) fdvalue[j]=6.9; //ns                                         
 


		  fdelay[k][j]=fdvalue[j];                                        
		  DelaytoBias(fdvalue[j], &fixeddelay, 
			      &biasvalue,&attenuationvalue);
		  
		  
		  data_delay=(int)(biasvalue *del_conv);                       
		  att[j]=initialatt[j]-9.07 + attenuationvalue;           
		  


		  transfer(FUNC_CLIP_DEL ,CalSet.Lboard[j],
			   CalSet.Lchannel[j]-1,data_delay);
		  transfer(FUNC_CLIP_SWC ,CalSet.Lboard[j],
			   CalSet.Lchannel[j]-1,fixeddelay);
		  transfer(FUNC_CLIP_ATT ,CalSet.Lboard[j],
			   CalSet.Lchannel[j]-1,(int)(att[j]*att_conv));


		 		  
		}    
	      
	      
	      transfer(FUNC_ASTRO_RES,0,0,0); //start counting                  
	      usleep(1000*ASData.Tmax);                                
	      changeflag2=0;                             
	      
	      for (j=1;j<56;j++)                                     
		{                                                                       
		  Macrocell=j;                           
		  transfer_c(FUNC_ASTRO_TIM, &data1, &data2, &Macrocell);
		  rate[k][j-1]=Macrocell;
   
		  if(frate[k][j-1]<1) kchange2[j-1]=1;                     
		}                                    
	      
	      Tfile= fopen ((char *)CalSet.outputname, "a" );               
	      
	      fprintf(Tfile,"#Macrocel    #Delay[s]    #Rate[Hz]\n");           
	      for (j=1;j<56;j++)                                    
		{                                                                       
		  printf("%f [s] Delay   ", fdvalue[j-1]);                   
		  printf("Macrocel %i, rate %i [Hz]\n", j,frate[k][j-1]);       
		  fprintf(Tfile,"%i    ", j);                                     
		  fprintf(Tfile,"%f    ", fdvalue[j-1]);                              
		  fprintf(Tfile,"%i    \n", frate[k][j-1]);                    
		}     

	      fclose(Tfile);
	      
	      /*
	      Tfile= fopen ((char *)CalSet.outputname, "a" );                                      
	      
	      fprintf(Tfile,"#Macrocel     #Delay \n");                                       
	      for (j=1;j<56;j++)                                              
		{                                                            
		  fprintf(Tfile,"%i    %f \n", 
			  j,gstep*kchange[j-1]+kchange2[j-1]*fstep); 
		}                                         
	      
	      fprintf(Tfile,"\n");
	      */
	    }
	  
	}  
	  
	 	    
	  
      if(strcmp(verb,"DELAYSCAN")==0) 
	{  
	  FILE *Tfile;
	  int data_delay=0;
	  float initialatt[57];
	  float att[57];
	  float delay[57]={-999};
	  float fdelay[101][55]={{-999}};
	  int rate[101][55]={{-999}};//thresold-macrocell
	  int frate[101][55]={{-999}};//thresold-macrocell
	  int kchange[57]={-999};//thresold-macrocell
	  int kchange2[57]={-999};//thresold-macrocell
	  float gstep=0.25;
	  float fstep= 0.05;
	  float dvalue;
	  float fdvalue[57]={-999};
	  int data=0,data1=0,data2=0,Macrocell=0;
	  int i,j,k,m;
	  int changeflag1=0;
	  int changeflag2=0;
	  int chanflag1[55]={0};
	  int chanflag2[55]={0};
	  for(i=0;i<57;i++)
	    {
	      kchange2[i]=-999;
	      kchange[i]=-999;
	      fdvalue[i]=-999;
	      delay[i]=-999;//
	      initialatt[57]=0;
	    }
	  
	  for(j=0;j<55;j++)
	    {
	      for(i=0;i<101;i++)
		{
		  rate[i][j]=0;//delay-macrocell
		  frate[i][j]=0;//delay-macrocell
		}
	      
	    }
	  
	  //Check the inital gain values
	  for(j=0;j<MC_N;j++)
	    {
	      for(i=0;i<PX_N;i++)
		{
		  if(CalSet.Pixel[j]== MAP[i].PIXEL) 
		    initialatt[j]=ClipData[CalSet.Lboard[j]-1].
		      att[CalSet.Lchannel[j]-1];
		}
	    }
	  
	  
	  
	  for (k=0;k<28;k++)
	    {
	      if (changeflag1!=55)
		{
		  dvalue=gstep*k+0.01;//start at 0
		  delay[k]=dvalue;
		  DelaytoBias(dvalue,&fixeddelay, &biasvalue,&attenuationvalue);
		  data_delay=(int)(biasvalue *del_conv);
		  fixeddelay=abs(fixeddelay-1);
		  printf("***************DBG**********************\n");	
		  printf("Data value %f\n",dvalue);	
		  printf("Data bias %f\n",biasvalue);	
		  printf("Data delay %i\n",data_delay);	
		  printf("***************DBG**********************\n");	
		  CPLD_conect();
		  for(j=0;j<MC_N;j++)
		    {
		      if (CalSet.Pixel[j]!=-999)
			{
			  att[j]=(initialatt[j]-9.07 + attenuationvalue);
			  
			  transfer(FUNC_CLIP_DEL ,CalSet.Lboard[j],
				   CalSet.Lchannel[j]-1,data_delay);
			  transfer(FUNC_CLIP_SWC ,CalSet.Lboard[j],
				   CalSet.Lchannel[j]-1,fixeddelay);
			  transfer(FUNC_CLIP_ATT ,CalSet.Lboard[j],
				   CalSet.Lchannel[j]-1,(int)(att[j]*att_conv));
			  
			  printf("***************DBG1**********************\n");	
			  printf("Pixel %i\n",CalSet.Pixel[j]);	
			  printf("board %i\n",CalSet.Lboard[j]);	
			  printf("channel %i\n",CalSet.Lchannel[j]);	
			  printf("***************DBG1**********************\n");	
			}
		    }
		  
		  CPLD_disconect();
		  transfer(FUNC_ASTRO_RES,0,0,0); //start counting  
		  usleep(1000*ASData.Tmax);
		  
		  for (j=1;j<56;j++)
		    {
		      Macrocell=j;
		      transfer_c(FUNC_ASTRO_TIM,&data1,&data2,&Macrocell);   
		      rate[k][j-1]=Macrocell;
		      if (rate[k][j-1]<cal_rate && kchange[j-1]<-99) kchange[j-1]=k-1;
		      
		    }
		  
		  Tfile= fopen ((char *)CalSet.outputname, "a" );     
		  fprintf(Tfile,"#Delay ");    
		  fprintf(Tfile,"%f    ", dvalue);    
		  printf("%f [ns] Delay \n", dvalue);
		  
		  for (j=1;j<56;j++)
		    {
		      printf("Macrocel %i, rate %i [Hz]\n", j,rate[k][j-1]);
		      //fprintf(Tfile, "Macrocel %i, rate %i [Hz]\n", j,rate[k][j-1]);
		      //fprintf(Tfile, "1st loop kchange %i \n", kchange[j-1]);
		      fprintf(Tfile,"%i    ", rate[k][j-1]);
		    }
		  
		  
		  fprintf(Tfile,"\n");
		  fclose ( Tfile );
		  changeflag1=0;
		  
		}
	      
	      
	    }   
	  
	}
      
      /* 
       * OUTPUT     
       */	
      
      
      
      if(strcmp(verb,"REFDELAYSCAN")==0) // scanning the reference delay                               
	{
	  FILE *Tfile;
	  float startdelay=3.5;
	  int data_delay=0;
	  int rate[101][55]={{-999}};//thresold-macrocell	\
	  
	  int frate[101][55]={{-999}};//thresold-macrocell                                             
	  float gstep=0.143; //+-2ns devided in 28 steps                                               
	  
	  float dvalue;
	  float fdvalue[57]={-999};
	  int data=0,data1=0,data2=0,Macrocell=0;
	  int i,j,k,m;
	  int changeflag1=0;
	  int changeflag2=0;
	  int chanflag1[55]={0};
	  int chanflag2[55]={0};
	  
	  
	  //      get_real("Scan around t0 [ns]",&startdelay, 2.0, 4.6);                               
	  
	  for (k=0;k<28;k++)
	    {
	      if (changeflag1!=55)
		{
		  dvalue=startdelay-2.0+gstep*k;//start at startdelay-2.0 \
		  
		  //      delay[k]=dvalue;                                                             
		  data_delay=(int)((startdelay + 2.0 - gstep*k)*ddel_conv);
		  printf("***************DBG**********************\n");
		  printf("Data value %f\n",dvalue);
		  printf("Data delay %i\n",data_delay);
		  
		  printf("***************DBG**********************\n");
		  //CPLD_conect();                                                                     
		  
		  transfer(FUNC_ASTRO_ADEL, 1, 0, (int)(data_delay));
		  
		  
		  //CPLD_disconect();                                                                  
		  transfer(FUNC_ASTRO_RES,0,0,0); //start counting	\
                  
		  usleep(1000*ASData.Tmax);
		  
		  for (j=1;j<56;j++)
		    {
       
		      Macrocell=j;
		      transfer_c(FUNC_ASTRO_TIM,&data1,&data2,&Macrocell);
		      rate[k][j-1]=Macrocell;
		    }
		  
		  Tfile= fopen ((char *)CalSet.outputname, "a" );
		  fprintf(Tfile,"#Delay ");
		  fprintf(Tfile,"%f    ", dvalue);
		  printf("%f [ns] Delay \n", dvalue);
		  for (j=1;j<56;j++)
		    {
		      printf("Macrocel %i, rate %i [Hz]\n", j,rate[k][j-1]);
		      fprintf(Tfile,"%i    ", rate[k][j-1]);
		      
		    }
		  fprintf(Tfile,"\n");
		  fclose ( Tfile );
		  changeflag1=0;
		  
		}
	    }
	}
      
      
      
      
      if(strcmp(verb,"OUTPUTNAME")==0) 
	{                                                
	  get_text("Enter output file tname",(char*)CalSet.outputname);
	  printf("Output file name defined: %s\n", CalSet.outputname); 
	}
      
      if(strcmp(verb,"OUTPUTFOLDER")==0) 
	{                                                
	  get_text("Enter output folder name",(char*)CalSet.outputfolder);
	  printf("Output file name defined: %s\n", CalSet.outputfolder); 
	}
      
      
      /* 
       * Turn on channel x on all macro of Layout y 
       */	
      
      
      if(strcmp(verb,"CHANNEL_LAYOUT")==0)
	{
	  short clip_channel;
	  FILE *Tfile;
	  int i,j,k;
	  int NumLines = 559;
	  for(i=0;i<55;i++)
	    {
	      CalSet.Lmacro[i]= -999;       
	      CalSet.Lboard[i]= -999;	
	      CalSet.Lchannel[i]= -999;
	      CalSet.Pixel[i]= -999;
	    }
	  get_short("Enter Layout number",(int2*) &layer,1,3);
	  get_short("Enter the channel of the Macrocell",(int2*) &channel,1,19);
	  Tfile= fopen ((char *)CalSet.outputname, "a" );     
	  fprintf(Tfile,"#PIXEL  #BOARD  #CHANNEL  #MACROCELL L%i  #ALL MACROCELLS  #ATTENUATION\n",
		  layer); 
	  CPLD_conect();
	  
	  for (i=0;i<NumLines;i++)
	    {  
	      for (j=0;j<3;j++)
		{    
		  if (MAP[i].S_SUM_LAYER[j]==layer && MAP[i].S_SUM_CHANNEL[j]==channel)
		    {
		      clip_channel=(MAP[i].S_CLIP_CONNECTOR[j]-1)*8+MAP[i].S_CLIP_CHANNEL[j];
		      k=MAP[i].S_SUM_MACROCELL[j] -1;
		      
		      CalSet.Lmacro[k]= k +1;       
		      CalSet.Lboard[k]= MAP[i].CLIP_SLOT;	
		      CalSet.Lchannel[k]= clip_channel;
		      CalSet.Pixel[k]= MAP[i].PIXEL;
		      
		      transfer(FUNC_CLIP_DIS , MAP[i].CLIP_SLOT,clip_channel-1, 1);
		      
		      printf("Pixel %i in Macrocell %i    ",MAP[i].PIXEL,MAP[i].S_SUM_MACROCELL[j]);
		      printf("Board %i   ",MAP[i].CLIP_SLOT); 
		      printf("Connector in board %i   ",MAP[i].S_CLIP_CONNECTOR[j]); 
		      printf("Channel in connector %i\n",MAP[i].S_CLIP_CHANNEL[j]); 
		      
		      fprintf(Tfile,"%i        %i        ", MAP[i].PIXEL, MAP[i].CLIP_SLOT); 
		      fprintf(Tfile,"%i        %i        ", clip_channel, MAP[i].S_SUM_MACROCELL[j]); 
		      fprintf(Tfile,"%i,%i,%i        ", MAP[i].S_SUM_MACROCELL[0], 
			      MAP[i].S_SUM_MACROCELL[1], MAP[i].S_SUM_MACROCELL[2]);
		      fprintf(Tfile,"%f\n", ClipData[CalSet.Lboard[k]-1].att[clip_channel-1]); 
		      
		      
		    }
		}
	    }
	  CPLD_disconect();
	  fclose ( Tfile );
	}
      if(strcmp(verb,"RETURN")==0)
	return;
      
    }
}
//added by Daisuke
void DelaytoBias(float Delay, int *fixed, float *Bias, float *Attenuation)
{
  
  int i = 0;
  bool Found = false;
  
  
  if(Delay<0) Delay=0;
  if(Delay>6.9) Delay=6.9;
  
  
  for(i=1;i<N_LUT;i++)
    {
      
      if(Delay <DelayLUT[i-1].Delay && Delay>=DelayLUT[i].Delay &&
	 DelayLUT[i-1].Fixed == DelayLUT[i].Fixed)
	{
	  float slope_bias = (DelayLUT[i].Bias - DelayLUT[i-1].Bias)/
	    (DelayLUT[i].Delay - DelayLUT[i-1].Delay);
	  *Bias = slope_bias * (Delay  - DelayLUT[i-1].Delay)+DelayLUT[i-1].Bias;
	  float slope_att = (DelayLUT[i].Attenuation - DelayLUT[i-1].Attenuation)/
	    (DelayLUT[i].Delay - DelayLUT[i-1].Delay);
	  *Attenuation = slope_att * (Delay  - DelayLUT[i-1].Delay)+DelayLUT[i-1].Attenuation;
	  *fixed = DelayLUT[i-1].Fixed;
	  Found = true;
	}

      //if(Found)break; //If the delay is in the overlapped reagion,  
      //Always a value without fixed-delay is taken

    }

  if(!Found )
    {
      printf("Proper value not found : check something \n");
      printf("Delay %f Fixex %i \n",Delay, fixed);
    }

  
}


void BiastoDelay(float Bias, int fixed, float *Delay, float *Attenuation)
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
}
