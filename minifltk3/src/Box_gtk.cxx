//
// "$Id: gtk.cxx 9638 2012-07-24 04:41:38Z matt $"
//
// "GTK" drawing routines for the Fast Light Tool Kit (FLTK).
//
// These box types provide a GTK+ look, based on Red Hat's Bluecurve
// theme...
//
// Copyright 2006-2010 by Michael Sweet.
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

// Box drawing code for an obscure box type.
// These box types are in separate files so they are not linked
// in if not used.

#include "run.h"
#include "draw.h"
#include "Box.h"

static void gtk_color(fltk3::Color c) 
{
	if (fltk3::draw_box_active()) fltk3::color(c);
	else fltk3::color(fltk3::inactive(c));
}

static void gtk_up_frame(int x, int y, int w, int h, fltk3::Color c) 
{
	gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.5));
	fltk3::xyline(x + 2, y + 1, x + w - 3);
	fltk3::yxline(x + 1, y + 2, y + h - 3);

	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.5));
	fltk3::begin_loop();
	fltk3::vertex(x, y + 2);
	fltk3::vertex(x + 2, y);
	fltk3::vertex(x + w - 3, y);
	fltk3::vertex(x + w - 1, y + 2);
	fltk3::vertex(x + w - 1, y + h - 3);
	fltk3::vertex(x + w - 3, y + h - 1);
	fltk3::vertex(x + 2, y + h - 1);
	fltk3::vertex(x, y + h - 3);
	fltk3::end_loop();
}


static void gtk_up_box(int x, int y, int w, int h, fltk3::Color c) 
{
	gtk_up_frame(x, y, w, h, c);

	gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.4f));
	fltk3::xyline(x + 2, y + 2, x + w - 3);
	gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.2f));
	fltk3::xyline(x + 2, y + 3, x + w - 3);
	gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.1f));
	fltk3::xyline(x + 2, y + 4, x + w - 3);
	gtk_color(c);
	fltk3::rectf(x + 2, y + 5, w - 4, h - 7);
	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.025f));
	fltk3::xyline(x + 2, y + h - 4, x + w - 3);
	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.05f));
	fltk3::xyline(x + 2, y + h - 3, x + w - 3);
	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.1f));
	fltk3::xyline(x + 2, y + h - 2, x + w - 3);
	fltk3::yxline(x + w - 2, y + 2, y + h - 3);
}


static void gtk_down_frame(int x, int y, int w, int h, fltk3::Color c) {
	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.5));
	fltk3::begin_loop();
	fltk3::vertex(x, y + 2);
	fltk3::vertex(x + 2, y);
	fltk3::vertex(x + w - 3, y);
	fltk3::vertex(x + w - 1, y + 2);
	fltk3::vertex(x + w - 1, y + h - 3);
	fltk3::vertex(x + w - 3, y + h - 1);
	fltk3::vertex(x + 2, y + h - 1);
	fltk3::vertex(x, y + h - 3);
	fltk3::end_loop();

	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.1f));
	fltk3::xyline(x + 2, y + 1, x + w - 3);
	fltk3::yxline(x + 1, y + 2, y + h - 3);

	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.05f));
	fltk3::yxline(x + 2, y + h - 2, y + 2, x + w - 2);
}


static void gtk_down_box(int x, int y, int w, int h, fltk3::Color c) {
	gtk_down_frame(x, y, w, h, c);

	gtk_color(c);
	fltk3::rectf(x + 3, y + 3, w - 5, h - 4);
	fltk3::yxline(x + w - 2, y + 3, y + h - 3);
}


static void gtk_thin_up_frame(int x, int y, int w, int h, fltk3::Color c) {
	gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.6f));
	fltk3::xyline(x + 1, y, x + w - 2);
	fltk3::yxline(x, y + 1, y + h - 2);

	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.4f));
	fltk3::xyline(x + 1, y + h - 1, x + w - 2);
	fltk3::yxline(x + w - 1, y + 1, y + h - 2);
}


static void gtk_thin_up_box(int x, int y, int w, int h, fltk3::Color c) {
	gtk_thin_up_frame(x, y, w, h, c);

	gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.4f));
	fltk3::xyline(x + 1, y + 1, x + w - 2);
	gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.2f));
	fltk3::xyline(x + 1, y + 2, x + w - 2);
	gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.1f));
	fltk3::xyline(x + 1, y + 3, x + w - 2);
	gtk_color(c);
	fltk3::rectf(x + 1, y + 4, w - 2, h - 8);
	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.025f));
	fltk3::xyline(x + 1, y + h - 4, x + w - 2);
	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.05f));
	fltk3::xyline(x + 1, y + h - 3, x + w - 2);
	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.1f));
	fltk3::xyline(x + 1, y + h - 2, x + w - 2);
}


