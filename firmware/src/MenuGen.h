#ifndef MenuGen_h
#define MenuGen_h

// Tp3  manipulation MenuGen avec PEC12
// C. HUBER  03.02.2016
// Fichier MenuGen.h
// Gestion du menu  du générateur
// Traitement cyclique à 1 ms du Pec12


#include <stdbool.h>
#include <stdint.h>
#include "DefMenuGen.h"

//enumeration gestion aafichage
typedef enum  { MENU_FORME, MENU_FORME_VALEUR, MENU_FREQU, MENU_FREQU_VALEUR, MENU_AMPLI, MENU_AMPLI_VALEUR, MENU_OFFSET, MENU_OFFSET_VALEUR} E_MENU;

//structure pour les valeur pas encore sauvgarder
typedef struct {
      uint8_t Forme;
      uint16_t Frequence;
      uint16_t Amplitude;
      int16_t Offset;

} S_No_save;

typedef struct {
            bool SAVE : 1;             // événement incrément
            bool REFRESH  : 1;             // événement décrément
} S_Flag;

////////////*prototype de fonction*//////////
// Initialisation du menu et des parametres
void MENU_Initialize(S_ParamGen *pParam);
//gere l'affichage LCD pour le menu principal
void Menu_interface(S_ParamGen *pParam);
// Execution du menu, appel cyclique depuis l'application
void MENU_Execute(S_ParamGen *pParam, bool Local);
/*Design menu de sauvgade*/
void Menu_Save();

//afficge le #devant les 4 lignes
void Pt_AffichageRemote();
/*Supprimer toutes les ligne du LCD*/
void Clear_LCD();
/*gestion de l'affichage avec le PEG12*/
void Menu_GESTION_PEG12(S_ParamGen *pParam);      

//flag Save
bool  Flag_Save_OK(void);

void FlagSave_Clear(void);

bool  Flag_RefreshLCD(void);

void FlagRefreshLCD_Clear(void);

void MAJ_Valeur (S_ParamGen *pParam);


#endif




  
   







