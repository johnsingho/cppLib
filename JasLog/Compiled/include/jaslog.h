///////////////////////////////////////////////////////////////////////////////
//
//  Module: jaslog.h
//
//  Desc: simple log for ansi only
//
//  Writer: H.Z.XIN 2016-09-13
//
//  Copyright (c) 2016 cygcontrol
//
///////////////////////////////////////////////////////////////////////////////
#ifndef JASLOG_H_2016_
#define JASLOG_H_2016_

#ifdef JOHN_EXPORTS
#include "dllexport.h"
#else
#if defined(_WIN32) || defined(_MSC_VER)
#define OS_WIN   1
#define JOHN_DLL __declspec(dllimport)
#else
#define OS_POSIX 1
#if __GNUC__ >= 4
#define JOHN_DLL  __attribute__ ((visibility ("default")))
#else
#define JOHN_DLL
#endif
#endif
#endif

//#include <string>
#include <stdarg.h>

#ifndef JOHN_CDECL
#ifdef _MSC_VER
#  define JOHN_CDECL  __cdecl
#else
#  define JOHN_CDECL
#endif
#endif

#if defined(_MSC_VER) && !defined(JOHN_EXPORTS)
#if defined(DEBUG) || defined(_DEBUG)
#if defined(_WIN64)
#pragma comment(lib, "JasLogX64D.lib")
#else
#pragma comment(lib, "JasLogX86D.lib")
#endif

#else
#if defined(_WIN64)
#pragma comment(lib, "JasLogX64.lib")
#else
#pragma comment(lib, "JasLogX86.lib")
#endif
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#else
#define bool _Bool
#define true 1
#define false 0
#endif


#if !defined(ENABLE_DLOG_UNICODE) && (defined(_UNICODE)||defined(UNICODE))
#define ENABLE_DLOG_UNICODE 1
#endif

#if defined(DEBUG) || defined(_DEBUG) || defined(DLOG_ALWAYS_ON)
#define ENABLE_DLOG 1
#else
#define ENABLE_DLOG 0
#endif


// Where to record logging output?
// A flat file and/or system debug log via OutputDebugString.
typedef enum
{
    LOG_NONE                = 0,
    LOG_TO_FILE             = 1 << 0,
    LOG_TO_SYSTEM_DEBUG_LOG = 1 << 1,
	LOG_TO_ALL              = LOG_TO_FILE | LOG_TO_SYSTEM_DEBUG_LOG,
#if defined(OS_WIN)
	LOG_TO_WINEVENT_LOG     = 1 << 2, /*生成windows事件日志*/
	LOG_TO_WINCONSOLE_LOG   = 1 << 3, /*生成windows console*/
	LOG_TO_WINGUI_LOG       = 1 << 4, /*生成windows log dialog*/
#endif

    //默认输出
#if defined(OS_WIN)
    LOG_DEFAULT = LOG_TO_FILE,
#elif defined(OS_POSIX)
    LOG_DEFAULT = LOG_TO_SYSTEM_DEBUG_LOG,
#endif
} EJasLogDest;


//日志文件是否有共享锁定
typedef enum
{
    LOCK_LOG_FILE,
    DONT_LOCK_LOG_FILE
} EJasLogLocking;

//是否追加日志
typedef enum
{
    DELETE_OLD_LOG_FILE,
    APPEND_TO_OLD_LOG_FILE
} EJasLogOldFileState;


//此函数的调用不是必须的。
//一般使用方式： JasLog_Init(LOG_DEFAULT, NULL, LOCK_LOG_FILE, APPEND_TO_OLD_LOG_FILE);
//默认日志输出文件是 trace.log
JOHN_DLL bool JasLog_Init(unsigned int logging_dest/*=LOG_DEFAULT*/,
                          const char* log_file/*=NULL*/,
                          EJasLogLocking lock_log/*=LOCK_LOG_FILE*/,
                          EJasLogOldFileState delete_old/*=APPEND_TO_OLD_LOG_FILE*/);
#ifdef OS_WIN
//for windows only
JOHN_DLL bool JasLog_InitW(unsigned int logging_dest/*=LOG_DEFAULT*/,
                           const wchar_t* log_file/*=NULL*/,
                           EJasLogLocking /*lock_log=LOCK_LOG_FILE*/,
                           EJasLogOldFileState delete_old/*=APPEND_TO_OLD_LOG_FILE*/);
