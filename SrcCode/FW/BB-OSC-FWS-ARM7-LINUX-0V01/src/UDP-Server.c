#include "../include/main.h"

int InitServer()
{
	int iRetVal = 0;

	iRetVal = pthread_create(&g_Handle.UDPServerID, NULL, (void *)UDPServerTask, NULL);
	if(iRetVal != BB_OSC_SUCCESS)
	{
		printf("\r\nERROR: UDP Server Thread creation failed..!");
		return BB_OSC_FAILURE;
	}

	return iRetVal;
}

int UDPServerTask()
{
	int iRetVal = 0;
	int iClientAddrLen	= 0;

	unsigned long ulTxMessageLen		= 0;
	unsigned long ulBuffIndex			= 0;

	struct sockaddr saClient_SocAddr 	= {0};
	struct sockaddr_in saServer_SocAddr = {0};

	static unsigned char st_ucTxBuffer[BB_OSC_MAX_UDP_STACK_SIZE] = {0};
	static unsigned char st_ucRxBuffer[BB_OSC_MAX_UDP_STACK_SIZE] = {0};

	saServer_SocAddr.sin_family			= AF_INET;
	saServer_SocAddr.sin_addr.s_addr	= INADDR_ANY;
	saServer_SocAddr.sin_port			= htons((unsigned short)BB_OSC_SERVER_PORT);	

	// Create the UDP Socket 
	if((g_Handle.iServerSkt = socket(saServer_SocAddr.sin_family, SOCK_DGRAM, IPPROTO_UDP)) < 0) 
	{
		printf("\r\nUDP Socket Creation Fail\t\t\t\t <FAILURE>");
		return BB_OSC_FAILURE;
	}

	// Bind the Server Socket to Listen the Client
	if((bind(g_Handle.iServerSkt, (struct sockaddr*)&saServer_SocAddr, sizeof(saServer_SocAddr))) < 0)
	{
		printf("\r\nUDP Socket Binding Fail\t\t\t\t <FAILURE>");
		return BB_OSC_FAILURE;
	}

	iClientAddrLen = sizeof(saClient_SocAddr);

	while(1)
	{
		iRetVal = recvfrom(g_Handle.iServerSkt, st_ucRxBuffer, BB_OSC_MAX_UDP_STACK_SIZE, 0, (struct sockaddr *)&saClient_SocAddr, &iClientAddrLen);

		if(iRetVal < 0)
		{
			printf("\r\nERROR: %s", strerror(errno));
			continue;
		}
		else
		{
			ulBuffIndex = 0;

			// copy the header
			memcpy(&g_Handle.S_UDPRXPacket.stNetHeader, &st_ucRxBuffer[ulBuffIndex], BB_OSC_NET_HEADER_LEN);
			ulBuffIndex += BB_OSC_NET_HEADER_LEN;

			// peform endian swap 
			BB_OSC_NetHeader_EndSwap(&g_Handle.S_UDPRXPacket.stNetHeader);

			// validate header
			if(BB_OSC_ValidateHeader(&g_Handle.S_UDPRXPacket.stNetHeader) == BB_OSC_SUCCESS)
			{
				BB_OSC_PrintNetHeader(&g_Handle.S_UDPRXPacket.stNetHeader);

				// copy the data
				memcpy(&g_Handle.S_UDPRXPacket.ucData, &st_ucRxBuffer[ulBuffIndex], g_Handle.S_UDPRXPacket.stNetHeader.ulPktLen);

				iRetVal = BB_OSC_CommandProcessor(g_Handle.S_UDPRXPacket.stNetHeader.ulSubItem,
													g_Handle.S_UDPRXPacket.ucData,
													g_Handle.S_UDPRXPacket.stNetHeader.ulPktLen,
													g_Handle.S_UDPTXPacket.ucData,
													&ulTxMessageLen);

				BB_OSC_FrameHeader(iRetVal, ulTxMessageLen);

				BB_OSC_FrameTXBuffer(st_ucTxBuffer, &ulTxMessageLen);

				iRetVal = sendto(g_Handle.iServerSkt, (char *)st_ucTxBuffer, ulTxMessageLen, 0, &saClient_SocAddr, iClientAddrLen);
				if(iRetVal != ulTxMessageLen)
				{
					printf("\r\nUDP Response Packet Sending Failed : %d", iRetVal);
					printf("\r\n[%d]: %s", strerror(errno));
				}
			}
			else
			{
				printf("\r\nInvalid packet received");
				BB_OSC_PrintNetHeader(&g_Handle.S_UDPRXPacket.stNetHeader);
			}
		}
	}

	return iRetVal;
}


