//
// "$Id: LightButton.h 9436 2012-05-02 21:24:21Z matt $"
//
// Lighted button header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::LightButton widget . */

#ifndef Fltk3_Light_Button_H
#define Fltk3_Light_Button_H

#include "Button.h"

#include "run.h"
#include "draw.h"
#include "src/flstring.h"

namespace fltk3
{

/**
 This subclass displays the "on" state by turning on a light,
 rather than drawing pushed in.  The shape of the  "light"
 is initially set to fltk3::DOWN_BOX.  The color of the light when
 on is controlled with selection_color(), which defaults to fltk3::YELLOW.

 Buttons generate callbacks when they are clicked by the user.  You
 control exactly when and how by changing the values for type() and when().
 <P ALIGN=CENTER>\image html fltk3::LightButton.png</P>
 \image latex fltk3::LightButton.png "fltk3::LightButton" width=4cm
 */
class FLTK3_EXPORT LightButton : public Button
{
protected:
	virtual void draw() {
		fltk3::Color col;
		if (this==fltk3::pushed()) {
			col = fltk3::color_average(fltk3::BLACK, color(), 0.2f);
		} else {
			col = color();
		}
		//if (box()!=fltk3::NO_BOX) draw_box(this==fltk3::pushed() ? fltk3::down(box()) : box(), col);
		if (box()!=fltk3::NO_BOX) draw_box(this==fltk3::pushed() ? box()->downbox() : box(), col);
		col = value() ? (active_r() ? selection_color() : fltk3::inactive(selection_color())) : color();
		int W;
		int dx, dy;

		W  = labelsize();
		dx = fltk3::box_dx(box()) + 2;
		dy = (h() - W) / 2;
		// if (dy < 0) dy = 0;         // neg. offset o.k. for vertical centering

		if (down_box()!=fltk3::NO_BOX) {
			// draw other down_box() styles:
			fltk3::Box* db = down_box();
			if ( db==fltk3::DOWN_BOX || db==fltk3::UP_BOX || db==fltk3::PLASTIC_DOWN_BOX || db==fltk3::PLASTIC_UP_BOX) {
				// Check box...
				draw_box(down_box(), dx, dy, W, W, fltk3::BACKGROUND2_COLOR);
				if (value()) {
					if (!fltk3::scheme()) {
						fltk3::color(fltk3::SELECTION_COLOR);
					} else {
						fltk3::color(col);
					}
					int tx = dx + 3;
					int tw = W - 6;
					int d1 = tw/3;
					int d2 = tw-d1;
					int ty = dy + (W+d2)/2-d1-2;
					for (int n = 0; n < 3; n++, ty++) {
						fltk3::line(tx, ty, tx+d1, ty+d1);
						fltk3::line(tx+d1, ty+d1, tx+tw-1, ty+d1-d2+1);
					}
				}
			} else if ( db==fltk3::ROUND_DOWN_BOX || db==fltk3::ROUND_UP_BOX) {
				// Radio button...
				draw_box(down_box(), dx, dy, W, W, fltk3::BACKGROUND2_COLOR);
				if (value()) {
					int tW = (W - fltk3::box_dw(down_box())) / 2 + 1;
					if ((W - tW) & 1) tW++; // Make sure difference is even to center
					int tdx = dx + (W - tW) / 2;
					int tdy = dy + (W - tW) / 2;

					if (!fltk3::scheme()) {
						fltk3::color(fltk3::SELECTION_COLOR);
						tW --;
						fltk3::pie(tdx - 1, tdy - 1, tW + 3, tW + 3, 0.0, 360.0);
						fltk3::color(fltk3::color_average(fltk3::WHITE, fltk3::SELECTION_COLOR, 0.2f));
					} else {
						fltk3::color(col);
					}

					switch (tW) {
						// Larger circles draw fine...
					default :
						fltk3::pie(tdx, tdy, tW, tW, 0.0, 360.0);
						break;

						// Small circles don't draw well on many systems...
					case 6 :
						fltk3::rectf(tdx + 2, tdy, tW - 4, tW);
						fltk3::rectf(tdx + 1, tdy + 1, tW - 2, tW - 2);
						fltk3::rectf(tdx, tdy + 2, tW, tW - 4);
						break;

					case 5 :
					case 4 :
					case 3 :
						fltk3::rectf(tdx + 1, tdy, tW - 2, tW);
						fltk3::rectf(tdx, tdy + 1, tW, tW - 2);
						break;

					case 2 :
					case 1 :
						fltk3::rectf(tdx, tdy, tW, tW);
						break;
					}

					if (!fltk3::scheme()) {
						fltk3::color(fltk3::color_average(fltk3::WHITE, fltk3::SELECTION_COLOR, 0.5));
						fltk3::arc(tdx, tdy, tW + 1, tW + 1, 60.0, 180.0);
					}
				}
			} else {
				draw_box(down_box(), dx, dy, W, W, col);
			}
		} else {
			// if down_box() is zero, draw light button style:
			int hh = h()-2*dy - 2;
			int ww = W/2+1;
			int xx = dx;
			if (w()<ww+2*xx) xx = (w()-ww)/2;
			if (fltk3::scheme() && !strcmp(fltk3::scheme(), "plastic")) {
				col = active_r() ? selection_color() : fltk3::inactive(selection_color());
				fltk3::color(value() ? col : fltk3::color_average(col, fltk3::BLACK, 0.5f));
				fltk3::pie(xx, dy+1, ww, hh, 0, 360);
			} else {
				draw_box(fltk3::THIN_DOWN_BOX, xx, dy+1, ww, hh, col);
			}
			dx = (ww + 2 * dx - W) / 2;
		}
		draw_label(W+2*dx, 0, w()-W-2*dx, h());
		if (fltk3::focus() == this) draw_focus();
	}

public:

	virtual int handle(int event) {
		switch (event) {
		case fltk3::RELEASE:
			if (box()!=fltk3::NO_BOX) redraw();
		default:
			return Button::handle(event);
		}
	}

	LightButton(int X,int Y,int W,int H,const char *l = 0): fltk3::Button(X, Y, W, H, l) {
		type(fltk3::TOGGLE_BUTTON);
		selection_color(fltk3::YELLOW);
		align(fltk3::ALIGN_LEFT|fltk3::ALIGN_INSIDE);
	}
};

}

#endif

//
// End of "$Id: LightButton.h 9436 2012-05-02 21:24:21Z matt $".
//
