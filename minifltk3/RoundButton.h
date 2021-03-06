//
// "$Id: RoundButton.h 9300 2012-03-23 20:36:00Z ianmacarthur $"
//
// Round button header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::RoundButton widget . */

#ifndef Fltk3_Round_Button_H
#define Fltk3_Round_Button_H

#include "LightButton.h"

namespace fltk3
{

/**
 Buttons generate callbacks when they are clicked by the user.  You
 control exactly when and how by changing the values for type()
 and when().
 <P ALIGN=CENTER>\image html fltk3::RoundButton.png</P>
 \image latex  fltk3::RoundButton.png " fltk3::RoundButton" width=4cm
 <P>The fltk3::RoundButton subclass display the "on" state by
 turning on a light, rather than drawing pushed in.  The shape of the
 "light" is initially set to fltk3::ROUND_DOWN_BOX.  The color of the light
 when on is controlled with selection_color(), which defaults to
 fltk3::FOREGROUND_COLOR.
 */
class FLTK3_EXPORT RoundButton : public LightButton
{
public:
	RoundButton(int X,int Y,int W,int H,const char *l = 0): fltk3::LightButton(X,Y,W,H,l) {
		box(fltk3::NO_BOX);
		down_box(fltk3::ROUND_DOWN_BOX);
		selection_color(fltk3::FOREGROUND_COLOR);
	}
};

}

#endif

//
// End of "$Id: RoundButton.h 9300 2012-03-23 20:36:00Z ianmacarthur $".
//
