/*
 * Drivers.h
 *
 *  Created on: Aug 29, 2025
 *      Author: user
 */

#ifndef DRIVERS_DRIVERS_H_
#define DRIVERS_DRIVERS_H_

// Include peripheral related files
#include "..\f2806x\common\include\F2806x_EPwm_defines.h"


// Basic chip level initialization routines
void InitSysCtrl(void);
void InitPieCtrl(void);
void InitPieVectTable(void);
void InitFlash(void);
#endif /* DRIVERS_DRIVERS_H_ */
