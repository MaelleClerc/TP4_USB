#ifndef Mc32Gest_SERCOMM_H
#define Mc32Gest_SERCOMM_H
/*--------------------------------------------------------*/
// Mc32Gest_SerComm.h
/*--------------------------------------------------------*/
//	Description :	emission et reception specialisee
//			pour TP4 2015-2016
//
//	Auteur 		: 	C. HUBER
//
//	Version		:	V1.2
//	Compilateur	:	XC32 V1.40 + Harmony 1.06
//
/*--------------------------------------------------------*/

#include <stdint.h>
#include "DefMenuGen.h"

/*--------------------------------------------------------*/
// Prototypes des fonctions 
/*--------------------------------------------------------*/

void SendMessage(int8_t *USBReadBuffer,int8_t *USBSendBuffer, bool Saved);
void GetMessage(char *USBReadBuffer, S_ParamGen *pParam );

#endif
