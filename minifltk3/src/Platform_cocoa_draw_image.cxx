//
// "$Id: cocoa_draw_image.cxx 9408 2012-04-29 19:10:11Z matt $"
//
// MacOS image drawing code for the Fast Light Tool Kit (FLTK).
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

////////////////////////////////////////////////////////////////

#ifdef __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_MAC

#include <config.h>
#include "run.h"
#include "draw.h"
#include "Printer.h"
#include "x.h"

#define MAXBUFFER 0x40000 // 256k

static void dataReleaseCB(void *info, const void *data, size_t size)
{
	delete[] (uchar *)data;
}

/*
 * draw an image based on the input parameters
 *
 * buf:       image source data
 * X, Y:      position (in buffer?!)
 * W, H:      size of picture (in pixel?)
 * delta:     distance from pixel to pixel in buf in bytes
 * linedelta: distance from line to line in buf in bytes
 * mono:      if set, pixel is one byte - if zero, pixel is 3 byte
 * cb:        callback to copy image data into (RGB?) buffer
 *   buf:       pointer to first byte in image source
 *   x, y:      position in buffer
 *   w:         width (in bytes?)
 *   dst:       destination buffer
 * userdata:  ?
 */
static void innards(const uchar *buf, int X, int Y, int W, int H,
                    int delta, int linedelta, int mono,
                    fltk3::DrawImageCb cb, void* userdata)
{
	if (!linedelta) linedelta = W*delta;

	const void *array = buf;
	uchar *tmpBuf = 0;
	if (cb || fltk3::SurfaceDevice::surface() != fltk3::DisplayDevice::display_device()) {
		tmpBuf = new uchar[ H*W*delta ];
		if (cb) {
			for (int i=0; i<H; i++) {
				cb(userdata, 0, i, W, tmpBuf+i*W*delta);
			}
		} else {
			uchar *p = tmpBuf;
			for (int i=0; i<H; i++) {
				memcpy(p, buf+i*linedelta, W*delta);
				p += W*delta;
			}
		}
		array = (void*)tmpBuf;
		linedelta = W*delta;
	}
	// create an image context
	CGColorSpaceRef   lut = 0;
	if (delta<=2)
		lut = CGColorSpaceCreateDeviceGray();
	else
		lut = CGColorSpaceCreateDeviceRGB();
	// a release callback is necessary when the fl_gc is a print context because the image data
	// must be kept until the page is closed. Thus tmpBuf can't be deleted here. It's too early.
	CGDataProviderRef src = CGDataProviderCreateWithData( 0L, array, linedelta*H,
	                        tmpBuf ? dataReleaseCB : NULL
	                                                    );
	CGImageRef        img = CGImageCreate( W, H, 8, 8*delta, linedelta,
	                                       lut, delta&1?kCGImageAlphaNone:kCGImageAlphaNoneSkipLast,
	                                       //lut, delta&1?kCGImageAlphaNone:kCGImageAlphaLast,
	                                       src, 0L, false, kCGRenderingIntentDefault);
	// draw the image into the destination context
	if (img) {
		//CGRect rect = { { X, Y }, { W, H } };
		CGRect rect = { { (CGFloat)(X), (CGFloat)(Y) }, { (CGFloat)(W), (CGFloat)(H) } };
		Fl_X::q_begin_image(rect, 0, 0, W, H);
		CGContextDrawImage(fl_gc, rect, img);
		Fl_X::q_end_image();
		// release all allocated resources
		CGImageRelease(img);
	}
	CGColorSpaceRelease(lut);
	CGDataProviderRelease(src);
	if (img) return; // else fall through to slow mode
	// following the very save (and very slow) way to write the image into the give port
	CGContextSetShouldAntialias(fl_gc, false);
	if ( cb ) {
		uchar *tmpBuf = new uchar[ W*4 ];
		for ( int i=0; i<H; i++ ) {
			uchar *src = tmpBuf;
			cb( userdata, 0, i, W, tmpBuf );
			for ( int j=0; j<W; j++ ) {
				if ( mono ) {
					fltk3::color( src[0], src[0], src[0] );
				} else {
					fltk3::color( src[0], src[1], src[2] );
				}
				CGContextMoveToPoint(fl_gc, X+j, Y+i);
				CGContextAddLineToPoint(fl_gc, X+j, Y+i);
				CGContextStrokePath(fl_gc);
				src+=delta;
			}
		}
		delete[] tmpBuf;
	} else {
		for ( int i=0; i<H; i++ ) {
			const uchar *src = buf+i*linedelta;
			for ( int j=0; j<W; j++ ) {
				if ( mono )
					fltk3::color( src[0], src[0], src[0] );
				else
					fltk3::color( src[0], src[1], src[2] );
				CGContextMoveToPoint(fl_gc, X+j, Y+i);
				CGContextAddLineToPoint(fl_gc, X+j, Y+i);
				CGContextStrokePath(fl_gc);
				src += delta;
			}
		}
	}
	CGContextSetShouldAntialias(fl_gc, true);
}

void fltk3::QuartzGraphicsDriver::draw_image(const uchar* buf, int x, int y, int w, int h, int d, int l)
{
	innards(buf,x+origin_x(),y+origin_y(),w,h,d,l,(d<3&&d>-3),0,0);
}
void fltk3::QuartzGraphicsDriver::draw_image(fltk3::DrawImageCb cb, void* data,
int x, int y, int w, int h,int d)
{
	innards(0,x+origin_x(),y+origin_y(),w,h,d,0,(d<3&&d>-3),cb,data);
}
void fltk3::QuartzGraphicsDriver::draw_image_mono(const uchar* buf, int x, int y, int w, int h, int d, int l)
{
	innards(buf,x+origin_x(),y+origin_y(),w,h,d,l,1,0,0);
}
void fltk3::QuartzGraphicsDriver::draw_image_mono(fltk3::DrawImageCb cb, void* data,
int x, int y, int w, int h,int d)
{
	innards(0,x+origin_x(),y+origin_y(),w,h,d,0,1,cb,data);
}

void fltk3::rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b)
{
	fltk3::color(r,g,b);
	fltk3::rectf(x,y,w,h);
}

#endif // #if TARGET_OS_MAC
#endif // #ifdef __APPLE__

//
// End of "$Id: cocoa_draw_image.cxx 9408 2012-04-29 19:10:11Z matt $".
//
