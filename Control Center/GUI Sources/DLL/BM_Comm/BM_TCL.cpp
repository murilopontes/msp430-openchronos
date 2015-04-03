// *************************************************************************************************
//
//	Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/ 
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
// Exported TCL/Tk functions for use by TCL/Tk graphical user interface
// *************************************************************************************************

// *************************************************************************************************
// Include section

#include "stdafx.h"
#include <tcl.h>

#include "BM_API.h"
#include "BM_Sync.h"


// *************************************************************************************************
// Extern section

// Generic
extern bool BM_Reset(void);
extern bool BM_GetStatus(WORD& wStatus);
extern bool BM_GetID(DWORD& dwID);
extern bool BM_GetCOM(unsigned char * portNb);
extern bool BM_OpenCOM(char * strPortName, DWORD dwBaudrate, WORD wTimeout, bool bRTS, bool bDTR);
extern void BM_CloseCOM(void);
// BlueRobin
extern bool BM_BR_GetID(DWORD& dwID);
extern bool BM_BR_SetID(DWORD dwID);
extern bool BM_BR_Start(void);
extern bool BM_BR_Stop(void);
extern bool BM_BR_SetHeartrate(WORD wHeartrate);
extern bool BM_BR_SetSpeed(WORD wSpeed, WORD wDistance);
// SimpliciTI acc/ppt mode
extern bool BM_SPL_Start(void);
extern bool BM_SPL_Stop(void);
extern bool BM_SPL_GetData(DWORD& dwData);
// SimpliciTI Sync mode
extern bool BM_SYNC_Start(void);
extern bool BM_SYNC_SendCommand(BYTE * bData);
extern bool BM_SYNC_GetBufferStatus(WORD& wStatus);
extern bool BM_SYNC_ReadBuffer(BYTE * bData);
// WBSL
extern bool BM_WBSL_Start(void);
extern bool BM_WBSL_Stop();
extern bool BM_WBSL_GetStatus(void);
extern bool BM_WBSL_Send_Data(BYTE * data, DWORD& dwData);
extern bool BM_WBSL_GetMaxPayload(DWORD& dwData);
// Test
extern bool BM_InitTest(WORD wTestPw);
extern bool BM_NextTest(void);
extern bool BM_WriteByte(BYTE bByte, WORD wAddr);
extern bool BM_GetTestResult(WORD& wResult);


// *************************************************************************************************
// COM port management

// Get COM port where RF access point is plugged in
static int tcl_BM_GetCOM (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	const char numpar = 0;	// parameter count

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp, "Error: Parameter count mismatch.", TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	unsigned char portNb;
	char str[16];
	res = BM_GetCOM(portNb);
	if (res)
	{
	    sprintf_s(str, "\\\\.\\COM%d", portNb);
	}
	else
	{
		sprintf_s(str, "NOCOM");
	}

	// Return modified parameter
	ret = Tcl_NewStringObj(str, 10);
    Tcl_SetObjResult(interp, ret);

    if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}


// Open COM port
static int tcl_BM_OpenCOM (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	const char numpar = 5;	// parameter count
	char * str;
	int len;
	long baudrate;
	int timeout;
	int rts;
	int dtr;

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp, "Error: Parameter count mismatch.", TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	str = Tcl_GetStringFromObj(objv[1], &len);
	Tcl_GetLongFromObj(interp, objv[2], &baudrate);
	Tcl_GetIntFromObj(interp, objv[3], &timeout);
	Tcl_GetIntFromObj(interp, objv[4], &rts);
	Tcl_GetIntFromObj(interp, objv[5], &dtr);
	res = BM_OpenCOM(str, baudrate, timeout, false, false);

	// Return modified parameter
	ret = Tcl_NewIntObj(res);
    Tcl_SetObjResult(interp, ret);

    if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}


// Close COM port
static int tcl_BM_CloseCOM (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// function specific
	const char numpar = 0;	// parameter count

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	BM_CloseCOM();

    return TCL_OK;
}


