/*
 * LoaderProtocol.c
 *
 *  Created on: Sep 24, 2025
 *      Author: user
 */




#include "StructDef.h"

CanMsg_T Msg11 ;
CanMsg_T Msg11Tx ;


void InitLoader()
{
    J1939ProtocolEnable = 1 ;
    LoadProtocolEnable = 0 ;
    LoaderState.State = 0 ;
    LoaderState.Erased = 0 ;
}

/*
 * Test that user program region is erased
 */
short CheckBlank()
{
    long unsigned *ptr ;

    ptr = ( long unsigned *) CodeLeastAddress ;
    while ( (long unsigned) ptr < BootLeastAddress)
    {
        if ( *ptr != 0xffffffff )
        {
            return 0 ;
        }
        ptr += 1 ;
    }
    return 1 ;
}


void SetWordInProgramBuffer( long unsigned nextWord , short unsigned data)
{
    if (nextWord <= 0xffff )
    {
        ProgramBuffer[nextWord] = data ;
        ProgramBufferDirty[nextWord>>3] |= ( 1 << (nextWord&7) ) ;
    }
}

/*
 * Check that all the words in a given range had been accessed for programming
 */
short unsigned CheckAllProgrammed()
{
    short unsigned cnt ;
    for ( cnt = LoaderState.NextRelAddres ; cnt < LoaderState.NextRelAddres + LoaderState.NextLength - 1 ; cnt++ )
    {
        if  ( ( ProgramBufferDirty[cnt>>3] & ( 1 << (cnt&7) )) == 0 )
        {
            return 0 ;
        }
    }
    return 1 ;
}



short unsigned InterpretLoader(CanMsg_T *msg)
{
    short unsigned cnt , good ,ErrorCode  ;
    long unsigned nextWord , cs ;
    ErrorCode = 0 ;
    switch (LoaderState.State)
    {
    case 0:

        if ( (msg->Id == CanId) && ( msg->data[1] == 0x12345678) )
        {
            LoaderState.SenderAddress =  msg->data[0] >> 16 ;
            LoaderState.NextSector =  msg->data[0] & 0xffff ;
        }

        if ( ( LoaderState.SenderAddress > 239 ) || (LoaderState.NextSector < 1) ||(LoaderState.NextSector == 0) || (LoaderState.NextSector > 7))
        {
            break ;
        }

        LoadProtocolEnable = 1 ; // No more J1939, we are on the go
        if ( !LoaderState.Erased )
        {
            if ( !CheckBlank() )
            {
                EraseAllUserSectors() ; // Erase all the sectors
            }
            LoaderState.Erased = 0 ;
        }
        for ( cnt = 0 ; cnt < 16384 ; cnt++)
        {
            ProgramBuffer[cnt] = 0xffff ;
        }
        for ( cnt = 0 ; cnt < 1024 ; cnt++)
        {
            ProgramBufferDirty[cnt] = 0  ;
        }
        Msg11Tx.DLC = 8 ;
        Msg11Tx.Id  = LoaderState.SenderAddress ;
        Msg11Tx.data[0] = msg->data[0] ;
        Msg11Tx.data[1] = msg->data[1] ;

        TxMessage( &Msg11Tx, 0 );

        LoaderState.State = 1 ;
        break ;
    case 1:
        if ( msg->Id == (CanId + ( LoaderState.NextSector << 8 ) ) )
        {
            nextWord = msg->data[0] & 0xffff  ;
            SetWordInProgramBuffer( nextWord++ , (short unsigned) (msg->data[0] >> 16)) ;
            SetWordInProgramBuffer( nextWord++ , (short unsigned) (msg->data[1] & 0xffff )) ;
            SetWordInProgramBuffer( nextWord++ , (short unsigned) (msg->data[1] >> 16)) ;
        }
        if (  msg->Id ==  CanId)
        {
            LoaderState.NextRelAddres =   (short unsigned) (msg->data[0] & 0xffff) ;
            LoaderState.NextLength  =   (short unsigned) (msg->data[0] >> 16);
            LoaderState.Adler32CS  = msg->data[1]     ;
            good = 1 ;

            if ( (long unsigned) LoaderState.NextRelAddres + (long unsigned) LoaderState.NextLength > 16384UL)
            {
                ErrorCode = 1  ;
                good = 0 ;
            }
            else
            {
                good = CheckAllProgrammed() ;
            }
            if ( good )
            {
                cs = ComputeAdler32((long unsigned) &ProgramBuffer[LoaderState.NextRelAddres] , LoaderState.NextLength);
                if ( cs != LoaderState.Adler32CS)
                {
                    ErrorCode = 2 ;
                    good = 0 ;
                }
            }
            if ( good )
            {
                good =  ProgramIntoSector(LoaderState.NextSector , LoaderState.NextRelAddres , ProgramBuffer  , LoaderState.NextLength);
                if ( good != 1 )
                {
                    ErrorCode = 3 ;
                    good = 0 ;
                }
            }

            Msg11Tx.DLC = 8 ;
            Msg11Tx.Id  = LoaderState.SenderAddress ;
            Msg11Tx.data[0] = good + (ErrorCode << 8) + 0xffff0000  ;
            Msg11Tx.data[1] = 0xffffffff  ;

            TxMessage( &Msg11Tx, 0 );

            LoaderState.State = 0 ; // Go to final check
            break ;

        }
        break ;
    }
    return 1 ;
}


void LoaderProtocol()
{
    short unsigned cnt;
    for ( cnt = 0 ; cnt < 16 ; cnt++)
    {
        if ( GetNextId11Message(&Msg11 ) != 1 )
        {
            break ;
        }
        if ( LoadProtocolEnable==1 )
        {
            InterpretLoader ( &Msg11  );
        }
    }

}
