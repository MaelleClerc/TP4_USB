#ifndef Generateur_h
#define Generateur_h

// TP3 MenuGen 2016
// C. HUBER  03.02.2016
// Fichier Generateur.h
// Prototypes des fonctions du g�n�rateur  de signal


#include <stdint.h>
#include "DefMenuGen.h"


typedef struct {
            uint16_t Nb_Tic;
            uint16_t Min;             // Amplitude min
            uint16_t Max;             // Amplitude max  
            
} S_Amplitude;

// Initialisation du  g�n�rateur
void  GENSIG_Initialize(S_ParamGen *pParam);


// Mise � jour de la periode d'�chantillonage
void  GENSIG_UpdatePeriode(S_ParamGen *pParam);


// Mise � jour du signal (forme, amplitude, offset)
void  GENSIG_UpdateSignal(S_ParamGen *pParam);

// A appeler dans int Timer3
void  GENSIG_Execute(void);


#endif