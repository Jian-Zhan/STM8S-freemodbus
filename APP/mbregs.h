#include "mb.h"
#include "mbport.h"

extern uint8_t flagRunning;

/* holding register address */
#define REG_HOLDING_START 0x0000
/* number of regs */
#define REG_HOLDING_NREGS 1

/* holding register address */
#define REG_HOLDING_EEPROM_START 0x1000
/* number of regs */
#define REG_HOLDING_EEPROM_NREGS 3

/* holding input address */
#define REG_INPUT_START 0x0000
/* number of regs */
#define REG_INPUT_NREGS 1
/* content of the input regs
IR[0]: 0 0 0 0 0 0 0 0  RUNNING 0 nFAULT CurrentBits[5bits]
*/

void clearRestartFlag( void );

uint8_t getRestartFlag( void );

uint8_t getSlaveID( void );

uint32_t getBaudRate( void );

uint8_t getParity( void );