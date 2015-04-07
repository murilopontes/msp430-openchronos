// *************************************************************************************************
//
//	Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
//
//
//	  Redistribution and use in source and binary forms, with or without
//	  modification, are permitted provided that the following conditions
//	  are met:
//
//	    Redistributions of source code must retain the above copyright
//	    notice, this list of conditions and the following disclaimer.
//
//	    Redistributions in binary form must reproduce the above copyright
//	    notice, this list of conditions and the following disclaimer in the
//	    documentation and/or other materials provided with the
//	    distribution.
//
//	    Neither the name of Texas Instruments Incorporated nor the names of
//	    its contributors may be used to endorse or promote products derived
//	    from this software without specific prior written permission.
//
//	  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//	  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//	  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//	  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//	  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//	  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//	  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//	  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//	  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//	  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//	  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// *************************************************************************************************
#include <intrinsics.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "project.h"
//#include "ccSPI.h"
//#include "ccxx00.h"

#include "USB_config/descriptors.h"

#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/types.h"               //Basic Type declarations
#include "USB_API/USB_Common/usb.h"                 //USB-specific functions

#include "F5xx_F6xx_Core_Lib/HAL_UCS.h"
#include "F5xx_F6xx_Core_Lib/HAL_PMM.h"

#include "USB_API/USB_CDC_API/UsbCdc.h"
#include "usbConstructs.h"


#include "BM_API.h"
//#include "WBSL/wbsl.h"
#include "WBSL/spi_wbsl.h"


//Function declarations
void InitPorts_v(void);
void InitClock_v(void);

//Global flags set by events
volatile u8 bCDCDataReceived_event = FALSE;   //Indicates data has been received without an open rcv operation

int novo_comando=0;
uint8_t usb_bufferRX[64];
uint8_t usb_bufferTX[64];
uint8_t txlen=0;

WORD bytesReceived_usbevent;

void delay_us(int us){
	int i;
	for(i=0;i<us;i++){
		__delay_cycles(16);
	}
}
void delay_ms(int ms){
	int i;
	for(i=0;i<ms;i++){
		delay_us(1000);
	}
}


void usb_reset(){

	// As fast as possible 26MHz to GDO2 pin of CC1101 as it is required to clock USB
	wbsl_SpiInit();
	wbsl_SpiWriteReg(IOCFG0,0x2e); //(0x2E) High impedance (3-state)
	wbsl_SpiWriteReg(IOCFG2,0x30); //(0x30) CLK_XOSC/1


	//
	UCSCTL6 |= XT2BYPASS;

	// Initialize unused port pins
	InitPorts_v();

	// Set Vcore to the level required for USB operation
	SetVCore(3);

	// Initialize clock system
	InitClock_v();

	// Initialize debug output pins
	INIT_TX_ACTIVITY;
	INIT_RX_ACTIVITY;

	// Enable interrupts
	__enable_interrupt();

	// Initialize USB port
	USB_init();

	// Enable various USB event handling routines
	USB_setEnabledEvents(  kUSB_VbusOnEvent + kUSB_VbusOffEvent + kUSB_receiveCompletedEvent
			+ kUSB_dataReceivedEvent + kUSB_UsbSuspendEvent + kUSB_UsbResumeEvent + kUSB_UsbResetEvent);

	// See if we are already attached physically to USB, and if so, connect to it
	// Normally applications don't invoke the event handlers, but this is an exception.
	if (USB_connectionInfo() & kUSB_vbusPresent)
	{
		USB_handleVbusOnEvent();
	}
}

void main (void)
{
	// Stop WDT
	WDTCTL = WDTPW + WDTHOLD;


	usb_reset();

	while (1)
	{
		LED_ON;
		delay_ms(50);
		LED_OFF;
		delay_ms(50);



	}

}





uint8_t cc1101_fifo[64];

