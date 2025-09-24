/*
 * FlashDrv.c
 *
 *  Created on: Sep 24, 2025
 *      Author: user
 */


#include "..\Application\StructDef.h"

#pragma CODE_SECTION(EraseAllUserSectors,"ramfuncs");

short EraseAllUserSectors(void)
{
    //
    // Example_CallFlashAPI - This function will interface to the flash API.
    //
    // Parameters:
    // Return Value:
    // Notes:  This function will be executed from SARAM
    //
    Uint16  Status;
    Uint16  VersionHex;     // Version of the API in decimal encoded hex
    FLASH_ST FlashStatus;

    VersionHex = Flash_APIVersionHex();
    if(VersionHex != 0x0100)
    {
        //
        // Unexpected API version
        // Make a decision based on this info.
        //
        asm("    ESTOP0");
        return -2;
    }
    //
    Status = Flash_Erase((SECTORB|SECTORC|SECTORD|SECTORE|SECTORF|SECTORG|
                          SECTORH),&FlashStatus);
    if(Status != STATUS_SUCCESS)
    {
        return -1;
    }

    return 0 ;
}


#pragma CODE_SECTION(ProgramIntoSector,"ramfuncs");
short ProgramIntoSector(short unsigned sector , short unsigned offset , short unsigned * Buffer , short unsigned Length)
{
    Uint16 *Flash_ptr;
    FLASH_ST FlashStatus;
    Uint16 Status ;
    Flash_ptr = (Uint16 *) Sector[sector].StartAddr + offset;
    Status = Flash_Program(Flash_ptr,(Uint16 *)Buffer+ offset,(long unsigned) Length,&FlashStatus);
    if(Status != STATUS_SUCCESS)
    {
        return -1 ;
    }

    //
    // Verify the values programmed.  The Program step itself does a verify
    // as it goes.  This verify is a 2nd verification that can be done.
    //
    Status = Flash_Verify(Flash_ptr,(Uint16 *)Buffer+ offset,Length,&FlashStatus);
    if(Status != STATUS_SUCCESS)
    {
        return -1 ;
    }
    return 1 ;
}
