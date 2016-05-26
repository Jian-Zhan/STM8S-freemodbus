#include "stm8s.h"

#include "mb.h"
#include "mbport.h"

/* holding register address */
#define REG_HOLDING_START 0x0000
/* number of regs */
#define REG_HOLDING_NREGS 2
/* content of the holding regs */
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS]@"reg_eeprom" = {
	0x147b,
	0x3f8e,
};

    
/* holding input address */
#define REG_INPUT_START 0x0000
/* number of regs */
#define REG_INPUT_NREGS 1
/* content of the holding regs */
uint16_t usRegInputBuf[REG_INPUT_NREGS] = {
	0x1234,
};

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    /* error state */
    eMBErrorCode    eStatus = MB_ENOERR;
    /* offset */
    int16_t iRegIndex;
    
    /* test if the reg is in the range */
    if (((int16_t)usAddress-1 >= REG_HOLDING_START) 
        && (usAddress-1 + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS))
    {
        /* compute the reg's offset */
        iRegIndex = (int16_t)(usAddress-1 - REG_HOLDING_START);
        switch (eMode)
        {
            case MB_REG_READ:
                while (usNRegs > 0)
                {
                    *pucRegBuffer++ = (uint8_t)( usRegHoldingBuf[iRegIndex] >> 8 );
                    *pucRegBuffer++ = (uint8_t)( usRegHoldingBuf[iRegIndex] & 0xff);
                    iRegIndex ++;
                    usNRegs --;
                }
                break;
            case MB_REG_WRITE:
                while (usNRegs > 0)
                {
                    FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
                    usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                    FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
                    usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                    FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
                    iRegIndex ++;
                    usNRegs --;
                }
                break;
                
        }
    }
    else{
        eStatus = MB_ENOREG;
    }
    
    return eStatus;
    
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    /* error state */
    eMBErrorCode    eStatus = MB_ENOERR;
    /* offset */
    int16_t iRegIndex;
    
    /* test if the reg is in the range */
    if (((int16_t)usAddress-1 >= REG_INPUT_START) 
        && (usAddress-1 + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
    {
        /* compute the reg's offset */
        iRegIndex = (int16_t)(usAddress-1 - REG_INPUT_START);
        while (usNRegs > 0)
        {
            *pucRegBuffer++ = (uint8_t)( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = (uint8_t)( usRegInputBuf[iRegIndex] & 0xff);
            iRegIndex ++;
            usNRegs --;
        }
    }
    else{
        eStatus = MB_ENOREG;
    }
    
    return eStatus;
}

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}