void USB_Handler_v(void)
{
	LED_ON;

	memset(usb_bufferTX,0,sizeof(usb_bufferTX));
	txlen=sizeof(usb_bufferTX);

	// From PC to MSP430
	if (bCDCDataReceived_event)
	{
		bCDCDataReceived_event = FALSE;                    // Clear flag early -- just in case execution breaks below because of an error

		bytesReceived_usbevent = USBCDC_bytesInUSBBuffer(0);
		if(bytesReceived_usbevent>sizeof(usb_bufferRX)){
			bytesReceived_usbevent=sizeof(usb_bufferRX);
		}
		USBCDC_receiveData(usb_bufferRX, bytesReceived_usbevent, 0);

	}



	__disable_interrupt();

	//stroke
	if(usb_bufferRX[0]==1 && bytesReceived_usbevent==2 && usb_bufferRX[1]>=0x30 && usb_bufferRX[1]<=0x3d){
		//
		usb_bufferTX[0]=usb_bufferRX[0];
		usb_bufferTX[1]=usb_bufferRX[1];
		usb_bufferTX[2]=wbsl_SpiCmdStrobe(usb_bufferRX[1]);
		if(usb_bufferRX[1]==0x30){
			//reset
			PMMCTL0 |= PMMSWPOR;
		}
		txlen=3;
	}
	//reg read
	if(usb_bufferRX[0]==2 && bytesReceived_usbevent==2 && usb_bufferRX[1]>=0x0 && usb_bufferRX[1]<=0x3f){
		usb_bufferTX[0]=usb_bufferRX[0];
		usb_bufferTX[1]=usb_bufferRX[1];
		usb_bufferTX[2]=wbsl_SpiReadReg(usb_bufferRX[1]);
		txlen=3;
	}
	//reg write
	if(usb_bufferRX[0]==3 && bytesReceived_usbevent==3 && usb_bufferRX[1]>=0x0 && usb_bufferRX[1]<=0x3f){
		usb_bufferTX[0]=usb_bufferRX[0];
		usb_bufferTX[1]=usb_bufferRX[1];

		int i;
		for(i=0;i<100;i++){
		  wbsl_SpiWriteReg(usb_bufferRX[1],usb_bufferRX[2]);
		  usb_bufferTX[2]=wbsl_SpiReadReg(usb_bufferRX[1]);
		  if(usb_bufferTX[2]==usb_bufferRX[2]){
			  break;
		  }
		}
		usb_bufferTX[3]=i;
		txlen=4;
	}
	//fifo read
	if(usb_bufferRX[0]==4 && bytesReceived_usbevent==1){
		int rxlen=wbsl_SpiReadReg(RXBYTES);
		if(rxlen>0){
			wbsl_SpiReadRxFifo(cc1101_fifo,rxlen);
			memcpy(usb_bufferTX,cc1101_fifo,rxlen);
			txlen=rxlen;
		}
		else {
			memset(usb_bufferTX,0xff,64);
			txlen=64;
		}
	}

	//fifo write (max 63 bytes payload)
	if(usb_bufferRX[0]==5 && bytesReceived_usbevent>=1 && bytesReceived_usbevent<=64){
		memcpy(cc1101_fifo,&usb_bufferRX[1],bytesReceived_usbevent-1);
		wbsl_SpiWriteTxFifo(cc1101_fifo,bytesReceived_usbevent-1);
		memcpy(usb_bufferTX,usb_bufferRX,bytesReceived_usbevent);
		txlen=bytesReceived_usbevent;
	}

	//clear RX
	memset(usb_bufferRX,0,sizeof(usb_bufferRX));


	__enable_interrupt();


	WORD bytesSent;
	WORD bytesReceived;
	if ((USBCDC_intfStatus(0, &bytesSent, &bytesReceived) & kUSBCDC_waitingForSend) == 0)
	{
		//we can send
		// Disable interrupts
		__disable_interrupt();
		switch (USBCDC_sendData(usb_bufferTX, txlen, 0))
		{
		case kUSBCDC_sendStarted:
			break;
		case kUSBCDC_busNotAvailable:
			break;
		case kUSBCDC_sendComplete:

			break;
		default:
			break;
		}

		// Enable interrupts
		__enable_interrupt();
	}


	LED_OFF;
}

