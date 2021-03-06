//
// "$Id: Positioner.cxx 9048 2011-09-18 20:11:00Z matt $"
//
// Positioner widget for the Fast Light Tool Kit (FLTK).
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


// The positioner widget from Forms, gives 2D input
// Written by: Mark Overmars

#include "run.h"
#include "Positioner.h"
#include "draw.h"

static double flinear(double val, double smin, double smax, double gmin, double gmax)
{
	if (smin == smax) return gmax;
	else return gmin + (gmax - gmin) * (val - smin) / (smax - smin);
}

void fltk3::Positioner::draw(int X, int Y, int W, int H)
{
	int x1 = X + 4;
	int y1 = Y + 4;
	int w1 = W - 2 * 4;
	int h1 = H - 2 * 4;
	int xx = int(flinear(xvalue(), xmin, xmax, x1, x1+w1-1)+.5);
	int yy = int(flinear(yvalue(), ymin, ymax, y1, y1+h1-1)+.5);
	draw_box(box(), X, Y, W, H, color());
	fltk3::color(selection_color());
	fltk3::xyline(x1, yy, x1+w1);
	fltk3::yxline(xx, y1, y1+h1);
}

void fltk3::Positioner::draw()
{
	draw(x(), y(), w(), h());
	draw_label();
}

/** Returns the current position in x and y.*/
int fltk3::Positioner::value(double X, double Y)
{
	clear_changed();
	if (X == xvalue_ && Y == yvalue_) return 0;
	xvalue_ = X;
	yvalue_ = Y;
	redraw();
	return 1;
}

/** Sets the X axis coordinate.*/
int fltk3::Positioner::xvalue(double X)
{
	return(value(X, yvalue_));
}

/** Sets the Y axis coordinate.*/
int fltk3::Positioner::yvalue(double Y)
{
	return(value(xvalue_, Y));
}

int fltk3::Positioner::handle(int event, int X, int Y, int W, int H)
{
	switch (event) {
	case fltk3::PUSH:
	case fltk3::DRAG:
	case fltk3::RELEASE: {
		double x1 = X + 4;
		double y1 = Y + 4;
		double w1 = W - 2 * 4;
		double h1 = H - 2 * 4;
		double xx = flinear(fltk3::event_x(), x1, x1+w1-1.0, xmin, xmax);
		if (xstep_) xx = int(xx/xstep_+0.5) * xstep_;
		if (xmin < xmax) {
			if (xx < xmin) xx = xmin;
			if (xx > xmax) xx = xmax;
		} else {
			if (xx > xmin) xx = xmin;
			if (xx < xmax) xx = xmax;
		}
		double yy = flinear(fltk3::event_y(), y1, y1+h1-1.0, ymin, ymax);
		if (ystep_) yy = int(yy/ystep_+0.5) * ystep_;
		if (ymin < ymax) {
			if (yy < ymin) yy = ymin;
			if (yy > ymax) yy = ymax;
		} else {
			if (yy > ymin) yy = ymin;
			if (yy < ymax) yy = ymax;
		}
		if (xx != xvalue_ || yy != yvalue_) {
			xvalue_ = xx;
			yvalue_ = yy;
			set_changed();
			redraw();
		}
	}
	if (!(when() & fltk3::WHEN_CHANGED ||
	      (when() & fltk3::WHEN_RELEASE && event == fltk3::RELEASE))) return 1;
	if (changed() || when()&fltk3::WHEN_NOT_CHANGED) {
		if (event == fltk3::RELEASE) clear_changed();
		do_callback();
	}
	return 1;
	default:
		return 0;
	}
}

int fltk3::Positioner::handle(int e)
{
	return handle(e, x(), y(), w(), h());
}

/**
  Creates a new fltk3::Positioner widget using the given position,
  size, and label string. The default boxtype is fltk3::NO_BOX.
*/
fltk3::Positioner::Positioner(int X, int Y, int W, int H, const char* l)
	: fltk3::Widget(X, Y, W, H, l)
{
	box(fltk3::DOWN_BOX);
	selection_color(fltk3::RED);
	align(fltk3::ALIGN_BOTTOM);
	when(fltk3::WHEN_CHANGED);
	xmin = ymin = 0;
	xmax = ymax = 1;
	xvalue_ = yvalue_ = .5;
	xstep_ = ystep_ = 0;
}

/** Sets the X axis bounds.*/
void fltk3::Positioner::xbounds(double a, double b)
{
	if (a != xmin || b != xmax) {
		xmin = a;
		xmax = b;
		redraw();
	}
}

/** Sets the Y axis bounds.*/
void fltk3::Positioner::ybounds(double a, double b)
{
	if (a != ymin || b != ymax) {
		ymin = a;
		ymax = b;
		redraw();
	}
}

//
// End of "$Id: Positioner.cxx 9048 2011-09-18 20:11:00Z matt $".
//
