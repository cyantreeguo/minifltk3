//
// "$Id: get_key.cxx 9011 2011-08-25 21:50:59Z matt $"
//
// Keyboard state routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#ifdef WIN32
#  include "Platform_win32_get_key.cxx"
#elif defined(__APPLE__)
	#include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
        // iOS Simulator
		#include "Platform_ios_get_key.cxx"
    #elif TARGET_OS_IPHONE
        // iOS device
		#include "Platform_ios_get_key.cxx"
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
		#include "Platform_cocoa_get_key.cxx"
    #else
        // Unsupported platform
		#error unsupported platform
    #endif
#else
#  include "Platform_x11_get_key.cxx"
#endif

//
// End of "$Id: get_key.cxx 9011 2011-08-25 21:50:59Z matt $".
//