// Reset RF access point
static int tcl_BM_Reset (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	const char numpar = 0;	// parameter count

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	res = BM_Reset();

	// Return modified parameter
	ret = Tcl_NewIntObj(res);
    Tcl_SetObjResult(interp, ret);

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}



// Get RF access point dummy ID - used by live check
static int tcl_BM_GetID (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	const char numpar = 0;	// parameter count
	long tcl_id;
	DWORD dwID;

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	res = BM_GetID(dwID);
	tcl_id = (long)dwID;

	// Return modified parameter
	if (res == true)
	{
		ret = Tcl_NewLongObj(tcl_id);
		Tcl_SetObjResult(interp,ret);
		return TCL_OK;
	}
	else return TCL_ERROR;
}


// Get generic hardware status
// Returns: HW_IDLE, HW_SIMPLICITI_STOPPED, HW_SIMPLICITI_TRYING_TO_LINK, HW_SIMPLICITI_LINKED,
// HW_BLUEROBIN_STOPPED, HW_BLUEROBIN_TRANSMITTING, HW_ERROR, HW_NO_ERROR, HW_NOT_CONNECTED
static int tcl_BM_GetStatus (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	const char numpar = 0;	// parameter count
	int tcl_status;
	WORD wStatus;

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	res = BM_GetStatus(wStatus);

	tcl_status = wStatus;

	// Return modified parameter
	ret = Tcl_NewIntObj(tcl_status);
    Tcl_SetObjResult(interp, ret);

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}



// *************************************************************************************************
// BlueRobin 

// Set BlueRobin transmitter ID
static int tcl_BM_BR_SetID (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	const char numpar = 1;	// parameter count
	int tcl_id;
	DWORD dwID;

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	Tcl_GetIntFromObj(interp,objv[1],&tcl_id);
	dwID = tcl_id;
	res = BM_BR_SetID(dwID);

	// Return modified parameter
	ret = Tcl_NewIntObj(res);
    Tcl_SetObjResult(interp, ret);

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}


// Get BlueRobin transmitter ID
static int tcl_BM_BR_GetID (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	const char numpar = 0;	// parameter count
	long tcl_id;
	DWORD dwID;

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	res = BM_BR_GetID(dwID);
	tcl_id = (long)dwID;

	// Return modified parameter
	ret = Tcl_NewLongObj(tcl_id);
    Tcl_SetObjResult(interp,ret);

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}




// Start BlueRobin transmitter channel
static int tcl_BM_BR_Start (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	const char numpar = 0;	// parameter count

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	res = BM_BR_Start();

	// Return modified parameter
	ret = Tcl_NewIntObj(res);
    Tcl_SetObjResult(interp, ret);

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}


// Stop BlueRobin transmitter
static int tcl_BM_BR_Stop (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	const char numpar = 0;	// parameter count

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	res = BM_BR_Stop();

	// Return modified parameter
	ret = Tcl_NewIntObj(res);
    Tcl_SetObjResult(interp, ret);

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}


// Set BlueRobin heart rate to be transmitted
static int tcl_BM_BR_SetHeartrate (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	const char numpar = 1;	// parameter count
	int tcl_heartrate;
	WORD wHeartrate;

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	Tcl_GetIntFromObj(interp, objv[1], &tcl_heartrate);
	wHeartrate = tcl_heartrate;

	res = BM_BR_SetHeartrate(wHeartrate);

	tcl_heartrate = wHeartrate;

	// Return modified parameter
	ret = Tcl_NewIntObj(tcl_heartrate);
    Tcl_SetObjResult(interp, ret);

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}


// Set BlueRobin speed that will be transmitted
static int tcl_BM_BR_SetSpeed (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	const char numpar = 2;	// parameter count
	int tcl_speed;
	int tcl_distance;
	WORD wSpeed;
	WORD wDistance;

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	Tcl_GetIntFromObj(interp, objv[1], &tcl_speed);
	Tcl_GetIntFromObj(interp, objv[2], &tcl_distance);
	wSpeed = tcl_speed;
	wDistance = tcl_distance;

	res = BM_BR_SetSpeed(wSpeed, wDistance);

	tcl_speed = wSpeed;

	// Return modified parameter
	ret = Tcl_NewIntObj(tcl_speed);
    Tcl_SetObjResult(interp, ret);

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}


