/*
 * StructDef.h
 *
 *  Created on: Aug 30, 2025
 *      Author: user
 */

#ifndef APPLICATION_STRUCTDEF_H_
#define APPLICATION_STRUCTDEF_H_

#include "..\f2806x\MWare\boot_loader\Flash2806x_API_Library.h"
#include "..\Drivers\DSP28x_Project.h"     // Device Header file and Examples Include File
#include "..\Drivers\Flash2806x_API.h"
#include "..\Drivers\Drivers.h"     // Device Header file and Examples Include File
#include "..\f2806x\common\include\F2806x_Examples.h"
#include "constdef.h"
//#include "Example_Flash2806x_API.h"
#include <stdio.h>

#include "../Drivers/CanDrv.h"

#ifndef SET_GLOBAL
#ifndef VARS_OWNER
#define SET_GLOBAL extern
#else
#define SET_GLOBAL
#endif
#endif



typedef void (*VoidFun)(void) ;

SET_GLOBAL struct
{
    long  unsigned password  ;
    short unsigned DefaultDeviceAddress  ;
    short unsigned ClaimedDeviceAddress  ;
} BootInfo ;


//
// Sector address info
//
typedef struct
{
    Uint16 *StartAddr;
    Uint16 *EndAddr;
} SECTOR;



#ifdef VARS_OWNER
#pragma DATA_SECTION(BootInfo,".bootinfo");
#pragma DATA_SECTION(ProgramBuffer,".progbuffer")
#pragma DATA_SECTION(ProgramBufferDirty,".dirtybuffer") ;


SECTOR Sector[8]= {
         (Uint16 *) 0x3D8000,(Uint16 *) 0x3DBFFF,
         (Uint16 *) 0x3DC000,(Uint16 *) 0x3DFFFF,
         (Uint16 *) 0x3E0000,(Uint16 *) 0x3E3FFF,
         (Uint16 *) 0x3E4000,(Uint16 *) 0x3E7FFF,
         (Uint16 *) 0x3E8000,(Uint16 *) 0x3EBFFF,
         (Uint16 *) 0x3EC000,(Uint16 *) 0x3EFFFF,
         (Uint16 *) 0x3F0000,(Uint16 *) 0x3F3FFF,
         (Uint16 *) 0x3F4000,(Uint16 *) 0x3F7FFF,
};




const short unsigned Verse[64] =
        { 73    ,110  ,  32  , 116  , 104 ,  101  ,  32 ,   98 ,  101 ,  103 ,  105  , 110  , 110 ,  105 ,  110  , 103 ,   32 ,   71,
          111   ,100  ,  32  ,  99  , 114 ,  101  ,  97 ,  116 ,  101 ,  100 ,   32  , 116  , 104 ,  101 ,   32  , 104 ,  101 ,   97,
          118   ,101  , 110  ,  32  ,  97 ,  110  , 100 ,  32  ,  116 ,  104 ,  101  ,  32  , 101 ,   97 ,  114  , 116 ,  104 ,   46,
          0,0,0,0,0,0,0,0,0,0} ;

#else
    extern const short unsigned Verse[];
    extern SECTOR Sector[] ;
#endif


typedef struct
{
    long unsigned Id ;
    short unsigned DLC ;
    long unsigned data[2] ;
} CanMsg_T  ;


typedef struct
{
    short unsigned Priority ;
    short unsigned DataPage ;
    short unsigned PduFormat ;
    short unsigned PduSpecific ;
    short unsigned SourceAddress ;
} J1939Id_T ;


typedef struct
{
     short unsigned State ;
     short unsigned Erased ;
     short unsigned NextSector ;
     short unsigned NextRelAddres ;
     short unsigned NextLength    ;
     long  unsigned Adler32CS     ;
     short unsigned SenderAddress ;
}LoadeState_T;

SET_GLOBAL LoadeState_T LoaderState ;

SET_GLOBAL short unsigned CanId ;
SET_GLOBAL short unsigned J1939ProtocolEnable  ;
SET_GLOBAL short unsigned LoadProtocolEnable  ;


SET_GLOBAL short unsigned ProgramBuffer[16384] ;
SET_GLOBAL short unsigned ProgramBufferDirty[1024] ;


// Define the functions. Always last as data structures must be predefined
#include "functions.h"

#endif /* APPLICATION_STRUCTDEF_H_ */
