#ifndef BB_OSC_UDP_SERVER_H
#define BB_OSC_UDP_SERVER_H

#define	BB_OSC_MAX_UDP_STACK_SIZE	(64 * 1024)
#define	BB_OSC_MAX_UDP_MSG_SIZE		BB_OSC_MAX_UDP_STACK_SIZE -30 
#define BB_OSC_NET_HEADER_LEN		sizeof(NetworkHeader)

#define BB_OSC_SERVER_PORT		5000
#define BB_OSC_GUI_SRC_ID		0xFF
#define BB_OSC_BBFW_SRC_ID		0xAA

/* Ehternet command IDs*/
#define BB_OSC_ETH_CMD_BASE						0x00
#define BB_OSC_READ_VERSION						(BB_OSC_ETH_CMD_BASE + 1)

#define BB_OSC_MAX_CMD_ID_VALUE					(BB_OSC_ETH_CMD_BASE + 1)

// ethernet command and response structures
#pragma pack(push, 1)

/*Network Packet header which is common for all network packets*/
typedef struct
{
	unsigned long 	ulPktLen;			// Packet Length
	unsigned long 	ulSrc;				// Source ID
	unsigned long 	ulDest;
	unsigned long 	ulcategory;			// Category Id of the packet
	unsigned long 	ulSubItem;			// Sub Item Id of the packet
	unsigned long 	ulNoOfFragments;	// Number of Fragments (in case of data that won't fit into a single packet)
	unsigned long 	ulFragmentNo;		// Fragment Number
	unsigned long 	ulSequenceNo;		// Sequence Number can be maintained for each type of packet by the application
	int 			iRetVal;
	unsigned long 	ulAAAA5555;			// Flag value = 0xAAAA5555

}NetworkHeader;

typedef struct
{
	NetworkHeader 	stNetHeader;									//Network Packet header
	unsigned char 	ucData[BB_OSC_MAX_UDP_MSG_SIZE - BB_OSC_NET_HEADER_LEN];	//Network Packet data

}NetworkPacket;

/*************************** BB_OSC_READ_VERSION Response structure ***************************/
typedef struct 
{
	float fFirmwareVersion;	

}BB_OSC_RES_READ_VERSION;

#pragma pack(pop)

/****************************** Union for All GUI Response **********************************/
typedef union
{	
	BB_OSC_RES_READ_VERSION			pS_ResReadVersion;
	
	unsigned long *p_ulAddress;

}U_RESPONSE_STRUCT_PTR;

/****************************** Union for All GUI Commands **********************************/
typedef union
{	
	
	unsigned long *p_ulAddress;

}U_CMD_STRUCT_PTR;

/********************************************************************************************/

// function declarations
int InitServer();
int UDPServerTask();
int BB_OSC_ValidateHeader(NetworkHeader *pS_NetHeader);
int BB_OSC_PrintNetHeader(NetworkHeader *pS_NetHeader);
void BB_OSC_FrameHeader(short in_iMsgStatus, unsigned long in_ulResMessageSize);
void BB_OSC_FrameTXBuffer(char *out_cTxBuffer, unsigned long *out_ulTxPacketLength);
int BB_OSC_CommandProcessor(unsigned long in_ulCmdID, unsigned char *in_ucBuffer, unsigned long in_ulBufferLen,
							unsigned char *out_ucBuffer, unsigned long *out_ulBuffLen);
#endif // BB_OSC_UDP_SERVER_H