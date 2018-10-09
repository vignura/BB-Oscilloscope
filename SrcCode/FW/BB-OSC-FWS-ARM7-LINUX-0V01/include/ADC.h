#ifndef BB_OSC_ADC_H
#define BB_OSC_ADC_H

#define BB_OSC_ADC_ID			"TI-am335x-adc"
#define BB_OSC_ENABLE_ADC_CMD	"BB-ADC" 
#define BB_OSC_CAPEMGR_PATH		"/sys/devices/platform/bone_capemgr/slots"
#define BB_OSC_ADC_DEV_PATH 	"/sys/bus/iio/devices/iio:device0"

int BB_OSC_EnableADC();
int BB_OSC_ReadADCID();
int BB_OSC_InitADC();

#endif // BB_OSC_ADC_H