// *************************************************************************************************
// SimpliciTI acc/ppt functions

// Start SimpliciTI stack in acc/ppt mode
static int tcl_BM_SPL_Start (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	const char numpar = 0;	// parameter count

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	res = BM_SPL_Start();

	// Return modified parameter
	ret = Tcl_NewIntObj(res);
    Tcl_SetObjResult(interp, ret);

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}


// Stop SimpliciTI stack
static int tcl_BM_SPL_Stop (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	const char numpar = 0;	// parameter count

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	res = BM_SPL_Stop();

	// Return modified parameter
	ret = Tcl_NewIntObj(res);
    Tcl_SetObjResult(interp, ret);

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}


// Get SimpliciTI acc/ppt data (4 bytes)
static int tcl_BM_SPL_GetData (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	const char numpar = 0;	// parameter count
	long tcl_data;
	DWORD dwData;

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	res = BM_SPL_GetData(dwData);
	tcl_data = (long)dwData;

	// Return modified parameter
	ret = Tcl_NewLongObj(tcl_data);
    Tcl_SetObjResult(interp,ret);

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}


// *************************************************************************************************
// Mouse cursor functions

// Get current mouse cursor X coordinates
static int tcl_BM_GetMouseX (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	POINT ptCursorPos;
	
	// function specific
	const char numpar = 0;	// parameter count
	long tcl_data;

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	GetCursorPos(&ptCursorPos);
	tcl_data = ptCursorPos.x;

	// Return modified parameter
	ret = Tcl_NewLongObj(tcl_data);
    Tcl_SetObjResult(interp,ret);

	return TCL_OK;
}


// Get current mouse cursor Y coordinates
static int tcl_BM_GetMouseY (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	POINT ptCursorPos;

	// function specific
	const char numpar = 0;	// parameter count
	long tcl_data;

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	GetCursorPos(&ptCursorPos);
	tcl_data = ptCursorPos.y;

	// Return modified parameter
	ret = Tcl_NewLongObj(tcl_data);
    Tcl_SetObjResult(interp,ret);

	return TCL_OK;
}


// Set new mouse cursor coordinates
static int tcl_BM_SetMouseXY (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// function specific
	const char numpar = 2;	// parameter count
	int tcl_x, tcl_y;

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	Tcl_GetIntFromObj(interp,objv[1],&tcl_x);
	Tcl_GetIntFromObj(interp,objv[2],&tcl_y);
	SetCursorPos(tcl_x, tcl_y);

	// Return modified parameter
	return TCL_OK;
}

// Simulate mouse click
static int tcl_BM_SetMouseClick (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	int mouse;

	// Check if right number of parameters have been passed
	if(objc != 1+1)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters 
	Tcl_GetIntFromObj(interp, objv[1], &mouse);

	if (mouse == 1)	// Left click
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}
	else if (mouse == 2)	// Right click
	{
		mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
	}
	else if (mouse == 3)	// Left double click
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		Sleep(50);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}

	// Return modified parameter
	return TCL_OK;
}


