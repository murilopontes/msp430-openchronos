/*
Module : enumser.h
Purpose: Defines the interface for a class to enumerate the serial ports installed on a PC
         using a number of different approaches
Created: PJN / 03-11-1998

Copyright (c) 1998 - 2008 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


///////////////////////// Macros / Structs etc ////////////////////////////////

#pragma once

#ifndef __ENUMSER_H__
#define __ENUMSER_H__

#ifndef CENUMERATESERIAL_EXT_CLASS
#define CENUMERATESERIAL_EXT_CLASS
#endif


///////////////////////// Includes ////////////////////////////////////////////                      

#ifdef _AFX
  #ifndef __AFXTEMPL_H__
    #include <afxtempl.h> 
    #pragma message("To avoid this message, please put afxtempl.h in your pre compiled header (normally stdafx.h)")
  #endif
#else
  #ifndef __ATLSTR_H__
    #include <atlstr.h>
    #pragma message("To avoid this message, please put atlstr.h in your pre compiled header (normally stdafx.h). Note non MFC mode is not supported on VC 6")
  #endif  
#endif


///////////////////////// Classes /////////////////////////////////////////////

class CENUMERATESERIAL_EXT_CLASS CEnumerateSerial
{
public:
	
#ifndef NO_ENUMSERIAL_USING_SETUPAPI2
  #ifdef _AFX
    static BOOL UsingSetupAPI2(CUIntArray& ports, CStringArray& sFriendlyNames);
  #else
    static BOOL UsingSetupAPI2(CSimpleArray<UINT>& ports, CSimpleArray<CString>& sFriendlyNames);
  #endif
#endif

protected:
//Methods
  static BOOL IsNumeric(LPCTSTR pszString, BOOL bIgnoreColon);
};

#endif //__ENUMSER_H__
