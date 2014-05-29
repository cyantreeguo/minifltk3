//
// "$Id: TiledImage.h 9300 2012-03-23 20:36:00Z ianmacarthur $"
//
// Tiled image header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::TiledImage widget . */

#ifndef Fltk3_Tiled_Image_H
#  define Fltk3_Tiled_Image_H

#include "run.h"
#include "Image.h"
#include "draw.h"

namespace fltk3
{

/**
 This class supports tiling of images
 over a specified area. The source (tile) image is <B>not</B>
 copied unless you call the color_average(),
 desaturate(),
 or inactive()
 methods.
 */
class FLTK3_EXPORT TiledImage : public fltk3::Image
{
protected:
	fltk3::Image *image_;		// The image that is shared
	int alloc_image_;		// Did we allocate this image?

public:
	/**
	The constructors create a new tiled image containing the specified image.
	Use a width and height of 0 to tile the whole window/widget.
	*/
	TiledImage(fltk3::Image *i,	// I - Image to tile
				int      W=0,	// I - Width of tiled area
				int      H=0) :	// I - Height of tiled area
				fltk3::Image(W,H,0)
	{
		image_       = i;
		alloc_image_ = 0;

		if (W == 0) w(fltk3::w());
		if (H == 0) h(fltk3::h());
	}

	/**
	The destructor frees all memory and server resources that are used by
	the tiled image.
	*/
	virtual ~TiledImage()
	{
		if (alloc_image_) delete image_;
	}

	// 'fltk3::TiledImage::copy()' - Copy and resize a tiled image...
	// virtual fltk3::Image *copy(int W, int H);
	virtual fltk3::Image *			// O - New image
		copy(
			int W,	// I - New width
			int H)  	// I - New height
	{
		if (W == w() && H == h()) return this;
		else return new fltk3::TiledImage(image_, W, H);
	}
	
	fltk3::Image *copy() {
		return copy(w(), h());
	}

	// 'fltk3::TiledImage::color_average()' - Blend colors...
	//virtual void color_average(fltk3::Color c, float i);
	virtual void color_average(fltk3::Color c,	// I - Color to blend with
			float    i)  	// I - Blend fraction
	{
		if (!alloc_image_) {
			image_       = image_->copy();
			alloc_image_ = 1;
		}

		image_->color_average(c, i);
	}

	// 'fltk3::TiledImage::desaturate()' - Convert the image to grayscale...
	virtual void desaturate()
	{
		if (!alloc_image_) {
			image_ = image_->copy();
			alloc_image_ = 1;
		}

		image_->desaturate();
	}

	// 'fltk3::TiledImage::draw()' - Draw a shared image...
	virtual void draw(int X,	// I - Starting X position
		int Y,	// I - Starting Y position
		int W,	// I - Width of area to be filled
		int H,	// I - Height of area to be filled
		int cx,	// I - "Source" X position
		int cy)  	// I - "Source" Y position
	{
		if (!image_->w() || !image_->h()) return;
		if (W == 0) W = fltk3::w();
		if (H == 0) H = fltk3::h();

		fltk3::push_clip(X, Y, W, H);

		X += cx;
		Y += cy;

		X = X - (X % image_->w());
		Y = Y - (Y % image_->h());

		W += X;
		H += Y;

		for (int yy = Y; yy < H; yy += image_->h())
			for (int xx = X; xx < W; xx += image_->w())
				image_->draw(xx, yy);

		fltk3::pop_clip();
	}

	void draw(int X, int Y) { draw(X, Y, w(), h(), 0, 0); }
	/** Gets The image that is shared */
	fltk3::Image *image() { return image_; }
};

}

#endif // !Fltk3_Tiled_Image_H

//
// End of "$Id: TiledImage.h 9300 2012-03-23 20:36:00Z ianmacarthur $"
//
