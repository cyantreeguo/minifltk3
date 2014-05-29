//
// "$Id: Box.cxx 9638 2012-07-24 04:41:38Z matt $"
//
// Box widget for the Fast Light Tool Kit (FLTK).
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

#include "run.h"
#include "draw.h"
#include "Box.h"

static uchar active_ramp[24] = {
	fltk3::GRAY_RAMP+0, fltk3::GRAY_RAMP+1, fltk3::GRAY_RAMP+2, fltk3::GRAY_RAMP+3,
	fltk3::GRAY_RAMP+4, fltk3::GRAY_RAMP+5, fltk3::GRAY_RAMP+6, fltk3::GRAY_RAMP+7,
	fltk3::GRAY_RAMP+8, fltk3::GRAY_RAMP+9, fltk3::GRAY_RAMP+10,fltk3::GRAY_RAMP+11,
	fltk3::GRAY_RAMP+12,fltk3::GRAY_RAMP+13,fltk3::GRAY_RAMP+14,fltk3::GRAY_RAMP+15,
	fltk3::GRAY_RAMP+16,fltk3::GRAY_RAMP+17,fltk3::GRAY_RAMP+18,fltk3::GRAY_RAMP+19,
	fltk3::GRAY_RAMP+20,fltk3::GRAY_RAMP+21,fltk3::GRAY_RAMP+22,fltk3::GRAY_RAMP+23
};
static uchar inactive_ramp[24] = {
	43, 43, 44, 44, // ABCD
	44, 45, 45, 46, // EFGH
	46, 46, 47, 47, // IJKL
	48, 48, 48, 49, // MNOP
	49, 49, 50, 50, // QRST
	51, 51, 52, 52  // UVWX
};
static int draw_it_active = 1;

/**
  Determines if the current draw box is active or inactive.
  If inactive, the box color is changed by the inactive color.
*/
int fltk3::draw_box_active()
{
	return draw_it_active;
}

namespace fltk3
{
uchar *gray_ramp()
{
	return (draw_it_active?active_ramp:inactive_ramp)-'A';
}
}

/**
  Draws a series of line segments around the given box.
  The string \p s must contain groups of 4 letters which specify one of 24
  standard grayscale values, where 'A' is black and 'X' is white.
  The order of each set of 4 characters is: top, left, bottom, right.
  The result of calling fltk3::frame() with a string that is not a multiple
  of 4 characters in length is undefined.
  The only difference between this function and fltk3::frame2() is the order
  of the line segments.
  \param[in] s sets of 4 grayscale values in top, left, bottom, right order
  \param[in] x, y, w, h position and size
*/
void fltk3::frame(const char* s, int x, int y, int w, int h)
{
	uchar *g = fltk3::gray_ramp();
	if (h > 0 && w > 0) {
		for (; *s;) {
			// draw top line:
			fltk3::color(g[(int)*s++]);
			fltk3::xyline(x, y, x+w-1);
			y++;
			if (--h <= 0) break;
			// draw left line:
			fltk3::color(g[(int)*s++]);
			fltk3::yxline(x, y+h-1, y);
			x++;
			if (--w <= 0) break;
			// draw bottom line:
			fltk3::color(g[(int)*s++]);
			fltk3::xyline(x, y+h-1, x+w-1);
			if (--h <= 0) break;
			// draw right line:
			fltk3::color(g[(int)*s++]);
			fltk3::yxline(x+w-1, y+h-1, y);
			if (--w <= 0) break;
		}
	}
}

/**
  Draws a series of line segments around the given box.
  The string \p s must contain groups of 4 letters which specify one of 24
  standard grayscale values, where 'A' is black and 'X' is white.
  The order of each set of 4 characters is: bottom, right, top, left.
  The result of calling fltk3::frame2() with a string that is not a multiple
  of 4 characters in length is undefined.
  The only difference between this function and fltk3::frame() is the order
  of the line segments.
  \param[in] s sets of 4 grayscale values in bottom, right, top, left order
  \param[in] x, y, w, h position and size
*/
void fltk3::frame2(const char* s, int x, int y, int w, int h)
{
	uchar *g = fltk3::gray_ramp();
	if (h > 0 && w > 0) {
		for (; *s;) {
			// draw bottom line:
			fltk3::color(g[(int)*s++]);
			fltk3::xyline(x, y+h-1, x+w-1);
			if (--h <= 0) break;
			// draw right line:
			fltk3::color(g[(int)*s++]);
			fltk3::yxline(x+w-1, y+h-1, y);
			if (--w <= 0) break;
			// draw top line:
			fltk3::color(g[(int)*s++]);
			fltk3::xyline(x, y, x+w-1);
			y++;
			if (--h <= 0) break;
			// draw left line:
			fltk3::color(g[(int)*s++]);
			fltk3::yxline(x, y+h-1, y);
			x++;
			if (--w <= 0) break;
		}
	}
}

