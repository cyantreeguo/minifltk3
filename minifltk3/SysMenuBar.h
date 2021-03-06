//
// "$Id: SysMenuBar.h 9300 2012-03-23 20:36:00Z ianmacarthur $"
//
// MacOS system menu bar header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Bill Spitzak and others.
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

#ifndef Fltk3_Sys_Menu_Bar_H
#define Fltk3_Sys_Menu_Bar_H

#include "MenuBar.h"
#include "x.h"

namespace fltk3
{

#if defined(__APPLE__) || defined(FLTK3_DOXYGEN)
	#include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
        // iOS Simulator
		typedef MenuBar SysMenuBar;
    #elif TARGET_OS_IPHONE
        // iOS device
		typedef MenuBar SysMenuBar;
    #elif TARGET_OS_MAC
	
extern class SysMenuBar *sys_menu_bar;

/**
 @brief A class to create, modify and delete menus that appear on Mac OS X in the menu bar at the top of the screen.
 *
 * On other than Mac OS X platforms, fltk3::SysMenuBar is a synonym of class fltk3::MenuBar.
 */
class FLTK3_EXPORT SysMenuBar : public fltk3::MenuBar
{
protected:
	void draw();
public:
	/**
	 @brief The constructor.
	 *
	 * On Mac OS X, all arguments are unused. On other platforms they are used as by fltk3::MenuBar::MenuBar().
	 */
	SysMenuBar(int x,int y,int w,int h,const char *l=0)
		: fltk3::MenuBar(x,y,w,h,l) {
		deactivate();			// don't let the old area take events
		fltk3::sys_menu_bar = this;
	}
	/** The destructor */
	~SysMenuBar() {
		clear();
		fltk3::sys_menu_bar = NULL;
	}
	void menu(const fltk3::MenuItem *m);
	int add(const char* label, unsigned int shortcut, fltk3::Callback*, void *user_data=0, int flags=0);
	int insert(int index, const char* label, unsigned int shortcut, fltk3::Callback *cb, void *user_data=0, int flags=0);
	void remove(int n);
	void replace(int rank, const char *name);
	void clear();
	int clear_submenu(int index);
};

	#else
        // Unsupported platform
		#error unsupported platform
    #endif
#else

typedef MenuBar SysMenuBar;

#endif // defined(__APPLE__) || defined(FLTK3_DOXYGEN)

}

#endif // Fltk3_Sys_Menu_Bar_H

//
// End of "$Id: SysMenuBar.h 9300 2012-03-23 20:36:00Z ianmacarthur $".
//
