//
// "$Id: ImageXPM.h 9300 2012-03-23 20:36:00Z ianmacarthur $"
//
// XPM image header file for the Fast Light Tool Kit (FLTK).
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

 fltk3::ImageXPM class . */

#ifndef Fltk3_ImageXPM_H
#define Fltk3_ImageXPM_H

#include "run.h"
#include "Pixmap.h"
#include <stdio.h>
#include <stdlib.h>
#include "utf8.h"
#include "src/flstring.h"

namespace fltk3
{
/**
 The fltk3::ImageXPM class supports loading, caching,
 and drawing of X Pixmap (XPM) images, including transparency.
 */
class FLTK3_EXPORT ImageXPM : public fltk3::Pixmap
{
public:
#define ImageXPM_MAXSIZE 2048
#define ImageXPM_INITIALLINES 256
	ImageXPM(const char* filename) : fltk3::Pixmap((char *const*)0) 
	{
		FILE *f;

		if ((f = fltk3::fopen(filename, "rb")) == NULL) return;

		// read all the c-strings out of the file:
		char** new_data = new char *[ImageXPM_INITIALLINES];
		char** temp_data;
		int malloc_size = ImageXPM_INITIALLINES;
		char buffer[ImageXPM_MAXSIZE+20];
		int i = 0;
		int W,H,ncolors,chars_per_pixel;
		while (fgets(buffer,ImageXPM_MAXSIZE+20,f)) {
			if (buffer[0] != '\"') continue;
			char *myp = buffer;
			char *q = buffer+1;
			while (*q != '\"' && myp < buffer+ImageXPM_MAXSIZE) {
				if (*q == '\\') switch (*++q) {
					case '\r':
					case '\n':
						if (!fgets(q, (int)(buffer+ImageXPM_MAXSIZE+20-q), f)) {
							/* no problem if we hit EOF */
						} break;
					case 0:
						break;
					case 'x': {
						q++;
						int n = 0;
						for (int x = 0; x < 3; x++) {
							int xd = hexdigit(*q);
							if (xd > 15) break;
							n = (n<<4)+xd;
							q++;
						}
						*myp++ = n;
					}
					break;
					default: {
						int c = *q++;
						if (c>='0' && c<='7') {
							c -= '0';
							for (int x=0; x<2; x++) {
								int xd = hexdigit(*q);
								if (xd>7) break;
								c = (c<<3)+xd;
								q++;
							}
						}
						*myp++ = c;
					}
					break;
					}
				else {
					*myp++ = *q++;
				}
			}
			*myp++ = 0;
			if (i >= malloc_size) {
				temp_data = new char *[malloc_size + ImageXPM_INITIALLINES];
				memcpy(temp_data, new_data, sizeof(char *) * malloc_size);
				delete[] new_data;
				new_data = temp_data;
				malloc_size += ImageXPM_INITIALLINES;
			}
			// first line has 4 ints: width, height, ncolors, chars_per_pixel
			// followed by color segment: 
			//   if ncolors < 0 this is FLTK (non standard) compressed colormap - all colors coded in single line of 4*ncolors bytes
			//   otherwise - ncolor lines of at least chars_per_pixel bytes
			// followed by pic segment: H lines of at least chars_per_pixel*W bytes
			// next line: would have loved to use measure_pixmap, but it doesn't return all the data!
			if ((!i) && (sscanf(buffer,"%d%d%d%d", &W, &H, &ncolors, &chars_per_pixel) < 4)) goto bad_data; // first line
			else if ((i > (ncolors<0?1:ncolors)) && (myp-buffer-1<W*chars_per_pixel)) goto bad_data; // pic segment
			else if (myp-buffer-1<(ncolors<0?-ncolors*4:chars_per_pixel)) goto bad_data; // color segment
			new_data[i] = new char[myp-buffer+1];
			memcpy(new_data[i], buffer,myp-buffer);
			new_data[i][myp-buffer] = 0;
			i++;
		}

		fclose(f);
		f = NULL;
		if ((!i) || (i<1+(ncolors<0?1:ncolors)+H)) goto bad_data;
		data((const char **)new_data, i);
		alloc_data = 1;

		measure();
		return;
		// dealloc and close as needed when bad data was found
bad_data:
		while (i > 0) delete[] new_data[--i];
		delete[] new_data;
		if (f) fclose(f);
	}

protected:
	// 'hexdigit()' - Convert a hex digit to an integer.
	int hexdigit(int x) {	// I - Hex digit...
		if (isdigit(x)) return x-'0';
		if (isupper(x)) return x-'A'+10;
		if (islower(x)) return x-'a'+10;
		return 20;
	}
};

}

#endif // !fltk3::ImageXPM

//
// End of "$Id: ImageXPM.h 9300 2012-03-23 20:36:00Z ianmacarthur $".
//
