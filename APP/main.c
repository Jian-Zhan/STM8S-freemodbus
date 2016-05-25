#include "stm8s.h"

#include "mb.h"
#include "mbport.h"


int
main( void )
{

  /* Clock configuration -----------------------------------------*/
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);  
  
  eMBInit( MB_RTU, 0x01, 0, 9600, MB_PAR_NONE );
  eMBEnable(  );
  
  while (1)
  {
    ( void )eMBPoll(  );
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
  while (1);
}
#endif