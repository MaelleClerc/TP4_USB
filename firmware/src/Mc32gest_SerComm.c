// Mc32Gest_SerComm.C
// fonction d'�mission et de r�ception des message
// transmis en USB CDC
// Canevas TP4 SLO2 2015-2015


#include "app.h"
#include "Mc32gest_SerComm.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// Fonction de reception  d'un  message
// Met � jour les param�tres du generateur a partir du message recu
// Format du message
//  !S=TF=2000A=10000O=+5000D=100W=0#
//  !S=PF=2000A=10000O=-5000D=100W=1#


bool GetMessage(int8_t *USBReadBuffer, S_ParamGen *pParam, bool *SaveTodo)
{
    // Code pour r�cup�rer les donn�es de l'USB en utilisant SPI
    // Ici, on suppose que les donn�es sont stock�es dans un tableau nomm� "usbData"
    // et que la fonction SPITransfer() est utilis�e pour la communication SPI
    SPITransfer(usbData);
  
} // GetMessage


// Fonction d'envoi d'un  message
// Rempli le tampon d'�mission pour USB en fonction des param�tres du g�n�rateur
// Format du message
// !S=TF=2000A=10000O=+5000D=25WP=0#
// !S=TF=2000A=10000O=+5000D=25WP=1#    // ack sauvegarde



void SendMessage(int8_t *USBSendBuffer, S_ParamGen *pParam, bool Saved )
{
   // Code pour envoyer les donn�es � l'EEprom en utilisant SPI
  // Ici, on suppose que les donn�es sont stock�es dans un tableau nomm� "eepromData"
  // et que la fonction SPITransfer() est utilis�e pour la communication SPI
  // On commence par s�lectionner le circuit d'EEprom en abaissant sa broche de s�lection
  digitalWrite(EEPROM_CS_PIN, LOW);
  SPITransfer(eepromData);
  // Puis on d�s�lectionne le circuit d'EEprom en remontant sa broche de s�lection
  digitalWrite(EEPROM_CS_PIN, HIGH);
} // SendMessage