int BB_OSC_ValidateHeader(NetworkHeader *pS_NetHeader)
{
	if(pS_NetHeader == NULL)
	{
		return BB_OSC_NULL_PTR_ACCESS;
	}
	
	if(pS_NetHeader->ulAAAA5555 != 0xAAAA5555)
	{
		return BB_OSC_INVALID_FLAG;
	}

	if(pS_NetHeader->ulSrc != BB_OSC_GUI_SRC_ID)
	{
		return BB_OSC_INVALID_SOURCE;
	}

	if(pS_NetHeader->ulPktLen > BB_OSC_MAX_UDP_MSG_SIZE)
	{
		return BB_OSC_INVALID_PKT_LEN;
	}

	if(pS_NetHeader->ulSubItem > BB_OSC_MAX_CMD_ID_VALUE)
	{
		return BB_OSC_INVALID_CMD_ID;
	}

	return BB_OSC_SUCCESS;
}

int BB_OSC_PrintNetHeader(NetworkHeader *pS_NetHeader)
{
	if(pS_NetHeader == NULL)
	{
		return BB_OSC_NULL_PTR_ACCESS;
	}

	printf("\r\nPacket length:   %d", pS_NetHeader->ulPktLen);
	printf("\r\nSource ID:       %#x", pS_NetHeader->ulSrc);
	printf("\r\nDestination ID:  %#x", pS_NetHeader->ulDest);
	printf("\r\nCategory:        %d", pS_NetHeader->ulcategory);
	printf("\r\nSubitem ID:      %d", pS_NetHeader->ulSubItem);
	printf("\r\nNo of Fragments: %d", pS_NetHeader->ulNoOfFragments);
	printf("\r\nFragment No:     %d", pS_NetHeader->ulFragmentNo);
	printf("\r\nSequence No:     %d", pS_NetHeader->ulSequenceNo);
	printf("\r\nReturn value:    %d", pS_NetHeader->iRetVal);
	printf("\r\nFlag value:      %d", pS_NetHeader->ulAAAA5555);
	
	return BB_OSC_SUCCESS;
}

int BB_OSC_CommandProcessor(unsigned long in_ulCmdID, unsigned char *in_ucBuffer, unsigned long in_ulBufferLen,
							unsigned char *out_ucBuffer, unsigned long *out_ulBuffLen)
{
	int iRetVal = 0;

	U_CMD_STRUCT_PTR in_UCommandData		= {0};
	U_RESPONSE_STRUCT_PTR out_UResponseData = {0};

	if((out_ucBuffer == NULL) || (out_ulBuffLen == NULL))
	{
		return BB_OSC_NULL_PTR_ACCESS;
	}

	// Copy Input Buffer Address to union pointer 
	in_UCommandData.p_ulAddress  = (unsigned long *) in_ucBuffer;
	
	// Copy Output Buffer Address to union pointer 
	out_UResponseData.p_ulAddress = (unsigned long *) out_ucBuffer;

	*out_ulBuffLen = 0;

	switch(in_ulCmdID)
	{
		case BB_OSC_READ_VERSION:

			break; 
	}

	return iRetVal;
}

void BB_OSC_FrameHeader(short in_iMsgStatus, unsigned long in_ulResMessageSize)
{	
	g_Handle.S_UDPTXPacket.stNetHeader.ulPktLen			= htonl(in_ulResMessageSize);
	g_Handle.S_UDPTXPacket.stNetHeader.ulSrc			= htonl(BB_OSC_BBFW_SRC_ID);
	g_Handle.S_UDPTXPacket.stNetHeader.ulcategory		= htonl(0);
	g_Handle.S_UDPTXPacket.stNetHeader.ulSubItem		= htonl(g_Handle.S_UDPRXPacket.stNetHeader.ulSubItem);
	g_Handle.S_UDPTXPacket.stNetHeader.ulNoOfFragments	= htonl(1);
	g_Handle.S_UDPTXPacket.stNetHeader.ulFragmentNo		= htonl(0);
	g_Handle.S_UDPTXPacket.stNetHeader.ulSequenceNo		= htonl(1);
	g_Handle.S_UDPTXPacket.stNetHeader.iRetVal			= htonl(in_iMsgStatus);
	g_Handle.S_UDPTXPacket.stNetHeader.ulAAAA5555		= htonl(0xAAAA5555);	
}

void BB_OSC_FrameTXBuffer(char *out_cTxBuffer, unsigned long *out_ulTxPacketLength)
{
	unsigned long ulMessageLength	= 0;
	
	if((out_ulTxPacketLength == NULL) || (out_cTxBuffer == NULL))
	{
		ulMessageLength = htonl(g_Handle.S_UDPTXPacket.stNetHeader.ulPktLen);
			
		memcpy(out_cTxBuffer, &g_Handle.S_UDPTXPacket.stNetHeader, BB_OSC_NET_HEADER_LEN);
		memcpy(&out_cTxBuffer[BB_OSC_NET_HEADER_LEN], &g_Handle.S_UDPTXPacket.ucData, ulMessageLength);

		*out_ulTxPacketLength = (BB_OSC_NET_HEADER_LEN + ulMessageLength);
	}
}
