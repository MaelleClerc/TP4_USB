// Tp3  manipulation MenuGen avec PEC12
// C. HUBER  10/02/2015 pour SLO2 2014-2015
// Fichier MenuGen.c
// Gestion du menu  du g�?©n�?©rateur
// Traitement cyclique �?  10 ms



#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "MenuGen.h"
#include "Generateur.h"
#include "Mc32NVMUtil.h"
#include "DefMenuGen.h"
#include "Mc32DriverLcd.h"
#include "GesPec12.h"
#include "bsp.h"

#define TIMER_LCD_SAUVGARDE 100
#define TIMER_S9_SAVE 199

E_MENU SELECTION_MENU;
S_No_save Val;
// Structure pour les traitement du Pec12
S_Pec12_Descriptor Pec12;
S_S9_Descriptor S9;


//d�claration constate tableau
const char tb_MenuFormes [4] [21] = { "Sinus", "Triangle", "DentDeScie", "Carre" };

uint16_t static MAJ_LCD_Menu;

// Initialisation du menu et des parametres
void MENU_Initialize(S_ParamGen *pParam)
{   

    // INIT valeur pour l'affichage
    Val.Forme = pParam->Forme;
    Val.Frequence = pParam->Frequence;
    Val.Amplitude = pParam->Amplitude;
    Val.Frequence = pParam->Frequence;
   
    
    //menu principale
    Menu_interface(pParam);
    
    //initaliser premiemiere parametre a  pointer dans le menu
    SELECTION_MENU = MENU_FORME;  
}

//gere l'affichage LCD pour le menu principal
void Menu_interface(S_ParamGen *pParam)
{
    //clear LCD
    Clear_LCD();
    //GERER L'AFFICHAGE DU MENU 
    //Ligne 1
    lcd_bl_on();    
    lcd_gotoxy(2,1);
    printf_lcd("Forme");         
    lcd_gotoxy(11,1);
    printf_lcd("%s", tb_MenuFormes[pParam->Forme]);
    
    //ligne 2
    lcd_gotoxy(2,2);    
    printf_lcd("Freq[Hz]"); 
    lcd_gotoxy(13,2);
    printf_lcd("%d", pParam->Frequence);
    
    //ligne 3
    lcd_gotoxy(2,3);    
    printf_lcd("Ampl[mV]"); 
    lcd_gotoxy(13,3);
    printf_lcd("%d", pParam->Amplitude);
    
    //ligne 4
    lcd_gotoxy(2,4);    
    printf_lcd("Offset[mV]");  
    lcd_gotoxy(13,4);
    printf_lcd("%d", (int)pParam->Offset);
    
    //initaliser premiemiere parametre a  pointer dans le menu
    SELECTION_MENU = MENU_FORME;  
}


// Execution du menu, appel cyclique depuis l'application
void MENU_Execute(S_ParamGen *pParam, bool Local)
{ 
    //initalisation des variable
       
    if (Local == 0)
    {
        if (Local == 1)
        {
            Menu_DemandeSave();
        }
        else 
        {
            Menu_interface(pParam);
            //ajouter les # aux d�but des 24 ligne
            Pt_AffichageRemote();
        }          
    }                      
    
    else
    {
        //mettre � jour l'affichage si le menu de sauvegarde a �t� activ�
        if (MAJ_LCD_Menu == 0)
        {
            /*gestion de l'affichage avec le PEG12*/
            Menu_GESTION_PEG12(pParam);    
        }
        else 
        {
           
            //menu principale
            Menu_interface(pParam);
            /*gestion de l'affichage avec le PEG12*/
            Menu_GESTION_PEG12(pParam);  
            //menu mis � jour
            MAJ_LCD_Menu = 0;

        }
    }
}    


/*Design menu de sauvgade*/
void Menu_DemandeSave()
{
    //executer 1 seul fois
    if(MAJ_LCD_Menu == 0)
    {
       // //gestion LCD// //
        //clear LCD
        Clear_LCD();
        //ecrire sur l'affichage
        lcd_gotoxy(6,2);    
        printf_lcd("Sauvegarde"); //ligne 2
        lcd_gotoxy(9,3);    
        printf_lcd("OK!"); //ligne 3 
        
        //enregistrer les param�tre re�u//
        
    }
    //ne plus remettre � jour l'affichage save
    MAJ_LCD_Menu = 1;
    
}

