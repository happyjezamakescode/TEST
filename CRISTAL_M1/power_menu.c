void power_menu(void)
{
	
	float power_n = 0 ;
	char *verb;
	char *tablepath;
	char *tablename;
	int4 iverb,status;

	while(True)
	{
		verb = action_menu( "POWER_CONTROL_MENU", &iverb);
		if(strcmp(verb,"CURRENT_ALL_READ")==0)
		{
			printf("reading..\n");
		}
    
		if(strcmp(verb,"CURRENT_ONE_READ")==0)
		{
		        status= get_integer("Power supply number ",(int4*) &power_n,1,10);
			printf("reading..\n");
		}
    
		if(strcmp(verb,"EXTEMP_READ")==0)
		{
			printf("reading..\n");
		}
    
		if(strcmp(verb,"VOLTAGE_ALL_READ")==0)
		{
			printf("reading..\n");
		}
    
		if(strcmp(verb,"VOLTAGE_ONE_READ")==0)
		{
		        status= get_integer("Power supply number ",(int4*) &power_n,1,10);
			printf("reading..\n");
		}
		if(strcmp(verb,"OFF_ALL")==0)
		{
			printf("off\n");
		}
		
		if(strcmp(verb,"OFF_ONE")==0)
		{
		        status= get_integer("Power supply number ",(int4*) &power_n,1,10);
			printf("off\n");
		}
		
		if(strcmp(verb,"ON_ALL")==0)
		{
			printf("on\n");
		}
		
		if(strcmp(verb,"ON_ONE")==0)
		{
		        status= get_integer("Power supply number ",(int4*) &power_n,1,10);
			printf("on\n");
		}
		
		if(strcmp(verb,"RETURN")==0)
			return;
	}
}
