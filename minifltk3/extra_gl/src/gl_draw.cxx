//
// "$Id: gl_draw.cxx 8991 2011-08-22 15:21:44Z matt $"
//
// OpenGL drawing support routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2011 by Bill Spitzak and others.
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

// Functions from <fltk3/gl.h>
// See also fltk3::GLWindow and gl_start.cxx

#include "src/flstring.h"
#if HAVE_GL || defined(FLTK3_DOXYGEN)

#include "run.h"
#include "extra_gl/gl.h"
#include "x.h"
#include "draw.h"
#include "Device.h"
#include "GLChoice.h"
#include "src/font.h"
#include "utf8.h"

#if !defined(WIN32) && !defined(__APPLE__)
#include "Xutf8.h"
#endif

#if USE_XFT
//extern XFontStruct* fl_xxfont();
#endif // USE_XFT

/** Returns the current font's height */
int   gl_height() {return fltk3::height();}
/** Returns the current font's descent */
int   gl_descent() {return fltk3::descent();}
/** Returns the width of the string in the current fnt */
double gl_width(const char* s) {return fltk3::width(s);}
/** Returns the width of n characters of the string in the current font */
double gl_width(const char* s, int n) {return fltk3::width(s,n);}
/** Returns the width of the character in the current font */
double gl_width(uchar c) {return fltk3::width(c);}

static Fl_Font_Descriptor *gl_fontsize;

#define GL_DRAW_USES_TEXTURES  (defined(__APPLE__) && !__ppc__) // 1 only for non-PPC OSX
#ifndef __APPLE__
#  define USE_OksiD_style_GL_font_selection 1  // Most hosts except OSX
#else
#  undef USE_OksiD_style_GL_font_selection  // OSX
#endif

#if USE_XFT
#  undef USE_OksiD_style_GL_font_selection  // turn this off for XFT also
#endif

/**
  Sets the current OpenGL font to the same font as calling fltk3::font()
  */
void  gl_font(int fontid, int size) {
  fltk3::font(fontid, size);
  Fl_Font_Descriptor *fontsize = fltk3::graphics_driver->font_descriptor();
#if !GL_DRAW_USES_TEXTURES
  if (!fontsize->listbase) {

#ifdef  USE_OksiD_style_GL_font_selection
    fontsize->listbase = glGenLists(0x10000);
#else // Fltk-1.1.8 style GL font selection

#if defined (USE_X11) // X-windows options follow, either XFT or "plain" X
// FIXME:  warning Ideally, for XFT, we really need a glXUseXftFont implementation here...
// FIXME:  warning GL font selection is basically wrong here
/* OksiD had a fairly sophisticated scheme for storing multiple X fonts in a XUtf8FontStruct,
 * then sorting through them at draw time (for normal X rendering) to find which one can
 * render the current glyph... But for now, just use the first font in the list for GL...
 */
    XFontStruct *font = fl_xfont;
    int base = font->min_char_or_byte2;
    int count = font->max_char_or_byte2-base+1;
    fontsize->listbase = glGenLists(256);
    glXUseXFont(font->fid, base, count, fontsize->listbase+base);
# elif defined(WIN32)
    // this is unused because USE_OksiD_style_GL_font_selection == 1
    int base = fontsize->metr.tmFirstChar;
    int count = fontsize->metr.tmLastChar-base+1;
    HFONT oldFid = (HFONT)SelectObject(fl_gc, fontsize->fid);
    fontsize->listbase = glGenLists(256);
    wglUseFontBitmaps(fl_gc, base, count, fontsize->listbase+base);
    SelectObject(fl_gc, oldFid);
# elif defined(__APPLE_QUARTZ__)
//AGL is not supported for use in 64-bit applications:
//http://developer.apple.com/mac/library/documentation/Carbon/Conceptual/Carbon64BitGuide/OtherAPIChanges/OtherAPIChanges.html
    short font, face, size;
    uchar fn[256];
    fn[0]=strlen(fontsize->q_name);
    strcpy((char*)(fn+1), fontsize->q_name);
    GetFNum(fn, &font);
    face = 0;
    size = fontsize->size;
    fontsize->listbase = glGenLists(256);
	aglUseFont(aglGetCurrentContext(), font, face,
               size, 0, 256, fontsize->listbase);
# else 
#   error unsupported platform
# endif

#endif // USE_OksiD_style_GL_font_selection
  }
  glListBase(fontsize->listbase);
#endif // !GL_DRAW_USES_TEXTURES
  gl_fontsize = fontsize;
}