#endif

//最低记录的日志LogSeverity等级，默认是0，所有
JOHN_DLL void JasLog_SetMinLevel(int level);

// Gets the current log level.
JOHN_DLL int JasLog_GetMinLevel();

// 是否应该输出此等级的日志
JOHN_DLL bool JasLog_ShouldOutputLog(int severity);

// Sets the common items you want to be prepended to each log message.
// process and thread IDs default to off, the timestamp defaults to on.
// If this function is not called, logging defaults to writing the timestamp only.
JOHN_DLL void JasLog_SetLogDetail(bool enable_process_id/*=false*/, 
								  bool enable_thread_id/*=false*/,
                                  bool enable_timestamp/*=true*/, 
								  bool enable_tickcount/*=false*/,
								  bool enable_src_code_info/*=true*/
								 );

// for windows only
// 在fata时显示一个MessageBox
// Sets whether or not you'd like to see fatal debug messages popped up in
// a dialog box or not.
// Dialogs are not shown by default.
JOHN_DLL void JasLog_SetShowErrorDlg(bool enable_dialogs);

// Returns true if logging to file is enabled.
JOHN_DLL bool JasLog_IsLoggingToFileEnabled();

// 检查错误后的处理
// 默认处理是显示错误信息，然后终止程序
typedef void (*pfnLogAssertHandler)(const char* pstr);
JOHN_DLL void JasLog_SetLogCheckHandler(pfnLogAssertHandler handler);


// 优先处理日志
// 返回true的话，不被其它处理器处理
typedef bool (*pfnLogMessageHandler)(int severity,
                                     const char* file,
                                     int line,
                                     size_t message_start,
                                     const char* str);
JOHN_DLL void JasLog_SetLogHandler(pfnLogMessageHandler handler);
JOHN_DLL pfnLogMessageHandler JasLog_GetLogHandler();

typedef int LogSeverity;
const LogSeverity LOG_VERBOSE = -1;

const LogSeverity LOG_INFO = 0;
const LogSeverity LOG_WARNING = 1;
const LogSeverity LOG_ERROR = 2;
const LogSeverity LOG_FATAL = 3;
const LogSeverity LOG_NUM_SEVERITIES = 4;


/////////////////////////////////////////
JOHN_DLL void JasLogSimpleA(const char* file, int line, LogSeverity severity, const char* pstr);
JOHN_DLL void JOHN_CDECL JohnLogMoreA(const char* file, int line, LogSeverity severity, const char* fmt, ...);
JOHN_DLL void JOHN_CDECL JohnLogVA(const char* file, int line, LogSeverity severity, const char* fmt, va_list ap);
JOHN_DLL void JasPLogA(const char* file, int line, LogSeverity severity, const char* pstr);
JOHN_DLL int JasLogHex(const void* data, size_t data_length, LogSeverity severity);

#if defined(OS_WIN)
JOHN_DLL void JasLogSimpleW(const char* file, int line, LogSeverity severity, const wchar_t* pstr);
JOHN_DLL void JOHN_CDECL JohnLogMoreW(const char* file, int line, LogSeverity severity, const wchar_t* fmt, ...);
JOHN_DLL void JOHN_CDECL JohnLogVW(const char* file, int line, LogSeverity severity, const wchar_t* fmt, va_list ap);
JOHN_DLL void JasPLogW(const char* file, int line, LogSeverity severity, const wchar_t* pstr);
#endif

#if defined(OS_WIN) && defined(ENABLE_DLOG_UNICODE) && (ENABLE_DLOG_UNICODE>0)
#define JasLogSimple	JasLogSimpleW
#define JohnLogMore		JohnLogMoreW
#define JohnLogV		JohnLogVW
#define JasPLog			JasPLogW
#else
#define JasLogSimple	JasLogSimpleA
#define JohnLogMore		JohnLogMoreA
#define JohnLogV		JohnLogVA
#define JasPLog			JasPLogA
#endif

#define JASLOG_CHECK(cond) if(!(cond)){JasLogSimple(__FILE__, __LINE__, LOG_FATAL, #cond);}
#define JASLOG_ASSERT(cond) JASLOG_CHECK(cond)

