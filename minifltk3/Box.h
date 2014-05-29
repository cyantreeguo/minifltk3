//
// "$Id: Box.h 9658 2012-08-03 18:28:53Z matt $"
//
// Box header file for the Fast Light Tool Kit (FLTK).
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

/* \file
 fltk3::Widget widget . */

#ifndef FLTK3_BOX_H
#define FLTK3_BOX_H

#include "Symbol.h"
#include "Widget.h"

/* Comment from matt , r9557
 * Removed fltk3::Box as a wiget class. In FLTK2, boxes are drawn simply by
 * using fltk::Widget. I assume we could add fltk3::BoxWidget. Why all this?
 * 'Box' will be used later as a base class for a much more flexible Boxtype.
 *
 * Dejan: I don't think so - in FLTK2 fltk::Box is an *alias* for fltk::Symbol.
 *        :) FLTK2's FrameBox, HighlightBox, and FlatBox do similar to what you
 *        do here. The only difference is that FLTK2's class hierarchy is
 *        little bit less complicated. I suggest you take a look at the source.
 */

namespace fltk3
{


class FLTK3_EXPORT BoxWidget : public Widget
{
public:
	BoxWidget(fltk3::Box* b, int x, int y, int w, int h, const char *l=0) : Widget(b, x, y, w, h, l) {}
	BoxWidget(int x, int y, int w, int h, const char *l=0) : Widget(x, y, w, h, l) {}
};


class FLTK3_EXPORT Box : public Symbol
{
public:
	typedef unsigned int Flags;
	enum {
		PRESSED      = 0x0001,
		HOVERING     = 0x0002,
		FOCUSED      = 0x0004,
		ACTIVE       = 0x0008,
		TIE_LEFT     = 0x0100,
		TIE_RIGHT    = 0x0200,
		TIE_TOP      = 0x0400,
		TIE_BOTTOM   = 0x0800,
		TIE_MASK     = 0x0f00,
		TIE_HIDDEN   = 0x1000
	};

	Box(const char *name=0L) : Symbol(name) {}

	/**
	 For shaded boxes, return the box with "raised" graphics.
	 */
	virtual Box* upbox() { return this; }

	/**
	 For shaded boxes, return the box with "lowered" graphics.
	 */
	virtual Box* downbox() { return this; }

	/**
	 Return the corresponding frame type (box without background).
	 */
	virtual Box* upframe() { return this; }

	virtual Box* downframe() { return this; }

	/**
	 Return the corresponding filled box type (box with background).
	 */
	virtual Box* box() { return this; }

	virtual void draw(const Rectangle& r, Flags flags) const { _draw(r); }
};

fltk3::Box *scheme_check(fltk3::Box *b);

// ===================================================
// scheme: default
// ===================================================

class FLTK3_EXPORT NoBox : public Box
{
public:
	void _draw(const Rectangle&) const {}
	NoBox(const char* name) : fltk3::Box(name) { set_inset(0, 0, 0, 0); }
};

class FLTK3_EXPORT FlatBox : public Box
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return true; }
	FlatBox(const char* name) : fltk3::Box(name) { set_inset(0, 0, 0, 0); }
};

class FLTK3_EXPORT UpBox : public FlatBox
{
public:
	Box* downbox() { return DOWN_BOX; }
	Box* upframe() { return UP_FRAME; }
	void _draw(const Rectangle&) const;
	UpBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT DownBox : public FlatBox
{
public:
	Box* upbox() { return UP_BOX; }
	Box* downframe() { return DOWN_FRAME; }
	void _draw(const Rectangle&) const;
	DownBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT ThinUpBox : public FlatBox
{
public:
	Box* downbox() { return THIN_DOWN_BOX; }
	void _draw(const Rectangle&) const;
	ThinUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
};

class FLTK3_EXPORT ThinDownBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	ThinDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
};

class FLTK3_EXPORT EngravedBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	EngravedBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT EmbossedBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	EmbossedBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT BorderBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	BorderBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
};

class FLTK3_EXPORT ShadowBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	ShadowBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -5, -5); }
};

class FLTK3_EXPORT RoundedBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	RoundedBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, 2, 2); }
};

class FLTK3_EXPORT RShadowBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	RShadowBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
};

class FLTK3_EXPORT RFlatBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	RFlatBox(const char* name) : fltk3::FlatBox(name) { set_inset(0, 0, 0, 0); }
};

class FLTK3_EXPORT RoundUpBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	RoundUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT RoundDownBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	RoundDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT DiamondUpBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	DiamondUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(0, 0, 0, 0); }
};

class FLTK3_EXPORT DiamondDownBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	DiamondDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(0, 0, 0, 0); }
};

class FLTK3_EXPORT OvalBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	OvalBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
};

class FLTK3_EXPORT OShadowBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	OShadowBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
};

class FLTK3_EXPORT OFlatBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	OFlatBox(const char* name) : fltk3::FlatBox(name) { set_inset(0, 0, 0, 0); }
};

class FLTK3_EXPORT BorderFrame : public Box
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	BorderFrame(const char* name) : fltk3::Box(name) { set_inset(1, 1, -2, -2); }
};

class FLTK3_EXPORT UpFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	UpFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT DownFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	DownFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT ThinUpFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	ThinUpFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(1, 1, -2, -2); }
};

