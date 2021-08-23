#include "../include/main.h"

int BB_OSC_NetHeader_EndSwap(NetworkHeader *pS_NetHeader)
{
	int iRetVal = 0;

	if(pS_NetHeader == NULL)
	{
		return BB_OSC_NULL_PTR_ACCESS;
	}

	pS_NetHeader->ulPktLen			= htonl(pS_NetHeader->ulPktLen);
	pS_NetHeader->ulSrc				= htonl(pS_NetHeader->ulSrc);
	pS_NetHeader->ulcategory		= htonl(pS_NetHeader->ulcategory);
	pS_NetHeader->ulSubItem			= htonl(pS_NetHeader->ulSubItem);
	pS_NetHeader->ulNoOfFragments	= htonl(pS_NetHeader->ulNoOfFragments);
	pS_NetHeader->ulFragmentNo		= htonl(pS_NetHeader->ulFragmentNo);
	pS_NetHeader->ulSequenceNo		= htonl(pS_NetHeader->ulSequenceNo);
	pS_NetHeader->iRetVal			= htonl((unsigned long)pS_NetHeader->iRetVal);
	pS_NetHeader->ulAAAA5555		= htonl(pS_NetHeader->ulAAAA5555);

	return iRetVal;
}

int BB_OSC_ResReadVersion_EndSwap(BB_OSC_RES_READ_VERSION *pS_ResReadVersion)
{
	int iRetVal = 0;

	if(pS_ResReadVersion == NULL)
	{
		return BB_OSC_NULL_PTR_ACCESS;
	}

	*(unsigned long *)(&pS_ResReadVersion->fFirmwareVersion) = htonl(*(unsigned long *)(&pS_ResReadVersion->fFirmwareVersion));

	return iRetVal;
}
