//
// "$Id: font.cxx 9330 2012-04-09 08:53:49Z manolo $"
//
// Font selection code for the Fast Light Tool Kit (FLTK).
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
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
/* We require Windows 2000 features such as GetGlyphIndices */
# if !defined(WINVER) || (WINVER < 0x0500)
#  ifdef WINVER
#   undef WINVER
#  endif
#  define WINVER 0x0500
# endif
# if !defined(_WIN32_WINNT) || (_WIN32_WINNT < 0x0500)
#  ifdef _WIN32_WINNT
#   undef _WIN32_WINNT
#  endif
#  define _WIN32_WINNT 0x0500
# endif
#endif

// Select fonts from the FLTK font table.
#include "flstring.h"
#include "run.h"
#include "draw.h"
#include "x.h"
#include "font.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#  include "Platform_win32_font.cxx"
#elif defined(__APPLE__)
	#include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
        // iOS Simulator
		#include "Platform_ios_font.cxx"
    #elif TARGET_OS_IPHONE
        // iOS device
		#include "Platform_ios_font.cxx"
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
		#include "Platform_cocoa_font.cxx"
    #else
        // Unsupported platform
		#error unsupported platform
    #endif
#elif USE_XFT
#  include "Platform_xft_font.cxx"
#else
#  include "Platform_x11_font.cxx"
#endif // WIN32

namespace fltk3
{

double width(const char* c)
{
	if (c) return width(c, (int)strlen(c));
	else return 0.0f;
}

void draw(const char* str, int x, int y)
{
	draw(str, (int)strlen(str), x, y);
}

void draw(int angle, const char* str, int x, int y)
{
	draw(angle, str, (int)strlen(str), x, y);//must be fixed!
}

void text_extents(const char *c, int &dx, int &dy, int &w, int &h)
{
	if (c)  fltk3::text_extents(c, (int)strlen(c), dx, dy, w, h);
	else {
		w = 0;
		h = 0;
		dx = 0;
		dy = 0;
	}
} // fltk3::text_extents


void draw(const char* str, int l, float x, float y)
{
#ifdef __APPLE__
	fltk3::graphics_driver->draw(str, l, x, y);
#else
	fltk3::draw(str, l, (int)x, (int)y);
#endif
}

}

//
// End of "$Id: font.cxx 9330 2012-04-09 08:53:49Z manolo $".
//
