#-------------------------------------------------
#
# for use JasLog
# by H.Z.XIN 2020-03-31T11:43:38
#
#-------------------------------------------------

#for debug, use JasLog
#CONFIG(debug, debug|release){
#DEFINES += JOHN_DEBUG
##include($$(HELIB)/JasLog/JasLog.pri)
#include($$(HELIB)/JasLog/Compiled/JasLog.pri)
#}

#in source code
#ifdef JOHN_DEBUG
#define ENABLE_DLOG_UNICODE 0
#include <jaslog.h>
#endif


INCLUDEPATH += $$PWD/include
contains(QT_ARCH, i386) {
    # 32-bit
    LIBS += -L$$PWD/libX86/dll -lJasLogX86
} else {
    # 64-bit
    LIBS += -L$$PWD/libX64/dll -lJasLogX64
}



