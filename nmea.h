/* **************************************************************************
               UNIVERSIDAD DE LAS PALMAS DE GRAN CANARIA
       ESCUELA DE INGENIERIA DE TELECOMUNICACIONES Y ELECTRONICA
   
   Institution: Instituto para el Desarrollo Tecnologico y la Innovacion en
   Comunicaciones (IDeTIC)
   
   Summary: this library is extracted from the website Open Hardware
            http://http://www.open-hw.net
               
   Authors: Francisco Jose Cabrera Almeida
            Nicolas Molina Padron
 *************************************************************************** */
//----------------------------------------------------------------------------
// AIS Decoder
//
// NMEA.h - NMEA Output header
//
//----------------------------------------------------------------------------


#ifndef	NMEA_H
#define	NMEA_H

//************************************************************************
//
// DECLARATIONS AND MACROS
//
//************************************************************************


//************************************************************************
//
// GLOBAL VARIABLES
//
//************************************************************************



//************************************************************************
//
// EXPORTED FUNCTIONS
//
//************************************************************************


void NMEA_Send(unsigned char *pkt, uint8_t pkt_size);


#endif		// NMEA_H

//************************************************************************
//
// EOF
//
//************************************************************************