#ifndef __APPLE__
static void get_list(int r) {
  gl_fontsize->glok[r] = 1;
#if defined(USE_X11)
# if USE_XFT
// FIXME
# else
  unsigned int ii = r * 0x400;
  for (int i = 0; i < 0x400; i++) {
    XFontStruct *font = NULL;
    unsigned short id;
    XGetUtf8FontAndGlyph(gl_fontsize->font, ii, &font, &id);
    if (font) glXUseXFont(font->fid, id, 1, gl_fontsize->listbase+ii);
    ii++;
   }
# endif
#elif defined(WIN32)
  unsigned int ii = r * 0x400;
  HFONT oldFid = (HFONT)SelectObject(fl_gc, gl_fontsize->fid);
  wglUseFontBitmapsW(fl_gc, ii, ii + 0x03ff, gl_fontsize->listbase+ii);
  SelectObject(fl_gc, oldFid);
#elif defined(__APPLE_QUARTZ__)
// handled by textures
#else
#  error unsupported platform
#endif
} // get_list
#endif

void gl_remove_displaylist_fonts()
{
# if HAVE_GL

  // clear variables used mostly in fltk3::font
  fltk3::graphics_driver->font(0, 0);

  for (int j = 0 ; j < fltk3::FREE_FONT ; ++j)
  {
    Fl_Font_Descriptor* past = 0;
    Fl_Fontdesc* s    = fltk3::fonts + j ;
    Fl_Font_Descriptor* f    = s->first;
    while (f != 0) {
      if(f->listbase) {
        if(f == s->first) {
          s->first = f->next;
        }
        else {
          past->next = f->next;
        }

        // It would be nice if this next line was in a desctructor somewhere
        glDeleteLists(f->listbase, 256);

        Fl_Font_Descriptor* tmp = f;
        f = f->next;
        delete tmp;
      }
      else {
        past = f;
        f = f->next;
      }
    }
  }

#endif
}

#if GL_DRAW_USES_TEXTURES
static void gl_draw_textures(const char* str, int n);
#endif

/**
  Draws an array of n characters of the string in the current font
  at the current position.
 \see On the Mac OS X platform, see gl_texture_pile_height(int)
  */
void gl_draw(const char* str, int n) {
#ifdef __APPLE__  
  
#if GL_DRAW_USES_TEXTURES
  gl_draw_textures(str, n);
#else
  glCallLists(n, GL_UNSIGNED_BYTE, str);
#endif
  
#else
  static xchar *buf = NULL;
  static int l = 0;
  int wn = fltk3::utf8toUtf16(str, n, (unsigned short*)buf, l);
  if(wn >= l) {
    buf = (xchar*) realloc(buf, sizeof(xchar) * (wn + 1));
    l = wn + 1;
    wn = fltk3::utf8toUtf16(str, n, (unsigned short*)buf, l);
  }
  n = wn;

  int i;
  for (i = 0; i < n; i++) {
    unsigned int r;
    r = (str[i] & 0xFC00) >> 10;
    if (!gl_fontsize->glok[r]) get_list(r);
  }
  glCallLists(n, GL_UNSIGNED_SHORT, buf);
#endif
}

/**
  Draws n characters of the string in the current font at the given position 
 \see On the Mac OS X platform, see gl_texture_pile_height(int)
  */
void gl_draw(const char* str, int n, int x, int y) {
  glRasterPos2i(x, y);
  gl_draw(str, n);
}

/**
  Draws n characters of the string in the current font at the given position 
 \see On the Mac OS X platform, see gl_texture_pile_height(int)
  */
void gl_draw(const char* str, int n, float x, float y) {
  glRasterPos2f(x, y);
  gl_draw(str, n);
}