// Simulate complex key press sequence
static int tcl_BM_SetKey (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	int key, windows, alt, ctrl, shift;

	// Check if right number of parameters have been passed
	if(objc != 1+5)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters 
	Tcl_GetIntFromObj(interp, objv[1], &key);
	Tcl_GetIntFromObj(interp, objv[2], &windows);
	Tcl_GetIntFromObj(interp, objv[3], &alt);
	Tcl_GetIntFromObj(interp, objv[4], &ctrl);
	Tcl_GetIntFromObj(interp, objv[5], &shift);

	// Simulate key press
	if (shift) 
	{
		keybd_event( VK_SHIFT, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
	}
	if (ctrl)
	{
		keybd_event( VK_CONTROL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
	}
	if (alt)
	{
		keybd_event( VK_MENU, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
	}
	if (windows)
	{
		keybd_event( VK_LWIN, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
	}
	keybd_event( (BYTE)key, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );

	// Simulate key release
	keybd_event( (BYTE)key, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,	0);
	if (shift) 
	{
		keybd_event( VK_SHIFT, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
	}
	if (ctrl)
	{
		keybd_event( VK_CONTROL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
	}
	if (alt)
	{
		keybd_event( VK_MENU, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
	}
	if (windows)
	{
		keybd_event( VK_LWIN, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
	}	

	return TCL_OK;
}


// ----------------------------------------------------------------------------------------------
// SimpliciTI Sync functions

// Start SimpliciTI sync mode
static int tcl_BM_SYNC_Start (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	bool res;
	
	// Check if right number of parameters have been passed
	if(objc != 1+0)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	res = BM_SYNC_Start();

	// Return modified parameter
	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}

// Send SimpliciTI sync command and parameters to watch
static int tcl_BM_SYNC_SendCommand (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	bool res;
	BYTE data[BM_SYNC_DATA_LEN];
	int tcl_data, i;
	
	// Check if right number of parameters have been passed
	if(objc < 2)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	for (i=1; i<objc; i++)
	{
		Tcl_GetIntFromObj(interp, objv[i], &tcl_data);
		data[i-1] = (BYTE)tcl_data;
	}
	for (i=objc; i<=BM_SYNC_DATA_LEN; i++)
	{
		data[i-1] = 0;
	}
	
	res = BM_SYNC_SendCommand(data);
	
	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}


// Get SimpliciTI sync buffer status
static int tcl_BM_SYNC_GetBufferStatus (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	const char numpar = 0;	// parameter count
	WORD wStatus;

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	res = BM_SYNC_GetBufferStatus(wStatus);

	// Return modified parameter
	ret = Tcl_NewIntObj(wStatus);
    Tcl_SetObjResult(interp, ret);

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}


// Read SimpliciTI sync buffer content
static int tcl_BM_SYNC_ReadBuffer (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	const char numpar = 0;	// parameter count
	BYTE data[BM_SYNC_DATA_LEN+1];
	int i;

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Call function
	res = BM_SYNC_ReadBuffer(data);
	
	// Return modified parameter
	ret = Tcl_NewObj();
	unsigned char *pChar = Tcl_SetByteArrayLength(ret, BM_SYNC_DATA_LEN);
	for (i=0; i<BM_SYNC_DATA_LEN; i++) pChar[i] = data[i];
	Tcl_InvalidateStringRep(ret);
	Tcl_SetObjResult(interp, ret); 

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}


// ----------------------------------------------------------------------------------------------
// Wireless Updater functions

// WBSL Stack routines
static int tcl_BM_WBSL_Start (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	bool res;

	// Check if right number of parameters have been passed
	if(objc != 1+0)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	res = BM_WBSL_Start();

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}

static int tcl_BM_WBSL_Stop (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	bool res;
	
	// Check if right number of parameters have been passed
	if(objc != 1+0)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	res = BM_WBSL_Stop();

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}

static int tcl_BM_WBSL_GetMaxPayload (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	long tcl_data;
	DWORD dwData;

	// Check if right number of parameters have been passed
	if(objc != 1+0)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	res = BM_WBSL_GetMaxPayload(dwData);
	tcl_data = (long)dwData;

	// Return modified parameter
	ret = Tcl_NewLongObj(tcl_data);
    Tcl_SetObjResult(interp,ret);

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}

static int tcl_BM_WBSL_SendData (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	long tcl_status;
	DWORD dwData;
	int total_bytes;
	int data_or_info;
	BYTE * data;
	BYTE * totalBytes;
	BYTE * extraData;
	int len;

	// Check if right number of parameters have been passed
	if(objc < 3)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }
    
    // Extract parameters and call function
	Tcl_GetIntFromObj(interp, objv[1], &data_or_info);
	if(data_or_info == 0)
	{
		totalBytes = new BYTE[3];
		Tcl_GetIntFromObj(interp, objv[2], &total_bytes);
		totalBytes[0] = WBSL_INFO_PACKET; // Tell the Dongle that this packets is telling how many bytes are there in total
		totalBytes[1] = (total_bytes & 0xFF);
        totalBytes[2] = ((total_bytes >> 8) & 0xFF);
		res = BM_WBSL_Send_Data(totalBytes, 3, dwData);
		delete totalBytes;
	}
	else
	{
		data = Tcl_GetByteArrayFromObj(objv[2], &len);
		extraData = new BYTE[len+2]; // Create an array which holds the data plus 2 bytes (one indicating the size of the packet and the other a flag to tell if it includes an address
		//Save the length of the packet on the first byte
		extraData[1] = len;
		for(int i = 0; i < len; i++)
		{
			// Copy the data to the new array
			extraData[i+2] = data[i];
		}
		// Check to see if the packet contains an address or not
		extraData[0] = (data_or_info == 1 ? WBSL_ADDRESS_PACKET : WBSL_NORMAL_PACKET);
		
		res = BM_WBSL_Send_Data(extraData, len+2, dwData);
		delete extraData;
	}

	
	tcl_status = (long)dwData;

	// Return modified parameter
	ret = Tcl_NewIntObj(tcl_status);
    Tcl_SetObjResult(interp, ret);

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}

static int tcl_BM_WBSL_GetPacketStatus (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
    // Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	const char numpar = 0;	// parameter count
	long tcl_data;
	DWORD dwData;

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	res = BM_WBSL_GetPacketStatus(dwData);
	tcl_data = (long)dwData;

	// Return modified parameter
	ret = Tcl_NewLongObj(tcl_data);
    Tcl_SetObjResult(interp,ret);

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}

static int tcl_BM_WBSL_GetStatus (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	
	// function specific
	const char numpar = 0;	// parameter count
	long tcl_data;
	DWORD dwData;

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	res = BM_WBSL_GetStatus(dwData);
	tcl_data = (long)dwData;

	// Return modified parameter
	ret = Tcl_NewLongObj(tcl_data);
    Tcl_SetObjResult(interp,ret);

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}


// ----------------------------------------------------------------------------------------------
// Production test functions

// Init hardware test
static int tcl_BM_InitTest (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	bool res;
	
	// function specific
	const char numpar = 1;	// parameter count
	int tcl_test_pw;
	WORD wTestPw;

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	Tcl_GetIntFromObj(interp, objv[1], &tcl_test_pw);
	wTestPw = tcl_test_pw;
	res = BM_InitTest(wTestPw);

	// Return modified parameter
	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}

// Continue to next test item
static int tcl_BM_NextTest (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	bool res;
	
	// function specific
	const char numpar = 0;	// parameter count

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	res = BM_NextTest();

	// Return modified parameter
	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}


// Write byte to flash memory
static int tcl_BM_WriteByte (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	bool res;
	
	// function specific
	const char numpar = 2;	// parameter count
	int tcl_byte;
	int tcl_addr;
	BYTE bByte;
	WORD wAddr;

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	Tcl_GetIntFromObj(interp, objv[1], &tcl_byte);
	Tcl_GetIntFromObj(interp, objv[2], &tcl_addr);
	bByte = (BYTE)tcl_byte;
	wAddr = tcl_addr;
	res = BM_WriteByte(bByte, wAddr);

	// Return modified parameter
	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}


// Read test result variable from RF access point
static int tcl_BM_GetTestResult (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
{
	// Returned object
	Tcl_Obj *ret;
	bool res;
	WORD testresult;
	int tcl_testresult;
	
	// function specific
	const char numpar = 0;	// parameter count

	// Check if right number of parameters have been passed
	if(objc != 1+numpar)
	{
		Tcl_SetResult(interp,"Error: Parameter count mismatch.",TCL_STATIC);
        return TCL_ERROR;
    }

	// Extract parameters and call function
	res = BM_GetTestResult(testresult);
	tcl_testresult = (int)testresult;

	// Return modified parameter
	ret = Tcl_NewIntObj(tcl_testresult);
    Tcl_SetObjResult(interp, ret);

	if (res == true)	return TCL_OK;
	else				return TCL_ERROR;
}



// ----------------------------------------------------------------------------------------------
// TCL interface section
//
// This part exports wraps above functions and makes them accessible to TCL.

#ifdef __cplusplus
extern "C"
#endif
BR_COMM_API int Ez_Init(Tcl_Interp *interp) 
{
	if (Tcl_InitStubs(interp, TCL_VERSION, 1) == NULL) {
			Tcl_SetResult(interp, "Tcl_InitStubs failed", TCL_STATIC);
		return TCL_ERROR;
	}
	else Tcl_SetResult(interp, "Library loaded successfully.", TCL_STATIC);

	Tcl_PkgProvide(interp, "eZ430_Chronos_CC", "1.0");

	Tcl_CreateObjCommand(interp, "BM_Reset", tcl_BM_Reset, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_GetStatus", tcl_BM_GetStatus, NULL, NULL);
	Tcl_CreateObjCommand(interp, "BM_GetID", tcl_BM_GetID, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_OpenCOM", tcl_BM_OpenCOM, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_CloseCOM", tcl_BM_CloseCOM, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_GetCOM", tcl_BM_GetCOM, NULL, NULL);

	Tcl_CreateObjCommand(interp, "BM_BR_SetID", tcl_BM_BR_SetID, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_BR_GetID", tcl_BM_BR_GetID, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_BR_Start", tcl_BM_BR_Start, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_BR_Stop", tcl_BM_BR_Stop, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_BR_SetHeartrate", tcl_BM_BR_SetHeartrate, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_BR_SetSpeed", tcl_BM_BR_SetSpeed, NULL, NULL);

	Tcl_CreateObjCommand(interp, "BM_SPL_Start", tcl_BM_SPL_Start, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_SPL_Stop", tcl_BM_SPL_Stop, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_SPL_GetData", tcl_BM_SPL_GetData, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_GetMouseX", tcl_BM_GetMouseX, NULL, NULL);
	Tcl_CreateObjCommand(interp, "BM_GetMouseY", tcl_BM_GetMouseY, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_SetMouseXY", tcl_BM_SetMouseXY, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_SetMouseClick", tcl_BM_SetMouseClick, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_SetKey", tcl_BM_SetKey, NULL, NULL);

	Tcl_CreateObjCommand(interp, "BM_SYNC_Start", tcl_BM_SYNC_Start, NULL, NULL);
	Tcl_CreateObjCommand(interp, "BM_SYNC_SendCommand", tcl_BM_SYNC_SendCommand, NULL, NULL);
	Tcl_CreateObjCommand(interp, "BM_SYNC_GetBufferStatus", tcl_BM_SYNC_GetBufferStatus, NULL, NULL);
	Tcl_CreateObjCommand(interp, "BM_SYNC_ReadBuffer", tcl_BM_SYNC_ReadBuffer, NULL, NULL);

    Tcl_CreateObjCommand(interp, "BM_WBSL_Start", tcl_BM_WBSL_Start, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_WBSL_Stop", tcl_BM_WBSL_Stop, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_WBSL_GetStatus", tcl_BM_WBSL_GetStatus, NULL, NULL);
	Tcl_CreateObjCommand(interp, "BM_WBSL_SendData", tcl_BM_WBSL_SendData, NULL, NULL);
	Tcl_CreateObjCommand(interp, "BM_WBSL_GetPacketStatus", tcl_BM_WBSL_GetPacketStatus, NULL, NULL);
	Tcl_CreateObjCommand(interp, "BM_WBSL_GetMaxPayload", tcl_BM_WBSL_GetMaxPayload, NULL, NULL);

    Tcl_CreateObjCommand(interp, "BM_InitTest", tcl_BM_InitTest, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_NextTest", tcl_BM_NextTest, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_WriteByte", tcl_BM_WriteByte, NULL, NULL);
    Tcl_CreateObjCommand(interp, "BM_GetTestResult", tcl_BM_GetTestResult, NULL, NULL);

    return TCL_OK;
}
