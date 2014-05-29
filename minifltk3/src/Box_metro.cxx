#include "run.h"
#include "draw.h"
#include "Box.h"
#include "flstring.h"

static void Metro_color(fltk3::Color c)
{
	if (fltk3::draw_box_active()) fltk3::color(c);
	else fltk3::color(fltk3::inactive(c));
}

static void shade_rect_top_bottom(int x, int y, int w, int h, fltk3::Color fg1, fltk3::Color fg2, float th)
{
	Metro_color(fg1);
	fltk3::rectf(x, y, w, h);
	return;

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
		Metro_color(fltk3::color_average(fltk3::color_average(fg1, fg2, th), fg1, k));
		fltk3::line(x, y+j, x+w, y+j);
		j++;
	}
	Metro_color(fg1);
	fltk3::rectf(x, y+h_top, w+1, h_flat);
	// This loop generates the gradient at the bottom of the widget	
	for (k = 1; k >= 0; k -= step_size_bottom) {
		Metro_color(fltk3::color_average(fg1,fltk3::color_average(fg1, fg2, th), k));
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
	Metro_color(fg1);
	fltk3::line(x, y+h, x, y);         //Go from bottom to top left side
	fltk3::line(x+w, y+h, x+w, y);     //Go from bottom to top right side
	fltk3::line(x, y, x+w, y);         //Go across the top
	fltk3::line(x, y+h, x+w, y+h);     //Go across the bottom
	return;
	Metro_color(fg2);
	fltk3::line(x+1, y+h-2, x+1, y+2);     //Go from bottom to top left side
	fltk3::line(x+w-1, y+h-2, x+w-1, y+2); //Go from bottom to top right side
	Metro_color(lc);
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
	shade_rect_top_bottom_up(x, y, w, h, c, .0f);
	frame_rect_up(x, y, w, h, c, fltk3::color_average(c, fltk3::WHITE, .05f), .15f, .05f);
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
void fltk3::MetroUpFrame::_draw(const Rectangle &r) const
{
	up_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::MetroDownFrame::_draw(const Rectangle &r) const
{
	down_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::MetroUpBox::_draw(const Rectangle &r) const
{
	up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::MetroDownBox::_draw(const Rectangle &r) const
{
	down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::MetroThinUpBox::_draw(const Rectangle &r) const
{
	thin_up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::MetroThinDownBox::_draw(const Rectangle &r) const
{
	thin_down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::MetroRoundUpBox::_draw(const Rectangle &r) const
{
	up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
void fltk3::MetroRoundDownBox::_draw(const Rectangle &r) const
{
	down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}
//------------------------------------------------------------------------------
static fltk3::MetroUpFrame       metroUpFrame("metroUpFrame");
static fltk3::MetroDownFrame     metroDownFrame("metroDownFrame");

static fltk3::MetroUpBox         metroUpBox("metroUpBox");
static fltk3::MetroDownBox       metroDownBox("metroDownBox");
static fltk3::MetroThinUpBox     metroThinUpBox("metroThinUpBox");
static fltk3::MetroThinDownBox   metroThinDownBox("metroThinDownBox");
static fltk3::MetroRoundUpBox    metroRoundUpBox("metroRoundUpBox");
static fltk3::MetroRoundDownBox  metroRoundDownBox("metroRoundDownBox");
//------------------------------------------------------------------------------
fltk3::Box* fltk3::METRO_UP_FRAME        = &metroUpFrame;
fltk3::Box* fltk3::METRO_DOWN_FRAME      = &metroDownFrame;

fltk3::Box* fltk3::METRO_UP_BOX          = &metroUpBox;
fltk3::Box* fltk3::METRO_DOWN_BOX        = &metroDownBox;
fltk3::Box* fltk3::METRO_THIN_UP_BOX     = &metroThinUpBox;
fltk3::Box* fltk3::METRO_THIN_DOWN_BOX   = &metroThinDownBox;
fltk3::Box* fltk3::METRO_ROUND_UP_BOX    = &metroRoundUpBox;
fltk3::Box* fltk3::METRO_ROUND_DOWN_BOX  = &metroRoundDownBox;
//------------------------------------------------------------------------------
