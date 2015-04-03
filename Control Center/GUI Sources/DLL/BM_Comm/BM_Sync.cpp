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
// Generic defines and DLL export section
// *************************************************************************************************


// *************************************************************************************************
// Include section

#include "stdafx.h"

#include <time.h>
#include <devguid.h>
#include <regstr.h>
#include <stdlib.h>
#include <tchar.h>
#include <string>

#include "BM_API.h"
#include "BM_Driver.h"
#include "BM_Sync.h"


// *************************************************************************************************
// Defines section

// USB 
#define SYNC_USB_DATA_EMPTY			(0u)
#define SYNC_USB_DATA_READY			(1u)
#define SYNC_USB_DATA_LOCKED		(2u)

// USB transmission packet length
#define SYNC_HEADER_LENGTH			(2u)
#define SYNC_DATA_LENGTH			(26u)

// Shortcuts to address packet byte-wise
#define PACKET_BYTE_START			0
#define PACKET_BYTE_CMD				1
#define PACKET_BYTE_SIZE			2
#define PACKET_DATA_START			3


// *************************************************************************************************
// Extern section

extern bool BM_COM_is_open;
extern bool BM_Channel_is_started;
extern unsigned char BM_errorstate;
extern unsigned char tx_buf[PACKET_TOTAL_BYTES+1];
extern unsigned char rx_buf[PACKET_TOTAL_BYTES+1];

extern void CreatePacket(char cmd, int len, char * data);
extern void send_receive(WORD wlen, WORD rlen, WORD delay);


// *************************************************************************************************
// SimpliciTI sync mode

// Start RF access point in sync mode
bool BM_SYNC_Start(void)
{
	if (BM_Channel_is_started == true) 
	{
		return false;
	}
	else
	{
		// Create packet
		CreatePacket(BM_SYNC_START, 0, NULL);

		// Send packet, do not wait, read packet, flush port
		send_receive(0, 0, 0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;
	}

	return true;
}


// Send command packet to watch
bool BM_SYNC_SendCommand(BYTE * bData)
{
	if (BM_COM_is_open == true) 
	{
		CreatePacket(BM_SYNC_SEND_COMMAND, BM_SYNC_DATA_LEN, (char*)bData);

		// Send packet, do not wait, read packet, flush port
		send_receive(BM_SYNC_DATA_LEN, 0, 0);

		// Check response
		BM_errorstate = rx_buf[PACKET_BYTE_CMD];
		if (BM_errorstate != HW_NO_ERROR) return false;
	}
	else 
	{
		// Update error state
		BM_errorstate = HW_ERROR;
		return false;
	}
	return true;

}


// Get sync buffer status from RF access point
bool BM_SYNC_GetBufferStatus(WORD& wStatus)
{
	char data[2];

	if (BM_COM_is_open == true) 
	{
		data[0] = 0;
		data[1] = 0;

		// Create packet
		CreatePacket(BM_SYNC_GET_BUFFER_STATUS, 0, data);
		
		// Send packet, do not wait, read packet, flush port
		send_receive(0, 1, 0);

		// Extract data
		wStatus = rx_buf[PACKET_DATA_START];

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;
	}
	else 
	{
		// Update error state
		BM_errorstate = HW_ERROR;
		return false;
	}
	return true;
}


// Read 32+2 data bytes from sync buffer
bool BM_SYNC_ReadBuffer(BYTE * bData)
{
	char data[1];
	int i;

	if (BM_COM_is_open == true) 
	{
		CreatePacket(BM_SYNC_READ_BUFFER, 0, data);

		// Send packet, do not wait, read packet, flush port
		send_receive(0, BM_SYNC_DATA_LEN, 0);

		// Check response
		BM_errorstate = rx_buf[PACKET_BYTE_CMD];
		if (BM_errorstate != HW_NO_ERROR) return false;
		
		// Hand over received data
		for (i=0; i<BM_SYNC_DATA_LEN; i++) *(bData+i) = rx_buf[PACKET_DATA_START+i];
	}
	else 
	{
		// Update error state
		BM_errorstate = HW_ERROR;
		return false;
	}
	return true;
}