#if defined(_MSC_VER) && (_MSC_VER<1400)
#define NO_VARGS 1
#endif


#ifdef NO_VARGS
//用于VC6, 仅能使用单条文本
//不支持printf风格
#define JASLOG_SIMPLE(astr)        JasLogSimple(__FILE__, __LINE__, LOG_INFO, astr)
#define JASLOG_OUT(s, astr)        JasLogSimple(__FILE__, __LINE__, s, astr)
#define JASLOG_INFO(astr)          JASLOG_OUT(LOG_INFO, astr)
#define JASLOG_WARNING(astr)       JASLOG_OUT(LOG_WARNING, astr)
#define JASLOG_ERROR(astr)         JASLOG_OUT(LOG_ERROR, astr)
#define JASLOG_FATAL(astr)         JASLOG_OUT(LOG_FATAL, astr)
#define JASLOG_OUT_P(s,astr)       JasPLog(__FILE__, __LINE__, s, astr)
#define JASLOG_INFO_P(astr)        JASLOG_OUT_P(LOG_INFO,astr)
#define JASLOG_WARNING_P(astr)     JASLOG_OUT_P(LOG_WARNING,astr)
#define JASLOG_ERROR_P(astr)       JASLOG_OUT_P(LOG_ERROR,astr)
#define JASLOG_FATAL_P(astr)       JASLOG_OUT_P(LOG_FATAL,astr)

//cond成立时才输出日志
#define JASLOG_OUT_IF(cond, s, astr)  if(cond){JasLogSimple(__FILE__, __LINE__, s, astr);}
#define JASLOG_INFO_IF(cond,astr)     JASLOG_OUT_IF(cond,LOG_INFO,astr)
#define JASLOG_WARNING_IF(cond,astr)  JASLOG_OUT_IF(cond,LOG_WARNING,astr)
#define JASLOG_ERROR_IF(cond,astr)    JASLOG_OUT_IF(cond,LOG_ERROR,astr)
#define JASLOG_FATAL_IF(cond,astr)    JASLOG_OUT_IF(cond,LOG_FATAL,astr)

#if ENABLE_DLOG
#define DJASLOG_SIMPLE(astr)         JASLOG_SIMPLE(astr)
#define DJASLOG_OUT(s, astr)         JASLOG_OUT(s,astr)
#define DJASLOG_INFO(astr)           JASLOG_INFO(astr)
#define DJASLOG_WARNING(astr)        JASLOG_WARNING(astr)
#define DJASLOG_ERROR(astr)          JASLOG_ERROR(astr)
#define DJASLOG_FATAL(astr)          JASLOG_FATAL(astr)
#define DJASLOG_CHECK(cond)          JASLOG_CHECK(cond)
#define DJASLOG_OUT_P(s,astr)        JASLOG_OUT_P(s,astr)
#define DJASLOG_INFO_P(astr)         JASLOG_INFO_P(astr)
#define DJASLOG_WARNING_P(astr)      JASLOG_WARNING_P(astr)
#define DJASLOG_ERROR_P(astr)        JASLOG_ERROR_P(astr)
#define DJASLOG_FATAL_P(astr)        JASLOG_FATAL_P(astr)
#else
#define DJASLOG_SIMPLE(str)      ((void)0)
#define DJASLOG_OUT(s, aa)       ((void)0)
#define DJASLOG_INFO(aa)         ((void)0)
#define DJASLOG_WARNING(aa)      ((void)0)
#define DJASLOG_ERROR(aa)        ((void)0)
#define DJASLOG_FATAL(aa)        ((void)0)
#define DJASLOG_CHECK(cond)      ((void)0)
#define DJASLOG_OUT_P(s,str)     ((void)0)
#define DJASLOG_INFO_P(str)      ((void)0)
#define DJASLOG_WARNING_P(str)   ((void)0)
#define DJASLOG_ERROR_P(str)     ((void)0)
#define DJASLOG_FATAL_P(str)     ((void)0)
#endif
#else//NO_VARGS
#define JASLOG_SIMPLE(str)        JasLogSimple(__FILE__, __LINE__, LOG_INFO, str)
#define JASLOG_OUT(s,fmt,...)     JohnLogMore(__FILE__, __LINE__, s, fmt, ##__VA_ARGS__)
#define JASLOG_INFO(fmt, ...)     JASLOG_OUT(LOG_INFO, fmt, ##__VA_ARGS__)
#define JASLOG_WARNING(fmt, ...)  JASLOG_OUT(LOG_WARNING, fmt, ##__VA_ARGS__)
#define JASLOG_ERROR(fmt, ...)    JASLOG_OUT(LOG_ERROR, fmt, ##__VA_ARGS__)
#define JASLOG_FATAL(fmt, ...)    JASLOG_OUT(LOG_FATAL, fmt, ##__VA_ARGS__)
#define JASLOG_OUT_P(s,str)       JasPLog(__FILE__, __LINE__, s, str)
#define JASLOG_INFO_P(str)        JASLOG_OUT_P(LOG_INFO,str)
#define JASLOG_WARNING_P(str)     JASLOG_OUT_P(LOG_WARNING,str)
#define JASLOG_ERROR_P(str)       JASLOG_OUT_P(LOG_ERROR,str)
#define JASLOG_FATAL_P(str)       JASLOG_OUT_P(LOG_FATAL,str)

