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


// CAN module
void SetupCan(long unsigned CanId_UL );
short GetNextId29Message(CanMsg_T *Msg );
short TxJ1939Message(CanMsg_T * _TxMsg) ;


// J1939 module
void J1939Handler(void);
void InterpretJ1939 (CanMsg_T *msg , J1939Id_T *Id );
#endif /* APPLICATION_FUNCTIONS_H_ */
