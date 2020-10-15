#ifndef _USE_COMMRTL_2019_H__
#define _USE_COMMRTL_2019_H__
#pragma once

/*现在只有V141_XP版本*/
#define USE_XP_PLATFORM

/*使用调试版的dll, 不过会有额外的dll依赖*/
//#define USE_SINGLIB_DEBUGDLL

#include "CommRtl.h"


//is using xp platform tools
#if defined(USE_XP_PLATFORM)
#define JIS_XP 1
#else
#define JIS_XP 0
#endif

#if defined(_MT) && (defined(_DLL) || defined(_USEDLL) || defined(_AFXDLL))
//is md
#define JIS_MD 1
#else
//is mt	
#define JIS_MD 0
#endif

//is 64bit?
#if (__amd64 || __amd64__ || _M_X64) || defined(_WIN64) || defined(WIN64) //x64
#define JIS_64bit 1
#else
#define JIS_64bit 0
#endif

#if JIS_64bit>0 //x64
#define JF_BIT "x64"
#ifdef _DEBUG
//debug
#if JIS_MD
//MDd
#define JF_CRT "MD"	
#else
//MTd
#define JF_CRT "MT"
#endif
#else
//release
#error **not have Release version!
#endif

#else //x86
#define JF_BIT "x86"
#ifdef _DEBUG
//debug
#if JIS_MD
//MDd
#define JF_CRT "MD"	
#else
//MTd
#define JF_CRT "MT"
#endif
#else
//release
#error **not have Release version!
#endif

#endif


#if defined(USE_SINGLIB_DEBUGDLL) && ((defined(_DEBUG) || defined(DEBUG)))
#define JF_DEBUG "D"
#else
#define JF_DEBUG ""
#endif // 

#if JIS_XP
#define JF_PLAT "_xp"
#else
#define JF_PLAT ""
#endif

/////////////////////////////////////////////////


#pragma comment(lib, "../lib/" JF_BIT JF_DEBUG  "/dll/" "CommRtl" JF_BIT JF_DEBUG ".lib") 


//for include:  $(HELIB)\SingLib\Compiled_CommRtl\include
//for lib:      $(HELIB)\SingLib\Compiled_CommRtl\lib




#endif // !_USE_COMMRTL_2019_H__