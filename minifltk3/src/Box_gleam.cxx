#include "run.h"
#include "draw.h"
#include "Box.h"
#include "flstring.h"

static void gleam_color(fltk3::Color c)
{
	if (fltk3::draw_box_active()) fltk3::color(c);
	else fltk3::color(fltk3::inactive(c));
}

static void shade_rect_top_bottom(int x, int y, int w, int h, fltk3::Color fg1, fltk3::Color fg2, float th)
{
	// Draws the shiny using maximum limits
	int h_top    = ((h/2) < (20) ? (h/2) : (20)); // min(h/2,20);
	int h_bottom = ((h/6) < (15) ? (h/6) : (15)); // min(h/6,15);
	int h_flat = h-(h_top+h_bottom);
	int j = 0;
	float step_size_top = (float)(h_top>1?(0.999/(float)(h_top)):1);
	float step_size_bottom = (float)(h_bottom>1?(0.999/(float)(h_bottom)):1);
	float k;
	// This loop generates the gradient at the top of the widget
	for (k = 1; k >= 0; k -= step_size_top) {
		gleam_color(fltk3::color_average(fltk3::color_average(fg1, fg2, th), fg1, k));
		fltk3::line(x, y+j, x+w, y+j);
		j++;
	}
	gleam_color(fg1);
	fltk3::rectf(x, y+h_top, w+1, h_flat);
	// This loop generates the gradient at the bottom of the widget	
	for (k = 1; k >= 0; k -= step_size_bottom) {
		gleam_color(fltk3::color_average(fg1,fltk3::color_average(fg1, fg2, th), k));
		fltk3::line(x, y+j+h_flat-1, x+w, y+j+h_flat-1);
		j++;
	}
}

static void shade_rect_top_bottom_up(int x, int y, int w, int h, fltk3::Color bc, float th)
{
	shade_rect_top_bottom(x,y,w,h,bc,fltk3::WHITE,th);
}

static void shade_rect_top_bottom_down(int x, int y, int w, int h, fltk3::Color bc, float th)
{
	shade_rect_top_bottom(x,y,w,h,bc,fltk3::BLACK,th);
}

static void frame_rect(int x, int y, int w, int h, fltk3::Color fg1, fltk3::Color fg2, fltk3::Color lc)
{
	gleam_color(fg1);
	fltk3::line(x, y+h-1, x, y+1);         //Go from bottom to top left side
	fltk3::line(x+w, y+h-1, x+w, y+1);     //Go from bottom to top right side
	fltk3::line(x+1, y, x+w-1, y);         //Go across the top
	fltk3::line(x+1, y+h, x+w-1, y+h);     //Go across the bottom
	gleam_color(fg2);
	fltk3::line(x+1, y+h-2, x+1, y+2);     //Go from bottom to top left side
	fltk3::line(x+w-1, y+h-2, x+w-1, y+2); //Go from bottom to top right side
	gleam_color(lc);
	fltk3::line(x+2, y+1, x+w-3, y+1);     //Go across the top
	fltk3::line(x+2, y+h-1, x+w-3, y+h-1); //Go across the bottom
}

static void frame_rect_up(int x, int y, int w, int h, fltk3::Color bc, fltk3::Color lc, float th1, float th2)
{
	frame_rect(x,y,w,h,fltk3::color_average(fltk3::darker(bc), fltk3::BLACK, th1),fltk3::color_average(bc, fltk3::WHITE, th2),lc);
}

static void frame_rect_down(int x, int y, int w, int h, fltk3::Color bc, fltk3::Color lc, float th1, float th2)
{
	frame_rect(x,y,w,h,fltk3::color_average(bc, fltk3::WHITE, th1),fltk3::color_average(fltk3::BLACK, bc, th2),lc);
}

static void up_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*)
{
	frame_rect_up(x, y, w-1, h-1, c, fltk3::color_average(c, fltk3::WHITE, .25f), .55f, .05f);
}

static void up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*)
{
	shade_rect_top_bottom_up(x+2, y+1, w-5, h-3, c, .15f);
	frame_rect_up(x, y, w-1, h-1, c, fltk3::color_average(c, fltk3::WHITE, .05f), .15f, .05f);
}

static void thin_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*)
{
	shade_rect_top_bottom_up(x+2, y+1, w-5, h-3, c, .25f);
	frame_rect_up(x, y, w-1, h-1, c, fltk3::color_average(c, fltk3::WHITE, .45f), .25f, .15f);
}

static void down_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*)
{
	frame_rect_down(x, y, w-1, h-1, fltk3::darker(c), fltk3::darker(c), .25f, .95f);
}

static void down_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*)
{
	shade_rect_top_bottom_down(x+1, y+1, w-3, h-3, c, .65f);
	frame_rect_down(x, y, w-1, h-1, c, fltk3::color_average(c, fltk3::BLACK, .05f), .05f, .95f);
}

static void thin_down_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*)
{
	shade_rect_top_bottom_down(x+1, y+1, w-3, h-3, c, .85f);
	frame_rect_down(x, y, w-1, h-1, c, fltk3::color_average(c, fltk3::BLACK, .45f), .35f, 0.85f);
}

//------------------------------------------------------------------------------
void fltk3::GleamUpFrame::_draw(const Rectangle &r) const
{
	up_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::GleamDownFrame::_draw(const Rectangle &r) const
{
	down_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::GleamUpBox::_draw(const Rectangle &r) const
{
	up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::GleamDownBox::_draw(const Rectangle &r) const
{
	down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::GleamThinUpBox::_draw(const Rectangle &r) const
{
	thin_up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::GleamThinDownBox::_draw(const Rectangle &r) const
{
	thin_down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::GleamRoundUpBox::_draw(const Rectangle &r) const
{
	up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::GleamRoundDownBox::_draw(const Rectangle &r) const
{
	down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
//------------------------------------------------------------------------------
static fltk3::GleamUpFrame       gleamUpFrame("gleamUpFrame");
static fltk3::GleamDownFrame     gleamDownFrame("gleamDownFrame");

static fltk3::GleamUpBox         gleamUpBox("gleamUpBox");
static fltk3::GleamDownBox       gleamDownBox("gleamDownBox");
static fltk3::GleamThinUpBox     gleamThinUpBox("gleamThinUpBox");
static fltk3::GleamThinDownBox   gleamThinDownBox("gleamThinDownBox");
static fltk3::GleamRoundUpBox    gleamRoundUpBox("gleamRoundUpBox");
static fltk3::GleamRoundDownBox  gleamRoundDownBox("gleamRoundDownBox");
//------------------------------------------------------------------------------
fltk3::Box* fltk3::GLEAM_UP_FRAME        = &gleamUpFrame;
fltk3::Box* fltk3::GLEAM_DOWN_FRAME      = &gleamDownFrame;

fltk3::Box* fltk3::GLEAM_UP_BOX          = &gleamUpBox;
fltk3::Box* fltk3::GLEAM_DOWN_BOX        = &gleamDownBox;
fltk3::Box* fltk3::GLEAM_THIN_UP_BOX     = &gleamThinUpBox;
fltk3::Box* fltk3::GLEAM_THIN_DOWN_BOX   = &gleamThinDownBox;
fltk3::Box* fltk3::GLEAM_ROUND_UP_BOX    = &gleamRoundUpBox;
fltk3::Box* fltk3::GLEAM_ROUND_DOWN_BOX  = &gleamRoundDownBox;
//------------------------------------------------------------------------------
