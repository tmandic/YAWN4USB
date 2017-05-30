#include <xc.h>

#include "system.h"

#include "usb.h"
#include "usb_device.h"
#include "usb_device_cdc.h"

#include "spi.h"

#define _XTAL_FREQ 48000000

static uint8_t readBuffer[CDC_DATA_OUT_EP_SIZE];
static uint8_t writeBuffer[CDC_DATA_IN_EP_SIZE];

MAIN_RETURN main(void)
{
    SYSTEM_Initialize(SYSTEM_STATE_USB_START);
    USBDeviceInit();
    USBDeviceAttach();
    CDCInitEP();
    CDCSetBaudRate(1200);
    
    SPI_Init();

    // set RC5 == SYNC as output
    TRISC5 = 0;
    TRISC2 = 0;
    ANSC2 = 0;
    RC2 = 0;

    while(1){
        if( USBUSARTIsTxTrfReady() == true){
            uint8_t i, numBytesRead, a;

            numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer));

            if(numBytesRead > 0){
                // SYNC falling edge
                RC5 = 1;
                RC5 = 0;

                for(i=0; i<numBytesRead; i++){
                    a = readBuffer[i];
                    SPI_Write(a);
                    writeBuffer[i] = readBuffer[i];
                }
                // SYNC rising edge
                RC5 = 1;
                putUSBUSART(writeBuffer, numBytesRead);
            }
        }
        CDCTxService();
    }
}
