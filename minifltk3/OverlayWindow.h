//
// "$Id: OverlayWindow.h 9638 2012-07-24 04:41:38Z matt $"
//
// Overlay window header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::OverlayWindow class . */

#ifndef Fltk3_Overlay_Window_H
#define Fltk3_Overlay_Window_H

#include "DoubleWindow.h"


namespace fltk3
{

/**
 This window provides double buffering and also the ability to draw the
 "overlay" which is another picture placed on top of the main image. The
 overlay is designed to be a rapidly-changing but simple graphic such as
 a mouse selection box. fltk3::OverlayWindow uses the overlay
 planes provided by your graphics hardware if they are available.
 <P>If no hardware support is found the overlay is simulated by drawing
 directly into the on-screen copy of the double-buffered window, and
 "erased" by copying the backbuffer over it again.  This means the
 overlay will blink if you change the image in the window.
 */
class FLTK3_EXPORT OverlayWindow : public fltk3::DoubleWindow
{

#ifndef FLTK3_DOXYGEN
	friend class _Fl_Overlay;
#endif

protected:

	/**
	 You must subclass fltk3::OverlayWindow and provide this method.
	 It is just like a draw() method, except it draws the overlay.
	 The overlay will have already been "cleared" when this is called.  You
	 can use any of the routines described in &lt;fltk3/draw.h&gt;.
	 */
	virtual void draw_overlay() {
		/* must be overridden */
	}

	fltk3::Window *overlay_;

public:

	void show();

	void flush();

	void hide();

	void resize(int,int,int,int);

	~OverlayWindow();

	/** Returns non-zero if there's hardware overlay support */
	int can_do_overlay();

	void redraw_overlay();

	/**
	 See fltk3::OverlayWindow::OverlayWindow(int X, int Y, int W, int H, const char *l)
	 */
	OverlayWindow(int W, int H, const char *l=0);

	/**
	 Creates a new fltk3::OverlayWindow widget using the given
	 position, size, and label (title) string. If the
	 positions (x,y) are not given, then the window manager
	 will choose them.
	 */
	OverlayWindow(int X, int Y, int W, int H, const char *l=0);

	void show(int a, char **b) {
		DoubleWindow::show(a,b);
	}
};

}

#endif

//
// End of "$Id: OverlayWindow.h 9638 2012-07-24 04:41:38Z matt $".
//
