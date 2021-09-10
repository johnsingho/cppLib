#-------------------------------------------------
#
# for use SingLib dll
# by H.Z.XIN 2021-05-21T20:43:38
#
#-------------------------------------------------

#for pro
##include($$(HELIB)/SingLib/SingLib.pri)
#include($$(HELIB)/SingLib/Compiled/SingLib.pri)


#in source code
#include <SingLib.h>


INCLUDEPATH += $$PWD/include
contains(QT_ARCH, i386) {
	# 32-bit
	CONFIG(debug, debug|release) {
		LIBS += -L$$PWD/lib/X86D/dll -lSingLibx86D
	} else {
		LIBS += -L$$PWD/lib/X86/dll -lSingLibx86
	} 
} else {
    # 64-bit
	CONFIG(debug, debug|release) {
		LIBS += -L$$PWD/lib/X64D/dll -lSingLibx64D
	} else {
		LIBS += -L$$PWD/lib/X64/dll -lSingLibx64
	} 
}