//cond成立时才输出日志
#define JASLOG_OUT_IF(cond, s, fmt, ...)  if(cond){JASLOG_OUT(s, fmt, ##__VA_ARGS__);}
#define JASLOG_INFO_IF(cond,fmt, ...)     JASLOG_OUT_IF(cond,LOG_INFO,fmt,##__VA_ARGS__)
#define JASLOG_WARNING_IF(cond,fmt, ...)  JASLOG_OUT_IF(cond,LOG_WARNING,fmt,##__VA_ARGS__)
#define JASLOG_ERROR_IF(cond,fmt, ...)    JASLOG_OUT_IF(cond,LOG_ERROR,fmt,##__VA_ARGS__)
#define JASLOG_FATAL_IF(cond,fmt, ...)    JASLOG_OUT_IF(cond,LOG_FATAL,fmt,##__VA_ARGS__)

#if ENABLE_DLOG
#define DJASLOG_SIMPLE(str)         JASLOG_SIMPLE(str)
#define DJASLOG_OUT(s, fmt, ...)    JASLOG_OUT(s,fmt,##__VA_ARGS__)
#define DJASLOG_INFO(fmt,...)       JASLOG_INFO(fmt,##__VA_ARGS__)
#define DJASLOG_WARNING(fmt,...)    JASLOG_WARNING(fmt,##__VA_ARGS__)
#define DJASLOG_ERROR(fmt,...)      JASLOG_ERROR(fmt,##__VA_ARGS__)
#define DJASLOG_FATAL(fmt,...)      JASLOG_FATAL(fmt,##__VA_ARGS__)
#define DJASLOG_CHECK(cond)         JASLOG_CHECK(cond)
#define DJASLOG_OUT_P(s,str)        JASLOG_OUT_P(s,str)
#define DJASLOG_INFO_P(str)         JASLOG_INFO_P(str)
#define DJASLOG_WARNING_P(str)      JASLOG_WARNING_P(str)
#define DJASLOG_ERROR_P(str)        JASLOG_ERROR_P(str)
#define DJASLOG_FATAL_P(str)        JASLOG_FATAL_P(str)
#else
#define DJASLOG_SIMPLE(str)      ((void)0)
#define DJASLOG_OUT(s, fmt, ...) ((void)0)
#define DJASLOG_INFO(fmt,...)    ((void)0)
#define DJASLOG_WARNING(fmt,...) ((void)0)
#define DJASLOG_ERROR(fmt,...)   ((void)0)
#define DJASLOG_FATAL(fmt,...)   ((void)0)
#define DJASLOG_CHECK(cond)      ((void)0)
#define DJASLOG_OUT_P(s,str)     ((void)0)
#define DJASLOG_INFO_P(str)      ((void)0)
#define DJASLOG_WARNING_P(str)   ((void)0)
#define DJASLOG_ERROR_P(str)     ((void)0)
#define DJASLOG_FATAL_P(str)     ((void)0)
#endif
#endif//NO_VARGS



#ifdef __cplusplus
}
#endif
#endif // JASLOG_H_2016_