class FLTK3_EXPORT ThinDownFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	ThinDownFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(1, 1, -2, -2); }
};

class FLTK3_EXPORT EngravedFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	EngravedFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT EmbossedFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	EmbossedFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT ShadowFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	ShadowFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(1, 1, -5, -5); }
};

class FLTK3_EXPORT RoundedFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	RoundedFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(1, 1, -2, -2); }
};

class FLTK3_EXPORT OvalFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	OvalFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(1, 1, -2, -2); }
};

// ===================================================
// scheme: GTK+
// ===================================================
class FLTK3_EXPORT GtkUpBox : public FlatBox
{
public:
	Box* downbox() { return GTK_DOWN_BOX; }
	Box* upframe() { return GTK_DOWN_FRAME; }
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	GtkUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT GtkDownBox : public FlatBox
{
public:
	Box* upbox() { return GTK_UP_BOX; }
	Box* downframe() { return GTK_DOWN_FRAME; }
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	GtkDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT GtkThinUpBox : public FlatBox
{
public:
	Box* downbox() { return GTK_THIN_DOWN_BOX; }
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	GtkThinUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
};

class FLTK3_EXPORT GtkThinDownBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	GtkThinDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
};

class FLTK3_EXPORT GtkRoundUpBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	GtkRoundUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT GtkRoundDownBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	GtkRoundDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT GtkUpFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	GtkUpFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT GtkDownFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	GtkDownFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT GtkThinUpFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	GtkThinUpFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(1, 1, -2, -2); }
};

class FLTK3_EXPORT GtkThinDownFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	GtkThinDownFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(1, 1, -2, -2); }
};

// ===================================================
// scheme: Plastic
// ===================================================
class FLTK3_EXPORT PlasticUpBox : public FlatBox
{
public:
	Box* downbox() { return PLASTIC_DOWN_BOX; }
	Box* upframe() { return PLASTIC_DOWN_FRAME; }
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	PlasticUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(4, 4, -8, -8); }
};

class FLTK3_EXPORT PlasticDownBox : public FlatBox
{
public:
	Box* upbox() { return PLASTIC_UP_BOX; }
	Box* downframe() { return PLASTIC_DOWN_FRAME; }
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	PlasticDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT PlasticThinUpBox : public FlatBox
{
public:
	Box* downbox() { return PLASTIC_THIN_DOWN_BOX; }
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	PlasticThinUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT PlasticThinDownBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	PlasticThinDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT PlasticRoundUpBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	PlasticRoundUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT PlasticRoundDownBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	bool fills_rectangle() const { return false; }
	PlasticRoundDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT PlasticUpFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	PlasticUpFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT PlasticDownFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	PlasticDownFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
};

// ===================================================
// scheme: Gleam
// ===================================================
class FLTK3_EXPORT GleamUpBox : public FlatBox
{
public:
	Box* downbox() { return GLEAM_DOWN_BOX; }
	Box* upframe() { return GLEAM_DOWN_FRAME; }
	void _draw(const Rectangle&) const;
	GleamUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT GleamDownBox : public FlatBox
{
public:
	Box* upbox() { return GLEAM_UP_BOX; }
	Box* downframe() { return GLEAM_DOWN_FRAME; }
	void _draw(const Rectangle&) const;
	GleamDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT GleamThinUpBox : public FlatBox
{
public:
	Box* downbox() { return GLEAM_THIN_DOWN_BOX; }
	void _draw(const Rectangle&) const;
	GleamThinUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
};

class FLTK3_EXPORT GleamThinDownBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	GleamThinDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
};

class FLTK3_EXPORT GleamRoundUpBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	GleamRoundUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT GleamRoundDownBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	GleamRoundDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT GleamUpFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	GleamUpFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT GleamDownFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	GleamDownFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
};

// ===================================================
// scheme: Metro
// ===================================================
class FLTK3_EXPORT MetroUpBox : public FlatBox
{
public:
	Box* downbox() { return METRO_DOWN_BOX; }
	Box* upframe() { return METRO_DOWN_FRAME; }
	void _draw(const Rectangle&) const;
	MetroUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(0, 0, 1, 1); }
};

class FLTK3_EXPORT MetroDownBox : public FlatBox
{
public:
	Box* upbox() { return METRO_UP_BOX; }
	Box* downframe() { return METRO_DOWN_FRAME; }
	void _draw(const Rectangle&) const;
	MetroDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(0, 0, 1, 1); }
};

class FLTK3_EXPORT MetroThinUpBox : public FlatBox
{
public:
	Box* downbox() { return METRO_THIN_DOWN_BOX; }
	void _draw(const Rectangle&) const;
	MetroThinUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
};

class FLTK3_EXPORT MetroThinDownBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	MetroThinDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
};

class FLTK3_EXPORT MetroRoundUpBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	MetroRoundUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT MetroRoundDownBox : public FlatBox
{
public:
	void _draw(const Rectangle&) const;
	MetroRoundDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT MetroUpFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	MetroUpFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
};

class FLTK3_EXPORT MetroDownFrame : public BorderFrame
{
public:
	void _draw(const Rectangle&) const;
	MetroDownFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
};

}

#endif

//
// End of "$Id: Box.h 9658 2012-08-03 18:28:53Z matt $".
//
