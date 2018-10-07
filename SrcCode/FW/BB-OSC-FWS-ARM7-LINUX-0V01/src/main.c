#include "../include/main.h"

BB_OSC_GLOBAL_HANDLE  g_Handle = {0};

int main()
{
	int iRetVal = 0;
	printf("\r\nBeagleBone Oscilloscope");

	iRetVal = InitServer();
	if(iRetVal != BB_OSC_SUCCESS)
	{
		BB_OSC_DisplayErrors(iRetVal);
	}
	else
	{
		iRetVal = pthread_join(g_Handle.UDPServerID, NULL); 	
		if(iRetVal != BB_OSC_SUCCESS)
		{
			printf("\r\nUDP Server PThread Join State\t\t\t\t <FAILURE>");	
		}
	}

	return 0;
}
