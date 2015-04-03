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
// Driver API
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
#include "enumser.h"

#include "BM_API.h"
#include "BM_Comm.h"
#include "BM_Driver.h"

// Include enumser library
#pragma comment(lib,"enumser.lib")


// *************************************************************************************************
// Variable section

bool BM_COM_is_open = false;
bool BM_Channel_is_started = false;
unsigned char BM_errorstate = HW_NO_ERROR;
unsigned char tx_buf[PACKET_TOTAL_BYTES];
unsigned char rx_buf[PACKET_TOTAL_BYTES];


// *************************************************************************************************
// COM port management

// Create a COM port packet (header + body)
void CreatePacket(char cmd, int len, char * data)
{
	unsigned char i;

	tx_buf[0] = 0xFF;		// Start marker
	tx_buf[1] = cmd;		// Command code
	tx_buf[2] = len + PACKET_OVERHEAD_BYTES;						// Packet length
	for(int i=0; i<len; i++) tx_buf[3+i] = * (data + i);			// Packet data (no reordering)

	// Clear RX buffer
	for (i=0; i<PACKET_TOTAL_BYTES; i++)
	{
		rx_buf[i] = 0;
	}
}


// Send packet via COM port, wait for defined milliseconds, then read from COM port
void send_receive(WORD wlen, WORD rlen, WORD delay)
{
	// Write to COM port
	WriteCOM(0, wlen+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

	// Delay 
	if (delay > 0) Sleep(delay);

	// Get response packet
	ReadCOM(0, rlen+PACKET_OVERHEAD_BYTES, rx_buf);

	// Flush COM port
	FlushCOM(0);
}


// Open COM port
bool BM_OpenCOM(char * strPortName, DWORD dwBaudrate, WORD wTimeout, bool bRTS, bool bDTR)
{
	BM_COM_is_open = OpenCOM(0, strPortName);

	// Update error state
	if (BM_COM_is_open == true) BM_errorstate = HW_NO_ERROR;
	else						BM_errorstate = HW_ERROR;

	return BM_COM_is_open;
}


// Close COM port
void BM_CloseCOM(void)
{
	CloseCOM(0);
	BM_COM_is_open = false;

	// Update error state
	BM_errorstate = HW_NO_ERROR;
}


// Get COM port number from Windows system using friendly name
bool BM_GetCOM(unsigned char& bPortNb)
{
  CUIntArray ports;
  INT_PTR i;
  CStringArray sFriendlyNames;
  BYTE port = 0;

  // Use external function from EnumSerialPorts library
  // See http://www.naughter.com/enumser.html for source code.
  if (CEnumerateSerial::UsingSetupAPI2(ports, sFriendlyNames))
  {
    for (i=0; i<ports.GetSize(); i++)
	{
		port = (BYTE)ports[i];

		// Extract strings
  		std::string s1 = "TI CC1111 Low-Power RF to USB CDC Serial Port";
		std::string s2 = sFriendlyNames[i].operator LPCTSTR();

		// Compare strings
		if(s1 == s2)
		{
			// We have a match
			bPortNb = port;
			return true;
		}
	}
	return false;
  }
  else return false;
}


// *************************************************************************************************
// Generic functions

// Reset RF access point
bool BM_Reset(void)
{
	char data[1];
	int len;

	if (BM_Channel_is_started == true) 
	{
		return false;
	}
	else
	{
		// 0x00 = No data
		data[0] = 0x00;

		// Create packet
		len = 1;
		CreatePacket(BM_RESET, len, data);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;
	}

	return true;
}


// Get RF access point dummy ID - used by live check
bool BM_GetID(DWORD& dwID)
{
	char data[4];
	int len;

	if (BM_COM_is_open == true) 
	{
		// Erase data
		data[0] = 0x00;
		data[1] = 0x00;
		data[2] = 0x00;
		data[3] = 0x00;

		// Create packet
		len = 4;
		CreatePacket(BM_GET_PRODUCT_ID, len, data);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;

		// Extract ID
		dwID = (rx_buf[6]<<24) + (rx_buf[5]<<16) + (rx_buf[4]<<8) + (rx_buf[3]);
	}
	else 
	{
		// Update error state
		BM_errorstate = HW_ERROR;
		return false;
	}

	return true;
}


// Get generic hardware status
// Returns: HW_IDLE, HW_SIMPLICITI_STOPPED, HW_SIMPLICITI_TRYING_TO_LINK, HW_SIMPLICITI_LINKED,
// HW_BLUEROBIN_STOPPED, HW_BLUEROBIN_TRANSMITTING, HW_ERROR, HW_NO_ERROR, HW_NOT_CONNECTED
bool BM_GetStatus(WORD& wStatus)
{
	int len;
	char data[1];

	if (BM_COM_is_open == true) 
	{
		// Create packet
		data[0] = 0x00;
		len = 1;
		CreatePacket(BM_GET_STATUS, len, data);

		// Flush COM port
		FlushCOM(0);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;

		// Extract status
		wStatus = rx_buf[3];
	}
	else 
	{
		// Update error state
		wStatus = HW_NOT_CONNECTED;
		return false;
	}

	return true;
}



// *************************************************************************************************
// BlueRobin 


// Set BlueRobin transmitter ID
bool BM_BR_SetID(DWORD dwID)
{
	char data[5];
	int len;

	if (BM_COM_is_open == true) 
	{
		// Split long ID to 4 bytes
		data[0] = (char) dwID;
		data[1] = (char) (dwID >> 8);
		data[2] = (char) (dwID >> 16);
		data[3] = (char) (dwID >> 24);

		// Create packet
		len = 4;
		CreatePacket(BM_SET_BLUEROBIN_ID, len, data);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

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


// Get BlueRobin transmitter ID
bool BM_BR_GetID(DWORD& dwID)
{
	char data[4];
	int len;

	if (BM_COM_is_open == true) 
	{
		// Erase data
		data[0] = 0x00;
		data[1] = 0x00;
		data[2] = 0x00;
		data[3] = 0x00;

		// Create packet
		len = 4;
		CreatePacket(BM_GET_BLUEROBIN_ID, len, data);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;

		// Extract ID
		dwID = (rx_buf[6]<<24) + (rx_buf[5]<<16) + (rx_buf[4]<<8) + (rx_buf[3]);
	}
	else 
	{
		// Update error state
		BM_errorstate = HW_ERROR;
		return false;
	}

	return true;
}


// Start BlueRobin transmitter channel
bool BM_BR_Start(void)
{
	int len;

	if (BM_Channel_is_started == true) 
	{
		return false;
	}
	else
	{
		// Create packet
		len = 0;
		CreatePacket(BM_START_BLUEROBIN, len, NULL);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;
	}

	return true;
}


// Stop BlueRobin transmitter
bool BM_BR_Stop(void)
{
	int len;

	if (BM_Channel_is_started == true) 
	{
		// Update error state
		return false;
	}
	else
	{
		// Create packet
		len = 0;
		CreatePacket(BM_STOP_BLUEROBIN, len, NULL);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;
	}

	return true;
}


// Set BlueRobin heart rate to be transmitted
bool BM_BR_SetHeartrate(WORD wHeartrate)
{
	char data[2];
	int len;

	if (BM_COM_is_open == true) 
	{
		// Set data
		data[0] = (unsigned char) wHeartrate;

		// Create packet
		len = 1;
		CreatePacket(BM_SET_HEARTRATE, len, data);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;
	}
	else 
	{
		// Update error state
		return false;
	}

	return true;
}


// Set BlueRobin speed that will be transmitted
bool BM_BR_SetSpeed(WORD wSpeed, WORD wDistance)
{
	char data[3];
	int len;

	if (BM_COM_is_open == true) 
	{
		data[0] = (char) wSpeed;
		data[1] = (char) wDistance;
		data[2] = (char) (wDistance >> 8);
		// Create packet
		len = 3;
		CreatePacket(BM_SET_SPEED, len, data);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;
	}
	else 
	{
		// Update error state
		return false;
	}

	return true;
}


// *************************************************************************************************
// SimpliciTI acc/ppt functions

// Start SimpliciTI stack in acc/ppt mode
bool BM_SPL_Start(void)
{
	int len;

	if (BM_Channel_is_started == true) 
	{
		return false;
	}
	else
	{
		// Create packet
		len = 0;
		CreatePacket(BM_START_SIMPLICITI, len, NULL);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;
	}

	return true;
}


// Stop SimpliciTI stack
bool BM_SPL_Stop(void)
{
	int len;

	if (BM_Channel_is_started == true) 
	{
		// Update error state
		return false;
	}
	else
	{
		// Create packet
		len = 0;
		CreatePacket(BM_STOP_SIMPLICITI, len, NULL);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;
	}

	return true;
}


// Get SimpliciTI acc/ppt data (4 bytes)
bool BM_SPL_GetData(DWORD& dwData)
{
	char data[5];
	int len;

	if (BM_COM_is_open == true) 
	{
		// Erase data
		data[0] = 0x00;
		data[1] = 0x00;
		data[2] = 0x00;
		data[3] = 0x00;

		// Create packet
		len = 4;
		CreatePacket(BM_GET_SIMPLICITIDATA, len, data);

		// Flush COM port
		FlushCOM(0);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;

		// Copy data
		dwData = (rx_buf[6]<<24) + (rx_buf[5]<<16) + (rx_buf[4]<<8) + (rx_buf[3]);
	}
	else 
	{
		// Update error state
		BM_errorstate = HW_ERROR;
		return false;
	}

	return true;
}




// ----------------------------------------------------------------------------------------------
// Wireless Update functions

bool BM_WBSL_Start(){

	int len;

	if (BM_Channel_is_started == true) 
	{
		return false;
	}
	else
	{
		// Create packet
		len = 0;
		CreatePacket(BM_START_WBSL, len, NULL);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;
	}

	return true;
}


bool BM_WBSL_Stop(){

	int len;

	if (BM_Channel_is_started == true) 
	{
		return false;
	}
	else
	{
		// Create packet
		len = 0;
		CreatePacket(BM_STOP_WBSL, len, NULL);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;
	}

	return true;
}

bool BM_WBSL_GetMaxPayload(DWORD& dwData){
	
	char data;
	int len;

	if (BM_COM_is_open == true) 
	{
		// Erase data
		data = 0x0;
	
		// Create packet
		len = 1;
		CreatePacket(BM_GET_MAX_PAYLOAD_WBSL, len, &data);

		// Flush COM port
		FlushCOM(0);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;

		// Copy data
		dwData = (rx_buf[6]<<24) + (rx_buf[5]<<16) + (rx_buf[4]<<8) + (rx_buf[3]);

		return true;
	}
	else
	{
		// Update error state
		BM_errorstate = HW_ERROR;
		return false;
	}
}

bool BM_WBSL_GetPacketStatus(DWORD& dwData){

	char data;
	int len;

	if (BM_COM_is_open == true) 
	{
		// Erase data
		data = 0x0;
	
		// Create packet
		len = 1;
		CreatePacket(BM_GET_PACKET_STATUS_WBSL, len, &data);

		// Flush COM port
		FlushCOM(0);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;

		// Copy data
		dwData = (rx_buf[6]<<24) + (rx_buf[5]<<16) + (rx_buf[4]<<8) + (rx_buf[3]);
		return true;
	}
	else
	{
		// Update error state
		BM_errorstate = HW_ERROR;
		return false;
	}
}

bool BM_WBSL_GetStatus(DWORD& dwData){

	char data;
	int len;

	if (BM_COM_is_open == true) 
	{
		// Erase data
		data = 0x0;
	
		// Create packet
		len = 1;
		CreatePacket(BM_GET_WBSL_STATUS, len, &data);

		// Flush COM port
		FlushCOM(0);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;

		// Copy data
		dwData = (rx_buf[6]<<24) + (rx_buf[5]<<16) + (rx_buf[4]<<8) + (rx_buf[3]);
	}
	else 
	{
		// Update error state
		BM_errorstate = HW_ERROR;
		return false;
	}
	return true;
}


bool BM_WBSL_Send_Data(BYTE * data, int len, DWORD& dwData){

	unsigned char tP;   // Auxiliar pointer to step through the whole array

	if (BM_COM_is_open == true) 
	{
		tP = 0;
		
		// Check if packet length is more than the maximum bytes that can be sent through the COM
		// if so, we need to to separate the packet in multiple packets of maximum length
		while(len > PACKET_DATA_BYTES)
		{
			CreatePacket(BM_SEND_DATA_WBSL, PACKET_DATA_BYTES, (char *)(&data[tP]));  // Start from the byte next to the total packet lenght
			
			// Send and Receive with 1 ms delay
			send_receive(PACKET_DATA_BYTES,0,2);
			
			// Check response
			BM_errorstate = rx_buf[1];
			if (BM_errorstate != HW_NO_ERROR) return false;

			len -= PACKET_DATA_BYTES;
			tP += PACKET_DATA_BYTES;
			// To make the USB Dongle receive the packets
			Sleep(5);
		}

		// Send last packet which is less or equal to the maximum permited by the COM
		CreatePacket(BM_SEND_DATA_WBSL, len, (char *)(&data[tP]));

		// Send and Receive with 1 ms delay
		send_receive(len,0,2);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;

		// Copy data
		dwData = /*(rx_buf[6]<<24) + (rx_buf[5]<<16) + (rx_buf[4]<<8) +*/ len;//(rx_buf[3]);
	}
	else
	{
		// No COM is available
		return false;
	}
	return true;
}



// ----------------------------------------------------------------------------------------------
// Production test functions

// Init hardware test
bool BM_InitTest(WORD wTestPw)
{
	char data[3];
	int len;

	if (BM_COM_is_open == true) 
	{
		// Set data
		data[0] = (char) wTestPw;
		data[1] = (char) (wTestPw >> 8);

		// Create packet
		len = 2;
		CreatePacket(BM_INIT_TEST, len, data);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;
	}
	else 
	{
		// Update error state
		return false;
	}

	return true;
}


// Continue to next test item
bool BM_NextTest(void)
{
	int len;
	char data[2];

	if (BM_COM_is_open == true) 
	{
		// Create packet
		len = 0;
		CreatePacket(BM_NEXT_TEST, len, data);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 1ms delay
		//Sleep(1);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;
	}
	else 
	{
		// Update error state
		return false;
	}

	return true;
}


// Write byte to flash memory
bool BM_WriteByte(BYTE bByte, WORD wAddr)
{
	int len;
	char data[4];

	if (BM_COM_is_open == true) 
	{
		// Set data
		data[0] = (char) bByte;
		data[1] = (char) wAddr;
		data[2] = (char) (wAddr >> 8);

		// Create packet
		len = 3;
		CreatePacket(BM_WRITE_BYTE, len, data);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// 10ms delay
		Sleep(10);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;
	}
	else 
	{
		// Update error state
		return false;
	}

	return true;
}


// Read test result variable from RF access point
bool BM_GetTestResult(WORD& wResult)
{
	int len;
	char data[2];

	wResult = 0;

	if (BM_COM_is_open == true) 
	{
		// Set data
		data[0] = 0;
		data[1] = 0;

		// Create packet
		len = 2;
		CreatePacket(BM_GET_TEST_RESULT, len, data);

		// Write to COM port
		WriteCOM(0, len+PACKET_OVERHEAD_BYTES, (UCHAR *)tx_buf);

		// Get response packet
		ReadCOM(0, len+PACKET_OVERHEAD_BYTES, rx_buf);

		// Flush COM port
		FlushCOM(0);

		// Check response
		BM_errorstate = rx_buf[1];
		if (BM_errorstate != HW_NO_ERROR) return false;

		// Copy data
		wResult = (rx_buf[4]<<8) + (rx_buf[3]);
	}
	else 
	{
		// Update error state
		return false;
	}

	return true;

}
