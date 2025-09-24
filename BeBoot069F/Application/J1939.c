/*
 * J1939.c
 *
 *  Created on: Sep 24, 2025
 *      Author: user
 */
#include "StructDef.h"




const CanMsg_T DloadResponseMsgTemplate = {.Id = (6UL<<26)+ (0xd8UL<<16), .DLC = 8, .data = {0xffff0000UL , 0xffffffffUL } }  ;
const CanMsg_T NameReportTemplate = {.Id = 0x18ee0000, .DLC = 8, .data = { J1939_DEVICE_NAME_LSDW , J1939_DEVICE_NAME_MSDW } }  ;
CanMsg_T TxMsg;




/**
 * @brief Decode a 29-bit J1939 extended identifier into its fields.
 *
 * Extracts Priority, Data Page (DP), PDU Format (PF), PDU Specific (PS),
 * and Source Address (SA) from a raw 29-bit CAN ID value.
 *
 * @param[in]  data  Raw 29-bit J1939 ID (in an unsigned long); only bits [28:0] are used.
 * @param[out] id    Output struct populated with decoded J1939 fields.
 *
 * @note Assumes an extended (29-bit) identifier per SAE J1939/21.
 */
void InterpretJ1939Id(unsigned long data, J1939Id_T *id)
{
    id->Priority      = (data >> 26) & 0x7;    /**< Priority:   bits [28:26] (3 bits)     */
    id->DataPage      = (data >> 24) & 0x1;    /**< Data Page:  bit  [24]    (1 bit)      */
    id->PduFormat     = (data >> 16) & 0xFF;   /**< PDU Format: bits [23:16] (8 bits, PF) */
    id->PduSpecific   = (data >>  8) & 0xFF;   /**< PDU Spec.:  bits [15:8]  (8 bits, PS) */
    id->SourceAddress = (data >>  0) & 0xFF;   /**< Src Addr:   bits [7:0]   (8 bits, SA) */
}




/**
 * @brief Interpret an incoming J1939 frame and emit required responses.
 * @param[in]  msg Pointer to received CAN/J1939 message (ID + data).
 * @param[out] Id  Decoded J1939 identifier fields (filled here).
 *
 * @note Exits early when not in J1939 mode, when DP!=0, or when the
 *       message is not addressed to this ECU. On specific PGNs, builds
 *       and transmits a response via TxJ1939Message().
 */
void InterpretJ1939 (CanMsg_T *msg , J1939Id_T *Id )
{
    if ( J1939ProtocolEnable == 0 )
    {
        return ; // Already in 11 bit mode
    }

    InterpretJ1939Id ( msg->Id , Id ) ;
    if ( Id->DataPage )
    {
        return ; // Not our
    }
    if ( Id->PduFormat == J1939_DLOAD_PDU_FORMAT )
    {
        if ( Id->PduSpecific != CanId )
        {
            return ; // Not for us
        }
        if ( msg->data[0] != 0x01070001 )
        {
            return ; // Bad request
        }

        // Fire a response
        TxMsg = DloadResponseMsgTemplate ;
        TxMsg.Id |= ((CanId<<8) | Id->SourceAddress) ;

        // Wait response transmission complete
        TxJ1939Message( &TxMsg) ;

        // Kill J1939 protocol
        J1939ProtocolEnable = 0 ;
    }

    if ( Id->PduFormat == J1939_ADDRESS_CLAIM_PDU_FORMAT )
    {
        if ( ( Id->SourceAddress == 0x32) && ( Id->PduSpecific == CanId )  && ( TxMsg.data[0] == 0xff00ee00 ))
        {
            // Transmit our name
            TxMsg = NameReportTemplate ;
            TxMsg.Id |= CanId ;
        }

        // Wait response transmission complete
        TxJ1939Message(& TxMsg) ;

    }

}


/*
 * Before completing the identification mode, scan J1939 messages for
 * - Name report (following fictitious address claim)
 * - DM14 download request
 */
void J1939Handler(void)
{
// Mailboxes 16..23 are Rx for J1939 protocol
    CanMsg_T Msg ;
    J1939Id_T Id ;
    int cnt ;
    for ( cnt = 0 ; cnt < 3 ; cnt++)
    {
        if ( GetNextId29Message(&Msg ) != 1 )
        {
            break ;
        }
        InterpretJ1939 ( &Msg , &Id );
    }
}

