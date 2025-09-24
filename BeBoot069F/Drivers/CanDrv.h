/*
 * CanComm.h
 *
 *  Created on: Sep 24, 2025
 *      Author: user
 */

#ifndef DRIVERS_CANDRV_H_
#define DRIVERS_CANDRV_H_


#ifndef ASSERT
#define ASSERT(expr)
#endif

#define CANMC_RUN_FREE  (1UL<<16)
#define CANMC_CHANGE_CONFIG_REQUEST (1UL<<13)
#define CANMC_ECAN_MODE (1UL<<12)
#define CANMC_WAKEUP_BY_ACTIVITY (1UL<<9)
#define CANMC_USE_LSB_FIRST (1UL<<10)
#define CANMC_AUTO_BUS_ON (1UL<<7)


#define MSGID_29BIT (1UL<<31)
#define MSGID_USE_ACCEPTANCE_MASK (1UL<<30)
#define MSGID_11BIT_ADDRESS_SHIFT 18


#define CAN_IO_USE_PIN 0x8UL

inline volatile struct MBOX * CanBoxNumToAddr(volatile struct ECAN_MBOXES * BoxesBase, unsigned short BoxNum)
{
  ASSERT (BoxNum<32) ;
  return ( (struct MBOX *) ((unsigned long)(BoxesBase) + BoxNum * (sizeof(struct MBOX))) );
}


// Address of LAM register associated with CAN mail box
// using base address of LAM registers array and box number - from 0 to 31
inline union CANLAM_REG * CanLAMNumToAddr(volatile struct LAM_REGS * LAMsBase, short unsigned BoxNum)
{
  ASSERT (BoxNum<32) ;
  return ( (union CANLAM_REG *) ((unsigned long)(LAMsBase) + BoxNum * (sizeof(union CANLAM_REG))) );
}



#endif /* DRIVERS_CANDRV_H_ */
