SRCPATH = ./minifltk/src/
FLTK = -I./minifltk -I/usr/include/freetype2 -lXft  -lfontconfig -lXinerama -lpthread -ldl -lm  -lX11 \
	$(SRCPATH)abort.cxx               $(SRCPATH)FileIcon.cxx           $(SRCPATH)Pixmap.cxx            $(SRCPATH)scandir.cxx         $(SRCPATH)add_idle.cxx          $(SRCPATH)FileInput.cxx \
	$(SRCPATH)scheme_.cxx             $(SRCPATH)Adjuster.cxx           $(SRCPATH)filename_absolute.cxx $(SRCPATH)screen_xywh.cxx     $(SRCPATH)arc.cxx               $(SRCPATH)filename_expand.cxx \
	$(SRCPATH)scroll_area.cxx         $(SRCPATH)arci.cxx               $(SRCPATH)filename_ext.cxx      $(SRCPATH)Scrollbar.cxx       $(SRCPATH)arg.cxx               $(SRCPATH)filename_isdir.cxx \
	$(SRCPATH)ScrollGroup.cxx         $(SRCPATH)ask.cxx                $(SRCPATH)filename_list.cxx     $(SRCPATH)set_font.cxx	     $(SRCPATH)Bitmap.cxx            $(SRCPATH)filename_match.cxx \
	$(SRCPATH)set_fonts.cxx           $(SRCPATH)Box.cxx                $(SRCPATH)filename_setext.cxx   $(SRCPATH)ShapedWindow.cxx    $(SRCPATH)Box_gleam.cxx	     $(SRCPATH)flsleep.cxx \
	$(SRCPATH)SharedImage.cxx         $(SRCPATH)Box_gtk.cxx            $(SRCPATH)font.cxx              $(SRCPATH)shortcut.cxx        $(SRCPATH)Box_metro.cxx		 $(SRCPATH)get_key.cxx \
	$(SRCPATH)show_colormap.cxx       $(SRCPATH)Box_plastic.cxx	       $(SRCPATH)grab.cxx              $(SRCPATH)Slider.cxx          $(SRCPATH)Browser_.cxx		     $(SRCPATH)Group.cxx \
	$(SRCPATH)Spinner.cxx             $(SRCPATH)Browser.cxx            $(SRCPATH)HelpDialog.cxx        $(SRCPATH)Symbol.cxx          $(SRCPATH)Browser_load.cxx	     $(SRCPATH)HelpView.cxx \
	$(SRCPATH)symbols.cxx             $(SRCPATH)Button.cxx	           $(SRCPATH)Image.cxx             $(SRCPATH)TabGroup.cxx        $(SRCPATH)Chart.cxx		     $(SRCPATH)images_core.cxx \
	$(SRCPATH)Table.cxx               $(SRCPATH)CheckBrowser.cxx	   $(SRCPATH)Input_.cxx            $(SRCPATH)TableRow.cxx        $(SRCPATH)Choice.cxx            $(SRCPATH)Input.cxx \
	$(SRCPATH)TextBuffer.cxx          $(SRCPATH)Clock.cxx		       $(SRCPATH)jpgd.cxx              $(SRCPATH)TextDisplay.cxx     $(SRCPATH)ColorChooser.cxx	     $(SRCPATH)labeltype.cxx \
	$(SRCPATH)TextEditor.cxx          $(SRCPATH)color.cxx              $(SRCPATH)libnsgif.cxx          $(SRCPATH)TiledGroup.cxx      $(SRCPATH)compose.cxx           $(SRCPATH)line_style.cxx \
	$(SRCPATH)Tooltip.cxx             $(SRCPATH)Counter.cxx            $(SRCPATH)lock.cxx              $(SRCPATH)Tree.cxx            $(SRCPATH)cursor.cxx            $(SRCPATH)lodepng.cxx \
	$(SRCPATH)TreeItemArray.cxx       $(SRCPATH)curve.cxx              $(SRCPATH)Menu_add.cxx          $(SRCPATH)TreeItem.cxx        $(SRCPATH)Device.cxx            $(SRCPATH)MenuButton.cxx \
	$(SRCPATH)TreePrefs.cxx           $(SRCPATH)display.cxx            $(SRCPATH)Menu_.cxx             $(SRCPATH)utf8.cxx            $(SRCPATH)dnd.cxx		         $(SRCPATH)Menu.cxx \
	$(SRCPATH)utf.cxx                 $(SRCPATH)DoubleWindow.cxx       $(SRCPATH)Menu_global.cxx       $(SRCPATH)Valuator.cxx        $(SRCPATH)draw.cxx              $(SRCPATH)MenuWindow.cxx \
	$(SRCPATH)ValueInput.cxx          $(SRCPATH)draw_image.cxx         $(SRCPATH)MultiLabel.cxx        $(SRCPATH)Positioner.cxx		 $(SRCPATH)ValueOutput.cxx       $(SRCPATH)draw_pixmap.cxx \
	$(SRCPATH)NativeFileChooser_common.cxx $(SRCPATH)PostScript.cxx    $(SRCPATH)vertex.cxx            $(SRCPATH)encoding_latin1.cxx $(SRCPATH)NativeFileChooser.cxx $(SRCPATH)Preferences.cxx \
	$(SRCPATH)visual.cxx              $(SRCPATH)encoding_mac_roman.cxx $(SRCPATH)numericsort.cxx       $(SRCPATH)Printer.cxx		 $(SRCPATH)Widget.cxx            $(SRCPATH)engraved_label.cxx \
	$(SRCPATH)open_uri.cxx		      $(SRCPATH)ps_image.cxx		   $(SRCPATH)Window.cxx            $(SRCPATH)FileBrowser.cxx	 $(SRCPATH)overlay.cxx		     $(SRCPATH)read_image.cxx \
	$(SRCPATH)Window_fullscreen.cxx   $(SRCPATH)FileChooser2.cxx       $(SRCPATH)overlay_visual.cxx	   $(SRCPATH)rect.cxx			 $(SRCPATH)Window_hotspot.cxx    $(SRCPATH)FileChooser.cxx \
	$(SRCPATH)OverlayWindow.cxx	      $(SRCPATH)Roller.cxx	           $(SRCPATH)Window_iconize.cxx    $(SRCPATH)file_dir.cxx		 $(SRCPATH)own_colormap.cxx	     $(SRCPATH)run.cxx \
	$(SRCPATH)FileIcon2.cxx		      $(SRCPATH)PagedDevice.cxx		   $(SRCPATH)Scalebar.cxx          $(SRCPATH)flstring.c          $(SRCPATH)utf8_case.c           $(SRCPATH)utf8_is_right2left.c \
	$(SRCPATH)utf8_is_spacing.c       $(SRCPATH)utf8_mk_wcwidth.c      $(SRCPATH)vsnprintf.c           $(SRCPATH)utf8Wrap.c          $(SRCPATH)utf8Utils.c           $(SRCPATH)utf8Input.c \
	$(SRCPATH)keysym2Ucs.c

GLPATH = ./minifltk/extra_gl/src/
FLTK_GL = -lGL -lGLU \
          $(GLPATH)freeglut_geometry.cxx $(GLPATH)freeglut_stroke_roman.cxx $(GLPATH)GLChoice.cxx $(GLPATH)gl_draw.cxx $(GLPATH)gl_start.cxx \
	  $(GLPATH)glut_font.cxx $(GLPATH)freeglut_stroke_mono_roman.cxx $(GLPATH)freeglut_teapot.cxx $(GLPATH)GLDevicePlugin.cxx \
	  $(GLPATH)GLOverlay.cxx $(GLPATH)glut_compatability.cxx $(GLPATH)GLWindow.cxx
all:
	g++ -o demo glpuzzle.cxx trackball.c $(FLTK) $(FLTK_GL)
		
clean:
	rm -rf demo *.o
