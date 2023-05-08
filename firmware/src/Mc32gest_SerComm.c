// Mc32Gest_SerComm.C
// fonction d'émission et de réception des message
// transmis en USB CDC
// Canevas TP4 SLO2 2015-2015


#include "app_USB.h"
#include "Mc32gest_SerComm.h"
#include "Mc32DriverLcd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "DefMenuGen.h"

APP_DATA appData;


// Fonction de reception  d'un  message
// Met à jour les paramètres du generateur a partir du message recu
// Format du message
//  !S=TF=2000A=10000O=+5000D=100W=0#
//  !S=PF=2000A=10000O=-5000D=100W=1#


bool GetMessage(int8_t *USBReadBuffer, S_ParamGen *pParam, bool *SaveTodo)
{
    // Code pour récupérer les données de l'USB en utilisant SPI
    // Ici, on suppose que les données sont stockées dans un tableau nommé "usbData"
    // et que la fonction SPITransfer() est utilisée pour la communication SPI
    //SPITransfer();
    //afficher un caractère recu 
    
    // variable locales
    int16_t Frequence_recue = 0;
    int16_t Amplitude_recue = 0;
    int16_t Offset_recu = 0;

 
   if (USBReadBuffer[0] == '!')
   {
        // Traduction de la forme du signal
        switch (USBReadBuffer[3])
        {
            case 'T':

                pParam->Forme = SignalTriangle;

                break;

            case 'S':

                pParam->Forme = SignalSinus;

                break;

            case 'C':

                pParam->Forme = SignalCarre;

                break;

            case 'D':

                pParam->Forme = SignalDentDeScie;

                break;
        }

        //if(appData.newStringReceived[0])
        // Traduction de la frequence
        Frequence_recue = USBReadBuffer[6];

        // Traduction de l'amplitude
        Amplitude_recue = USBReadBuffer[12];

        // Traduction de l'offset
        Offset_recu = USBReadBuffer[19];

        // Mise a jour des parametres de pParam
        pParam->Frequence = Frequence_recue;
        pParam->Amplitude = Amplitude_recue;
        pParam->Offset = Offset_recu;

        //*SaveTodo = atoi(&appData.newStringReceived[26]);
    }
    return SaveTodo; 
    
} // GetMessage


// Fonction d'envoi d'un  message
// Rempli le tampon d'émission pour USB en fonction des paramètres du générateur
// Format du message
// !S=TF=2000A=10000O=+5000D=25WP=0#
// !S=TF=2000A=10000O=+5000D=25WP=1#    // ack sauvegarde



void SendMessage(int8_t *USBSendBuffer, S_ParamGen *pParam, bool Saved )
{
    USBSendBuffer[0] = '!';
    USBSendBuffer[1] = 'S';
    USBSendBuffer[2] = '=';
    switch (pParam->Forme)
    {
        case SignalTriangle:
            
            USBSendBuffer[3] = 'T';
            
            break;
            
        case SignalSinus:
            
            USBSendBuffer[3] = 'S';
            
            break;
            
        case SignalCarre:
            
            USBSendBuffer[3] = 'C';
                
            break;
            
        case SignalDentDeScie:
            
            USBSendBuffer[3] = 'D';
            
            break;
    }
    USBSendBuffer[4] = 'F';
    USBSendBuffer[5] = '=';
    USBSendBuffer[6] = (char) (pParam->Frequence / 1000);
    USBSendBuffer[7] = (char) ((pParam->Frequence / 100) % 10);
    USBSendBuffer[8] = (char) ((pParam->Frequence / 10) % 100);
    USBSendBuffer[9] = (char) (pParam->Frequence % 1000);
    USBSendBuffer[10] = 'A';
    USBSendBuffer[11] = '=';
    USBSendBuffer[12] = (char) (pParam->Amplitude / 10000);
    USBSendBuffer[13] = (char) ((pParam->Amplitude / 1000) % 10);
    USBSendBuffer[14] = (char) ((pParam->Amplitude / 100) % 100);
    USBSendBuffer[15] = (char) ((pParam->Amplitude / 10) % 1000);
    USBSendBuffer[16] = (char) (pParam->Amplitude % 10000);
    USBSendBuffer[17] = 'O';
    USBSendBuffer[18] = '=';
    if (pParam->Offset >= 0)
    {
        USBSendBuffer[19] = '+';
    }
    else
    {
        USBSendBuffer[19] = '-';
    }
    USBSendBuffer[20] = (char) (pParam->Offset / 1000);
    USBSendBuffer[21] = (char) ((pParam->Offset / 100) % 10);
    USBSendBuffer[22] = (char) ((pParam->Offset / 10) % 100);
    USBSendBuffer[23] = (char) (pParam->Offset % 1000);
    USBSendBuffer[24] = 'W';
    USBSendBuffer[25] = 'P';
    USBSendBuffer[26] = (char) Saved;
    USBSendBuffer[27] = '#';
} // SendMessage

