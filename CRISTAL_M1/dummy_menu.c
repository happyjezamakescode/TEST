#include "CristalSpi.h"
void dummy_menu(void)
{
	
	char *verb;
	char *tablepath;
    int status;
    short i;
    int4 iverb,eDelay,Delay,rate, patchnumber, ecount ;


    while(True)
	{
		verb = action_menu( "DUMMY_MENU", &iverb);
		if(strcmp(verb,"SET")==0)
		{
		 short data;
            get_short("Enter channel", &i, 0, 31);
            printf("setting dummy valor\n");
            get_short("dummy",&DummyData.channelvalue[i] ,1,64);
          //  DummyData.channelvalue[i] =data;
			printf("printing dummy valor %i\n",DummyData.channelvalue[i]);
		}
    
		if(strcmp(verb,"ALLSET")==0)
		{
         int j;
             for (j=0;j<32;j++)
             {
                get_short("Enter channel", &i, 0, 31);
                get_short("dummy",&DummyData.channelvalue[i] ,1,64);
                printf("channel %i\n",i);
			    printf("valor %i\n",DummyData.channelvalue[i]);
            }	
        }	
    
		if(strcmp(verb,"ALLCLEAR")==0)
		{
         int j;
             for (j=0;j<32;j++)
             {
                DummyData.channelvalue[j]=0;
                printf("channel %i\n",j);
			    printf("valor %i\n",DummyData.channelvalue[j]);
            }	
		}
    
		if(strcmp(verb,"ALLPRINT")==0)
		{
         int j;
             for (j=0;j<32;j++)
             {
                printf("channel %i\n",j);
			    printf("valor %i\n",DummyData.channelvalue[j]);
            }	
		}
    
		if(strcmp(verb,"PRINT")==0)
		{
			get_short("Enter channel", &i, 0, 31);
			printf("printing dummy valor %i\n",DummyData.channelvalue[i]);
		}
    
		if(strcmp(verb,"RETURN")==0)
			return;

		if(strcmp(verb,"READ")==0)
		{
            get_short("dummy",(int2*) &dummyvalor2,1,100);
			printf("printing dummy valor %i\n",dummyvalor2);
		    getdummy(&dummyvalor2);
			printf("printing dummy valor %i\n",dummyvalor2);
        }
        if(strcmp(verb,"DUMFROMTABLE")==0)  {                                                              
        char str[1024]; 
        str[0]='\0';
      strcat((char *)str,"ALLSET @");   //setdt
      strcat((char *)str,"dummy"); 
      printf("executing : \n %s \n", str);
      simulate_input((char *)str);
    }              
	}
}
