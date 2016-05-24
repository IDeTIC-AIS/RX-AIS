
/* **************************************************************************
               UNIVERSIDAD DE LAS PALMAS DE GRAN CANARIA
       ESCUELA DE INGENIERIA DE TELECOMUNICACIONES Y ELECTRONICA
   
   Institution: Instituto para el Desarrollo Tecnologico y la Innovacion en
   Comunicaciones (IDeTIC)
   
   Summary: this code allows to receive AIS data by means of AIS prototype
   based to a DV-MEGA GMSK modem and Radiometrix RX1 which that had been
   developed by IDeTIC in 2015.
   
   Authors: Francisco Jose Cabrera Almeida
            Nicolas Molina Padron
 *************************************************************************** */

#include "nmea.c"

// definitions
#define   pinRXCLK       2
#define   pinTXCLK       3
#define   pinSN          4
#define   pinRXD         5
#define   pinTXD         6
#define   pinPTT         8
#define   pinPLLACQ      9
#define   pinLEDTX       10
#define   pinLEDCOS      11
#define   pinLEDPWR      12
#define   pinArduinoLED  13

// variable limits
#define   MAX_DATA       23
#define   HEADER         0x7E

// global variables
uint8_t rxdata;
uint8_t rxdataNRZI;
uint8_t rxdataNRZI_prev = 0;
uint8_t rxByte;
unsigned char rxHeader = 0;
unsigned char rxTail = 0;
uint8_t counterBitRx = 0;
uint8_t counterByteRx = 0;
unsigned char bufferRx[MAX_DATA];
int flagStartFrame = 0;
int flagRx = 0;

// initialization
void setup() {
  // serial port initialization (9600 bps)
  Serial.begin(9600);
  Serial.flush();
  // pin initialization
  pinMode(pinSN, INPUT);    
  pinMode(pinRXD, INPUT);    
  pinMode(pinTXD, OUTPUT);
  pinMode(pinLEDTX, OUTPUT);    
  pinMode(pinLEDCOS, OUTPUT);
  pinMode(pinLEDPWR, OUTPUT);
  pinMode(pinPTT, OUTPUT);
  pinMode(pinPLLACQ, OUTPUT);
  // ISR initialization
  attachInterrupt(0, rxISR, FALLING);
}

// main program
void loop() {
  // if the end of data frame is reached, 
  // stored bytes are converted to NMEA format and sent by Serial port
  if(flagRx) {
    
    NMEA_Send(bufferRx, MAX_DATA);       
//    for(int k=0; k<MAX_DATA; k++) {
//      delay(10);
//      Serial.write(bufferRx[k]);
//    }
    // variables reset
    flagRx = 0;
    rxHeader = 0;
    rxTail = 0;
    // ISR is activated again
    attachInterrupt(0, rxISR, FALLING);
  }
}

// receiver ISR (Interrupt Service Routine)
void rxISR() {
  // store bits which has been read by means of RXD pin
  rxdata = digitalRead(pinRXD);
  // NRZI decode
  if(rxdataNRZI == HIGH) {
    rxdata = rxdataNRZI_prev ^ rxdataNRZI;
  } else {
    rxdata = rxdataNRZI_prev;
  }
  rxdataNRZI_prev = rxdataNRZI;
  // evaluation of AIS frame fields:
  // case 0 -> training sequence (0x55-0x55-0x55)
  // case 1 -> header (0x7E)
  // case 2 -> data and CRC field
  switch(flagStartFrame) {
    case 0:
      rxHeader = (rxHeader << 1) | rxdata;
      if(rxHeader == 0x7E) {
        flagStartFrame = 1;
      }
      break;
    case 1:
      rxByte = (rxByte << 1) | rxdata;
      if(counterBitRx >= 7) {
        bufferRx[counterByteRx] = rxByte;
        counterByteRx++;
        counterBitRx = 0;
        rxByte = 0;
        if(counterByteRx >= MAX_DATA) {
          flagStartFrame = 2;
        }
      } else {
        counterBitRx++;
      }
      break;
    case 2:
      rxTail = (rxTail << 1) | rxdata;
      if(counterBitRx >= 7) {
        flagStartFrame = 0;
        counterBitRx = 0;
        counterByteRx = 0;
        detachInterrupt(0);
        flagRx = 1;
      } else {
        counterBitRx++;
      }
      break;
  }
}