/**
  Draws a nul-terminated string in the current font at the current position
 \see On the Mac OS X platform, see gl_texture_pile_height(int)
  */
void gl_draw(const char* str) {
  gl_draw(str, strlen(str));
}

/**
  Draws a nul-terminated string in the current font at the given position 
 \see On the Mac OS X platform, see gl_texture_pile_height(int)
  */
void gl_draw(const char* str, int x, int y) {
  gl_draw(str, strlen(str), x, y);
}

/**
  Draws a nul-terminated string in the current font at the given position 
 \see On the Mac OS X platform, see gl_texture_pile_height(int)
  */
void gl_draw(const char* str, float x, float y) {
  gl_draw(str, strlen(str), x, y);
}

static void gl_draw_invert(const char* str, int n, int x, int y) {
  glRasterPos2i(x, -y);
  gl_draw(str, n);
}

/**
  Draws a string formatted into a box, with newlines and tabs expanded,
  other control characters changed to ^X. and aligned with the edges or
  center. Exactly the same output as fltk3::draw().
  */
void gl_draw(
  const char* str, 	// the (multi-line) string
  int x, int y, int w, int h, 	// bounding box
  fltk3::Align align) {
  fltk3::draw(str, x, -y-h, w, h, align, gl_draw_invert);
}

/** Measure how wide and tall the string will be when drawn by the gl_draw() function */
void gl_measure(const char* str, int& x, int& y) {fltk3::measure(str,x,y);}

/**
  Outlines the given rectangle with the current color.
  If fltk3::GLWindow::ortho() has been called, then the rectangle will
  exactly fill the given pixel rectangle.
  */
void gl_rect(int x, int y, int w, int h) {
  if (w < 0) {w = -w; x = x-w;}
  if (h < 0) {h = -h; y = y-h;}
  glBegin(GL_LINE_STRIP);
  glVertex2i(x+w-1, y+h-1);
  glVertex2i(x+w-1, y);
  glVertex2i(x, y);
  glVertex2i(x, y+h-1);
  glVertex2i(x+w, y+h-1);
  glEnd();
}

#if HAVE_GL_OVERLAY
extern uchar fl_overlay;
extern int fl_overlay_depth;
#endif

/**
  Sets the curent OpenGL color to an FLTK color.

  For color-index modes it will use fl_xpixel(c), which is only
  right if the window uses the default colormap!
  */
void gl_color(fltk3::Color i) {
#if HAVE_GL_OVERLAY
#if defined(WIN32)
  if (fl_overlay && fl_overlay_depth) {
    if (fl_overlay_depth < 8) {
      // only black & white produce the expected colors.  This could
      // be improved by fixing the colormap set in Fl_Gl_Overlay.cxx
      int size = 1<<fl_overlay_depth;
      if (!i) glIndexi(size-2);
      else if ((int)i >= size-2) glIndexi(size-1);
      else glIndexi(i);
    } else {
      glIndexi(i ? i : fltk3::GRAY_RAMP);
    }
    return;
  }
#else
  if (fl_overlay) {glIndexi(int(fl_xpixel(i))); return;}
#endif
#endif
  uchar red, green, blue;
  fltk3::get_color(i, red, green, blue);
  glColor3ub(red, green, blue);
}

void gl_draw_image(const uchar* b, int x, int y, int w, int h, int d, int ld) {
  if (!ld) ld = w*d;
  glPixelStorei(GL_UNPACK_ROW_LENGTH, ld/d);
  glRasterPos2i(x,y);
  glDrawPixels(w,h,d<4?GL_RGB:GL_RGBA,GL_UNSIGNED_BYTE,(const ulong*)b);
}

#if GL_DRAW_USES_TEXTURES || defined(FLTK3_DOXYGEN)

#include "extra_gl/glu.h"

// manages a fifo pile of pre-computed string textures
class gl_texture_fifo {
  friend void gl_draw_textures(const char *, int);
private:
  typedef struct { // information for a pre-computed texture
    GLuint texName; // its name
    char *utf8; //its text
    Fl_Font_Descriptor *fdesc; // its font
    int width; // its width
    int height; // its height
  } data;
  data *fifo; // array of pile elements
  int size_; // pile height
  int current; // the oldest texture to have entered the pile
  int last; // pile top
  int textures_generated; // true iff glGenTextures has been called
  void display_texture(int rank);
  int compute_texture(const char* str, int n);
  int already_known(const char *str, int n);
public:
  gl_texture_fifo(int max = 100); // 100 = default height of texture pile
  inline int size(void) {return size_; };
  ~gl_texture_fifo(void);
};

