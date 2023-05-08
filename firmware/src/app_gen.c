/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app_gen.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "app_gen.h"
#include "Mc32DriverLcd.h"
#include "DefMenuGen.h"
#include "MenuGen.h"
#include "bsp.h"
#include "GesPec12.h"
#include "Generateur.h"
#include "Mc32gestSpiDac.h"
#include "Mc32gest_SerComm.h"
#include "app_USB.h"
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_GEN_DATA app_genData;
S_ParamGen LocalParamGen;
S_ParamGen RemoteParamGen;
APP_DATA appData;
//flag permettant d'initialiser l'ecran
uint8_t flag_tour = 1 ;
bool Local;
// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_GEN_Initialize ( void )

  Remarks:
    See prototype in app_gen.h.
 */

void APP_GEN_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
     APP_GEN_UpdateState(APP_GEN_STATE_INIT);

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP_GEN_Tasks ( void )

  Remarks:
    See prototype in app_gen.h.
 */

void APP_GEN_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( app_genData.state )
    {
        /* Application's initial state. */
        case APP_GEN_STATE_INIT:
        {
            //reliser l'affichage d�marage
            lcd_init();
            lcd_bl_on();
            lcd_gotoxy(1,1);
            printf_lcd("TP4 UsbGen 2023");
            lcd_gotoxy(1,2);
            printf_lcd("Caroline Mieville");
            lcd_gotoxy(1,3);
            printf_lcd("Maelle Clerc");
            
            // Init SPI DAC
            SPI_InitLTC2604();  

            // Initialisation du generateur
            GENSIG_Initialize(&LocalParamGen);
            //realiser le signal
            GENSIG_UpdateSignal(&LocalParamGen);
            //mettre � jour la p�riode
            GENSIG_UpdatePeriode(&LocalParamGen);
            
            //Init Pec12Init
            Pec12Init();
            
            /* initialisation des timers */
            DRV_TMR0_Start();
            DRV_TMR1_Start();  
            
            //Synchroniser les param�tres
            RemoteParamGen = LocalParamGen;
            
            
            APP_GEN_UpdateState(APP_GEN_STATE_WAIT);
            
            break;
        }

        case APP_GEN_STATE_SERVICE_TASKS:
        {
            //toggle la led2
            LED2_W = !LED2_R;
         //Afficher le nouvel affichage lors du premier tours
            if (flag_tour == 1)
            {
                // Initialisation du menu
                MENU_Initialize(&LocalParamGen);
                //remettre le flag � 0 car s'initailise une seul fois
                flag_tour = 0;
            }
            //execution menu
            if (USB_DETECT)
            {
                Local = 0;              
                if(GetMessage((int8_t *)appData.newStringReceived, &RemoteParamGen, 0))
                {
                    FlagSave_OK();
                }
                else
                {
                    //clear flag save se fait losque les valeurs ont �t� enregistr�
                    FlagSave_Clear();
                }
                //SendMessage((int8_t *)appData.readBuffer, &RemoteParamGen, Flag_Save_OK());
                MENU_Execute(&RemoteParamGen, Local);
            }
            else
            {   
                Local = 1;
                MENU_Execute(&LocalParamGen, Local);
            }
            
            //le prochaine �tat est: attente
            APP_GEN_UpdateState(APP_GEN_STATE_WAIT);
            //toogle la LED2
            LED2_W = !LED2_R;
            
        
            break;
        }
        case APP_GEN_STATE_WAIT:
        {
          
            break;
        }

        /* TODO: implement your application state machine.*/
        

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

void APP_GEN_UpdateState(APP_GEN_STATES newState)
{
    app_genData.state = newState;
}


 

/*******************************************************************************
 End of File
 */
