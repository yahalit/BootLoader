/*
 * CanComm.c
 *
 *  Created on: Sep 24, 2025
 *      Author: user
 */
#include "..\Application\StructDef.h"


/*-------------------------------------------------------------------------------
*
*   procedure\function name : InitCanModule
*
*   Description :
*       Initialize the data of a CAN module, and prepare it for communication.
*
*-----------------------------------------------------------------------------*/


void SetupCan(long unsigned CanId_UL )
{
    unsigned long RxMASK;
    Uint32 * pULong ;
    Uint16    cnt ;
    short unsigned i ;
    volatile struct ECAN_REGS * pCan ;
    volatile struct ECAN_MBOXES *pBoxes ;
    volatile struct MBOX *      BoxPtr;
    volatile union CANLAM_REG * LAMptr;

    volatile struct LAM_REGS * LAMRegs;

    RxMASK = (unsigned long) 0x00FFFFFF;   // 8 TX, 24 Rx


    // Setup the local peripheral address
    pCan     = (volatile struct ECAN_REGS *)    &ECanaRegs ;
    pBoxes   = (volatile struct ECAN_MBOXES *)  &ECanaMboxes;
    LAMRegs  = (volatile struct LAM_REGS *)     &ECanaLAMRegs;

    // Allow access to all registers
    EALLOW;

    pCan->CANMC.all = 0x00002020 ; // restart CAN module


    pCan->CANTIOC.all = CAN_IO_USE_PIN ;
    pCan->CANRIOC.all = CAN_IO_USE_PIN ;


    // Set all the registers to disable
    pCan->CANME.all = (Uint32)0x00000000 ;


    // Request configuration change
    pCan->CANMC.all = CANMC_CHANGE_CONFIG_REQUEST | CANMC_ECAN_MODE ; // request to start initialization mode, in CAN (not SCC) mode


      // wait until CAN allows init mode, CCE bit must be 1 in order to set timing
    while (!pCan->CANES.bit.CCE ){} // Wait up to one half of second.


// For   SYSCLKOUT   = 150 MHz, BaudRate = 0.5 MHz=150/2 / (9+1)/(1+2+10+1+1)
// Bit Rate = SYSCLKOUT / (1+BRPREG) / (3+TSEG1REG+TSEG2REG)
// BT=15 (Sampling Point = 80%)
//  ECanaRegs.CANBTC.bit.BRPREG = 9;
//  ECanaRegs.CANBTC.bit.TSEG2REG = 2;
//  ECanaRegs.CANBTC.bit.TSEG1REG = 10;
//  ECanaRegs.CANBTC.bit.SAM  = 1;
//  ECanaRegs.CANBTC.bit.SJWREG = 0;
//

    // 250 KBaud at 90MHz
    ECanaRegs.CANBTC.bit.BRPREG = (10-1);
    ECanaRegs.CANBTC.bit.TSEG2REG = (3-1);
    ECanaRegs.CANBTC.bit.TSEG1REG = 12-1;
    ECanaRegs.CANBTC.bit.SAM  = 1;
    ECanaRegs.CANBTC.bit.SJWREG = 1;

// Kill the contents of all the mailboxes
    pULong = (Uint32 *) & ( pBoxes->MBOX0 ) ;
    for ( cnt = 0 ; cnt < 32*4 ; cnt+=1 , pULong ++  )
    {
        *pULong=(Uint32)0x0 ;
    }

// Set mailbox directions
    pCan->CANMD.all = (long unsigned)RxMASK ;

// Setup the 11 bit message boxes
   for ( i = 0; i <  16 ; i++ )
   {
        BoxPtr = CanBoxNumToAddr(pBoxes, i);
        LAMptr = CanLAMNumToAddr(LAMRegs, i);

        BoxPtr->MSGID.all = MSGID_USE_ACCEPTANCE_MASK + (CanId_UL<< MSGID_11BIT_ADDRESS_SHIFT) ;
        LAMptr->all = ~(CanId_UL<< MSGID_11BIT_ADDRESS_SHIFT) ;
   }

// Note that ID appears in the PF field of the 29 bit, thus shifted by 8
   for ( i = 16 ; i < 24 ; i++ )
   {
        BoxPtr = CanBoxNumToAddr(pBoxes, i);
        LAMptr = CanLAMNumToAddr(LAMRegs, i);

        BoxPtr->MSGID.all = MSGID_29BIT + MSGID_USE_ACCEPTANCE_MASK + (CanId_UL<< 8 )   ;
        LAMptr->all = 0xffffffff  ; // All pass
   }

   // Set normal operation
    pCan->CANMC.all = CANMC_RUN_FREE | CANMC_ECAN_MODE | CANMC_WAKEUP_BY_ACTIVITY | CANMC_USE_LSB_FIRST | CANMC_AUTO_BUS_ON ;


 // wait until getting out from init mode to normal operation
    while (pCan->CANES.bit.CCE){} // wait half of second


    // Set all the mailboxes for use
    pCan->CANME.all = (unsigned long)0xFFFFFFFF;

  /*#### CANRFP REGISTER - Remote-Frame-Pending Register*/
    pCan->CANRFP.all = (unsigned long)0xFFFFFFFF ;

   // do not allow RX mail boxes to be overwritten.
    pCan->CANOPC.all = (unsigned long)0xFFFFFFFF;

    // Remove any possible transmit from the control field using
    pCan->CANTRR.all = (Uint32)(~RxMASK) ;
    pCan->CANAA.all = (Uint32)(~RxMASK); ;   // clearing AAIF bit (if enabled) and CANAA

    pCan->CANRMP.all = RxMASK; // Remove any pending Rx message

    EDIS ;  // Normal return - limit the access to privileged registers

    return; // 0 ;
}



