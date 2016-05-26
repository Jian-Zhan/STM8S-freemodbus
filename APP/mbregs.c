#include "stm8s.h"

#include "mbregs.h"

uint8_t flagRunning = 0x01;

/* Content of the holding regs in MEMORY
HR[0]: Restart 0 0 TargetBitsMask[5bits]  DIR BRK 0 TargetBits[5bits]
Defaults:
HR[0]: 00011111 11000000
*/
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS] = {
    0x1FC0,
};

/* content of the holding regs in EEPROM
HR[0x1000]: Parity[2bits] 0 0 0 0 0  SlaveID[8bits]
HR[0x1001]: BaudRate[16bits]
HR[0x1002]: BaudRate[16bits]

Defaults:
HR[0x1000]: 00000000 00000000
HR[0x1001]: 00000000 00000000
HR[0x1002]: 00100101 10000000
*/
uint16_t usRegHoldingEEPROM[REG_HOLDING_EEPROM_NREGS]@"reg_eeprom" = {
	0x0000,
    0x0000,
    0x2580,
};

/*
void clearRestartFlag( void )
{
  usRegHoldingBuf[0] &= 0x7FFF;
}

uint8_t getRestartFlag( void )
{
  return (uint8_t)( usRegHoldingBuf[0] >> 15 );
}
*/

uint8_t getSlaveID( void )
{
  uint8_t ret = (uint8_t)( usRegHoldingEEPROM[0] & 0x00FF );
  if ((ret >= MB_ADDRESS_MIN) && (ret < MB_ADDRESS_MAX) && (ret != MB_ADDRESS_BROADCAST)) {
    return ret;
  }
  else {
    return MB_ADDRESS_MAX;
  }
}

uint32_t getBaudRate( void )
{
  uint32_t ret = (uint32_t)( usRegHoldingEEPROM[1] ) << 16 ;
  ret += (uint32_t)( usRegHoldingEEPROM[2] );
  return ret;
}

uint8_t getParity( void )
{
  uint8_t ret = (uint8_t)( usRegHoldingEEPROM[0] >> 13 );
  return ret;
}

/* Running 0 nFAULT CurrentBits[5bit] */
eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    /* error state */
    eMBErrorCode    eStatus = MB_ENOERR;
    /* offset */
    int16_t iRegIndex;
    
    /* test if the reg is in memory */
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
                    usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                    usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                    iRegIndex ++;
                    usNRegs --;
                }
                break;
                
        }
    }
    
    else if (((int16_t)usAddress-1 >= REG_HOLDING_EEPROM_START) 
        && (usAddress-1 + usNRegs <= REG_HOLDING_EEPROM_START + REG_HOLDING_EEPROM_NREGS))
    {
        /* compute the reg's offset */
        iRegIndex = (int16_t)(usAddress-1 - REG_HOLDING_EEPROM_START);
        switch (eMode)
        {
            case MB_REG_READ:
                while (usNRegs > 0)
                {
                    *pucRegBuffer++ = (uint8_t)( usRegHoldingEEPROM[iRegIndex] >> 8 );
                    *pucRegBuffer++ = (uint8_t)( usRegHoldingEEPROM[iRegIndex] & 0xff);
                    iRegIndex ++;
                    usNRegs --;
                }
                break;
            case MB_REG_WRITE:
                while (usNRegs > 0)
                {
                    FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
                    usRegHoldingEEPROM[iRegIndex] = *pucRegBuffer++ << 8;
                    FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
                    usRegHoldingEEPROM[iRegIndex] |= *pucRegBuffer++;
                    FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
                    iRegIndex ++;
                    usNRegs --;
                }
                break;
                
        }
    }

    /* Out of range */
    else{
        eStatus = MB_ENOREG;
    }
    
    return eStatus;
    
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    uint8_t         regBuf = 0x00;
    
    /* test if the reg is in the range */
    if (((int16_t)usAddress-1 < REG_INPUT_START) 
        || (usAddress-1 + usNRegs > REG_INPUT_START + REG_INPUT_NREGS))
      return MB_ENOREG;

    regBuf |= (GPIO_ReadInputData(GPIOC) & 0xF8) >> 3; // PC3 - PC7
    regBuf |= ( flagRunning << 7 );
    regBuf |= (GPIO_ReadInputData(GPIOD) & 0x08) << 2; // PD3 
    *pucRegBuffer++ = 0;
    *pucRegBuffer++ = regBuf;
        
    return MB_ENOERR;
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