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

#include "..\Drivers\CanComm.h"

#ifndef SET_GLOBAL
#ifndef VARS_OWNER
#define SET_GLOBAL extern
#else
#define SET_GLOBAL
#endif
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

SET_GLOBAL short unsigned CanId ;
SET_GLOBAL short unsigned J1939ProtocolEnable  ;

// Define the functions. Always last as data structures must be predefined
#include "functions.h"

#endif /* APPLICATION_STRUCTDEF_H_ */
