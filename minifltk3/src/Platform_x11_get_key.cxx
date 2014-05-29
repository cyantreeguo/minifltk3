// Return the current state of a key.  This is the X version.  I identify
// keys (mostly) by the X keysym.  So this turns the keysym into a keycode
// and looks it up in the X key bit vector, which Fl_x.cxx keeps track of.

#  include "run.h"
#  include "x.h"

extern char fl_key_vector[32]; // in Fl_x.cxx

int fltk3::event_key(unsigned int k)
{
	if (k > fltk3::MouseButton && k <= fltk3::MouseButton+8)
		return fltk3::event_state(8<<(k-fltk3::MouseButton));
	int i;
#  ifdef __sgi
	// get some missing PC keyboard keys:
	if (k == fltk3::MetaLKey) i = 147;
	else if (k == fltk3::MetaRKey) i = 148;
	else if (k == fltk3::MenuKey) i = 149;
	else
#  endif
		i = XKeysymToKeycode(fl_display, k);
	if (i==0) return 0;
	return fl_key_vector[i/8] & (1 << (i%8));
}

int fltk3::get_key(unsigned int k)
{
	fl_open_display();
	XQueryKeymap(fl_display, fl_key_vector);
	return event_key(k);
}
