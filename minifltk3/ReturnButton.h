//
// "$Id: ReturnButton.h 9433 2012-05-02 20:23:49Z matt $"
//
// Return button header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::ReturnButton widget . */

#ifndef Fltk3_Return_Button_H
#define Fltk3_Return_Button_H

#include "run.h"
#include "draw.h"
#include "Button.h"


namespace fltk3
{
/**
 The fltk3::ReturnButton is a subclass of fltk3::Button that
 generates a callback when it is pressed or when the user presses the
 Enter key.  A carriage-return symbol is drawn next to the button label.
 <P ALIGN=CENTER>\image html fltk3::ReturnButton.png
 \image latex fltk3::ReturnButton.png "fltk3::ReturnButton" width=4cm
 */
class FLTK3_EXPORT ReturnButton : public Button
{

protected:
	int fl_return_arrow(int x, int y, int w, int h) {
		int size = w;
		if (h<size) size = h;
		int d = (size+2)/4;
		if (d<3) d = 3;
		int t = (size+9)/12;
		if (t<1) t = 1;
		int x0 = x+(w-2*d-2*t-1)/2;
		int x1 = x0+d;
		int y0 = y+h/2;
		fltk3::color(fltk3::LIGHT3);
		fltk3::line(x0, y0, x1, y0+d);
		fltk3::yxline(x1, y0+d, y0+t, x1+d+2*t, y0-d);
		fltk3::yxline(x1, y0-t, y0-d);
		fltk3::color(fltk3::gray_ramp(0));
		fltk3::line(x0, y0, x1, y0-d);
		fltk3::color(fltk3::DARK3);
		fltk3::xyline(x1+1, y0-t, x1+d, y0-d, x1+d+2*t);
		return 1;
	}
	void draw() {
		if (type() == fltk3::HIDDEN_BUTTON) return;
		//draw_box(value() ? (down_box()?down_box():fltk3::down(box())) : box(), value() ? selection_color() : color());
		draw_box(value() ? (down_box()?down_box():box()->downbox()) : box(), value() ? selection_color() : color());
		int W = h();
		if (w()/3 < W) W = w()/3;
		fl_return_arrow(w()-W-4, 0, W, h());
		draw_label(0, 0, w()-W+4, h());
		if (fltk3::focus() == this) draw_focus();
	}

public:
	int handle(int event) {
		if (event == fltk3::SHORTCUT &&
		    (fltk3::event_key() == fltk3::EnterKey || fltk3::event_key() == fltk3::KPEnterKey)) {
			simulate_key_action();
			do_callback();
			return 1;
		} else
			return Button::handle(event);
	}
	/**
	 Creates a new fltk3::ReturnButton widget using the given
	 position, size, and label string. The default boxtype is fltk3::UP_BOX.
	 <P> The inherited destructor deletes the button.
	 */
	ReturnButton(int X, int Y, int W, int H,const char *l=0) : Button(X,Y,W,H,l) {}
};

}

#endif

//
// End of "$Id: ReturnButton.h 9433 2012-05-02 20:23:49Z matt $".
//
