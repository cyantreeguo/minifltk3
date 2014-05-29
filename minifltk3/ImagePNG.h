//
// "$Id: ImagePNG.h 9300 2012-03-23 20:36:00Z ianmacarthur $"
//
// PNG image header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::ImagePNG class . */

#ifndef Fltk3_ImagePNG_H
#define Fltk3_ImagePNG_H

#include "run.h"
#include "Image.h"
#include "SharedImage.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include "utf8.h"

#include "lodepng.h"

namespace fltk3
{

/**
 The fltk3::ImagePNG class supports loading, caching,
 and drawing of Portable Network Graphics (PNG) image files. The
 class loads colormapped and full-color images and handles color-
 and alpha-based transparency.
 */
class FLTK3_EXPORT ImagePNG : public fltk3::ImageRGB
{
protected:
	ImagePNG(const uchar *a, int b, int c, int d=3, int e=0) : ImageRGB(a, b, c, d, e) {}

public:
	ImagePNG(const char* filename) : fltk3::ImageRGB(0,0,0) 
	{
		FILE *fp; // File pointer
		int size;
		unsigned char *buf;

		if ((fp = fltk3::fopen(filename, "rb")) == NULL) return;
		fseek(fp, 0, SEEK_END);
		size = (int) ftell(fp);
		if ( size <= 0 ) { fclose(fp); return; }
		buf = (unsigned char*)malloc(size);
		fseek(fp, 0, SEEK_SET);
		size = (int) fread(buf, 1, size, fp);
		fclose(fp);
		
		unsigned error;
		unsigned char* image;
		unsigned int width, height;

		error = lodepng_decode32(&image, &width, &height, buf, size);
		if(error) {
			free(buf);
			return;
		}
		
		w(width); h(height); d(4);

		array = new uchar[w() * h() * d()];
		alloc_array = 1;
		unsigned char *ptr = (unsigned char *)array;
		memcpy(ptr, image, w() * h() * d());

		free(image);
		free(buf);
	}

	ImagePNG(const char *name_png, const unsigned char *buffer, int datasize) : fltk3::ImageRGB(0,0,0) 
	{
		unsigned error;
		unsigned char* image;
		unsigned int width, height;

		error = lodepng_decode32(&image, &width, &height, buffer, datasize);
		if(error) return;

		w(width); h(height); d(4);

		array = new uchar[w() * h() * d()];
		alloc_array = 1;
		unsigned char *ptr = (unsigned char *)array;
		memcpy(ptr, image, w() * h() * d());

		free(image);

		if (w() && h() && name_png) {
			fltk3::SharedImage *si = new fltk3::SharedImage(name_png, this);
			si->add();
		}
	}
};

}

#endif

//
// End of "$Id: ImagePNG.h 9300 2012-03-23 20:36:00Z ianmacarthur $".
//
