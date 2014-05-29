//
// "$Id: ImageJPEG.h 9300 2012-03-23 20:36:00Z ianmacarthur $"
//
// JPEG image header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::ImageJPEG class . */

#ifndef Fltk3_ImageJPEG_H
#define Fltk3_ImageJPEG_H

#include "Image.h"
#include "SharedImage.h"
#include "utf8.h"
#include "run.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>

#include "jpgd.h"

namespace fltk3
{

/**
 The fltk3::ImageJPEG class supports loading, caching,
 and drawing of Joint Photographic Experts Group (JPEG) File
 Interchange Format (JFIF) images. The class supports grayscale
 and color (RGB) JPEG image files.
 */
class FLTK3_EXPORT ImageJPEG : public fltk3::ImageRGB
{
public:
	ImageJPEG(const char *filename) : fltk3::ImageRGB(0,0,0) 
	{
		FILE		*fp;		// File pointer
		int size;
		unsigned char *buf;
		unsigned char *p, *ptr;	// Pointer to pixel values

		if ((fp = fltk3::fopen(filename, "rb")) == NULL) return;
		fseek(fp, 0, SEEK_END);
		size = (int) ftell(fp);
		if ( size <= 0 ) { fclose(fp); return; }
		buf = (unsigned char*)malloc(size);
		fseek(fp, 0, SEEK_SET);
		size = (int) fread(buf, 1, size, fp);
		fclose(fp);

		/*
		// Loads a JPEG image from a memory buffer.
		// req_comps can be 1 (grayscale), 3 (RGB), or 4 (RGBA).
		// On return, width/height will be set to the image's dimensions, and actual_comps will be set 
		// to either 1 (grayscale) or 3 (RGB).
		unsigned char *decompress_jpeg_image_from_memory(const unsigned char *pSrc_data, int src_data_size, 
			int *width, int *height, int *actual_comps, int req_comps);
		*/
		int width, height, ac;
		p = jpgd::decompress_jpeg_image_from_memory(buf, size, &width, &height, &ac, 3);
		if ( p == NULL ) {
			free(buf);
			return;
		}

		w(width); h(height); d(ac);
		array = new uchar[w() * h() * d()];
		alloc_array = 1;
		ptr = (unsigned char *)array;
		memcpy(ptr, p, w() * h() * d());

		free(p);
		free(buf);
	}

	/**
	 \brief The constructor loads the JPEG image from memory.

	 Construct an image from a block of memory inside the application. Fluid offers
	 "binary Data" chunks as a great way to add image data into the C++ source code.
	 name_png can be NULL. If a name is given, the image is added to the list of
	 shared images (see: fltk3::SharedImage) and will be available by that name.

	 The inherited destructor frees all memory and server resources that are used
	 by the image.

	 There is no error function in this class. If the image has loaded correctly,
	 w(), h(), and d() should return values greater zero.

	 \param name A unique name or NULL
	 \param data A pointer to the memory location of the JPEG image
	 */
	ImageJPEG(const char *name, const unsigned char *data, int size) : fltk3::ImageRGB(0,0,0) 
	{
		/*
		unsigned char *buf;
		unsigned char *ptr;	// Pointer to pixel values
		int		format;		// Format of PNM file

		if ( size <= 0 ) return;
		buf = (unsigned char *)data;
		
		njInit();
		if (njDecode(buf, size)) {
			njDone();
			return;
		}

		format = njIsColor() ? 6 : 5;
		w(njGetWidth());
		h(njGetHeight());

		// Allocate memory...
		if ( format == 5) d(1);
		else d(3);

		if (((size_t)w()) * h() * d() > max_size() ) {
			njDone();
			w(0); h(0); d(0);
			return;
		}
		array = new uchar[w() * h() * d()];
		alloc_array = 1;
		ptr = (unsigned char *)array;
		memcpy(ptr, njGetImage(), w() * h() * d());
		*/

		/*
		// Read the image file...
		int linesize = w() * d();
		unsigned char *p = njGetImage();
		for (y = 0; y < h(); y ++) {
			ptr = (uchar *)array + linesize;
			memcpy(ptr, p+y*linesize, linesize);
		}
		*/
		
		// njDone();

		unsigned char *p, *ptr;	// Pointer to pixel values
		int width, height, ac;
		p = jpgd::decompress_jpeg_image_from_memory(data, size, &width, &height, &ac, 3);
		if ( p == NULL ) return;

		w(width); h(height); d(ac);
		array = new uchar[w() * h() * d()];
		alloc_array = 1;
		ptr = (unsigned char *)array;
		memcpy(ptr, p, w() * h() * d());

		free(p);
		
		if (w() && h() && name) {
			fltk3::SharedImage *si = new fltk3::SharedImage(name, this);
			si->add();
		}
	}
};

}

#endif

//
// End of "$Id: ImageJPEG.h 9300 2012-03-23 20:36:00Z ianmacarthur $".
//
