//
// "$Id: TableRow.cxx 9048 2011-09-18 20:11:00Z matt $"
//
// fltk3::TableRow -- A row oriented table widget
//
//    A class specializing in a table of rows.
//    Handles row-specific selection behavior.
//
// Copyright 2002 by Greg Ercolano.
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
// Please report all bugs and problems to "erco at seriss dot com".
//
//
// TODO:
//    o Row headings (only column headings supported currently)
//

#include <stdio.h>		// for debugging
#include "run.h"
#include "draw.h"
#include "TableRow.h"

// Is row selected?
int fltk3::TableRow::row_selected(int row)
{
	if ( row < 0 || row >= rows() ) return(-1);
	return(_rowselect[row]);
}

// Change row selection type
void fltk3::TableRow::type(TableRowSelectMode val)
{
	_selectmode = val;
	switch ( _selectmode ) {
	case SELECT_NONE: {
		for ( int row=0; row<rows(); row++ ) {
			_rowselect[row] = 0;
		}
		redraw();
		break;
	}
	case SELECT_SINGLE: {
		int count = 0;
		for ( int row=0; row<rows(); row++ ) {
			if ( _rowselect[row] ) {
				if ( ++count > 1 ) {	// only one allowed
					_rowselect[row] = 0;
				}
			}
		}
		redraw();
		break;
	}
	case SELECT_MULTI:
		break;
	}
}

// Change selection state for row
//
//     flag:
//        0 - clear selection
//        1 - set selection
//        2 - toggle selection
//
//     Returns:
//        0 - selection state did not change
//        1 - selection state changed
//       -1 - row out of range or incorrect selection mode
//
int fltk3::TableRow::select_row(int row, int flag)
{
	int ret = 0;
	if ( row < 0 || row >= rows() ) {
		return(-1);
	}
	switch ( _selectmode ) {
	case SELECT_NONE:
		return(-1);

	case SELECT_SINGLE: {
		int oldval;
		for ( int t=0; t<rows(); t++ ) {
			if ( t == row ) {
				oldval = _rowselect[row];
				if ( flag == 2 ) {
					_rowselect[row] ^= 1;
				} else             {
					_rowselect[row] = flag;
				}
				if ( oldval != _rowselect[row] ) {
					redraw_range(row, row, leftcol, rightcol);
					ret = 1;
				}
			} else if ( _rowselect[t] ) {
				_rowselect[t] = 0;
				redraw_range(t, t, leftcol, rightcol);
			}
		}
		break;
	}

	case SELECT_MULTI: {
		int oldval = _rowselect[row];
		if ( flag == 2 ) {
			_rowselect[row] ^= 1;
		} else             {
			_rowselect[row] = flag;
		}
		if ( _rowselect[row] != oldval ) {		// select state changed?
			if ( row >= toprow && row <= botrow ) {		// row visible?
				// Extend partial redraw range
				redraw_range(row, row, leftcol, rightcol);
			}
			ret = 1;
		}
	}
	}
	return(ret);
}

// Select all rows to a known state
void fltk3::TableRow::select_all_rows(int flag)
{
	switch ( _selectmode ) {
	case SELECT_NONE:
		return;

	case SELECT_SINGLE:
		if ( flag != 0 ) return;
		//FALLTHROUGH

	case SELECT_MULTI: {
		char changed = 0;
		if ( flag == 2 ) {
			for ( int row=0; row<(int)_rowselect.size(); row++ ) {
				_rowselect[row] ^= 1;
			}
			changed = 1;
		} else {
			for ( int row=0; row<(int)_rowselect.size(); row++ ) {
				changed |= (_rowselect[row] != flag)?1:0;
				_rowselect[row] = flag;
			}
		}
		if ( changed ) {
			redraw();
		}
	}
	}
}

// Set number of rows
void fltk3::TableRow::rows(int val)
{
	Table::rows(val);
	while ( val > (int)_rowselect.size() ) {
		_rowselect.push_back(0);        // enlarge
	}
	while ( val < (int)_rowselect.size() ) {
		_rowselect.pop_back();        // shrink
	}
}

// #include "eventnames.h"		// debugging
// #include <stdio.h>

