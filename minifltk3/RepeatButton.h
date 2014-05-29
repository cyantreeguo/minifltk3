//
// "$Id: RepeatButton.h 9436 2012-05-02 21:24:21Z matt $"
//
// Repeat button header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::RepeatButton widget . */

#ifndef Fltk3_Repeat_Button_H
#define Fltk3_Repeat_Button_H

#include "run.h"
#include "Button.h"

namespace fltk3
{

/**
 The fltk3::RepeatButton is a subclass of fltk3::Button that
 generates a callback when it is pressed and then repeatedly generates
 callbacks as long as it is held down.  The speed of the repeat is fixed
 and depends on the implementation.
 */
#define INITIALREPEAT .5
#define REPEAT .1
class FLTK3_EXPORT RepeatButton : public Button
{
private:

	static void repeat_callback(void *v) {
		fltk3::Button *b = (fltk3::Button*)v;
		fltk3::add_timeout(REPEAT,repeat_callback,b);
		b->do_callback();
	}

public:

	int handle(int event) {
		int newval;
		switch (event) {
		case fltk3::HIDE:
		case fltk3::DEACTIVATE:
		case fltk3::RELEASE:
			newval = 0;
			goto J1;
		case fltk3::PUSH:
		case fltk3::DRAG:
			if (fltk3::visible_focus()) fltk3::focus(this);
			newval = fltk3::event_inside(0, 0, w(), h());
J1:
			if (!active())
				newval = 0;
			if (value(newval)) {
				if (newval) {
					fltk3::add_timeout(INITIALREPEAT,repeat_callback,this);
					do_callback();
				} else {
					fltk3::remove_timeout(repeat_callback,this);
				}
			}
			return 1;
		default:
			return Button::handle(event);
		}
	}

	/**
	 Creates a new fltk3::RepeatButton widget using the given
	 position, size, and label string. The default boxtype is fltk3::UP_BOX.
	 Deletes the button.
	 */
	RepeatButton(int X,int Y,int W,int H,const char *l=0): Button(X,Y,W,H,l) {
	}

	void deactivate() {
		remove_timeout(repeat_callback,this);
		Button::deactivate();
	}

};

}

#endif

//
// End of "$Id: RepeatButton.h 9436 2012-05-02 21:24:21Z matt $".
//
