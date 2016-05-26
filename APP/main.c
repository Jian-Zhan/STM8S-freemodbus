#include "stm8s.h"

#include "mb.h"
#include "mbport.h"

#include "mbregs.h"

int
main( void )
{ 
  /* Clock configuration -----------------------------------------*/
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);  
  
  /* GPIO configuration -----------------------------------------*/
  // PA3: PWM
  GPIO_DeInit(GPIOB);
  GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST); // BRK
  GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST); // DIR
  GPIO_DeInit(GPIOC);
  GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT); // BIT0
  GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT); // BIT1
  GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT); // BIT2
  GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT); // BIT3
  GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT); // BIT4
  GPIO_DeInit(GPIOD);
  GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT); // nFAULT
    
  /* Define FLASH programming time */
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
  /* Unlock Data memory */
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  
  UCHAR slaveID = getSlaveID();
  ULONG baudRate = getBaudRate();
  eMBParity parity = (eMBParity) getParity();
  /*
  uint8_t resetFlag = getRestartFlag();
  clearRestartFlag();
  */
  
  // Default (in EEPROM): SlaveID:247(0xF7) 9600 8N1
  eMBInit( MB_RTU, slaveID, 0, baudRate, parity );
  eMBEnable(  );
  
  while (1)
  {
    ( void )eMBPoll(  );
    
    /*
    if (getRestartFlag() == 0x01) {
      eMBDisable();
      goto START;
    }
    */
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
  while (1);
}
#endif