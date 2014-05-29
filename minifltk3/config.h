#ifdef WIN32
	#include "config_win32.h"
#elif defined(__APPLE__)
	#include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
        // iOS Simulator
		#include "config_ios.h"
    #elif TARGET_OS_IPHONE
        // iOS device
		#include "config_ios.h"
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
		#include "config_mac.h"
    #else
        // Unsupported platform
		#error unsupported platform
    #endif
#elif defined linux
	#include "config_linux.h"
#else
	#error unsupported platform
#endif
