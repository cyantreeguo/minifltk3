//
// "$Id: MenuWindow.h 9648 2012-07-28 13:38:18Z matt $"
//
// Menu window header file for the Fast Light Tool Kit (FLTK).
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

/* \file
 fltk3::MenuWindow widget . */

#ifndef Fltk3_Menu_Window_H
#define Fltk3_Menu_Window_H

#include "SingleWindow.h"

namespace fltk3
{

/**
 The fltk3::MenuWindow widget is a window type used for menus. By
 default the window is drawn in the hardware overlay planes if they are
 available so that the menu don't force the rest of the window to
 redraw.
 */
class FLTK3_EXPORT MenuWindow : public fltk3::SingleWindow
{

public:

	void show();

	void erase();

	void flush();

	void hide();

	/** Tells if hardware overlay mode is set */
	unsigned int overlay() {
		return !(flags()&NO_OVERLAY);
	}

	/** Tells FLTK to use hardware overlay planes if they are available.  */
	void set_overlay() {
		clear_flag(NO_OVERLAY);
	}

	/** Tells FLTK to use normal drawing planes instead of overlay planes.
	 This is usually necessary if your menu contains multi-color pixmaps. */
	void clear_overlay() {
		set_flag(NO_OVERLAY);
	}

	~MenuWindow();

	/** Creates a new fltk3::MenuWindow widget using the given size, and label string. */
	MenuWindow(int W, int H, const char *l = 0);

	/** Creates a new fltk3::MenuWindow widget using the given position, size, and label string. */
	MenuWindow(int X, int Y, int W, int H, const char *l = 0);

};



class PopupWindow : public fltk3::MenuWindow
{
	bool pUserPosition;
	fltk3::Widget* pTrigger;
public:
	static void hide_i(fltk3::Widget*, void*);
	int handle(int);
	PopupWindow(int X, int Y, int W, int H, const char *label = 0);
	PopupWindow(int W, int H, const char *label = 0);
	~PopupWindow();
	Widget* popup();
};


}


#endif

//
// End of "$Id: MenuWindow.h 9648 2012-07-28 13:38:18Z matt $".
//
