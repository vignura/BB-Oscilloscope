#ifndef BB_OSC_ENDIAN_SWAP_H
#define BB_OSC_ENDIAN_SWAP_H

int BB_OSC_NetHeader_EndSwap(NetworkHeader *pS_NetHeader);
int BB_OSC_ResReadVersion_EndSwap(BB_OSC_RES_READ_VERSION *pS_ResReadVersion);

#endif // BB_OSC_ENDIAN_SWAP_H