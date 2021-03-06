/*
 * "$Id: filename.h 9705 2012-10-19 11:40:56Z manolo $"
 *
 * Filename header file for the Fast Light Tool Kit (FLTK).
 *
 * Copyright 1998-2010 by Bill Spitzak and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems on the following page:
 *
 *     http://www.fltk.org/str.php
 */
/** \file
 File names and URI utility functions.
 */

/* Xcode on OS X includes files by recursing down into directories.
 * This code catches the cycle and directly includes the required file.
 */
#ifdef fl_dirent_h_cyclic_include
#  include "/usr/include/dirent.h"
#endif

#ifndef FLtk3_FILENAME_H
#define FLtk3_FILENAME_H

#if !defined (__cplusplus)
#error the file "filename.h" is now written in "C++" can no longer be included from "C"
#endif

#include "Export.h"

/** \addtogroup filenames File names and URI utility functions
 File names and URI functions defined in <fltk3/filename.h>
 @{ */
#ifdef _MAX_PATH
const unsigned int FLTK3_PATH_MAX = _MAX_PATH;
#elif defined(MAX_PATH)
const unsigned int FLTK3_PATH_MAX = MAX_PATH;
#else
const unsigned int FLTK3_PATH_MAX = 2048; /**< all path buffers should use this length */
#endif

namespace fltk3
{

/** Gets the file name from a path.
 Similar to basename(3), exceptions shown below.
 \code
 #include <fltk3/filename.h>
 [..]
 const char *out;
 out = fltk3::filename_name("/usr/lib");     // out="lib"
 out = fltk3::filename_name("/usr/");        // out=""      (basename(3) returns "usr" instead)
 out = fltk3::filename_name("/usr");         // out="usr"
 out = fltk3::filename_name("/");            // out=""      (basename(3) returns "/" instead)
 out = fltk3::filename_name(".");            // out="."
 out = fltk3::filename_name("..");           // out=".."
 \endcode
 \return a pointer to the char after the last slash, or to \p filename if there is none.
 */
FLTK3_EXPORT const char *filename_name(const char * filename);
FLTK3_EXPORT const char *filename_ext(const char *buf);
FLTK3_EXPORT char *filename_setext(char *to, int tolen, const char *ext);
FLTK3_EXPORT int filename_expand(char *to, int tolen, const char *from);
FLTK3_EXPORT int filename_absolute(char *to, int tolen, const char *from);
FLTK3_EXPORT int filename_relative(char *to, int tolen, const char *from);
FLTK3_EXPORT int filename_match(const char *name, const char *pattern);
FLTK3_EXPORT int filename_isdir(const char *name);

inline char *filename_setext(char *to, const char *ext)
{
	return filename_setext(to, FLTK3_PATH_MAX, ext);
}
inline int filename_expand(char *to, const char *from)
{
	return filename_expand(to, FLTK3_PATH_MAX, from);
}
inline int filename_absolute(char *to, const char *from)
{
	return filename_absolute(to, FLTK3_PATH_MAX, from);
}
FLTK3_EXPORT int filename_relative(char *to, int tolen, const char *from, const char *cwd);
inline int filename_relative(char *to, const char *from)
{
	return filename_relative(to, FLTK3_PATH_MAX, from);
}

}

#  if defined(WIN32) && !defined(__MINGW32__) && !defined(__CYGWIN__) && !defined(__WATCOMC__)

struct dirent {
	char d_name[1];
};

#  elif defined(__WATCOMC__)
#    include <sys/types.h>
#    include <direct.h>

#  else
/*
 * WARNING: on some systems (very few nowadays?) <dirent.h> may not exist.
 * The correct information is in one of these files:
 *
 *     #include <sys/ndir.h>
 *     #include <sys/dir.h>
 *     #include <ndir.h>
 *
 * plus you must do the following #define:
 *
 *     #define dirent direct
 *
 * It would be best to create a <dirent.h> file that does this...
 */
#    include <sys/types.h>
#    define fl_dirent_h_cyclic_include
#    include <dirent.h>
#    undef fl_dirent_h_cyclic_include
#  endif

namespace fltk3
{

#if !defined(FLTK3_DOXYGEN)
FLTK3_EXPORT int alphasort(struct dirent **, struct dirent **);
FLTK3_EXPORT int casealphasort(struct dirent **, struct dirent **);
FLTK3_EXPORT int casenumericsort(struct dirent **, struct dirent **);
FLTK3_EXPORT int numericsort(struct dirent **, struct dirent **);
#endif

typedef int (FileSortF)(struct dirent **, struct dirent **); /**< File sorting function. \see fltk3::filename_list() */

/*
 * Portable "scandir" function.  Ugly but necessary...
 */

FLTK3_EXPORT int filename_list(const char *d, struct dirent ***l,
                               fltk3::FileSortF *s = fltk3::numericsort);
FLTK3_EXPORT void filename_free_list(struct dirent ***l, int n);

/*
 * Generic function to open a Uniform Resource Identifier (URI) using a
 * system-defined program (added in FLTK 1.1.8)
 */

FLTK3_EXPORT int open_uri(const char *uri, char *msg = (char *)0,
                          int msglen = 0);

FLTK3_EXPORT void decode_uri(char *uri);

#    ifndef FLTK3_DOXYGEN
/*
 * fltk3::_filename_isdir_quick() is a private function that checks for a
 * trailing slash and assumes that the passed name is a directory if
 * it finds one.  This function is used by fltk3::FileBrowser and
 * fltk3::FileChooser to avoid extra stat() calls, but is not supported
 * outside of FLTK...
 */
int _filename_isdir_quick(const char *name);
#    endif

}

#endif /* FLtk3_FILENAME_H */

/** @} */

/*
 * End of "$Id: filename.h 9705 2012-10-19 11:40:56Z manolo $".
 */