/**
 * @brief Fetch the next received 11-bit (EXT) CAN message from ECAN mailboxes 0-15.
 *
 * Scans the ECAN Receive Message Pending register (CANRMP) for the first
 * pending message in mailboxes 0..15. If found, copies the message ID,
 * DLC, and payload into @p Msg and returns 1.
 *
 * @param[out] Msg  Destination for the extracted message (DLC, 11-bit Id, data words).
 *
 * @retval 1  A pending message was found and copied into @p Msg.
 * @retval 0  No pending message in mailboxes 16..31.
 *
 * @note
 *  - Expects the ECAN module to be configured and mailboxes 0..15 set as RX.
 *  - The 11-bit identifier is masked with 0x7F (standard ID only).
*/

short GetNextId11Message(CanMsg_T *Msg )
{

    volatile struct ECAN_REGS * pCan ;
    volatile struct ECAN_MBOXES *pBoxes ;
    volatile struct MBOX *      BoxPtr;

    short unsigned cnt ;


    // Setup the local peripheral address
    pCan     = (volatile struct ECAN_REGS *)    &ECanaRegs ;

    for ( cnt = 0 ; cnt < 15 ; cnt++)
    {
        if ( pCan->CANRMP.all & ( 1UL << cnt)  )
        {
            BoxPtr = CanBoxNumToAddr(pBoxes, cnt);
            Msg->DLC = BoxPtr->MSGCTRL.bit.DLC ;
            Msg->Id  = BoxPtr->MSGID.all & 0x7f ;
            Msg->data[0] = BoxPtr->MDL.all ;
            Msg->data[1] = BoxPtr->MDH.all ;
            pCan->CANRMP.all |= ( 1UL << cnt) ;
            return 1 ;
        }
    }
    return 0 ;
}



/**
 * @brief Fetch the next received 29-bit (EXT) CAN message from ECAN mailboxes 16–31.
 *
 * Scans the ECAN Receive Message Pending register (CANRMP) for the first
 * pending message in mailboxes 16..31. If found, copies the message ID,
 * DLC, and payload into @p Msg and returns 1.
 *
 * @param[out] Msg  Destination for the extracted message (DLC, 29-bit Id, data words).
 *
 * @retval 1  A pending message was found and copied into @p Msg.
 * @retval 0  No pending message in mailboxes 16..31.
 *
 * @note
 *  - Expects the ECAN module to be configured and mailboxes 16..31 set as RX.
 *  - The 29-bit identifier is masked with 0x1FFFFFFF (EXT ID only).
*/

