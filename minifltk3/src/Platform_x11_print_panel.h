//
// "$Id: print_panel.h 9011 2011-08-25 21:50:59Z matt $"
//
// Print panel for the Fast Light Tool Kit (FLTK).
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

//
// This is a temporary file.  It is only for development and will
// probably be removed later.
//

#ifndef print_panel_h
#define print_panel_h
#include "run.h"
#include "DoubleWindow.h"
#include "Group.h"
#include "Choice.h"
#include "Button.h"
#include "Box.h"
#include "RoundButton.h"
#include "Input.h"
#include "Spinner.h"
#include "CheckButton.h"
#include "ReturnButton.h"
#include "Progress.h"
static fltk3::DoubleWindow* make_print_panel();
static void print_cb(fltk3::ReturnButton *, void *); 
static void print_load();
static void print_update_status();
#endif

//
// End of "$Id: print_panel.h 9011 2011-08-25 21:50:59Z matt $".
//