#define D1 BORDER_WIDTH
#define D2 (BORDER_WIDTH+BORDER_WIDTH)

/** Draws a frame of type fltk3::ENGRAVED_FRAME */
void fl_engraved_frame(int x, int y, int w, int h, fltk3::Color, fltk3::Box*)
{
	fltk3::frame("HHWWWWHH",x,y,w,h);
}

/** Draws a box of type fltk3::ENGRAVED_BOX */
void fl_engraved_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t)
{
	fl_engraved_frame(x,y,w,h,c,t);
	fltk3::color(draw_it_active ? c : fltk3::inactive(c));
	fltk3::rectf(x+2, y+2, w-4, h-4);
}

/** Draws a frame of type fltk3::EMBOSSED_FRAME */
void fl_embossed_frame(int x, int y, int w, int h, fltk3::Color, fltk3::Box*)
{
	fltk3::frame("WWHHHHWW",x,y,w,h);
}

/** Draws a box of type fltk3::EMBOSSED_BOX */
void fl_embossed_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t)
{
	fl_embossed_frame(x,y,w,h,c,t);
	fltk3::color(draw_it_active ? c : fltk3::inactive(c));
	fltk3::rectf(x+2, y+2, w-4, h-4);
}

/**
  Draws a bounded rectangle with a given position, size and color.
  Equivalent to drawing a box of type fltk3::BORDER_BOX.
*/
void fl_rectbound(int x, int y, int w, int h, fltk3::Color bgcolor, fltk3::Box*)
{
	fltk3::color(draw_it_active ? fltk3::BLACK : fltk3::inactive(fltk3::BLACK));
	fltk3::rect(x, y, w, h);
	fltk3::color(draw_it_active ? bgcolor : fltk3::inactive(bgcolor));
	fltk3::rectf(x+1, y+1, w-2, h-2);
}
#define fl_border_box fl_rectbound	/**< allow consistent naming */

/**
  Draws a frame of type fltk3::BORDER_FRAME.
*/
void fl_border_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*)
{
	fltk3::color(draw_it_active ? c : fltk3::inactive(c));
	fltk3::rect(x, y, w, h);
}

////////////////////////////////////////////////////////////////

void fl_diamond_up_box(int x,int y,int w,int h,fltk3::Color bgcolor, fltk3::Box*)
{
	w &= -2;
	h &= -2;
	int x1 = x+w/2;
	int y1 = y+h/2;
	fltk3::color(bgcolor);
	fltk3::polygon(x+3, y1, x1,y+3, x+w-3,y1, x1,y+h-3);
	uchar *g = fltk3::gray_ramp();
	fltk3::color(g[(uchar)'W']);
	fltk3::line(x+1, y1, x1, y+1, x+w-1, y1);
	fltk3::color(g[(uchar)'U']);
	fltk3::line(x+2, y1, x1, y+2, x+w-2, y1);
	fltk3::color(g[(uchar)'S']);
	fltk3::line(x+3, y1, x1, y+3, x+w-3, y1);
	fltk3::color(g[(uchar)'P']);
	fltk3::line(x+3, y1, x1, y+h-3, x+w-3, y1);
	fltk3::color(g[(uchar)'N']);
	fltk3::line(x+2, y1, x1, y+h-2, x+w-2, y1);
	fltk3::color(g[(uchar)'H']);
	fltk3::line(x+1, y1, x1, y+h-1, x+w-1, y1);
	fltk3::color(g[(uchar)'A']);
	fltk3::loop(x, y1, x1, y, x+w, y1, x1, y+h);
}