// *************************************************************************************************
// Init clock system
// *************************************************************************************************
void InitClock_v(void)
{
	// Enable 32kHz ACLK on XT1 via external crystal and 26MHz on XT2 via external clock signal
	P5SEL |=  (BIT2 | BIT3 | BIT4 | BIT5); // Select XINs and XOUTs
	//  UCSCTL6 &= ~XT1OFF;        // Switch on XT1, keep highest drive strength - default
	//  UCSCTL6 |=  XCAP_3;        // Set internal load caps to 12pF - default
	//  UCSCTL4 |=  SELA__XT1CLK;  // Select XT1 as ACLK - default

	// Configure clock system
	_BIS_SR(SCG0);    // Disable FLL control loop
	UCSCTL0 = 0x0000; // Set lowest DCOx, MODx to avoid temporary overclocking
	// Select suitable DCO frequency range and keep modulation enabled
	UCSCTL1 = DCORSEL_5; // DCO frequency above 8MHz but not bigger than 16MHz
	UCSCTL2 = FLLD__2 | (((MCLK_FREQUENCY + 0x4000) / 0x8000) - 1); // Set FLL loop divider to 2 and
	// required DCO multiplier
	//  UCSCTL3 |= SELREF__XT1CLK;                    // Select XT1 as FLL reference - default
	//  UCSCTL4 |= SELS__DCOCLKDIV | SELM__DCOCLKDIV; // Select XT1 as ACLK and
	// divided DCO for SMCLK and MCLK - default
	_BIC_SR(SCG0);                                  // Enable FLL control loop again

	// Loop until XT1 and DCO fault flags are reset
	do
	{
		// Clear fault flags
		UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | DCOFFG);
		SFRIFG1 &= ~OFIFG;
	} while ((SFRIFG1 & OFIFG));

	// Worst-case settling time for the DCO when changing the DCO range bits is:
	// 32 x 32 x MCLK / ACLK
	__delay_cycles(((32 * MCLK_FREQUENCY) / 0x8000) * 32);

	_BIS_SR(SCG0);    // Disable FLL control loop
}

// *************************************************************************************************
// Init Ports
// *************************************************************************************************
void InitPorts_v(void)
{
	// Initialize all unused pins as low level output
	P4OUT  &= ~(                            BIT4 | BIT5 | BIT6 | BIT7);
	P5OUT  &= ~(BIT0 | BIT1                                          );
	P6OUT  &= ~(BIT0 | BIT1 | BIT2 | BIT3                            );
	P4DIR  |=  (                            BIT4 | BIT5 | BIT6 | BIT7);
	P5DIR  |=  (BIT0 | BIT1                                          );
	P6DIR  |=  (BIT0 | BIT1 | BIT2 | BIT3                            );
}



/*  
 * ======== UNMI_ISR ========
 */
#pragma vector = UNMI_VECTOR
__interrupt void UNMI_ISR(void)
{
	switch (__even_in_range(SYSUNIV, SYSUNIV_BUSIFG))
	{
	case SYSUNIV_NONE:
		__no_operation();
		break;
	case SYSUNIV_NMIIFG:
		__no_operation();
		break;
	case SYSUNIV_OFIFG:
		UCSCTL7 &= ~(DCOFFG + XT1LFOFFG + XT2OFFG); //Clear OSC fault flags
		SFRIFG1 &= ~OFIFG;                          //Clear OFIFG fault flag
		break;
	case SYSUNIV_ACCVIFG:
		__no_operation();
		break;
	case SYSUNIV_BUSIFG:
		//If bus error occured - the cleaning of flag and re-initializing of
		//USB is required.
		SYSBERRIV = 0;                                      //clear bus error flag
		USB_disable();                                      //Disable
		break;
	}
}





