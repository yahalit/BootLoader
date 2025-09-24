/*
 * Functions.h
 *
 *  Created on: Aug 30, 2025
 *      Author: user
 */

#ifndef APPLICATION_FUNCTIONS_H_
#define APPLICATION_FUNCTIONS_H_

// LowLevel.c
void SetPLL(void );
void  InitPeripherals(void);
void RebootViaWatchdog(void);

// CAN module
void SetupCan(long unsigned CanId_UL );
short GetNextId29Message(CanMsg_T *Msg );
short TxMessage(CanMsg_T * _TxMsg, short unsigned extended) ;
short GetNextId11Message(CanMsg_T *Msg );
void WaitTx29Complete( long unsigned usec);

// J1939 module
void J1939Handler(void);
void InterpretJ1939 (CanMsg_T *msg , J1939Id_T *Id );


// Flash drive
short EraseAllUserSectors(void);
short ProgramIntoSector(short unsigned sector , short unsigned offset , short unsigned * Buffer , short unsigned Length) ;


// Loader protocol
void InitLoader() ;

#endif /* APPLICATION_FUNCTIONS_H_ */