void fl_diamond_down_box(int x,int y,int w,int h,fltk3::Color bgcolor, fltk3::Box*)
{
	w &= -2;
	h &= -2;
	int x1 = x+w/2;
	int y1 = y+h/2;
	uchar *g = fltk3::gray_ramp();
	fltk3::color(g[(uchar)'P']);
	fltk3::line(x+0, y1, x1, y+0, x+w-0, y1);
	fltk3::color(g[(uchar)'N']);
	fltk3::line(x+1, y1, x1, y+1, x+w-1, y1);
	fltk3::color(g[(uchar)'H']);
	fltk3::line(x+2, y1, x1, y+2, x+w-2, y1);
	fltk3::color(g[(uchar)'W']);
	fltk3::line(x+2, y1, x1, y+h-2, x+w-2, y1);
	fltk3::color(g[(uchar)'U']);
	fltk3::line(x+1, y1, x1, y+h-1, x+w-1, y1);
	fltk3::color(g[(uchar)'S']);
	fltk3::line(x+0, y1, x1, y+h-0, x+w-0, y1);
	fltk3::color(bgcolor);
	fltk3::polygon(x+3, y1, x1,y+3, x+w-3,y1, x1,y+h-3);
	fltk3::color(g[(uchar)'A']);
	fltk3::loop(x+3, y1, x1, y+3, x+w-3, y1, x1, y+h-3);
}

// --------------------------------------------
#define BW 3

void fl_shadow_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*)
{
	fltk3::color(fltk3::DARK3);
	fltk3::rectf(x+BW, y+h-BW,  w - BW, BW);
	fltk3::rectf(x+w-BW,  y+BW, BW,  h - BW);
	fltk3::color(c);
	fltk3::rect(x,y,w-BW,h-BW);
}

void fl_shadow_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t)
{
	fltk3::color(c);
	fltk3::rectf(x+1,y+1,w-2-BW,h-2-BW);
	fl_shadow_frame(x,y,w,h,fltk3::GRAY0,t);
}
// --------------------------------------------

#define RN	5
#define RS	15
#define BW	3

static double offset[RN] = { 0.0, 0.07612, 0.29289, 0.61732, 1.0};

static void rbox(int fill, int x, int y, int w, int h)
{
	int i;
	int rsx ,rsy, rs;
	rsx = w*2/5;
	rsy = h*2/5;
	if (rsx > rsy) rs = rsy;
	else  rs = rsx;
	if (rs > RS) rs = RS;
	rsx = rs;
	rsy = rs;

	if (fill) fltk3::begin_polygon();
	else fltk3::begin_loop();
	for (i=0; i<RN; i++)
		fltk3::vertex(x + offset[RN-i-1]*rsx, y + offset[i] * rsy);
	for (i=0; i<RN; i++)
		fltk3::vertex(x + offset[i]*rsx, y + h-1 - offset[RN-i-1] * rsy);
	for (i=0; i<RN; i++)
		fltk3::vertex(x + w-1 - offset[RN-i-1]*rsx, y + h-1 - offset[i] * rsy);
	for (i=0; i<RN; i++)
		fltk3::vertex(x + w-1 - offset[i]*rsx, y + offset[RN-i-1] * rsy);
	if (fill) fltk3::end_polygon();
	else fltk3::end_loop();
}

void fl_rflat_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*)
{
	fltk3::color(c);
	rbox(1, x, y, w, h);
	rbox(0, x, y, w, h);
}

void fl_rounded_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*)
{
	fltk3::color(c);
	rbox(0, x, y, w, h);
}

void fl_rounded_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*)
{
	fltk3::color(c);
	rbox(1, x, y, w, h);
	fltk3::color(fltk3::BLACK);
	rbox(0, x, y, w, h);
}

void fl_rshadow_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t)
{
	// draw shadow:
	fltk3::color(fltk3::DARK3);
	rbox(1, x+BW, y+BW, w, h);
	rbox(0, x+BW, y+BW, w, h);
	// draw the box:
	fl_rounded_box(x, y, w, h, c, t);
}
/*
void fl_rflat_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_rounded_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_rounded_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
void fl_rshadow_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
*/

// --------------------------------------------
void fl_oval_flat_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*)
{
	fltk3::color(c);
	fltk3::pie(x, y, w, h, 0, 360);
}

void fl_oval_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*)
{
	fltk3::color(c);
	fltk3::arc(x, y, w, h, 0, 360);
}

void fl_oval_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t)
{
	fl_oval_flat_box(x,y,w,h,c,t);
	fl_oval_frame(x,y,w,h,fltk3::BLACK,t);
}

