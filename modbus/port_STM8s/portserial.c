/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#include "stm8s.h"

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */

    ENTER_CRITICAL_SECTION(  );
    
    if( xRxEnable ) {
        UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
        GPIO_WriteLow(GPIOD, GPIO_PIN_4);
    }
    else {
        UART1_ITConfig(UART1_IT_RXNE_OR, DISABLE);
        GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
    }
    
    if( xTxEnable ) {
        UART1_ITConfig(UART1_IT_TXE, ENABLE);
    }
    else{
        UART1_ITConfig(UART1_IT_TXE, DISABLE);
    }
    
    EXIT_CRITICAL_SECTION(  );
}

BOOL
xMBPortSerialInit( UCHAR ucPort, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    UART1_Parity_TypeDef     parity = UART1_PARITY_NO;
    UART1_WordLength_TypeDef wordLength = UART1_WORDLENGTH_8D;
    
	/* PD4: 485 DE & !RE */
	GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_WriteLow(GPIOD, GPIO_PIN_4);
    
	switch ( eParity )
	{
	case MB_PAR_NONE:
		parity = UART1_PARITY_NO;
        wordLength = UART1_WORDLENGTH_8D;
		break;
	case MB_PAR_ODD:
		parity = UART1_PARITY_ODD;
        wordLength = UART1_WORDLENGTH_9D;
		break;
	case MB_PAR_EVEN:
		parity = UART1_PARITY_EVEN;
        wordLength = UART1_WORDLENGTH_9D;
		break;
	}
	
	ENTER_CRITICAL_SECTION(  );
    UART1_DeInit();
    UART1_Init((uint32_t)ulBaudRate, wordLength, UART1_STOPBITS_1, parity,
               UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
	EXIT_CRITICAL_SECTION(  );
	
	return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
    
    UART1_SendData8(ucByte);
    
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
  
    *pucByte = UART1_ReceiveData8();
    
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}
