#include "../include/main.h"

int BB_OSC_DisplayErrors(int iErrCode)
{
	int iRetVal = 0;

	switch(iErrCode)
	{
		// printf("\r\n[%d] Error: ", iErrCode);
		case BB_OSC_NULL_PTR_ACCESS:
			printf("\r\n[%d] Error: Null pointer access", iErrCode);
			break;

		case BB_OSC_INVALID_FLAG:
			printf("\r\n[%d]Error: Invalid Flag value in Network Packet Header", iErrCode);
			break;

		case BB_OSC_INVALID_SOURCE:
			printf("\r\n[%d]Error: Invalid source in Network Packet Header", iErrCode);
			break;

		case BB_OSC_INVALID_DEST:
			printf("\r\n[%d]Error: Invalid destination in Network Packet Header", iErrCode);
			break;

		case BB_OSC_INVALID_PKT_LEN:
			printf("\r\n[%d]Error: Invalid packet length in Network Packet Header", iErrCode);
			break;

		case BB_OSC_INVALID_CMD_ID:
			printf("\r\n[%d]Error: Invalid command ID in Network Packet Header", iErrCode);
			break;

		case BB_OSC_CAPEMGR_FILE_OPEN_FAIL:
			printf("\r\n[%d]Error: Unable to open BB capemanager file", iErrCode);
			break;

		case BB_OSC_CAPEMGR_FILE_WRITE_FAIL:
			printf("\r\n[%d]Error: Unable to write BB capemanager file", iErrCode);
			break;

		case BB_OSC_CAPEMGR_FILE_READ_FAIL:
			printf("\r\n[%d]Error: Unable to read BB capemanager file", iErrCode);
			break;

		case BB_OSC_ADCIIO_FILE_OPEN_FAIL:
			printf("\r\n[%d]Error: Unable to open ADC IIO file", iErrCode);
			break;

		case BB_OSC_ADCIIO_FILE_WRITE_FAIL:
			printf("\r\n[%d]Error: Unable to write ADC IIO file", iErrCode);
			break;

		case BB_OSC_ADCIIO_FILE_READ_FAIL:
			printf("\r\n[%d]Error: Unable to read ADC IIO file", iErrCode);
			break;

		case BB_OSC_INVALID_ADC_ID:
			printf("\r\n[%d]Error: Invalid ADC ID", iErrCode);
			break;
		
		case BB_OSC_INVALID_ADC_CH_NO:
			printf("\r\n[%d]Error: Invalid ADC Channel number", iErrCode);
			break;
		
		case BB_OSC_INVALID_ADC_DATA_READ_SIZE:
			printf("\r\n[%d]Error: Invalid ADC Data read size", iErrCode);
			break;

		default:
			printf("\r\nUnknown Error Code %d", iErrCode);
	}

	return iRetVal;
}