static void gtk_thin_down_frame(int x, int y, int w, int h, fltk3::Color c) {
	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.4f));
	fltk3::xyline(x + 1, y, x + w - 2);
	fltk3::yxline(x, y + 1, y + h - 2);

	gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.6f));
	fltk3::xyline(x + 1, y + h - 1, x + w - 2);
	fltk3::yxline(x + w - 1, y + 1, y + h - 2);
}


static void gtk_thin_down_box(int x, int y, int w, int h, fltk3::Color c) {
	gtk_thin_down_frame(x, y, w, h, c);

	gtk_color(c);
	fltk3::rectf(x + 1, y + 1, w - 2, h - 2);
}

//------------------------
// new GTK+ style for round buttons
#if 1

static void fl_arc_i(int x,int y,int w,int h,double a1,double a2) 
{
	fltk3::arc(x,y,w,h,a1,a2);
}

enum {UPPER_LEFT, LOWER_RIGHT, CLOSED, FILL};

static void draw(int which, int x,int y,int w,int h, int inset)
{
	if (inset*2 >= w) inset = (w-1)/2;
	if (inset*2 >= h) inset = (h-1)/2;
	x += inset;
	y += inset;
	w -= 2*inset;
	h -= 2*inset;
	int d = w <= h ? w : h;
	if (d <= 1) return;
	void (*f)(int,int,int,int,double,double);
	f = (which==FILL) ? fltk3::pie : fl_arc_i;
	if (which >= CLOSED) {
		f(x+w-d, y, d, d, w<=h ? 0 : -90, w<=h ? 180 : 90);
		f(x, y+h-d, d, d, w<=h ? 180 : 90, w<=h ? 360 : 270);
	} else if (which == UPPER_LEFT) {
		f(x+w-d, y, d, d, 45, w<=h ? 180 : 90);
		f(x, y+h-d, d, d, w<=h ? 180 : 90, 225);
	} else { // LOWER_RIGHT
		f(x, y+h-d, d, d, 225, w<=h ? 360 : 270);
		f(x+w-d, y, d, d, w<=h ? 360 : 270, 360+45);
	}
	if (which == FILL) {
		if (w < h)
			fltk3::rectf(x, y+d/2, w, h-(d&-2));
		else if (w > h)
			fltk3::rectf(x+d/2, y, w-(d&-2), h);
	} else {
		if (w < h) {
			if (which != UPPER_LEFT) fltk3::yxline(x+w-1, y+d/2-1, y+h-d/2+1);
			if (which != LOWER_RIGHT) fltk3::yxline(x, y+d/2-1, y+h-d/2+1);
		} else if (w > h) {
			if (which != UPPER_LEFT) fltk3::xyline(x+d/2-1, y+h-1, x+w-d/2+1);
			if (which != LOWER_RIGHT) fltk3::xyline(x+d/2-1, y, x+w-d/2+1);
		}
	}
}

void gtk_round_up_box(int x, int y, int w, int h, fltk3::Color c) {
	fltk3::color(c);
	draw(FILL,	    x,   y, w,   h, 2);

	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.025f));
	draw(LOWER_RIGHT, x+1, y, w-2, h, 2);
	draw(LOWER_RIGHT, x,   y, w,   h, 3);
	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.05f));
	draw(LOWER_RIGHT, x+1, y, w-2, h, 1);
	draw(LOWER_RIGHT, x,   y, w,   h, 2);
	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.1f));
	draw(LOWER_RIGHT, x+1, y, w-2, h, 0);
	draw(LOWER_RIGHT, x,   y, w,   h, 1);

	gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.1f));
	draw(UPPER_LEFT,  x,   y, w,   h, 4);
	draw(UPPER_LEFT,  x+1, y, w-2, h, 3);
	gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.2f));
	draw(UPPER_LEFT,  x,   y, w,   h, 3);
	draw(UPPER_LEFT,  x+1, y, w-2, h, 2);
	gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.4f));
	draw(UPPER_LEFT,  x,   y, w,   h, 2);
	draw(UPPER_LEFT,  x+1, y, w-2, h, 1);
	gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.5f));
	draw(UPPER_LEFT,  x,   y, w,   h, 1);
	draw(UPPER_LEFT,  x+1, y, w-2, h, 0);

	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.5f));
	draw(CLOSED,	    x,   y, w,   h, 0);
}

