#include "../include/main.h"

static void getADCFilePath();
static int BB_OSC_ADCScan_Enable(int iShouldEnable, unsigned int uiChNo);
static int BB_OSC_ADCSetBufferLen(unsigned int uiBuffLen);
static int BB_OSC_ADC_Enable_Buffer(int iShouldEnable);

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

int BB_OSC_ReadADC_Oneshot(unsigned int uiChNo, unsigned short *pusADCData)
{
	int iRetVal = 0;
	char FilePath[256] = {0};

	if(pusADCData == NULL)
	{
		return BB_OSC_NULL_PTR_ACCESS;
	}

	// validate ADC channel number
	if((uiChNo == 0) && (uiChNo > BB_OSC_MAX_ADC_CH_NO))
	{
		return BB_OSC_INVALID_ADC_CH_NO;
	}

	// disable ADC continuous mode
	iRetVal = BB_OSC_ADC_Enable_Buffer(ADC_DISABLE);
	if(iRetVal != BB_OSC_SUCCESS)
	{
		printf("%s: BB_OSC_ADC_Enable_Buffer failed", __func__);
		return iRetVal;
	}

	getADCFilePath(FilePath, sizeof(FilePath), (uiChNo -1));
	printf("\r\nReading from %s", FilePath);
	FILE *fPtr = fopen(FilePath, "r");
	if(fPtr != NULL)
	{
		iRetVal = fscanf(fPtr, "%hu", pusADCData);
		if(iRetVal != 1)
		{
			printf("\r\n%s: Failed to read data from ADC Ch %d", __func__, uiChNo);
			iRetVal = BB_OSC_FAILURE;
		}
		else
		{
			iRetVal = BB_OSC_SUCCESS;
		}

		fclose(fPtr);
	}
	else
	{
		printf("\r\n%s: Failed to open ADC file", __func__);
	}

	return iRetVal;
}

static void getADCFilePath(char *FilePath, int iSize, unsigned int uiChNo)
{
	if(FilePath != NULL)
	{
		snprintf(FilePath, iSize, "%s/in_voltage%u_raw", BB_OSC_ADC_DEV_PATH, (uiChNo -1));
	}
}

int BB_OSC_ReadADC_Continuous(unsigned int uiChNo, unsigned short *pusADCData, unsigned int uiReadSize)
{
	int iRetVal = 0;

	if(pusADCData == NULL)
	{
		return BB_OSC_NULL_PTR_ACCESS;
	}

	// validate ADC channel number
	if((uiChNo == 0) && (uiChNo > BB_OSC_MAX_ADC_CH_NO))
	{
		return BB_OSC_INVALID_ADC_CH_NO;
	}

	// validate max ADC read size
	if(uiReadSize > BB_OSC_MAX_ADC_DATA_READ_SIZE)
	{
		return BB_OSC_INVALID_ADC_DATA_READ_SIZE;
	}

	// enable scanning
	iRetVal = BB_OSC_ADCScan_Enable(ADC_ENABLE, uiChNo);
	if(iRetVal != BB_OSC_SUCCESS)
	{
		printf("%s: BB_OSC_ADCScan_Enable failed", __func__);
	}

	// set buffer length
	iRetVal = BB_OSC_ADCSetBufferLen(BB_OSC_MAX_ADC_DATA_READ_SIZE);
	if(iRetVal != BB_OSC_SUCCESS)
	{
		printf("%s: BB_OSC_ADCSetBufferLen failed", __func__);
	}
	
	// enable buffer
	iRetVal = BB_OSC_ADC_Enable_Buffer(ADC_ENABLE);
	if(iRetVal != BB_OSC_SUCCESS)
	{
		printf("%s: BB_OSC_ADC_Enable_Buffer failed", __func__);
	}
	

	return iRetVal;
}

static int BB_OSC_ADCScan_Enable(int iShouldEnable, unsigned int uiChNo)
{
	char FilePath[256] = {0};
	FILE *fPtr =  NULL;

	snprintf(FilePath, sizeof(FilePath), "%s/scan_elements/in_voltage%hu_en", BB_OSC_ADC_DEV_PATH, uiChNo);
	printf("%s: %s scanning for %s", __func__, iShouldEnable ? "Enabling" : "Disabling", FilePath);
	
	fPtr = fopen(FilePath, "w");
	if(fPtr != NULL)
	{
		fprintf(fPtr, "%d", iShouldEnable ? ADC_ENABLE : ADC_DISABLE);
		fclose(fPtr);
	}
	else
	{
		return BB_OSC_FAILURE;
	}

	return BB_OSC_SUCCESS;
}

static int BB_OSC_ADCSetBufferLen(unsigned int uiBuffLen)
{
	char FilePath[256] = {0};
	FILE *fPtr =  NULL;

	snprintf(FilePath, sizeof(FilePath), "%s/buffer/length", BB_OSC_ADC_DEV_PATH);
	printf("%s: setting ADC buffer length to %d: %s", __func__, uiBuffLen, FilePath);
	
	fPtr = fopen(FilePath, "w");
	if(fPtr != NULL)
	{
		fprintf(fPtr, "%u", uiBuffLen);
		fclose(fPtr);
	}
	else
	{
		return BB_OSC_FAILURE;
	}

	return BB_OSC_SUCCESS;
}

static int BB_OSC_ADC_Enable_Buffer(int iShouldEnable)
{
	char FilePath[256] = {0};
	FILE *fPtr =  NULL;

	snprintf(FilePath, sizeof(FilePath), "%s/buffer/enable", BB_OSC_ADC_DEV_PATH);
	printf("%s: %s ADC buffer %s", __func__, iShouldEnable ? "Enabling" : "Disabling", FilePath);
	
	fPtr = fopen(FilePath, "w");
	if(fPtr != NULL)
	{
		fprintf(fPtr, "%d", iShouldEnable ? ADC_ENABLE : ADC_DISABLE);
		fclose(fPtr);
	}
	else
	{
		return BB_OSC_FAILURE;
	}

	return BB_OSC_SUCCESS;
}