void fl_oval_shadow_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t)
{
	fl_oval_flat_box(x+3,y+3,w,h,fltk3::DARK3,t);
	fl_oval_box(x,y,w,h,c,t);
}
// --------------------------------------------
//void fl_gtk_up_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
/** Draws a frame of type FL_UP_FRAME */
void fl_up_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t) 
{
#if BORDER_WIDTH == 1
	fltk3::frame2("HHWW",x,y,w,h);
#else
#if BORDER_WIDTH == 2
	fltk3::frame2("AAWWMMTT",x,y,w,h);
#else
	fltk3::frame("AAAAWWJJUTNN",x,y,w,h);
#endif
#endif
}

//void fl_gtk_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
#define D1 BORDER_WIDTH
#define D2 (BORDER_WIDTH+BORDER_WIDTH)
/** Draws a box of type FL_UP_BOX */
void fl_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t) 
{
	fl_up_frame(x,y,w,h,c,t);
	fltk3::color(draw_it_active ? c : fltk3::inactive(c));
	fltk3::rectf(x+D1, y+D1, w-D2, h-D2);
}

//void fl_gtk_down_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
/** Draws a frame of type FL_DOWN_FRAME */
void fl_down_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t) 
{
#if BORDER_WIDTH == 1
	fltk3::frame2("WWHH",x,y,w,h);
#else
#if BORDER_WIDTH == 2
	fltk3::frame2("WWMMPPAA",x,y,w,h);
#else
	fltk3::frame("NNTUJJWWAAAA",x,y,w,h);
#endif
#endif
}

//void fl_gtk_down_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
/** Draws a box of type FL_DOWN_BOX */
void fl_down_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t)
{
	fl_down_frame(x,y,w,h,c,t);
	fltk3::color(c); 
	fltk3::rectf(x+D1, y+D1, w-D2, h-D2);
}

//void fl_gtk_thin_up_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
/** Draws a frame of type FL_THIN_UP_FRAME */
void fl_thin_up_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t)
{
	fltk3::frame2("HHWW",x,y,w,h);
}

//void fl_gtk_thin_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
/** Draws a box of type FL_THIN_UP_BOX */
void fl_thin_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t)
{
	fl_thin_up_frame(x,y,w,h,c,t);
	fltk3::color(draw_it_active ? c : fltk3::inactive(c));
	fltk3::rectf(x+1, y+1, w-2, h-2);
}

//void fl_gtk_thin_down_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
/** Draws a frame of type FL_THIN_DOWN_FRAME */
void fl_thin_down_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t) {
	fltk3::frame2("WWHH",x,y,w,h);
}

//void fl_gtk_thin_down_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
/** Draws a box of type FL_THIN_DOWN_BOX */
void fl_thin_down_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t) 
{
	fl_thin_down_frame(x,y,w,h,c,t);
	fltk3::color(draw_it_active ? c : fltk3::inactive(c));
	fltk3::rectf(x+1, y+1, w-2, h-2);
}

//------------------------------------------------------------------------------
//void fl_gtk_round_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
//void fl_gtk_round_down_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*);
// A compiler from a certain very large software company will not compile
// the function pointer assignment due to the name conflict with fltk3::arc.
// This function is to fix that:
static void arc_i(int x,int y,int w,int h,double a1,double a2) {
	fltk3::arc(x,y,w,h,a1,a2);
}

enum {UPPER_LEFT, LOWER_RIGHT, CLOSED, FILL};

