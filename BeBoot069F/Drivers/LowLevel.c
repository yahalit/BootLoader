/*
 * LowLevel.c
 *
 *  Created on: Aug 30, 2025
 *      Author: user
 */
#include "..\Application\StructDef.h"


void SetPLL(void )
{
    if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 1)
    {
        if (SysCtrlRegs.PLLCR.bit.DIV != PLLCR_VALUE)
        {
            EALLOW;

            //
            // Before setting PLLCR turn off missing clock detect
            //
            SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
            SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;  // 1/4
            SysCtrlRegs.PLLCR.bit.DIV = PLLCR_VALUE;
            EDIS;

            //
            // Wait for PLL to lock.
            // During this time the CPU will switch to OSCCLK/2 until
            // the PLL is stable.  Once the PLL is stable the CPU will
            // switch to the new PLL value.
            //
            // This time-to-lock is monitored by a PLL lock counter.
            //
            // The watchdog should be disabled before this loop, or fed within
            // the loop.
            //
            EALLOW;
            SysCtrlRegs.WDCR= 0x0068;
            EDIS;

            //
            // Wait for the PLL lock bit to be set.
            // Note this bit is not available on 281x devices.
            // For those devices use a software loop to perform the required
            // count.
            //
            while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1)
            {

            }

            EALLOW;
            SysCtrlRegs.PLLSTS.bit.DIVSEL = 2;  // 1/2
            SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
            EDIS;
        }
    }

    //
    // If the PLL is in limp mode, shut the system down
    //
    else
    {
        //
        // Replace this line with a call to an appropriate
        //SystemShutdown(); function.
        //
        asm("        ESTOP0");
    }

}

//
// InitECanaGpio -
//
void InitECanGpio(void)
{
    EALLOW;

    //
    // Enable internal pull-up for the selected CAN pins
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;   // Enable pull-up for GPIO30 (CANRXA)
    GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;   // Enable pull-up for GPIO31 (CANTXA)

    //
    // Set qualification for selected CAN pins to asynch only
    // Inputs are synchronized to SYSCLKOUT by default.
    // This will select asynch (no qualification) for the selected pins.
    //
    GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;   // Asynch qual for GPIO30 (CANRXA)

    //
    // Configure eCAN-A pins using GPIO regs
    // This specifies which of the possible GPIO pins will be eCAN functional
    // pins.
    //

    //
    // Configure GPIO30 for CANRXA operation
    //
    GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;

    //
    // Configure GPIO31 for CANTXA operation
    //
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;

    EDIS;
}


void  InitPeripherals(void)
{
    //
    // Run the device calibration routine to trim the internal
    // oscillators to 10Mhz.
    //
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
    (*Device_cal)();
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 0;
    EDIS;

    //
    // Assuming PLLSTS[CLKINDIV] = 0 (default on XRSn).  If it is not
    // 0, then the PLLCR cannot be written to.
    // Make sure the PLL is not running in limp mode
    //
    SetPLL() ;

    //
    // Configure CAN pins at the GPIO MUX level
    //
    InitECanGpio();

}

