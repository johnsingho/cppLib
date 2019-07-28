using System;
using System.Runtime.InteropServices;

/// <summary>
/// to use Jaslog
/// H.Z.XIN 2017-04-24
///
/// </summary>
namespace JasLogCli
{
    [Flags]
    public enum EJasLogDest
    {
        LOG_NONE = 0,
        LOG_TO_FILE = 1 << 0,
        LOG_TO_SYSTEM_DEBUG_LOG = 1 << 1,
        LOG_TO_ALL = LOG_TO_FILE | LOG_TO_SYSTEM_DEBUG_LOG,
        LOG_DEFAULT = LOG_TO_SYSTEM_DEBUG_LOG,
		LOG_TO_WINEVENT_LOG     = 1 << 2, /*生成windows事件日志*/
		LOG_TO_WINCONSOLE_LOG   = 1 << 3, /*生成windows console*/
		LOG_TO_WINGUI_LOG       = 1 << 4, /*生成windows log dialog*/
    }

    public enum EJasLogLocking
    {
        LOCK_LOG_FILE=0,
        DONT_LOCK_LOG_FILE=1
    }

    public enum EJasLogOldFileState
    {
        DELETE_OLD_LOG_FILE=0,
        APPEND_TO_OLD_LOG_FILE=1
    }

    public enum ELogSeverity
    {
        LOG_VERBOSE = -1,
        LOG_INFO = 0,
        LOG_WARNING = 1,
        LOG_ERROR = 2,
        LOG_FATAL = 3,
        LOG_NUM_SEVERITIES = 4
    }

    public class JasLogCli
    {
        //选择不同版本动态库
#if _X64
        const string DLL_NAME= "JasLogX64";
#else
        const string DLL_NAME = "JasLogX86";
#endif

        [DllImport(DLL_NAME, EntryPoint = "JasLog_Init", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr JasLog_Init(EJasLogDest logging_dest,
                                                String log_file,
                                                EJasLogLocking lock_log,
                                                EJasLogOldFileState delete_old);

        // for windows only
        [DllImport(DLL_NAME, EntryPoint = "JasLog_InitW", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr JasLog_InitW(EJasLogDest logging_dest,
                                                 [MarshalAs(UnmanagedType.LPWStr)]String log_file,
                                                 EJasLogLocking lock_log,
                                                 EJasLogOldFileState delete_old);

        //最低记录的日志LogSeverity等级，默认是0，所有
        [DllImport(DLL_NAME, EntryPoint = "JasLog_SetMinLevel", CallingConvention = CallingConvention.Cdecl)]
        public static extern void JasLog_SetMinLevel(int level);

        [DllImport(DLL_NAME, EntryPoint = "JasLog_GetMinLevel", CallingConvention = CallingConvention.Cdecl)]
        public static extern ELogSeverity JasLog_GetMinLevel();

        [DllImport(DLL_NAME, EntryPoint = "JasLog_ShouldOutputLog", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool JasLog_ShouldOutputLog(ELogSeverity severity);

        [DllImport(DLL_NAME, EntryPoint = "JasLog_SetLogDetail", CallingConvention = CallingConvention.Cdecl)]
        public static extern void JasLog_SetLogDetail(bool enable_process_id,
                                                        bool enable_thread_id,
                                                        bool enable_timestamp,
                                                        bool enable_tickcount);

        [DllImport(DLL_NAME, EntryPoint = "JasLog_SetShowErrorDlg", CallingConvention = CallingConvention.Cdecl)]
        public static extern void JasLog_SetShowErrorDlg(bool enable_dialogs);

        [DllImport(DLL_NAME, EntryPoint = "JasLog_IsLoggingToFileEnabled")]
        public static extern bool JasLog_IsLoggingToFileEnabled();


//! TODO
//         [global::System.Runtime.InteropServices.DllImport(DLL_NAME, EntryPoint = "JasLog_SetLogCheckHandler")]
//         public static extern void JasLog_SetLogCheckHandler(global::System.Runtime.InteropServices.HandleRef jarg1);
// 
//         [global::System.Runtime.InteropServices.DllImport(DLL_NAME, EntryPoint = "JasLog_SetLogHandler")]
//         public static extern void JasLog_SetLogHandler(global::System.Runtime.InteropServices.HandleRef jarg1);
// 
//         [global::System.Runtime.InteropServices.DllImport(DLL_NAME, EntryPoint = "JasLog_GetLogHandler")]
//         public static extern global::System.IntPtr JasLog_GetLogHandler();
// 
//         [global::System.Runtime.InteropServices.DllImport(DLL_NAME, EntryPoint = "JohnLogMore")]
//         public static extern void JohnLogMore([MarshalAs(UnmanagedType.LPStr)]String jarg1, int jarg2, int jarg3, [MarshalAs(UnmanagedType.LPStr)]String jarg4);
//         [global::System.Runtime.InteropServices.DllImport(DLL_NAME, EntryPoint = "JohnLogV")]
//         public static extern void JohnLogV([MarshalAs(UnmanagedType.LPStr)]String jarg1, int jarg2, int jarg3, [MarshalAs(UnmanagedType.LPStr)]String jarg4, global::System.Runtime.InteropServices.HandleRef jarg5);

        [DllImport(DLL_NAME, EntryPoint = "JasLogSimple", CallingConvention = CallingConvention.Cdecl)]
        public static extern void JasLogSimple([MarshalAs(UnmanagedType.LPStr)]String file,
                                               int line,
                                               ELogSeverity severity,
                                               [MarshalAs(UnmanagedType.LPStr)]String str);

        [DllImport(DLL_NAME, EntryPoint = "JasPLog", CallingConvention = CallingConvention.Cdecl)]
        public static extern void JasPLog([MarshalAs(UnmanagedType.LPStr)]String file,
                                            int line,
                                            ELogSeverity severity,
                                            [MarshalAs(UnmanagedType.LPStr)]String str);

    }
}
