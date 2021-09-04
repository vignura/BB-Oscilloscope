#ifndef BB_OSC_ADC_H
#define BB_OSC_ADC_H

#define BB_OSC_ADC_ID			"TI-am335x-adc.0.auto"
#define BB_OSC_ENABLE_ADC_CMD	"BB-ADC" 
#define BB_OSC_CAPEMGR_PATH		"/sys/devices/platform/bone_capemgr/slots"
#define BB_OSC_ADC_DEV_PATH 	"/sys/bus/iio/devices/iio:device0"
#define BB_OSC_MAX_ADC_CH_NO    8
#define BB_OSC_MIN_ADC_CH_NO    1
#define BB_OSC_MAX_ADC_DATA_READ_SIZE   128

#define ADC_ENABLE          1
#define ADC_DISABLE         0

int BB_OSC_EnableADC();
int BB_OSC_ReadADCID();
int BB_OSC_InitADC();
int BB_OSC_ReadADC_Oneshot(unsigned int uiChNo, unsigned short *pusADCData);

#endif // BB_OSC_ADC_H