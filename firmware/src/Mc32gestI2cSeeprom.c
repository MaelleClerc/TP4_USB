//--------------------------------------------------------
// Mc32gestI2cEEprom.C
//--------------------------------------------------------
// Gestion I2C de la SEEPROM du MCP79411 (Solution exercice)
//	Description :	Fonctions pour EEPROM MCP79411
//
//	Auteur 		: 	C. HUBER
//      Date            :       26.05.2014
//	Version		:	V1.0
//	Compilateur	:	XC32 V1.31
// Modifications :
//
/*--------------------------------------------------------*/



#include "Mc32gestI2cSeeprom.h"
#include "Mc32_I2cUtilCCS.h"
#include "DefMenuGen.h"
#include <stdbool.h>  // <cstdbool> en C++

// Définition pour MCP79411
#define MCP79411_EEPROM_R    0xAF         // MCP79411 address for read
#define MCP79411_EEPROM_W    0xAE         // MCP79411 address for write
// La EEPROM du 79411 est de 1 Kbits donc 128 octets
#define MCP79411_EEPROM_BEG   0x00         // addr. début EEPROM
#define MCP79411_EEPROM_END   0x7F         // addr. fin EEPROM

// Definitions du bus (pour mesures)
// #define I2C-SCK  SCL2/RA2      PORTAbits.RA2   pin 58
// #define I2C-SDA  SDa2/RA3      PORTAbits.RA3   pin 59




// Initialisation de la communication I2C et du MCP79411
// ------------------------------------------------

void I2C_InitMCP79411(void)
{
   bool Fast = false;
   i2c_init( Fast );
} //end I2C_InitMCP79411

// Ecriture d'un bloc dans l'EEPROM du MCP79411 
void I2C_WriteSEEPROM(S_ParamGen *SrcData)
{
    int i;
    uint8_t *pt_charStruct; 
    static uint16_t NbBytes = 14;
    
    //pointeur sur la structure // 
    pt_charStruct = (uint8_t *)SrcData;
   
    // Ecriture
    i2c_start();
    i2c_write(MCP79411_EEPROM_W);// Adresse de MPC79411 + Ecriture
    for(i = 0; i < NbBytes; i++)
    {
        if (i % 7 == 0)
        {
            i2c_start();
            i2c_write(MCP79411_EEPROM_W);
        }
        
        i2c_write(*pt_charStruct);
        pt_charStruct++; 
    }

    i2c_stop();
} // end I2C_WriteSEEPROM

// Lecture d'un bloc dans l'EEPROM du MCP79411
void I2C_ReadSEEPROM(S_ParamGen *DstData)
{
    int i;
    static uint16_t NbBytes = 14;
    uint8_t *pt_charStruct; 
    uint8_t ack;
    
    //pointeur sur la structure // 
    pt_charStruct = (uint8_t *)DstData;
    
   // Lecture
    do{
        i2c_start();
        ack = i2c_write(MCP79411_EEPROM_R); // Adresse de MPC79411 + Ecriture
    }while(!ack);
    
    for(i = 0; i < NbBytes; i++)
    {
        if (i % 7 == 0)
        {
            i2c_start();
            i2c_write(MCP79411_EEPROM_R);
        }
        
        *pt_charStruct = i2c_read(0);
        pt_charStruct++;
    }
    
    i2c_stop();

    
} // end I2C_ReadSEEPROM
   





 



