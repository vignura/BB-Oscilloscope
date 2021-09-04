#ifndef BB_OSC_MAIN_H
#define BB_OSC_MAIN_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>	
#include <sys/errno.h>

#include <pthread.h>
#include <semaphore.h>

#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>	
#include <termios.h>
#include <sys/ioctl.h>
#include <termio.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <linux/serial.h>
#include <stdio_ext.h>
#include <sys/mman.h>
#include <sys/statvfs.h>
#include <time.h>

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>

// system Macros
#define BB_OSC_SUCCESS		0
#define BB_OSC_FAILURE		-1

#include "../include/UDP-Server.h"
#include "../include/Struct.h"
#include "../include/Errors.h"
#include "../include/EndianSwap.h"
#include "../include/ADC.h"

// declare the globale handle
extern BB_OSC_GLOBAL_HANDLE g_Handle;

int BB_OSC_Test_ADCOneShotMode();
#endif // BB_OSC_MAIN_H
