To use EEPROM in IAR, declare the variable as a static char. Place it in eeprom section "my_eeprom".

char array[]@ "my_eeprom" = {0X1C,0X1F,0X9F,0XFF};

and update the linker configuration file for example:

place in Eeprom             { rw section .eeprom.noinit,
                              rw section my_eeprom };

Then the array can be used as normal memory array. However:
1) The EEPROM must be unlocked before use:
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
2) Before and after a writing operation, it is suggested to wait for last operation.
    FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
    array[idx] = u8_val;
    FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);

Ref: https://www.iar.com/support/tech-notes/compiler/initializing-the-eeprom-at-program-download/