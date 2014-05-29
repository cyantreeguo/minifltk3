//
// "$Id: PackedGroup.h 9615 2012-06-15 21:11:24Z matt $"
//
// Pack header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::PackedGroup widget . */

#ifndef Fltk3_Pack_H
#define Fltk3_Pack_H

#include "run.h"
#include "Group.h"
#include "draw.h"

namespace fltk3
{

/**
 This widget was designed to add the functionality of compressing and
 aligning widgets.
 <P>If type() is fltk3::PackedGroup::HORIZONTAL all the children are
 resized to the height of the fltk3::PackedGroup, and are moved next to
 each other horizontally. If type() is not fltk3::PackedGroup::HORIZONTAL
 then the children are resized to the width and are stacked below each
 other.  Then the fltk3::PackedGroup resizes itself to surround the child
 widgets.
 <P>This widget is needed for the fltk3::TabGroup.
 In addition you may want to put the fltk3::PackedGroup inside an
 fltk3::ScrollGroup.

 <P>The resizable for fltk3::PackedGroup is set to NULL by default.</p>
 <P>See also: fltk3::Group::resizable()
 */
class FLTK3_EXPORT PackedGroup : public fltk3::Group
{
	int spacing_;

public:
	enum { // values for type(int)
		VERTICAL = 0,
		HORIZONTAL = 1
	};

protected:
	void draw()
	{
		int tx =     fltk3::box_dx(box());
		int ty =     fltk3::box_dy(box());
		int tw = w()-fltk3::box_dw(box());
		int th = h()-fltk3::box_dh(box());
		int rw, rh;
		int current_position = horizontal() ? tx : ty;
		int maximum_position = current_position;
		uchar d = damage();
		fltk3::Widget*const* a = array();
		if (horizontal()) {
			rw = -spacing_;
			rh = th;
			for (int i = children(); i--;) {
				if (child(i)->visible()) {
					if (child(i) != this->resizable()) rw += child(i)->w();
					rw += spacing_;
				}
			}
		} else {
			rw = tw;
			rh = -spacing_;
			for (int i = children(); i--;) {
				if (child(i)->visible()) {
					if (child(i) != this->resizable()) rh += child(i)->h();
					rh += spacing_;
				}
			}
		}
		for (int i = children(); i--;) {
			fltk3::Widget* o = *a++;
			if (o->visible()) {
				int X,Y,W,H;
				if (horizontal()) {
					X = current_position;
					W = o->w();
					Y = ty;
					H = th;
				} else {
					X = tx;
					W = tw;
					Y = current_position;
					H = o->h();
				}
				// Last child, if resizable, takes all remaining room
				if (i == 0 && o == this->resizable()) {
					if(horizontal())
						W = tw - rw;
					else
						H = th - rh;
				}
				if (spacing_ && current_position>maximum_position && box() &&
					(X != o->x() || Y != o->y() || d&fltk3::DAMAGE_ALL)) {
						fltk3::color(color());
						if (horizontal())
							fltk3::rectf(maximum_position, ty, spacing_, th);
						else
							fltk3::rectf(tx, maximum_position, tw, spacing_);
				}
				if (X != o->x() || Y != o->y() || W != o->w() || H != o->h()) {
					o->resize(X,Y,W,H);
					o->set_damage(fltk3::DAMAGE_ALL);
				}
				if (d&fltk3::DAMAGE_ALL) {
					draw_child(*o);
					draw_outside_label(*o);
				} else update_child(*o);
				// child's draw() can change it's size, so use new size:
				current_position += (horizontal() ? o->w() : o->h());
				if (current_position > maximum_position)
					maximum_position = current_position;
				current_position += spacing_;
			}
		}

		if (horizontal()) {
			if (maximum_position < tx+tw && box()) {
				fltk3::color(color());
				fltk3::rectf(maximum_position, ty, tx+tw-maximum_position, th);
			}
			tw = maximum_position-tx;
		} else {
			if (maximum_position < ty+th && box()) {
				fltk3::color(color());
				fltk3::rectf(tx, maximum_position, tw, ty+th-maximum_position);
			}
			th = maximum_position-ty;
		}

		tw += fltk3::box_dw(box());
		if (tw <= 0) tw = 1;
		th += fltk3::box_dh(box());
		if (th <= 0) th = 1;
		if (tw != w() || th != h()) {
			Widget::resize(x(),y(),tw,th);
			d = fltk3::DAMAGE_ALL;
		}
		if (d&fltk3::DAMAGE_ALL) {
			draw_box();
			draw_label();
		}
	}

public:
	/**
	Creates a new fltk3::PackedGroup widget using the given position, size,
	and label string. The default boxtype is fltk3::NO_BOX.
	<P>The destructor <I>also deletes all the children</I>. This allows a
	whole tree to be deleted at once, without having to keep a pointer to
	all the children in the user code. A kludge has been done so the
	fltk3::PackedGroup and all of it's children can be automatic (local)
	variables, but you must declare the fltk3::PackedGroup<I>first</I>, so
	that it is destroyed last.
	*/
	PackedGroup(int X, int Y, int W, int H,const char *l=0) : fltk3::Group(X, Y, W, H, l)
	{
		resizable(0);
		spacing_ = 0;
		// type(VERTICAL); // already set like this
	}

	/**
	 Gets the number of extra pixels of blank space that are added
	 between the children.
	 */
	int spacing() const { return spacing_; }

	/**
	 Sets the number of extra pixels of blank space that are added
	 between the children.
	 */
	void spacing(int i) { spacing_ = i; }

	/** Same as fltk3::Group::type() */
	uchar horizontal() const { return type(); }

	/** Recalculate the size of this widget and reposition all children. */
	// calculate the height and width of this widget.
	// FIXME: do not move any children. "draw()" does that for now.
	void layout()
	{
		int tw = w()-fltk3::box_dw(box());
		int th = h()-fltk3::box_dh(box());
		int rw, rh;
		if (horizontal()) {
			rw = -spacing_;
			rh = th;
			for (int i = children(); i--;) {
				if (child(i)->visible()) {
					if (child(i) != this->resizable()) rw += child(i)->w();
					rw += spacing_;
				}
			}
		} else {
			rw = tw;
			rh = -spacing_;
			for (int i = children(); i--;) {
				if (child(i)->visible()) {
					if (child(i) != this->resizable()) rh += child(i)->h();
					rh += spacing_;
				}
			}
		}
		size(rw+fltk3::box_dw(box()), rh+fltk3::box_dh(box()));
	}

	void resize(int x, int y, int w, int h) { Widget::resize(x, y, w, h); }
};

}

#endif

//
// End of "$Id: PackedGroup.h 9615 2012-06-15 21:11:24Z matt $".
//