gl_texture_fifo::gl_texture_fifo(int max)
{
  size_ = max;
  last = current = -1;
  textures_generated = 0;
  fifo = (data*)calloc(size_, sizeof(data));
}

gl_texture_fifo::~gl_texture_fifo()
{
  for (int i = 0; i < size_; i++) {
    if (fifo[i].utf8) free(fifo[i].utf8);
    if (textures_generated) glDeleteTextures(1, &fifo[i].texName);
    }
  free(fifo);
}

// displays a pre-computed texture on the GL scene
void gl_texture_fifo::display_texture(int rank)
{
  //setup matrices
  GLint matrixMode;
  glGetIntegerv (GL_MATRIX_MODE, &matrixMode);
  glMatrixMode (GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity ();
  glMatrixMode (GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity ();
  float winw = fltk3::Window::current()->w();
  float winh = fltk3::Window::current()->h();
  glScalef (2.0f / winw, 2.0f /  winh, 1.0f);
  glTranslatef (-winw / 2.0f, -winh / 2.0f, 0.0f);
  //write the texture on screen
  GLfloat pos[4];
  glGetFloatv(GL_CURRENT_RASTER_POSITION, pos);
  CGRect bounds = CGRectMake (pos[0], pos[1] - fltk3::descent(), fifo[rank].width, fifo[rank].height);
  
  // GL_COLOR_BUFFER_BIT for glBlendFunc, GL_ENABLE_BIT for glEnable / glDisable
  glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT); 
  glDisable (GL_DEPTH_TEST); // ensure text is not removed by depth buffer test.
  glEnable (GL_BLEND); // for text fading
  glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // ditto
  glEnable (GL_TEXTURE_RECTANGLE_EXT);	
  glDisable(GL_LIGHTING);
  glBindTexture (GL_TEXTURE_RECTANGLE_EXT, fifo[rank].texName);
  glBegin (GL_QUADS);
  glTexCoord2f (0.0f, 0.0f); // draw lower left in world coordinates
  glVertex2f (bounds.origin.x, bounds.origin.y);
  
  glTexCoord2f (0.0f, fifo[rank].height); // draw upper left in world coordinates
  glVertex2f (bounds.origin.x, bounds.origin.y + bounds.size.height);
  
  glTexCoord2f (fifo[rank].width, fifo[rank].height); // draw upper right in world coordinates
  glVertex2f (bounds.origin.x + bounds.size.width, bounds.origin.y + bounds.size.height);
  
  glTexCoord2f (fifo[rank].width, 0.0f); // draw lower right in world coordinates
  glVertex2f (bounds.origin.x + bounds.size.width, bounds.origin.y);
  glEnd ();
  glPopAttrib();
  
  // reset original matrices
  glPopMatrix(); // GL_MODELVIEW
  glMatrixMode (GL_PROJECTION);
  glPopMatrix();
  glMatrixMode (matrixMode);
  
  //set the raster position to end of string
  pos[0] += fifo[rank].width;
  GLdouble modelmat[16];
  glGetDoublev (GL_MODELVIEW_MATRIX, modelmat);
  GLdouble projmat[16];
  glGetDoublev (GL_PROJECTION_MATRIX, projmat);
  GLdouble objX, objY, objZ;
  GLint viewport[4];
  glGetIntegerv (GL_VIEWPORT, viewport);
  gluUnProject(pos[0], pos[1], pos[2], modelmat, projmat, viewport, &objX, &objY, &objZ);
  glRasterPos2d(objX, objY);
}

// pre-computes a string texture
int gl_texture_fifo::compute_texture(const char* str, int n)
{
  current = (current + 1) % size_;
  if (current > last) last = current;
  //write str to a bitmap just big enough  
  if ( fifo[current].utf8 ) free(fifo[current].utf8);
  fifo[current].utf8 = (char *)malloc(n + 1);
  memcpy(fifo[current].utf8, str, n);
  fifo[current].utf8[n] = 0;
  fifo[current].width = 0, fifo[current].height = 0;
  fltk3::measure(fifo[current].utf8, fifo[current].width, fifo[current].height, 0);
  CGColorSpaceRef lut = CGColorSpaceCreateDeviceRGB();
  void *base = calloc(4*fifo[current].width, fifo[current].height);
  if (base == NULL) return -1;
  fl_gc = CGBitmapContextCreate(base, fifo[current].width, fifo[current].height, 8, fifo[current].width*4, lut, kCGImageAlphaPremultipliedLast);
  CGColorSpaceRelease(lut);
  fltk3::graphics_driver->font_descriptor(gl_fontsize);
  GLfloat colors[4];
  glGetFloatv(GL_CURRENT_COLOR, colors);
  fltk3::color((uchar)(colors[0]*255), (uchar)(colors[1]*255), (uchar)(colors[2]*255));
  fltk3::draw(str, n, 0, fifo[current].height - fltk3::descent());
  //put this bitmap in a texture  
  glPushAttrib(GL_TEXTURE_BIT);
  glBindTexture (GL_TEXTURE_RECTANGLE_EXT, fifo[current].texName);
  glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_RECTANGLE_EXT, 0, GL_RGBA, fifo[current].width, fifo[current].height, 0,  GL_RGBA, GL_UNSIGNED_BYTE, base);
  glPopAttrib();
  CGContextRelease(fl_gc);
  fl_gc = NULL;
  free(base);
  fifo[current].fdesc = gl_fontsize;
  return current;
}

