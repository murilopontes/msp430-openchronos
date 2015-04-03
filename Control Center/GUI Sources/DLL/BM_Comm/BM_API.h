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

// COM port packet format
//
// Byte 0	Start marker (0xFF)
// Byte 1	Command code
// Byte 2	Packet size (including overhead)
// Byte 3+	Data
#define	PACKET_OVERHEAD_BYTES		3
#define	PACKET_DATA_BYTES			28
#define PACKET_TOTAL_BYTES			(PACKET_OVERHEAD_BYTES + PACKET_DATA_BYTES)

// Generic commands
#define BM_RESET					0x01
#define BM_GET_STATUS				0x00
#define BM_GET_PRODUCT_ID			0x20

// BlueRobin
#define BM_START_BLUEROBIN			0x02
#define BM_SET_BLUEROBIN_ID			0x03
#define BM_GET_BLUEROBIN_ID			0x04
#define BM_SET_HEARTRATE			0x05    
#define BM_SET_SPEED				0x0A
#define BM_STOP_BLUEROBIN			0x06

// SimpliciTI acc/ppt mode
#define BM_START_SIMPLICITI			0x07
#define BM_GET_SIMPLICITIDATA		0x08
#define BM_STOP_SIMPLICITI			0x09

// SimpliciTI sync mode
#define BM_SYNC_START			    0x30
#define BM_SYNC_SEND_COMMAND        0x31
#define BM_SYNC_GET_BUFFER_STATUS   0x32
#define BM_SYNC_READ_BUFFER			0x33

//Wireless BSL
#define BM_START_WBSL               0x40
#define BM_GET_WBSL_STATUS          0x41
#define BM_INIT_OK_WBSL             0x42
#define BM_INIT_INVALID_WBSL        0x43
#define BM_TRANSFER_OK_WBSL         0x44
#define BM_TRANSFER_INVALID_WBSL    0x45
#define BM_STOP_WBSL                0x46
#define BM_SEND_DATA_WBSL           0x47
#define BM_GET_PACKET_STATUS_WBSL   0x48
#define BM_GET_MAX_PAYLOAD_WBSL     0x49

//WBSL Packet Type flag
#define WBSL_INFO_PACKET            0x00
#define WBSL_ADDRESS_PACKET         0x01
#define WBSL_NORMAL_PACKET          0x02

// Production test commands
#define BM_INIT_TEST				0x70
#define BM_NEXT_TEST				0x71
#define BM_WRITE_BYTE				0x72
#define BM_GET_TEST_RESULT			0x73

// BM_GetStatus system states  
#define HW_IDLE                         0x00
#define HW_SIMPLICITI_STOPPED           0x01
#define HW_SIMPLICITI_TRYING_TO_LINK    0x02
#define HW_SIMPLICITI_LINKED            0x03
#define HW_BLUEROBIN_STOPPED            0x04
#define HW_BLUEROBIN_TRANSMITTING       0x05
#define HW_ERROR					    0x05
#define HW_NO_ERROR					    0x06
#define HW_NOT_CONNECTED				0x07

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL much easier. All files within the DLL are compiled with the BR_COMM_EXPORTS
// symbol defined on the command line. The symbol should not be defined in any project
// using the DLL. This way any other project whose source files include this header file see 
// BR_COMM_API functions as being imported from a DLL, whereas the DLL itself sees symbols
// defined with this macro as having to be exported.
#define BR_COMM_EXPORTS
#ifdef BR_COMM_EXPORTS
	#define BR_COMM_API __declspec(dllexport)
#else
	#define BR_COMM_API __declspec(dllimport)
#endif


// Open the communication port to the RF access point
BR_COMM_API bool BM_OpenCOM(char * strPortName, DWORD dwBaudrate = 115200, WORD wTimeout = 30, bool bRTS = false, bool bDTR = false);

// Close the communication port to the RF access point (if open)
BR_COMM_API void BM_CloseCOM();

// Get the COM port number where RF access point is plugged in
BR_COMM_API bool BM_GetCOM(unsigned char& bPortNb);

// Initialize driver DLL and RF access point
BR_COMM_API bool BM_Reset(void);

// Get status from the hardware
BR_COMM_API bool BM_GetStatus(WORD& wStatus);

// Set BlueRobin transmitter heart rate value
BR_COMM_API bool BM_BR_SetHeartrate(WORD wHeartrate);

// Set BlueRobin transmitter ID
BR_COMM_API bool BM_BR_SetID(DWORD dwID);

// Get BlueRobin transmitter ID 
BR_COMM_API bool BM_BR_GetID(DWORD& dwID);

// Start BlueRobin transmitter
BR_COMM_API bool BM_BR_Start(void);

// Stop BlueRobin transmitter
BR_COMM_API bool BM_BR_Stop(void);

// Start RF access point in SimpliciTI acc/ppt mode
BR_COMM_API bool BM_SPL_Start(void);

// Start RF access point in SimpliciTI sync mode
BR_COMM_API bool BM_SYNC_Start(void);

// Stop SimpliciTI
BR_COMM_API bool BM_SPL_Stop(void);

// Get SimpliciTI acc/ppt data
BR_COMM_API bool BM_SPL_GetData(DWORD& wData);

// Get sync buffer status from RF access point
BR_COMM_API bool BM_SYNC_GetBufferStatus(WORD& wStatus);

// Get SimpliciTI sync buffer content
BR_COMM_API bool BM_SYNC_ReadBuffer(BYTE * bData);

// Start WBSL
BR_COMM_API bool BM_WBSL_Start(void);

// Stop WBSL
BR_COMM_API bool BM_WBSL_Stop(void);

// Get status of update
BR_COMM_API bool BM_WBSL_GetStatus(DWORD& wData);

// Send Data (Text File parts) to the End Device
BR_COMM_API bool BM_WBSL_Send_Data(BYTE * data, int len, DWORD& dwData);

// Get status of packet (checks if a new packet needs to be sent from the GUI)
BR_COMM_API bool BM_WBSL_GetPacketStatus(DWORD& wData);