static void draw(int which, int x,int y,int w,int h, int inset, fltk3::Color color)
{
	if (inset*2 >= w) inset = (w-1)/2;
	if (inset*2 >= h) inset = (h-1)/2;
	x += inset;
	y += inset;
	w -= 2*inset;
	h -= 2*inset;
	int d = w <= h ? w : h;
	if (d <= 1) return;
	fltk3::color(color);
	void (*f)(int,int,int,int,double,double);
	f = (which==FILL) ? fltk3::pie : arc_i;
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

void fl_round_down_box(int x, int y, int w, int h, fltk3::Color bgcolor, fltk3::Box*) {
	uchar *g = fltk3::gray_ramp();
	draw(FILL,	    x,   y, w,   h, 2, bgcolor);
	draw(UPPER_LEFT,  x+1, y, w-2, h, 0, (fltk3::Color)g[(uchar)'N']);
	draw(UPPER_LEFT,  x+1, y, w-2, h, 1, (fltk3::Color)g[(uchar)'H']);
	draw(UPPER_LEFT,  x,   y, w,   h, 0, (fltk3::Color)g[(uchar)'N']);
	draw(UPPER_LEFT,  x,   y, w,   h, 1, (fltk3::Color)g[(uchar)'H']);
	draw(LOWER_RIGHT, x,   y, w,   h, 0, (fltk3::Color)g[(uchar)'S']);
	draw(LOWER_RIGHT, x+1, y, w-2, h, 0, (fltk3::Color)g[(uchar)'U']);
	draw(LOWER_RIGHT, x,   y, w,   h, 1, (fltk3::Color)g[(uchar)'U']);
	draw(LOWER_RIGHT, x+1, y, w-2, h, 1, (fltk3::Color)g[(uchar)'W']);
	draw(CLOSED,	    x,   y, w,   h, 2, (fltk3::Color)g[(uchar)'A']);
}

void fl_round_up_box(int x, int y, int w, int h, fltk3::Color bgcolor, fltk3::Box*) {
	uchar *g = fltk3::gray_ramp();
	draw(FILL,	    x,   y, w,   h, 2, bgcolor);
	draw(LOWER_RIGHT, x+1, y, w-2, h, 0, (fltk3::Color)g[(uchar)'H']);
	draw(LOWER_RIGHT, x+1, y, w-2, h, 1, (fltk3::Color)g[(uchar)'N']);
	draw(LOWER_RIGHT, x,   y, w,   h, 1, (fltk3::Color)g[(uchar)'H']);
	draw(LOWER_RIGHT, x,   y, w,   h, 2, (fltk3::Color)g[(uchar)'N']);
	draw(UPPER_LEFT,  x,   y, w,   h, 2, (fltk3::Color)g[(uchar)'U']);
	draw(UPPER_LEFT,  x+1, y, w-2, h, 1, (fltk3::Color)g[(uchar)'S']);
	draw(UPPER_LEFT,  x,   y, w,   h, 1, (fltk3::Color)g[(uchar)'W']);
	draw(UPPER_LEFT,  x+1, y, w-2, h, 0, (fltk3::Color)g[(uchar)'U']);
	draw(CLOSED,	    x,   y, w,   h, 0, (fltk3::Color)g[(uchar)'A']);
}
//------------------------------------------------------------------------------

/**
  Returns the X offset for the given boxtype.
  \see box_dy()
*/
int fltk3::box_dx(fltk3::Box* t)
{
	return t?t->dx():0;
}

/**
    Returns the Y offset for the given boxtype.

    These functions return the offset values necessary for a given
    boxtype, useful for computing the area inside a box's borders, to
    prevent overdrawing the borders.

    For instance, in the case of a boxtype like fltk3::DOWN_BOX
    where the border width might be 2 pixels all around, the above
    functions would return 2, 2, 4, and 4 for box_dx,
    box_dy, box_dw, and box_dh respectively.

    An example to compute the area inside a widget's box():
    \code
         int X = yourwidget->x() + fltk3::box_dx(yourwidget->box());
         int Y = yourwidget->y() + fltk3::box_dy(yourwidget->box());
         int W = yourwidget->w() - fltk3::box_dw(yourwidget->box());
         int H = yourwidget->h() - fltk3::box_dh(yourwidget->box());
    \endcode
    These functions are mainly useful in the draw() code
    for deriving custom widgets, where one wants to avoid drawing
    over the widget's own border box().
*/
int fltk3::box_dy(fltk3::Box* t)
{
	return t?t->dy():0;
}


/**
  Returns the width offset for the given boxtype.
  \see box_dy().
*/
int fltk3::box_dw(fltk3::Box* t)
{
	return t?t->dw():0;
}


/**
  Returns the height offset for the given boxtype.
  \see box_dy().
*/
int fltk3::box_dh(fltk3::Box* t)
{
	return t?t->dh():0;
}

/** Copies the from boxtype. */
//void fltk3::set_boxtype(fltk3::BoxPtr& to, fltk3::Box* from)
//void fltk3::set_boxtype(fltk3::Box *to, fltk3::Box* from)
//{
//	to = from;
//}


/**
  Draws a box using given type, position, size and color.
  \param[in] t box type
  \param[in] x, y, w, h position and size
  \param[in] c color
*/
void fltk3::draw_box(fltk3::Box* t, int x, int y, int w, int h, fltk3::Color c, fltk3::Box::Flags flags)
{
	if (t) {
		fltk3::color(c);
		t->draw(fltk3::Rectangle(x, y, w, h), flags);
	}
}

/** If fltk3::ALIGN_IMAGE_BACKDROP is set, the image or deimage will be drawn */
void fltk3::Widget::draw_backdrop() const
{
	if (align() & fltk3::ALIGN_IMAGE_BACKDROP) {
		const fltk3::Image *img = image();
		// if there is no image, we will not draw the deimage either
		if (img && deimage() && !active_r())
			img = deimage();
		if (img)
			((fltk3::Image*)img)->draw((w_-img->w())/2, (h_-img->h())/2);
	}
}

/** Draws a box of type t, of color c at the position X,Y and size W,H. */
void fltk3::Widget::draw_box(fltk3::Box* t, int X, int Y, int W, int H, fltk3::Color c) const
{
	if (t) {
		draw_it_active = active_r();
		fltk3::color(c);
		t->draw(fltk3::Rectangle(X, Y, W, H), (fltk3::Box::Flags)0);
		draw_it_active = 1;
	}
}

//------------------------------------------------------------------------------
void fltk3::UpBox::_draw(const Rectangle &r) const
{
	fl_up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), (fltk3::Box*)0);
}
void fltk3::DownBox::_draw(const Rectangle &r) const
{
	fl_down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), (fltk3::Box*)0);
}
void fltk3::ThinUpBox::_draw(const Rectangle &r) const
{
	fl_thin_up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::ThinDownBox::_draw(const Rectangle &r) const
{
	fl_thin_down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::BorderFrame::_draw(const Rectangle &r) const
{
	fl_border_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::EngravedBox::_draw(const Rectangle &r) const
{
	fl_engraved_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::EmbossedBox::_draw(const Rectangle &r) const
{
	fl_embossed_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::BorderBox::_draw(const Rectangle &r) const
{
	fl_border_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::ShadowBox::_draw(const Rectangle &r) const
{
	fl_shadow_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::RoundedBox::_draw(const Rectangle &r) const
{
	fl_rounded_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::RShadowBox::_draw(const Rectangle &r) const
{
	fl_rshadow_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::RFlatBox::_draw(const Rectangle &r) const
{
	fl_rflat_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::RoundUpBox::_draw(const Rectangle &r) const
{
	fl_round_up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::RoundDownBox::_draw(const Rectangle &r) const
{
	fl_round_down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::DiamondUpBox::_draw(const Rectangle &r) const
{
	fl_diamond_up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::DiamondDownBox::_draw(const Rectangle &r) const
{
	fl_diamond_down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::OvalBox::_draw(const Rectangle &r) const
{
	fl_oval_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::OShadowBox::_draw(const Rectangle &r) const
{
	fl_oval_shadow_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::OFlatBox::_draw(const Rectangle &r) const
{
	fl_oval_flat_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::UpFrame::_draw(const Rectangle &r) const
{
	fl_up_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::DownFrame::_draw(const Rectangle &r) const
{
	fl_down_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::ThinUpFrame::_draw(const Rectangle &r) const
{
	fl_thin_up_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::ThinDownFrame::_draw(const Rectangle &r) const
{
	fl_thin_down_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::EngravedFrame::_draw(const Rectangle &r) const
{
	fl_engraved_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::EmbossedFrame::_draw(const Rectangle &r) const
{
	fl_embossed_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::ShadowFrame::_draw(const Rectangle &r) const
{
	fl_shadow_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::RoundedFrame::_draw(const Rectangle &r) const
{
	fl_rounded_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::OvalFrame::_draw(const Rectangle &r) const
{
	fl_oval_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

void fltk3::FlatBox::_draw(const fltk3::Rectangle& r) const
{
	// FIXME: if (drawflags(INVISIBLE)) return;
	if (r.empty()) return;
	const fltk3::Color fg = fltk3::color();
	// FIXME: setcolor(fltk3::bgcolor());
	fltk3::rectf(r.x(), r.y(), r.w(), r.h());
	fltk3::color(fg);
}
//------------------------------------------------------------------------------
static fltk3::NoBox noBox("noBox");
static fltk3::FlatBox flatBox("flatBox");
static fltk3::UpBox upBox("upBox");
static fltk3::DownBox downBox("downBox");
static fltk3::ThinUpBox thinUpBox("thinUpBox");
static fltk3::ThinDownBox thinDownBox("thinDownBox");
static fltk3::OvalFrame ovalFrame("ovalFrame");
static fltk3::BorderFrame borderFrame("borderFrame");
static fltk3::EngravedBox engravedBox("engravedBox");
static fltk3::EmbossedBox embossedBox("embossedBox");
static fltk3::BorderBox borderBox("borderBox");
static fltk3::ShadowBox shadowBox("shadowBox");
static fltk3::RoundedBox roundedBox("roundedBox");
static fltk3::RShadowBox rShadowBox("rShadowBox");
static fltk3::RFlatBox rFlatBox("rFlatBox");
static fltk3::RoundUpBox roundUpBox("roundUpBox");
static fltk3::RoundDownBox roundDownBox("roundDownBox");
static fltk3::DiamondUpBox diamondUpBox("diamondUpBox");
static fltk3::DiamondDownBox diamondDownBox("diamondDownBox");
static fltk3::OvalBox ovalBox("ovalBox");
static fltk3::OShadowBox oShadowBox("oShadowBox");
static fltk3::OFlatBox oFlatBox("oFlatBox");
static fltk3::UpFrame upFrame("upFrame");
static fltk3::DownFrame downFrame("downFrame");
static fltk3::ThinUpFrame thinUpFrame("thinUpFrame");
static fltk3::ThinDownFrame thinDownFrame("thinDownFrame");
static fltk3::EngravedFrame engravedFrame("engravedFrame");
static fltk3::EmbossedFrame embossedFrame("embossedFrame");
static fltk3::ShadowFrame shadowFrame("shadowFrame");
static fltk3::RoundedFrame roundedFrame("roundedFrame");
//------------------------------------------------------------------------------
fltk3::Box* fltk3::NO_BOX           = 0L;
fltk3::Box* fltk3::FLAT_BOX         = &flatBox;
fltk3::Box* fltk3::UP_BOX           = &upBox;
fltk3::Box* fltk3::DOWN_BOX         = &downBox;
fltk3::Box* fltk3::THIN_UP_BOX      = &thinUpBox;
fltk3::Box* fltk3::THIN_DOWN_BOX    = &thinDownBox;
fltk3::Box* fltk3::OVAL_FRAME       = &ovalFrame;
fltk3::Box* fltk3::BORDER_FRAME     = &borderFrame;
fltk3::Box* fltk3::ENGRAVED_BOX     = &engravedBox;
fltk3::Box* fltk3::FRAME_BOX        = fltk3::ENGRAVED_BOX; // back-compatibility box types:
fltk3::Box* fltk3::EMBOSSED_BOX     = &embossedBox;
fltk3::Box* fltk3::BORDER_BOX       = &borderBox;
fltk3::Box* fltk3::SHADOW_BOX       = &shadowBox;
fltk3::Box* fltk3::ROUNDED_BOX      = &roundedBox;
fltk3::Box* fltk3::RSHADOW_BOX      = &rShadowBox;
fltk3::Box* fltk3::RFLAT_BOX        = &rFlatBox;
fltk3::Box* fltk3::ROUND_UP_BOX     = &roundUpBox;
fltk3::Box* fltk3::ROUND_DOWN_BOX   = &roundDownBox;
fltk3::Box* fltk3::DIAMOND_UP_BOX   = &diamondUpBox;
fltk3::Box* fltk3::DIAMOND_DOWN_BOX = &diamondDownBox;
fltk3::Box* fltk3::OVAL_BOX         = &ovalBox;
fltk3::Box* fltk3::OSHADOW_BOX      = &oShadowBox;
fltk3::Box* fltk3::OFLAT_BOX        = &oFlatBox;
fltk3::Box* fltk3::UP_FRAME         = &upFrame;
fltk3::Box* fltk3::DOWN_FRAME       = &downFrame;
fltk3::Box* fltk3::THIN_UP_FRAME    = &thinUpFrame;
fltk3::Box* fltk3::THIN_DOWN_FRAME  = &thinDownFrame;
fltk3::Box* fltk3::ENGRAVED_FRAME   = &engravedFrame;
fltk3::Box* fltk3::EMBOSSED_FRAME   = &embossedFrame;
fltk3::Box* fltk3::SHADOW_FRAME     = &shadowFrame;
fltk3::Box* fltk3::ROUNDED_FRAME    = &roundedFrame;
//------------------------------------------------------------------------------


//
// End of "$Id: Box.cxx 9638 2012-07-24 04:41:38Z matt $".
//