// returns rank of pre-computed texture for a string if it exists
int gl_texture_fifo::already_known(const char *str, int n)
{
  int rank;
  for ( rank = 0; rank <= last; rank++) {
    if ( memcmp(str, fifo[rank].utf8, n) == 0 && fifo[rank].utf8[n] == 0 &&
      fifo[rank].fdesc == gl_fontsize) return rank;
  }
  return -1;
}

static gl_texture_fifo *gl_fifo = NULL; // points to the texture pile class instance

// draws a utf8 string using pre-computed texture if available
static void gl_draw_textures(const char* str, int n) 
{
  if (! gl_fifo) gl_fifo = new gl_texture_fifo();
  if (!gl_fifo->textures_generated) {
    for (int i = 0; i < gl_fifo->size_; i++) glGenTextures (1, &(gl_fifo->fifo[i].texName));
    gl_fifo->textures_generated = 1;
  }
  int rank = gl_fifo->already_known(str, n);
  if (rank == -1) {
    rank = gl_fifo->compute_texture(str, n);
  }
  gl_fifo->display_texture(rank);
}

/** \addtogroup group_macosx
 @{ */

/**
 \brief Returns the current height of the pile of pre-computed string textures
 *
 The default value is 100
 */
int gl_texture_pile_height(void)
{
  if (! gl_fifo) gl_fifo = new gl_texture_fifo();
  return gl_fifo->size();
}

/**
 \brief Changes the height of the pile of pre-computed string textures
 *
 Strings that are often re-displayed can be processed much faster if
 this pile is set high enough to hold all of them.
 \param max Height of the texture pile
 */
void gl_texture_pile_height(int max)
{
  if (gl_fifo) delete gl_fifo;
  gl_fifo = new gl_texture_fifo(max);
}

/** @} */

#elif defined(__APPLE__)
// used only if __ppc__
int gl_texture_pile_height(void) {return 0;}
void gl_texture_pile_height(int max) {}
#endif // GL_DRAW_USES_TEXTURES
#if defined(__APPLE__)
void gl_texture_reset()
{
#if GL_DRAW_USES_TEXTURES
  if (gl_fifo) gl_texture_pile_height(gl_texture_pile_height());
#endif // GL_DRAW_USES_TEXTURES
}
#endif // __APPLE__

#endif // HAVE_GL

//
// End of "$Id: gl_draw.cxx 8991 2011-08-22 15:21:44Z matt $".
//
