//
// "$Id: win32_set_fonts.cxx 9330 2012-04-09 08:53:49Z manolo $"
//
// WIN32 font utilities for the Fast Light Tool Kit (FLTK).
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
// License along with this7 library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#ifdef WIN32

// This function fills in the FLTK font table with all the fonts that
// are found on the X server.  It tries to place the fonts into families
// and to sort them so the first 4 in a family are normal, bold, italic,
// and bold italic.
#include "utf8.h"
#ifdef __CYGWIN__
# include <wchar.h>
#endif

// Bug: older versions calculated the value for *ap as a side effect of
// making the name, and then forgot about it. To avoid having to change
// the header files I decided to store this value in the last character
// of the font name array.
#define ENDOFBUFFER 127 // sizeof(fltk3::Font.fontname)-1

// turn a stored font name into a pretty name:
const char* fltk3::get_font_name(fltk3::Font fnum, int* ap)
{
	Fl_Fontdesc *f = fltk3::fonts + fnum;
	if (!f->fontname[0]) {
		const char* p = f->name;
		if (!p || !*p) {
			if (ap) *ap = 0;
			return "";
		}
		int type;
		switch (*p) {
		case 'B':
			type = fltk3::BOLD;
			break;
		case 'I':
			type = fltk3::ITALIC;
			break;
		case 'P':
			type = fltk3::BOLD | fltk3::ITALIC;
			break;
		default:
			type = 0;
			break;
		}
		strlcpy(f->fontname, p+1, ENDOFBUFFER);
		if (type & fltk3::BOLD) strlcat(f->fontname, " bold", ENDOFBUFFER);
		if (type & fltk3::ITALIC) strlcat(f->fontname, " italic", ENDOFBUFFER);
		f->fontname[ENDOFBUFFER] = (char)type;
	}
	if (ap) *ap = f->fontname[ENDOFBUFFER];
	return f->fontname;
}

static int fl_free_font = fltk3::FREE_FONT;

static int CALLBACK
enumcbw(CONST LOGFONTW    *lpelf,
        CONST TEXTMETRICW * /*lpntm*/,
        DWORD            /*FontType*/,
        LPARAM           p)
{
	if (!p && lpelf->lfCharSet != ANSI_CHARSET) return 1;
	char *n = NULL;
	size_t l = wcslen(lpelf->lfFaceName);
	unsigned dstlen = fltk3::utf8fromwc(n, 0, (xchar*)lpelf->lfFaceName, (unsigned)l) + 1; // measure the string
	n = (char*) malloc(dstlen);
//n[fl_unicode2utf((xchar*)lpelf->lfFaceName, l, n)] = 0;
	dstlen = fltk3::utf8fromwc(n, dstlen, (xchar*)lpelf->lfFaceName, (unsigned)l); // convert the string
	n[dstlen] = 0;
	for (int i=0; i<fltk3::FREE_FONT; i++) // skip if one of our built-in fonts
		if (!strcmp(fltk3::get_font_name((fltk3::Font)i),n)) {
			free(n);
			return 1;
		}
	char buffer[LF_FACESIZE + 1];
	strcpy(buffer+1, n);
	buffer[0] = ' ';
	fltk3::set_font((fltk3::Font)(fl_free_font++), strdup(buffer));
	if (lpelf->lfWeight <= 400)
		buffer[0] = 'B', fltk3::set_font((fltk3::Font)(fl_free_font++), strdup(buffer));
	buffer[0] = 'I';
	fltk3::set_font((fltk3::Font)(fl_free_font++), strdup(buffer));
	if (lpelf->lfWeight <= 400)
		buffer[0] = 'P', fltk3::set_font((fltk3::Font)(fl_free_font++), strdup(buffer));
	free(n);
	return 1;
} /* enumcbw */

fltk3::Font fltk3::set_fonts(const char* xstarname)
{
	if (fl_free_font == fltk3::FREE_FONT) {// if not already been called
		if (!fl_gc) fl_GetDC(0);

		EnumFontFamiliesW(fl_gc, NULL, (FONTENUMPROCW)enumcbw, xstarname != 0);

	}
	return (fltk3::Font)fl_free_font;
}


static int nbSize;
static int cyPerInch;
static int sizes[128];
static int CALLBACK

EnumSizeCbW(CONST LOGFONTW    * /*lpelf*/,
            CONST TEXTMETRICW *lpntm,
            DWORD            fontType,
            LPARAM           /*p*/)
{
	if ((fontType & RASTER_FONTTYPE) == 0) {
		sizes[0] = 0;
		nbSize = 1;

		// Scalable font
		return 0;
	}

	int add = lpntm->tmHeight - lpntm->tmInternalLeading;
	add = MulDiv(add, 72, cyPerInch);

	int start = 0;
	while ((start < nbSize) && (sizes[start] < add)) {
		start++;
	}

	if ((start < nbSize) && (sizes[start] == add)) {
		return 1;
	}

	for (int i=nbSize; i>start; i--) sizes[i] = sizes[i - 1];

	sizes[start] = add;
	nbSize++;

	// Stop enum if buffer overflow
	return nbSize < 128;
}


int
fltk3::get_font_sizes(fltk3::Font fnum, int*& sizep)
{
	nbSize = 0;
	Fl_Fontdesc *s = fltk3::fonts+fnum;
	if (!s->name) s = fltk3::fonts; // empty slot in table, use entry 0

	if (!fl_gc) fl_GetDC(0);
	cyPerInch = GetDeviceCaps(fl_gc, LOGPIXELSY);
	if (cyPerInch < 1) cyPerInch = 1;

//  int l = fltk3::utf_nb_char((unsigned char*)s->name+1, strlen(s->name+1));
//  unsigned short *b = (unsigned short*) malloc((l + 1) * sizeof(short));
//  fl_utf2unicode((unsigned char*)s->name+1, l, (xchar*)b);
	const char *nm = (const char*)s->name+1;
	size_t len = strlen(s->name+1);
	unsigned l = fltk3::utf8toUtf16(nm, len, NULL, 0); // Pass NULL to query length required
	unsigned short *b = (unsigned short*) malloc((l + 1) * sizeof(short));
	l = fltk3::utf8toUtf16(nm, (unsigned)len, b, (l+1)); // Now do the conversion
	b[l] = 0;
	EnumFontFamiliesW(fl_gc, (WCHAR*)b, (FONTENUMPROCW)EnumSizeCbW, 0);
	free(b);

	sizep = sizes;
	return nbSize;
}

#endif // #ifdef WIN32

//
// End of "$Id: win32_set_fonts.cxx 9330 2012-04-09 08:53:49Z manolo $".
//
