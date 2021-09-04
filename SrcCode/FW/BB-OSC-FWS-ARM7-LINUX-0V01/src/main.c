#include "../include/main.h"

BB_OSC_GLOBAL_HANDLE  g_Handle = {0};

int main()
{
	int iRetVal = 0;
	printf("\r\nBeagleBone Oscilloscope\n");

	iRetVal = BB_OSC_InitADC();
	if(iRetVal != BB_OSC_SUCCESS)
	{
		printf("\r\nADC initialization failed");
	}

	// tests
	BB_OSC_Test_ADCOneShotMode();
	////////

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


int BB_OSC_Test_ADCOneShotMode()
{
	int iRetVal = 0;
	unsigned short usADCData = 0;

	for(unsigned int uiChNo = BB_OSC_MIN_ADC_CH_NO; uiChNo <= BB_OSC_MAX_ADC_CH_NO; uiChNo++)
	{
		iRetVal = BB_OSC_ReadADC_Oneshot(uiChNo, &usADCData);
		if(iRetVal == BB_OSC_SUCCESS)
		{
			printf("\r\n%s: ADC CH %u data: %u", __func__, uiChNo, usADCData);
		}
	}

	return 0;
}