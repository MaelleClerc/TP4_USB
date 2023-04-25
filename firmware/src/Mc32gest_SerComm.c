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
    
    // Traduction de la forme du signal
    switch (appData.newStringReceived[3])
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
    
    // Traduction de la frequence
    Frequence_recue = atoi(&appData.newStringReceived[6]);
    
    // Traduction de l'amplitude
    Amplitude_recue = atoi(&appData.newStringReceived[12]);
    
    // Traduction de l'offset
    Offset_recu = atoi(&appData.newStringReceived[19]);
    
    // Mise a jour des parametres de pParam
    pParam->Frequence = Frequence_recue;
    pParam->Amplitude = Amplitude_recue;
    pParam->Offset = Offset_recu;
    
    return atoi(&appData.newStringReceived[26]); 
} // GetMessage


// Fonction d'envoi d'un  message
// Rempli le tampon d'émission pour USB en fonction des paramètres du générateur
// Format du message
// !S=TF=2000A=10000O=+5000D=25WP=0#
// !S=TF=2000A=10000O=+5000D=25WP=1#    // ack sauvegarde



void SendMessage(int8_t *USBSendBuffer, S_ParamGen *pParam, bool Saved )
{
   // Code pour envoyer les données à l'EEprom en utilisant SPI
  // Ici, on suppose que les données sont stockées dans un tableau nommé "eepromData"
  // et que la fonction SPITransfer() est utilisée pour la communication SPI
  // On commence par sélectionner le circuit d'EEprom en abaissant sa broche de sélection
  //digitalWrite(EEPROM_CS_PIN, LOW);
  //SPITransfer(eepromData);
  // Puis on désélectionne le circuit d'EEprom en remontant sa broche de sélection
  //digitalWrite(EEPROM_CS_PIN, HIGH);
} // SendMessage

