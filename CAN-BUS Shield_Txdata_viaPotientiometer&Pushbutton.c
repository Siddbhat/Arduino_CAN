// demo: CAN-BUS Shield, send data
#include <SPI.h>
/*const int button = 3;
const int button_state = 0;*/
#define CAN_2515
// #define CAN_2518FD
// Set SPI CS Pin according to your hardware
#if defined(SEEED_WIO_TERMINAL) && defined(CAN_2518FD)
// For Wio Terminal w/ MCP2518FD RPi Hat：
// Channel 0 SPI_CS Pin: BCM 8
// Channel 1 SPI_CS Pin: BCM 7
// Interupt Pin: BCM25
const int SPI_CS_PIN  = BCM8;
const int CAN_INT_PIN = BCM25;
#else
// For Arduino MCP2515 Hat:
// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 53;
//const int CAN_INT_PIN = 2;
const int button = 3;
const int button_state = 0;
#endif
#ifdef CAN_2518FD
#include "mcp2518fd_can.h"
mcp2518fd CAN(SPI_CS_PIN); // Set CS pin
#endif
#ifdef CAN_2515
#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
#endif
const int ledHIGH    = 1;
const int ledLOW     = 0;  
void setup()
{
    SERIAL_PORT_MONITOR.begin(115200);

       while (CAN_OK != CAN.begin(CAN_500KBPS)) {             // init can bus : baudrate = 500k
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
    }
     pinMode(button, INPUT);
    SERIAL_PORT_MONITOR.println("CAN init ok!");
}

unsigned char stmp[8] = {0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void loop() 
{
    SERIAL_PORT_MONITOR.println("In loop");
   int potValue = analogRead(A0);
   int arr1 = (potValue >> 8)& 0xFF; 
   int arr2 = potValue & 0xFF;
   stmp[0] =arr1;
   stmp[1] = arr2;
   if(button_state == 1)
   {
     stmp[2]=1;
   }
   else
   {
     stmp[3] = 0;
   }
   // send data:  id = 0x70, standard frame, data len = 8, stmp: data buf
    CAN.MCP_CAN::sendMsgBuf(0X15EFAABB, 0, 8, stmp);
    delay(1000);                       // send data once per second
}
