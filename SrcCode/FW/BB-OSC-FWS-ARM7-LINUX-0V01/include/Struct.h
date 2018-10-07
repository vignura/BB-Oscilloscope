#ifndef BB_OSC_STRUCT_H
#define BB_OSC_STRUCT_H

#include "../include/main.h"

// the global structure
typedef struct 
{
	// thread IDs
	pthread_t	UDPServerID;

	// sockets
	int 		iServerSkt;	

	// command and response structures
	NetworkPacket	S_UDPTXPacket;
	NetworkPacket	S_UDPRXPacket;

}BB_OSC_GLOBAL_HANDLE;

#endif // BB_OSC_STRUCT_H