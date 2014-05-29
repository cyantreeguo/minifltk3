//
// "$Id: Progress.h 9300 2012-03-23 20:36:00Z ianmacarthur $"
//
// Progress bar widget definitions.
//
// Copyright 2000-2010 by Michael Sweet.
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
 fltk3::Progress widget . */

#ifndef _Fltk3_Progress_H_
#define _Fltk3_Progress_H_

#include "run.h"
#include "Widget.h"
#include "draw.h"

namespace fltk3
{

//
// Progress class...
//
/**
 Displays a progress bar for the user.
 */
class FLTK3_EXPORT Progress : public fltk3::Widget
{
	float	value_,
	        minimum_,
	        maximum_;

protected:
	virtual void draw() {
		int	progress;	// Size of progress bar...
		int	bx, by, bw, bh;	// Box areas...
		int	tx, tw;		// Temporary X + width


		// Get the box borders...
		bx = fltk3::box_dx(box());
		by = fltk3::box_dy(box());
		bw = fltk3::box_dw(box());
		bh = fltk3::box_dh(box());

		tx = x() + bx;
		tw = w() - bw;

		// Draw the progress bar...
		if (maximum_ > minimum_)
			progress = (int)(w() * (value_ - minimum_) / (maximum_ - minimum_) + 0.5f);
		else
			progress = 0;

		// Draw the box and label...
		if (progress > 0) {
			fltk3::Color c = labelcolor();
			labelcolor(fltk3::contrast(labelcolor(), selection_color()));

			fltk3::push_clip(x(), y(), progress + bx, h());
			draw_box(box(), x(), y(), w(), h(), active_r() ? selection_color() : fltk3::inactive(selection_color()));
			draw_label(tx, y() + by, tw, h() - bh);
			fltk3::pop_clip();

			labelcolor(c);

			if (progress<w()) {
				fltk3::push_clip(tx + progress, y(), w() - progress, h());
				draw_box(box(), x(), y(), w(), h(), active_r() ? color() : fltk3::inactive(color()));
				draw_label(tx, y() + by, tw, h() - bh);
				fltk3::pop_clip();
			}
		} else {
			draw_box(box(), x(), y(), w(), h(), active_r() ? color() : fltk3::inactive(color()));
			draw_label(tx, y() + by, tw, h() - bh);
		}
	}

public:
	Progress(int X, int Y, int W, int H, const char* L = 0) : fltk3::Widget(X, Y, W, H, L) {
		align(fltk3::ALIGN_INSIDE);
		box(fltk3::DOWN_BOX);
		color(fltk3::BACKGROUND2_COLOR, fltk3::YELLOW);
		minimum(0.0f);
		maximum(100.0f);
		value(0.0f);
	}

	/** Sets the maximum value in the progress widget.  */
	void maximum(float v) 
	{
		maximum_ = v;
		redraw();
	}
	/** Gets the maximum value in the progress widget.  */
	float maximum() const { return (maximum_); }

	/** Sets the minimum value in the progress widget.  */
	void minimum(float v) 
	{
		minimum_ = v;
		redraw();
	}
	/** Gets the minimum value in the progress widget.  */
	float minimum() const { return (minimum_); }

	/** Sets the current value in the progress widget.  */
	void value(float v) 
	{
		value_ = v;
		redraw();
	}
	/** Gets the current value in the progress widget.  */
	float value() const { return (value_); }
};

}

#endif // !_Fl_Progress_H_

//
// End of "$Id: Progress.h 9300 2012-03-23 20:36:00Z ianmacarthur $".
//