// Handle events
int fltk3::TableRow::handle(int event)
{
	//  fprintf(stderr, "** EVENT: %s: EVENT XY=%d,%d\n",
	//      eventnames[event], fltk3::event_x(), fltk3::event_y());	// debugging

	// Let base class handle event
	int ret = Table::handle(event);

	// The following code disables cell selection.. why was it added? -erco 05/18/03
	// if ( ret ) { _last_y = fltk3::event_y(); return(1); }	// base class 'handled' it (eg. column resize)

	int shiftstate = (fltk3::event_state() & fltk3::CTRL) ? fltk3::CTRL :
	                 (fltk3::event_state() & fltk3::SHIFT) ? fltk3::SHIFT : 0;

	// Which row/column are we over?
	int R, C;  				// row/column being worked on
	ResizeFlag resizeflag;		// which resizing area are we over? (0=none)
	TableContext context = cursor2rowcol(R, C, resizeflag);
	switch ( event ) {
	case fltk3::PUSH:
		if ( fltk3::event_button() == 1 ) {
			_last_push_x = fltk3::event_x();	// save regardless of context
			_last_push_y = fltk3::event_y();	// " "

			// Handle selection in table.
			//     Select cell under cursor, and enable drag selection mode.
			//
			if ( context == CONTEXT_CELL ) {
				// Ctrl key? Toggle selection state
				switch ( shiftstate ) {
				case fltk3::CTRL:
					select_row(R, 2);		// toggle
					break;

				case fltk3::SHIFT: {
					select_row(R, 1);
					if ( _last_row > -1 ) {
						int srow = R, erow = _last_row;
						if ( srow > erow ) {
							srow = _last_row;
							erow = R;
						}
						for ( int row = srow; row <= erow; row++ ) {
							select_row(row, 1);
						}
					}
					break;
				}

				default:
					select_all_rows(0);	// clear all previous selections
					select_row(R, 1);
					break;
				}

				_last_row = R;
				_dragging_select = 1;
				ret = 1;      // fltk3::PUSH handled (ensures fltk3::DRAG will be sent)
				// redraw();  // redraw() handled by select_row()
			}
		}
		break;

	case fltk3::DRAG: {
		if ( _dragging_select ) {
			// Dragged off table edges? Handle scrolling
			int offtop = toy - _last_y;			// >0 if off top of table
			int offbot = _last_y - (toy + toh);		// >0 if off bottom of table

			if ( offtop > 0 && row_position() > 0 ) {
				// Only scroll in upward direction
				int diff = _last_y - fltk3::event_y();
				if ( diff < 1 ) {
					ret = 1;
					break;
				}
				row_position(row_position() - diff);
				context = CONTEXT_CELL;
				C = 0;
				R = row_position();  // HACK: fake it
				if ( R < 0 || R > rows() ) {
					ret = 1;        // HACK: ugly
					break;
				}
			} else if ( offbot > 0 && botrow < rows() ) {
				// Only scroll in downward direction
				int diff = fltk3::event_y() - _last_y;
				if ( diff < 1 ) {
					ret = 1;
					break;
				}
				row_position(row_position() + diff);
				context = CONTEXT_CELL;
				C = 0;
				R = botrow;		// HACK: fake it
				if ( R < 0 || R > rows() ) {
					ret = 1;        // HACK: ugly
					break;
				}
			}
			if ( context == CONTEXT_CELL ) {
				switch ( shiftstate ) {
				case fltk3::CTRL:
					if ( R != _last_row ) {		// toggle if dragged to new row
						select_row(R, 2);		// 2=toggle
					}
					break;

				case fltk3::SHIFT:
				default:
					select_row(R, 1);
					if ( _last_row > -1 ) {
						int srow = R, erow = _last_row;
						if ( srow > erow ) {
							srow = _last_row;
							erow = R;
						}
						for ( int row = srow; row <= erow; row++ ) {
							select_row(row, 1);
						}
					}
					break;
				}
				ret = 1;				// drag handled
				_last_row = R;
			}
		}
		break;
	}

	case fltk3::RELEASE:
		if ( fltk3::event_button() == 1 ) {
			_dragging_select = 0;
			ret = 1;			// release handled
			// Clicked off edges of data table?
			//    A way for user to clear the current selection.
			//
			int databot = tiy + table_h,
			    dataright = tix + table_w;
			if (
			        ( _last_push_x > dataright && fltk3::event_x() > dataright ) ||
			        ( _last_push_y > databot && fltk3::event_y() > databot )
			) {
				select_all_rows(0);			// clear previous selections
			}
		}
		break;

	default:
		break;
	}
	_last_y = fltk3::event_y();
	return(ret);
}

//
// End of "$Id: TableRow.cxx 9048 2011-09-18 20:11:00Z matt $".
//
