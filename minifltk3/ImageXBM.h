//
// "$Id: ImageXBM.h 9300 2012-03-23 20:36:00Z ianmacarthur $"
//
// XBM image header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::ImageXBM class . */

#ifndef Fltk3_ImageXBM_H
#define Fltk3_ImageXBM_H

#include "run.h"
#include "Bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include "utf8.h"
#include "src/flstring.h"

namespace fltk3
{

/**
 The fltk3::ImageXBM class supports loading, caching,
 and drawing of X Bitmap (XBM) bitmap files.
 */
class FLTK3_EXPORT ImageXBM : public fltk3::Bitmap
{
public:
	ImageXBM(const char* filename) : fltk3::Bitmap((const char *)0,0,0) {
		FILE	*f;
		uchar	*ptr;

		if ((f = fltk3::fopen(filename, "rb")) == NULL) return;

		char buffer[1024];
		char junk[1024];
		int wh[2]; // width and height
		int i;
		for (i = 0; i<2; i++) {
			for (;;) {
				if (!fgets(buffer,1024,f)) {
					fclose(f);
					return;
				}
				int r = sscanf(buffer,"#define %s %d",junk,&wh[i]);
				if (r >= 2) break;
			}
		}

		// skip to data array:
		for (;;) {
			if (!fgets(buffer,1024,f)) {
				fclose(f);
				return;
			}
			if (!strncmp(buffer,"static ",7)) break;
		}

		// Allocate memory...
		w(wh[0]);
		h(wh[1]);

		int n = ((wh[0]+7)/8)*wh[1];
		array = new uchar[n];

		// read the data:
		for (i = 0, ptr = (uchar *)array; i < n;) {
			if (!fgets(buffer,1024,f)) {
				fclose(f);
				return;
			}
			const char *a = buffer;
			while (*a && i<n) {
				unsigned int t;
				if (sscanf(a," 0x%x",&t)>0) {
					*ptr++ = (uchar)t;
					i ++;
				}
				while (*a && *a++ != ',');
			}
		}

		fclose(f);
	}
};

}

#endif // !Fltk3_ImageXBM_H

//
// End of "$Id: ImageXBM.h 9300 2012-03-23 20:36:00Z ianmacarthur $".
//