void Pt_AffichageRemote()
{
    //ecrire sur la 1ere ligne de la 1ere colonne "#" au LCD
    lcd_gotoxy(1,1);
    printf_lcd("#");
    //ecrire sur la 2ere ligne de la 1ere colonne "#" au LCD
    lcd_gotoxy(1,2);
    printf_lcd("#");
    //ecrire sur la 3ere ligne de la 1ere colonne "#" au LCD
    lcd_gotoxy(1,3);
    printf_lcd("#");
    //ecrire sur la 4ere ligne de la 1ere colonne "#" au LCD
    lcd_gotoxy(1,4);
    printf_lcd("#");
}
/*Design menu de sauvgade OK*/
/*void Sauvgarde_OK()
{
    if(MAJ_LCD_SAVE == 1)
    {
        //clear LCD
        Clear_LCD();
        //ecrire sur l'affichage
        lcd_gotoxy(6,2);    
        printf_lcd("Sauvegarde"); //ligne 2
        
    }
    //ne plus remettre � jour l'affichage save OK
    MAJ_LCD_SAVE = 0;
    //mettre � jour le LCD
    MAJ_LCD_Menu = 1;    
}*/

/*Design menu de sauvgade ANNULER*/
/*void Sauvgarde_ANNULE()
{
    if(MAJ_LCD_SAVE == 1)
    {
        //clear LCD
        Clear_LCD();
        //ecrire sur l'affichage    
        lcd_gotoxy(6,2);    
        printf_lcd("Sauvegarde"); //ligne 2
        lcd_gotoxy(6,3);    
        printf_lcd("ANNULEE!"); //ligne 3 
    }
    //ne plus remettre � jour l'affichage save OK
    MAJ_LCD_SAVE = 0;
    //mettre � jour le LCD
    MAJ_LCD_Menu = 1; 
}*/


/*Supprimer toutes les ligne du LCD*/
void Clear_LCD()
{
    lcd_ClearLine(1);
    lcd_ClearLine(2);
    lcd_ClearLine(3);
    lcd_ClearLine(4);
}