short GetNextId29Message(CanMsg_T *Msg )
{

    volatile struct ECAN_REGS * pCan ;
    volatile struct ECAN_MBOXES *pBoxes ;
    volatile struct MBOX *      BoxPtr;

    short unsigned cnt ;


    // Setup the local peripheral address
    pCan     = (volatile struct ECAN_REGS *)    &ECanaRegs ;

    for ( cnt = 16 ; cnt < 32 ; cnt++)
    {
        if ( pCan->CANRMP.all & ( 1UL << cnt)  )
        {
            BoxPtr = CanBoxNumToAddr(pBoxes, cnt);
            Msg->DLC = BoxPtr->MSGCTRL.bit.DLC ;
            Msg->Id  = BoxPtr->MSGID.all & 0x1fffffff ;
            Msg->data[0] = BoxPtr->MDL.all ;
            Msg->data[1] = BoxPtr->MDH.all ;
            pCan->CANRMP.all |= ( 1UL << cnt) ;
            return 1 ;
        }
    }
    return 0 ;
}

long unsigned J1939TxMask ;

/*
 * Wait transmission of the last J1939 message , then wait @usec to complete transmission over the line
 */
void WaitTx29Complete( long unsigned usec)
{
    long unsigned value ;
    while ( ECanaRegs.CANTRS.all &  J1939TxMask ) {} ;
    value = usec  ;
    CpuTimer1Regs.TIM.all = value ;
    while ( (long unsigned)CpuTimer1Regs.TIM.all < (value+2)) ;
}

/**
 * @brief Transmit a single J1939 (29-bit) CAN message using ECANA mailboxes 16–31.
 *
 * Scans TX mailboxes 16..31 for one that was previously used and has its
 * transmit-acknowledge (TA) bit set, clears the TA, loads the frame, and
 * requests transmission.
 *
 * @param[in]  _TxMsg   Pointer to a prepared CAN message: _TxMsg->Id holds the
 *                     29-bit J1939 identifier (no IDE bit), _TxMsg->data[0..1]
 *                     hold the 8 data bytes as two 32-bit words (little-endian).
 * @return 1 on successfully queueing the frame for transmission; 0 if no TX
 *         mailbox was available in the 16..31 range.
 *
 * @pre
 *  - ECANA module is initialized and enabled.
 *  - Mailboxes 16..31 are configured for transmit.
 *  - @c CanBoxNumToAddr() returns a valid pointer to the mailbox @p cnt.
 *  - Global interrupts / protection configured appropriately for mailbox access.
 *
 * @note
 *  - Sets DLC to 8 unconditionally (J1939 fixed-length payload). If you need
 *    variable DLC, set from @p _TxMsg instead.
 *  - Sets IDE (extended ID) and writes the 29-bit ID: (1UL<<31) | _TxMsg->Id.
 *  - Clears CANTA bit by writing '1' before reuse (device-specific behavior).
 *  - @c pBoxes must point to ECAN mailbox memory; if @c CanBoxNumToAddr does not
 *    use @c pBoxes, it can be left NULL.
 */
short TxMessage(CanMsg_T * _TxMsg, short unsigned extended )
{
    volatile struct ECAN_REGS * pCan ;
    volatile struct ECAN_MBOXES *pBoxes ;
    volatile struct MBOX *      BoxPtr;

    short unsigned cnt ;

    // Setup the local peripheral address
    pCan     = (volatile struct ECAN_REGS *)    &ECanaRegs ;

    for ( cnt = 24 ; cnt < 32 ; cnt++)
    {
        J1939TxMask = ( 1UL << cnt);
        if ( pCan->CANTRS.all &  J1939TxMask  )
        {
            // Set Transmit acknowledge
            pCan->CANTA.all |= J1939TxMask;

            // Wait Transmit acknowledge to null
            while ( pCan->CANTA.all  & J1939TxMask) {} ;

            // Get next box
            BoxPtr = CanBoxNumToAddr(pBoxes, cnt);

            // Transmission
            BoxPtr->MSGCTRL.bit.DLC = 8 ;
            BoxPtr->MSGID.all = ( extended ?  (1UL<<31) : 0 )  + _TxMsg->Id ;
            BoxPtr->MDL.all = _TxMsg->data[0] ;
            BoxPtr->MDH.all =  _TxMsg->data[1];
            pCan->CANTRS.all |= J1939TxMask ;
            return 1 ;
        }
    }
    return 0 ;    // Could not transmit
}


