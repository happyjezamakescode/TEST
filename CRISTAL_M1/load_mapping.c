/*******************************************************************
 * Mapping menu
 * date: October 2013
 * by: jrg
 * Modified by: D. Nakajima (add layer information)
 ********************************************************************/
void mapping_menu()
{
  short pixel=0;
  char *verb;
  int4 iverb;
  
  int ii,ij;
  for (ii=0;ii<559;ii++)
    {    
      MAP[ii].CLIP_POSITIVE=(char*)malloc(10);
      MAP[ii].CLIP_NEGATIVE=(char*)malloc(10);
      
      for (ij=0;ij<3;ij++)
        {    
	  MAP[ii].S_CLIP_POSITIVE[ij]=(char*)malloc(10);
	  MAP[ii].S_CLIP_NEGATIVE[ij]=(char*)malloc(10);
	  MAP[ii].S_SUM_NEGATIVE[ij]=(char*)malloc(10);
	  MAP[ii].S_SUM_POSITIVE[ij]=(char*)malloc(10);
        }
    }
                    
  
  while(True)
    {
      verb = action_menu( "MAPPING_MENU", &iverb);
      
      if(strcmp(verb,"SETMAP")==0)
	{
	  int i,j;
	  int NumLines = 559;

	  for (i=0;i<NumLines;i++)
            {  
	      //	      printf(" DBG %i\n",i);
	      get_short("PIXEL",&MAP[i].PIXEL,0,750);
	      //printf("Pixel = %i ", MAP[i].PIXEL);
	      get_short("CONNECTOR",&(MAP[i].RECEIVER_CONNECTOR),0,70);
	      //	      printf(" DBG2%i\n",MAP[i].RECEIVER_CONNECTOR);
	      get_short("CHANNEL",&MAP[i].RECEIVER_CHANNEL,0,8);
	      //	      printf(" DBG3%i\n",i);
	      get_short("SLOT",&MAP[i].CLIP_SLOT,0,18);
	      //	      printf(" DBG4%i\n",i);
	      get_short("CONNECTOR",&MAP[i].CLIP_CONNECTOR,0,18);
	      //	      printf(" DBG5%i\n",i);
	      get_text("POSITIVE SIGNAL",(char *)MAP[i].CLIP_POSITIVE);
	      //	      printf(" DBG6%i\n",i);
	      get_text("NEGATIVE SIGNAL",(char *)MAP[i].CLIP_NEGATIVE);
	      //	      printf(" DBG7%i\n",i);
	      get_short("ERNI REAR",&MAP[i].CLIP_ERNI_REAR,0,4);
	      //	      printf(" DBG8%i\n",i);
	      for (j=0;j<3;j++)
                {    
		  get_short("ERNI ",&MAP[i].S_CLIP_ERNI[j],0,72);
		  //		  printf(" DBG1%i ,%i\n",i, j);
		  get_short("CHANNEL",&MAP[i].S_CLIP_CHANNEL[j],0,8);
		  //		  printf(" DBG2%i ,%i\n",i, j);
		  get_short("CONNECTOR",&MAP[i].S_CLIP_CONNECTOR[j],0,100);
		  //		  printf(" DBG3%i ,%i\n",i, j);
		  get_text("POSITIVE SIGNAL",(char *)MAP[i].S_CLIP_POSITIVE[j]);
		  //		  printf(" DBG4%i ,%i\n",i, j);
		  get_text("NEGATIVE SIGNAL",(char *)MAP[i].S_CLIP_NEGATIVE[j]);
		  //		  printf(" DBG5%i ,%i\n",i, j);
		  get_short("ERNI ",&MAP[i].S_SUM_ERNI[j],0,72);
		  //		  printf(" DBG6%i ,%i\n",i, j);
		  get_text("POSITIVE SIGNAL",(char *)MAP[i].S_SUM_POSITIVE[j]);
		  //		  printf(" DBG7%i ,%i\n",i, j);
		  get_text("NEGATIVE SIGNAL",(char *)MAP[i].S_SUM_NEGATIVE[j]);
		  //		  printf(" DBG8%i ,%i\n",i, j);
		  get_short("MACROCELL",&MAP[i].S_SUM_MACROCELL[j],0,55);
		  //		  printf(" DBG9%i ,%i\n",i, j);
		  get_short("CHANNEL",&MAP[i].S_SUM_CHANNEL[j],0,19);
		  //		  printf(" DBG10%i ,%i %i\n",i, j,MAP[i].S_SUM_CHANNEL[j]);
		  MAP[i].S_SUM_LAYER[j] = GetLayer(MAP[i].S_SUM_MACROCELL[j]);
                }
	      //	      printf(" DBG finis i loop\n");
	      if(i%10==0 && i>1)
		printf("pixel = %i : %i\n", MAP[i].PIXEL,i); 
	      else
		printf(".");
            }
	  //	  printf(" DBG finis j loop\n");
	}

      if(strcmp(verb,"PIXELINFO")==0)
	{
	  get_short("PIXEL",&pixel,1,716);
	  int i=0;
	  int flag=0;
	  int iflag[559];
	  int pixel_temp = pixel;
	  for(i=0;i<559;i++)
	    {
	      iflag[i]=0;
	      if (pixel_temp==MAP[i].PIXEL)
		{   
		  pixel=i;
		  iflag[i]=1;
		  printf("      ********************************        \n");
		  printf("      ******Pixel Info # %i********        \n",pixel_temp);
		  printf("      ********************************        \n");
		  printf("%i \n",i);
		  printf("CONNECTOR %i\n",MAP[pixel].RECEIVER_CONNECTOR);
		  printf("CHANNEL %i\n",MAP[pixel].RECEIVER_CHANNEL);
		  printf("SLOT %i\n",MAP[pixel].CLIP_SLOT);
		  printf("CONNECTOR %i\n",MAP[pixel].CLIP_CONNECTOR);
		  printf("POSITIVE SIGNAL %s\n",MAP[pixel].CLIP_POSITIVE);
		  printf("NEGATIVE SIGNAL %s\n",MAP[pixel].CLIP_NEGATIVE);
		  printf("ERNI REAR %i\n",MAP[pixel].CLIP_ERNI_REAR);
                
		  int j;
		  for (j=0;j<3;j++)
		    {  
		      if(MAP[pixel].S_SUM_LAYER[j]>=999)continue;
		      printf("----------------------------------------\n");
		      printf("FANOUT %i\n",j+1);
		      printf("ERNI %i\n",MAP[pixel].S_CLIP_ERNI[j]);
		      printf("CHANNEL %i\n",MAP[pixel].S_CLIP_CHANNEL[j]);
		      printf("CONNECTOR %i\n",MAP[pixel].S_CLIP_CONNECTOR[j]);
		      printf("POSITIVE SIGNAL %s\n",MAP[pixel].S_CLIP_POSITIVE[j]);
		      printf("NEGATIVE SIGNAL %s\n",MAP[pixel].S_CLIP_NEGATIVE[j]);
		      printf("ERNI %i\n",MAP[pixel].S_SUM_ERNI[j]);
		      printf("POSITIVE SIGNAL %s\n",MAP[pixel].S_SUM_POSITIVE[j]);
		      printf("NEGATIVE SIGNAL %s\n",MAP[pixel].S_SUM_NEGATIVE[j]);
		      printf("CONNECTOR",MAP[pixel].S_CLIP_CONNECTOR[j]);
		      printf("MACROCELL %i\n",MAP[pixel].S_SUM_MACROCELL[j]);
		      printf("CHANNEL %i\n",MAP[pixel].S_SUM_CHANNEL[j]);
		      printf("LAYER %i\n",MAP[pixel].S_SUM_LAYER[j]);
		    }
                }
            
            }
	  for(i=0;i<559;i++) flag=flag+iflag[i];
	  if (flag==0) printf("This pixel is not on the Trigger region\n"); 
        }

      if(strcmp(verb,"PIXELINMACRO")==0)
	{
	  get_short("PIXEL",&pixel,1,716);
	  int i=0;
	  int flag=0;
	  int iflag[559];
	  int pixel_temp = pixel;
	  for(i=0;i<559;i++)
	    {
	      iflag[i]=0;
	      if (pixel_temp==MAP[i].PIXEL)
		{   
		  pixel=i;
		  iflag[i]=1;
		  printf("      ************************************************\n");
		  printf("      ******Pixel # %i belongs to Layer: ",pixel_temp);
		  int j;
		  for (j=0;j<3;j++)
		    {  
		      if(MAP[pixel].S_SUM_LAYER[j]>=999)continue;
		      printf("  %i (ch %i) ",MAP[pixel].S_SUM_LAYER[j],MAP[pixel].S_SUM_CHANNEL[j]);
		    }
		  printf("\n");
		  printf("      **************************************************\n");		  
                }
            
            }
	  for(i=0;i<559;i++) flag=flag+iflag[i];
	  if (flag==0) printf("This pixel is not on the Trigger region\n"); 
	}

      if(strcmp(verb,"TABLE")==0)
        {                                                              
	  char str[1024]; 
	  str[0]='\0';
          strcat((char *)str,"SETMAP @");  
          //strcat((char *)str,"STmapping"); 
	  strcat((char *)str,"MAP.uic"); 
          printf("executing : \n %s \n", str);
          simulate_input((char *)str);
        }              
      if(strcmp(verb,"RETURN")==0)
        return;
    }
}

int GetLayer(int macrocell)
{
int Layer[56] =
  {
    //Layer Macrocell
    999,//0
    1,  // 1 
    2,  // 2 
    1,  // 3 
    2,  // 4 
    3,  // 5 
    3,  // 6 
    1,  // 7 
    2,  // 8 
    1,  // 9 
    3,  // 10
    2,  // 11
    1,  // 12
    3,  // 13
    2,  // 14
    1,  // 15
    3,  // 16
    3,  // 17
    1,  // 18
    2,  // 19
    3,  // 20
    3,  // 21
    2,  // 22
    1,  // 23
    2,  // 24
    2,  // 25
    3,  // 26
    1,  // 27
    1,  // 28
    3,  // 29
    1,  // 30
    1,  // 31
    2,  // 32
    3,  // 33
    3,  // 34
    2,  // 35
    1,  // 36
    2,  // 37
    3,  // 38
    2,  // 39
    3,  // 40
    2,  // 41
    1,  // 42
    2,  // 43
    3,  // 44
    1,  // 45
    2,  // 46
    1,  // 47
    3,  // 48
    1,  // 49
    2,  // 50
    2,  // 51
    3,  // 52
    1,  // 53
    3,  // 54
    1   // 55
  };

 return Layer[macrocell];

}



