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

		default:
			printf("\r\nUnknown Error Code %d", iErrCode);
	}

	return iRetVal;
}