void gtk_round_down_box(int x, int y, int w, int h, fltk3::Color c) {
	fltk3::color(c);
	draw(FILL,	    x,   y, w,   h, 2);

	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.05f));
	draw(UPPER_LEFT,  x,   y, w,   h, 2);
	draw(UPPER_LEFT,  x+1, y, w-2, h, 1);
	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.1f));
	draw(UPPER_LEFT,  x,   y, w,   h, 1);
	draw(UPPER_LEFT,  x+1, y, w-2, h, 0);

	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.5f));
	draw(CLOSED,	    x,   y, w,   h, 0);
}

#else

static void gtk_round_up_box(int x, int y, int w, int h, fltk3::Color c) {
	gtk_color(c);
	fltk3::pie(x, y, w, h, 0.0, 360.0);
	gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.5f));
	fltk3::arc(x, y, w, h, 45.0, 180.0);
	gtk_color(fltk3::color_average(fltk3::WHITE, c, 0.25f));
	fltk3::arc(x, y, w, h, 180.0, 405.0);
	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.5f));
	fltk3::arc(x, y, w, h, 225.0, 360.0);
}


static void gtk_round_down_box(int x, int y, int w, int h, fltk3::Color c) {
	gtk_color(c);
	fltk3::pie(x, y, w, h, 0.0, 360.0);
	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.2));
	fltk3::arc(x + 1, y, w, h, 90.0, 210.0);
	gtk_color(fltk3::color_average(fltk3::BLACK, c, 0.6));
	fltk3::arc(x, y, w, h, 0.0, 360.0);
}

#endif

//------------------------------------------------------------------------------
void fltk3::GtkUpBox::_draw(const Rectangle &r) const
{
	gtk_up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color());
}
void fltk3::GtkDownBox::_draw(const Rectangle &r) const
{
	gtk_down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color());
}
void fltk3::GtkThinUpBox::_draw(const Rectangle &r) const
{
	gtk_thin_up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color());
}
void fltk3::GtkThinDownBox::_draw(const Rectangle &r) const
{
	gtk_thin_down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color());
}
void fltk3::GtkRoundUpBox::_draw(const Rectangle &r) const
{
	gtk_round_up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color());
}
void fltk3::GtkRoundDownBox::_draw(const Rectangle &r) const
{
	gtk_round_down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color());
}
void fltk3::GtkUpFrame::_draw(const Rectangle &r) const
{
	gtk_up_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color());
}
void fltk3::GtkDownFrame::_draw(const Rectangle &r) const
{
	gtk_down_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color());
}
void fltk3::GtkThinUpFrame::_draw(const Rectangle &r) const
{
	gtk_thin_up_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color());
}
void fltk3::GtkThinDownFrame::_draw(const Rectangle &r) const
{
	gtk_thin_down_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color());
}
//------------------------------------------------------------------------------
static fltk3::GtkUpBox gtkUpBox("gtkUpBox");
static fltk3::GtkDownBox gtkDownBox("gtkDownBox");
static fltk3::GtkThinUpBox gtkThinUpBox("gtkThinUpBox");
static fltk3::GtkThinDownBox gtkThinDownBox("gtkThinDownBox");
static fltk3::GtkRoundUpBox gtkRoundUpBox("gtkRoundUpBox");
static fltk3::GtkRoundDownBox gtkRoundDownBox("gtkRoundDownBox");
static fltk3::GtkUpFrame gtkUpFrame("gtkUpFrame");
static fltk3::GtkDownFrame gtkDownFrame("gtkDownFrame");
static fltk3::GtkThinUpFrame gtkThinUpFrame("gtkThinUpFrame");
static fltk3::GtkThinDownFrame gtkThinDownFrame("gtkThinDownFrame");
//------------------------------------------------------------------------------
fltk3::Box* fltk3::GTK_UP_BOX          = &gtkUpBox;
fltk3::Box* fltk3::GTK_DOWN_BOX        = &gtkDownBox;
fltk3::Box* fltk3::GTK_THIN_UP_BOX     = &gtkThinUpBox;
fltk3::Box* fltk3::GTK_THIN_DOWN_BOX   = &gtkThinDownBox;
fltk3::Box* fltk3::GTK_ROUND_UP_BOX    = &gtkRoundUpBox;
fltk3::Box* fltk3::GTK_ROUND_DOWN_BOX  = &gtkRoundDownBox;
fltk3::Box* fltk3::GTK_UP_FRAME        = &gtkUpFrame;
fltk3::Box* fltk3::GTK_DOWN_FRAME      = &gtkDownFrame;
fltk3::Box* fltk3::GTK_THIN_UP_FRAME   = &gtkThinUpFrame;
fltk3::Box* fltk3::GTK_THIN_DOWN_FRAME = &gtkThinDownFrame;

//
// End of "$Id: gtk.cxx 9638 2012-07-24 04:41:38Z matt $".
//
