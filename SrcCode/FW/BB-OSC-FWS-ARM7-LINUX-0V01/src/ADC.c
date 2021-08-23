#include "../include/main.h"

int BB_OSC_InitADC()
{
	int iRetVal = 0;

	printf("%s\n", __func__);
	iRetVal = BB_OSC_ReadADCID();
	if(iRetVal != BB_OSC_SUCCESS)
	{
		BB_OSC_DisplayErrors(iRetVal);
	}

	return iRetVal;
}

/* not used not since cape manager is removed */
int BB_OSC_EnableADC()
{
	int iRetVal = 0;
	FILE *fptr = NULL;

	// check if adc is already enabled
	iRetVal = BB_OSC_ReadADCID();
	if(iRetVal == BB_OSC_SUCCESS)
	{
		printf("\r\nADC already enabled...");
	}
	else
	{
		printf("\r\nADC not enabled..., enabling ADC");

		fptr = fopen(BB_OSC_CAPEMGR_PATH, "w");
		if(fptr != NULL)
		{
			iRetVal = fprintf(fptr, "%s", BB_OSC_ENABLE_ADC_CMD);
			if(iRetVal != strlen(BB_OSC_ENABLE_ADC_CMD))
			{
				iRetVal = BB_OSC_CAPEMGR_FILE_WRITE_FAIL;
			}
			else
			{
				if(fptr != NULL)
				{
					fclose(fptr);
				}

				usleep(1000 * 1000);

				// check if adc files are created 
				iRetVal = BB_OSC_ReadADCID();
				if(iRetVal == BB_OSC_SUCCESS)
				{
					printf("\r\nADC enabled successfully");
				}
				else
				{
					printf("\r\nERROR: Unable to enable ADC");
				}

			}
		}
		else
		{
			iRetVal = BB_OSC_CAPEMGR_FILE_OPEN_FAIL;
		}
	}

	return iRetVal;
}

int BB_OSC_ReadADCID()
{
	int iRetVal = 0;
	FILE *fptr = NULL;
	char arrcADCID[32] = {0};
	char arrcPath[128] = {0};

	printf("%s\n", __func__);

	strcpy(arrcPath, BB_OSC_ADC_DEV_PATH);
	strcat(arrcPath, "/name");

	printf("%s: opening file %s\n", __func__, arrcPath);
	fptr = fopen(arrcPath, "r");
	if(fptr != NULL)
	{
		printf("%s: scanning file ...\n", __func__);
		iRetVal = fscanf(fptr, "%s", arrcADCID);
		if(iRetVal != 1)
		{
			printf("\r\n%s: ADC ID: %s\n", __func__, arrcADCID);
			iRetVal = BB_OSC_ADCIIO_FILE_READ_FAIL;
		}
		else
		{
			if(!strcmp(arrcADCID, BB_OSC_ADC_ID))
			{
				iRetVal = BB_OSC_SUCCESS;
				printf("\r\nADC ID: %s", arrcADCID);
			}
			else
			{
				iRetVal = BB_OSC_INVALID_ADC_ID;
			}

		}
	}
	else
	{
		printf("%s: failed to open file ...\n", __func__);
		iRetVal = BB_OSC_ADCIIO_FILE_OPEN_FAIL;
	}

	if(fptr != NULL)
	{
		fclose(fptr);
	}

	return iRetVal;
}