/*gestion de l'affichage avec le PEG12*/
void Menu_GESTION_PEG12(S_ParamGen *pParam)
{
   switch (SELECTION_MENU)
    {
        //Menu Forme//
        case MENU_FORME:
        {
            //ecrire sur la premiere ligne de la premiere colonne "*" du LCD
            lcd_gotoxy(1,1);
            printf_lcd("*");

            //Tester si une touche est active
            if(Pec12IsPlus()||Pec12IsMinus()||Pec12IsOK())
            {
                //incrementer choix du menu
                if (Pec12IsPlus())
                {
                    //modifier la sélection du menu
                    SELECTION_MENU = MENU_FREQU;
                }
                //decrementer choix du menu
                else if (Pec12IsMinus())
                {
                    //modifier la sélection du menu
                    SELECTION_MENU = MENU_OFFSET;
                }
                //Valider le choix
                else if (Pec12IsOK())
                {
                    //modifier la valeur 
                    SELECTION_MENU = MENU_FORME_VALEUR;
                }
                //effacer "*"
                lcd_gotoxy( 1, 1);
                printf_lcd(" ");
            }

        break;       
        }

        // Menu modifier la Forme de signal //
        case MENU_FORME_VALEUR:
        {
            lcd_gotoxy(1,1);
            printf_lcd("?");

            //Tester si une touche est active
            if(Pec12IsPlus()||Pec12IsMinus())
            {
                //Test si incrementer la forme
                if (Pec12IsPlus())
                {
                    //test si egal a la Singnal carree
                    if(Val.Forme == 3)
                    {
                        Val.Forme = 0;
                    }
                    //sinon incrementer pour obtenir la nouvelle forme
                    else
                    {
                        Val.Forme ++;
                    }  
                }
                //decrementer la frome
                else
                {
                    //test si egal Signal Sinus
                    if(Val.Forme ==  0)
                    {
                        Val.Forme = 3;
                    }
                    //sinon decrementer pour obtenir la nouvelle forme
                    else
                    {
                        Val.Forme --;
                    }
                }
                //supprimer les caract�re sur la 2ere ligne
                lcd_ClearLine(1);
                //afficher "?Forme"
                lcd_gotoxy(1,1);
                printf_lcd("?Forme");
                //afficher valeur
                lcd_gotoxy(11,1);
                printf_lcd("%s", tb_MenuFormes[Val.Forme]);
            }
            //Tester si une touche est active
            if(Pec12IsOK()||Pec12IsESC())
            {
                //si on appuye sur ok
                if (Pec12IsOK())
                {
                    //sauvgarder la nouvelle valeur
                    pParam->Forme = Val.Forme; 
                    //Mettre a jour forme signal
                    GENSIG_UpdateSignal(pParam);
                }
                //si on appuye sur esc, retourne sur affichage principal et garde l'ancienne val en memoire                    
                else 
                {
                    //R�cuperer la valeur de base
                    Val.Forme = pParam->Forme;
                }
                //GESTION AFFICHAGE//
                //supprimer les caract�re sur la 2ere ligne
                lcd_ClearLine(1);
                //afficher "*Forme"
                lcd_gotoxy(1,1);
                printf_lcd("*Forme"); 
                //afficher valeur
                lcd_gotoxy(11,1);
                printf_lcd("%s", tb_MenuFormes[Val.Forme]);

                //retourner sur le menu selection 
                SELECTION_MENU = MENU_FORME;
            }
        break;       
        }

        // Menu Frequence //
        case MENU_FREQU:
        {
            //ecrire sur la 2eme ligne de la premiere colonne "*" du LCD
            lcd_gotoxy(1,2);
            printf_lcd("*");
            //Tester si une touche est active
            if(Pec12IsPlus()||Pec12IsMinus()||Pec12IsOK())
            {
                //incrementer choix du menu
                if (Pec12IsPlus())
                {  
                    //modifier la sélection du menu
                    SELECTION_MENU = MENU_AMPLI;
                }
                //decrementer choix du menu
                else if (Pec12IsMinus())
                {
                    //modifier la sélection du menu
                    SELECTION_MENU = MENU_FORME;
                }
                //Valider le choix
                else if (Pec12IsOK())
                {
                    //modifier la valeur
                    SELECTION_MENU = MENU_FREQU_VALEUR;
                }
                //effacer "*"
                lcd_gotoxy( 1, 2);
                printf_lcd(" ");
            }
        break;       
        }
        // Menu modifier la frequence du signal //
        case MENU_FREQU_VALEUR:
        {
            //afiicher un "?" sur la 2 eme ligne, la premiere colonne
            lcd_gotoxy(1,2);
            printf_lcd("?");


            //Tester si une touche est active
            if(Pec12IsPlus()||Pec12IsMinus())
            {
                if (Pec12IsPlus())
                {
                    //test si superieur ou egal a la frequence max
                    if(Val.Frequence >= 2000 )
                    {
                        Val.Frequence = 20;                        
                    }
                    //sinon incrementer par pas de 20
                    else
                    {
                        Val.Frequence = Val.Frequence +20;
                    }                        
                }

                //decrementer la valeur de la frequence
                else
                {
                    //test si inferieur ou égal a la frequence min
                    if(Val.Frequence <= 20 )
                    {
                        Val.Frequence = 2000;
                    }
                    //sinon decrementer par pas de 20
                    else
                    {
                        //recuperer l'ancienne valeur
                        Val.Frequence = Val.Frequence -20;
                    }
                }
                //GESTION AFFICHAGE//
                //supprimer les caract�re sur la 2ere ligne
                lcd_ClearLine(2);
                //afficher "?Freq[Hz]"
                lcd_gotoxy(1,2);
                printf_lcd("?Freq[Hz]");
                //afficher valeur
                lcd_gotoxy(13,2);
                printf_lcd("%d", Val.Frequence);
            }
            //Tester si une touche est active
            if(Pec12IsOK()||Pec12IsESC())
            {
                //si on appuye sur ok
                if (Pec12IsOK())
                {
                    //sauvgarder la nouvelle valeur 
                    pParam->Frequence = Val.Frequence;
                    GENSIG_UpdatePeriode(pParam);
                }
                //si on appuye sur esc, retourne sur affichage principal et garde l'ancienne val en memoire
                else 
                {
                    Val.Frequence = pParam->Frequence;
                }
                //GESTION AFFICHAGE//
                //supprimer les caract�re sur la 2ere ligne
                lcd_ClearLine(2);
                //afficher "?Freq[Hz]"
                lcd_gotoxy(1,2);
                printf_lcd("*Freq[Hz]"); 
                //afficher valeur
                lcd_gotoxy(13,2);
                printf_lcd("%d", Val.Frequence);
                //retourner sur le menu selection 
                SELECTION_MENU = MENU_FREQU;
            }


        break;       
        }
        // Menu Amplification //
        case MENU_AMPLI:
        {
            //ecrire sur la 3eme ligne de la premiere colonne "*" du LCD
            lcd_gotoxy(1,3);
            printf_lcd("*");
            //Tester si une touche est active
            if(Pec12IsPlus()||Pec12IsMinus()||Pec12IsOK())
            {
                //incrementer choix du menu
                if (Pec12IsPlus())
                {
                    //modifier la sélection du menu
                    SELECTION_MENU = MENU_OFFSET;
                }
                //decrementer choix du menu
                else if (Pec12IsMinus())
                {
                    //modifier la sélection du menu
                    SELECTION_MENU = MENU_FREQU;
                }
                //Valider le choix
                else if (Pec12IsOK())
                {  
                    //modifier la valeur
                    SELECTION_MENU = MENU_AMPLI_VALEUR;
                }
                //effacer "*"
                lcd_gotoxy( 1, 3);
                printf_lcd(" ");
            }
        break;       
        }
        // Menu modifier l'amplitude du signal //
        case MENU_AMPLI_VALEUR:
        {
            //afiicher un "?" sur la 3 eme ligne, 1a 1 ere colonne
            lcd_gotoxy(1,3);
            printf_lcd("?");

            //Tester si une touche est active
            if(Pec12IsPlus()||Pec12IsMinus())
            {
                //incrementer la valeur de l'amplitude 
                if (Pec12IsPlus())
                {
                    //test si superieur ou egal a  l'amplitude max
                    if(Val.Amplitude >= 10000 )
                    {
                        Val.Amplitude = 0;
                    }
                    //sinon incrementer par pas de 100
                    else
                    {
                        Val.Amplitude = Val.Amplitude +100;
                    } 
                }
                //decrementer la valeur de l'amplitude 
                else
                {
                    //test si inferieur ou egal e  l'amplitude min
                    if(Val.Amplitude <= 0 )
                    {
                        Val.Amplitude = 10000;
                    }
                    //sinon decrementer par pas de 100
                    else
                    {
                        Val.Amplitude = Val.Amplitude -100;
                    }
                }
                //GESTION AFFICHAGE//
                //supprimer les caract�re sur la 3ere ligne
                lcd_ClearLine(3);
                //afficher "?Ampl[mV]"
                lcd_gotoxy(1,3);
                printf_lcd("?Ampl[mV]");
                //afficher valeur
                lcd_gotoxy(13,3);
                printf_lcd("%3d", Val.Amplitude);
            }
            //Tester si une touche est active
            if(Pec12IsOK()||Pec12IsESC())
            {
                //si on appuye sur ok
                if (Pec12IsOK())
                {
                    //sauvgarder la nouvelle valeur 
                     pParam->Amplitude = Val.Amplitude;

                    //mettre � jour l'amplitude du signal
                    GENSIG_UpdateSignal(pParam);

                }
                //si on appuye sur esc, retourne sur affichage principal et garde l'ancienne val en memoire                     
                else 
                {
                    //R�cuperer la valeur de base
                    Val.Amplitude = pParam->Amplitude;
                }
                //GESTION AFFICHAGE//
                //supprimer les caract�re sur la 2ere ligne
                lcd_ClearLine(3);
                //afficher "*Ampl[mV]"
                lcd_gotoxy(1,3);
                printf_lcd("*Ampl[mV]"); 
                //afficher valeur
                lcd_gotoxy(13,3);
                printf_lcd("%d", Val.Amplitude);

                //retourner sur la selection de l'amplitude
                SELECTION_MENU = MENU_AMPLI;
            }

        break;       
        }

        // Menu OFFSET //
        case MENU_OFFSET:
        {
            //ecrire sur la 4eme ligne de la premiere colonne "*" du LCD
            lcd_gotoxy(1,4);
            printf_lcd("*");

            //Tester si une touche est active
            if(Pec12IsPlus()||Pec12IsMinus()||Pec12IsOK())
            {
                //incrementer choix du menu
                if (Pec12IsPlus())
                {
                    //modifier la sélection du menu
                    SELECTION_MENU = MENU_FORME;
                }
                //decrementer choix du menu
                else if (Pec12IsMinus())
                {
                    //modifier la sélection du menu
                    SELECTION_MENU = MENU_AMPLI;
                }
                //Valider le choix
                else if (Pec12IsOK())
                {
                    //modifier la valeur
                    SELECTION_MENU = MENU_OFFSET_VALEUR;
                }
                //effacer "*"
                lcd_gotoxy( 1, 4);
                printf_lcd(" ");
            }

        break;       
        }
        // Menu modifier l'offset du signal //
        case MENU_OFFSET_VALEUR:
        {
            //afiicher un "?" sur la 4 eme ligne, 1 ere colonne
            lcd_gotoxy(1,4);
            printf_lcd("?");

            //Tester si une touche est active
            if(Pec12IsPlus()||Pec12IsMinus())
            {

                //incrementer la valeur de l'offset 
                if (Pec12IsPlus())
                {
                    //test si supperieur ou egal e l'offset max
                    if(Val.Offset >= 5000 )
                    {
                        Val.Offset = (5000);
                    }
                    //sinon incrementer par pas de 100
                    else
                    {
                        Val.Offset = (Val.Offset +100);
                    } 
                }
                //decrementer la valeur de l'offset
                else
                {
                    //test si inferieur ou egal a  l'offset min
                    if(Val.Offset <= -5000 )
                    {
                        Val.Offset = (-5000);
                    }
                    //sinon decrementer par pas de 100
                    else
                    {
                        Val.Offset = (Val.Offset -100);
                    }
                }
                //GESTION AFFICHAGE//
                //supprimer les caract�re sur la 4ere ligne
                lcd_ClearLine(4);
                //afficher "?Offset[mV]"
                lcd_gotoxy(1,4);
                printf_lcd("?Offset[mV]");
                //afficher valeur
                lcd_gotoxy(13,4);
                printf_lcd("%d", Val.Offset);
            }
            if(Pec12IsOK()||Pec12IsESC())
            {
                //si on appuye sur ok
                if (Pec12IsOK())
                {

                    //sauvgarder la nouvelle valeur 
                    pParam->Offset = Val.Offset;

                    //mettre � jour l'offset du signal
                    GENSIG_UpdateSignal(pParam);
                }
                //si on appuye sur esc, retourne sur affichage principal et garde l'ancienne val en memoire 
                else 
                {
                    //R�cuperer la valeur de base
                    Val.Offset = pParam->Offset;
                }
                //GESTION AFFICHAGE//
                //supprimer les caract�re sur la 4ere ligne
                lcd_ClearLine(4);
                //afficher "*Offset[mV]"
                lcd_gotoxy(1,4);
                printf_lcd("*Offset[mV]");
                //afficher valeur
                lcd_gotoxy(13,4);
                printf_lcd("%d", Val.Offset);

                //retourner sur la selection de l'offset
                SELECTION_MENU = MENU_OFFSET;
            }
        break;       
        } 
    
    }
//mettre � 0 les valeurs du PEG12
    Pec12ClearOK();
    Pec12ClearESC();
    Pec12ClearMinus();
    Pec12ClearPlus();
}
    
    



