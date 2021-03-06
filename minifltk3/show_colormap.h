//
// "$Id: show_colormap.h 8709 2011-05-21 21:41:12Z matt $"
//
// Colormap picker header file for the Fast Light Tool Kit (FLTK).
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

/** \file
 The fltk3::show_colormap() function hides the implementation classes used
 to provide the popup window and color selection mechanism.
 */

#ifndef fltk3_show_colormap_H
#define fltk3_show_colormap_H

namespace fltk3
{

/* doxygen comment here to avoid exposing ColorMenu in fltk3::show_colormap.cxx
 */

/** \addtogroup  fl_attributes
 @{ */

/**
 \brief Pops up a window to let the user pick a colormap entry.
 \image html fltk3::show_colormap.png
 \image latex fltk3::show_colormap.png "fltk3::show_colormap" height=10cm
 \param[in] oldcol color to be highlighted when grid is shown.
 \retval fltk3::Color value of the chosen colormap entry.
 \see fltk3::ColorChooser
 */
FLTK3_EXPORT fltk3::Color show_colormap(fltk3::Color oldcol);

/** @} */

}

#endif

//
// End of "$Id: show_colormap.h 8709 2011-05-21 21:41:12Z matt $".
//
