#ifndef Fltk3_ImageGIF_H
#define Fltk3_ImageGIF_H

#include "Image.h"
#include "SharedImage.h"
#include "run.h"
#include <stdio.h>
#include <stdlib.h>
#include "utf8.h"
#include "src/flstring.h"

#include "libnsgif.h"

namespace fltk3
{

class FLTK3_EXPORT ImageGIF : public fltk3::ImageRGB
{
private:	
	typedef struct {
		int width, height, depth;
		unsigned int delay_ms;
		int size;
		unsigned char *data;
	} GIFFRAME;
	GIFFRAME *frame_;
	int count_;
	int current_;

protected:
	static void *bitmap_create(int width, int height)
	{
		void *p = malloc(width * height * 4);
		memset(p, 0, width*height*4);
		return p;

		// return calloc(width * height, 4);
	}

	static void bitmap_set_opaque(void *bitmap, unsigned char/*bool*/ opaque)
	{
		//(void) opaque;  /* unused */
		//assert(bitmap);
	}

	static unsigned char/*bool*/ bitmap_test_opaque(void *bitmap)
	{
		//assert(bitmap);
		return NSGIF_FALSE;
	}

	static unsigned char *bitmap_get_buffer(void *bitmap)
	{
		//assert(bitmap);
		return (unsigned char *)bitmap;
	}

	static void bitmap_destroy(void *bitmap)
	{
		//assert(bitmap);
		if ( bitmap != NULL ) free(bitmap);
	}

	static void bitmap_modified(void *bitmap)
	{
		//assert(bitmap);
	}

protected:
	void FrameClean()
	{
		if ( count_ == 0 ) return;
		int i;
		for (i=0; i<(int)count_; i++) {
			if ( frame_[i].data != NULL ) {
				free(frame_[i].data);
				frame_[i].data = NULL;
			}
		}
		free(frame_);
		count_ = 0;
	}

public:
	ImageGIF(const char* filename) : fltk3::ImageRGB(0,0,0) 
	{
		count_ = 0;
		current_ = 0;

		FILE *fp;
		int size;
		unsigned char *data;

		if ((fp = fltk3::fopen(filename, "rb")) == NULL) return;
		fseek(fp, 0, SEEK_END);
		size = (int) ftell(fp);
		if ( size <= 0 ) { fclose(fp); return; }
		data = (unsigned char*)malloc(size);
		fseek(fp, 0, SEEK_SET);
		size = (int) fread(data, 1, size, fp);
		fclose(fp);

		gif_animation gif;
		gif_bitmap_callback_vt bitmap_callbacks = {
			ImageGIF::bitmap_create,
			ImageGIF::bitmap_destroy,
			ImageGIF::bitmap_get_buffer,
			ImageGIF::bitmap_set_opaque,
			ImageGIF::bitmap_test_opaque,
			ImageGIF::bitmap_modified
		};
		gif_result code;
		int i;

		gif_create(&gif, &bitmap_callbacks);
		do {
			code = gif_initialise(&gif, size, data);
			if (code != GIF_OK && code != GIF_WORKING) {
				gif_finalise(&gif);
				free(data);
				return;
			}
		} while (code != GIF_OK);

		if ( gif.frame_count == 0 ) {
			gif_finalise(&gif);
			free(data);
			return;
		}
		count_ = gif.frame_count;
		frame_ = (GIFFRAME *)malloc(sizeof(GIFFRAME) * count_);
		for (i=0; i<count_; i++) frame_[i].data = NULL;

		/* decode the frames */
		for (i = 0; i != gif.frame_count; i++) {
			code = gif_decode_frame(&gif, i);
			if (code != GIF_OK) {
				//warning("gif_decode_frame", code);
				FrameClean();
				gif_finalise(&gif);
				free(data);
				return;
			}
			frame_[i].size = gif.width * gif.height * 4;
			frame_[i].width = gif.width;
			frame_[i].height = gif.height;
			frame_[i].depth = 4;
			frame_[i].delay_ms = gif.frames->frame_delay*10;
			//if ( frame_[i].delay >= 100 ) frame_[i].delay = 18;
			frame_[i].data = (unsigned char *)malloc(frame_[i].size);
			memcpy(frame_[i].data, (unsigned char *)gif.frame_image, frame_[i].size);
			//printf("%s, loop count=%d, delay=%d\n", filename, gif.loop_count, frame_[i].delay);
		}

		gif_finalise(&gif);
		free(data);

		select_frame(current_);
	}

	ImageGIF(const char *name, const unsigned char *data, int size) : fltk3::ImageRGB(0,0,0)
	{
		count_ = 0;
		current_ = 0;

		gif_animation gif;
		gif_bitmap_callback_vt bitmap_callbacks = {
			ImageGIF::bitmap_create,
			ImageGIF::bitmap_destroy,
			ImageGIF::bitmap_get_buffer,
			ImageGIF::bitmap_set_opaque,
			ImageGIF::bitmap_test_opaque,
			ImageGIF::bitmap_modified
		};
		gif_result code;
		int i;

		unsigned char *p = (unsigned char *)data;
		gif_create(&gif, &bitmap_callbacks);
		do {
			code = gif_initialise(&gif, size, p);
			if (code != GIF_OK && code != GIF_WORKING) {
				gif_finalise(&gif);
				return;
			}
		} while (code != GIF_OK);

		if ( gif.frame_count == 0 ) {
			gif_finalise(&gif);
			return;
		}
		count_ = gif.frame_count;
		frame_ = (GIFFRAME *)malloc(sizeof(GIFFRAME) * count_);
		for (i=0; i<count_; i++) frame_[i].data = NULL;

		/* decode the frames */
		for (i = 0; i != gif.frame_count; i++) {
			code = gif_decode_frame(&gif, i);
			if (code != GIF_OK) {
				//warning("gif_decode_frame", code);
				FrameClean();
				gif_finalise(&gif);
				return;
			}
			frame_[i].size = gif.width * gif.height * 4;
			frame_[i].width = gif.width;
			frame_[i].height = gif.height;
			frame_[i].depth = 4;
			frame_[i].delay_ms = gif.frames->frame_delay*10;
			//if ( frame_[i].delay >= 100 ) frame_[i].delay = 18;
			frame_[i].data = (unsigned char *)malloc(frame_[i].size);
			memcpy(frame_[i].data, (unsigned char *)gif.frame_image, frame_[i].size);
			//printf("%s, loop count=%d, delay=%d\n", filename, gif.loop_count, frame_[i].delay);
		}

		gif_finalise(&gif);

		select_frame(current_);

		if (w() && h() && name) {
			fltk3::SharedImage *si = new fltk3::SharedImage(name, this);
			si->add();
		}
	}

	~ImageGIF() 
	{
		FrameClean();
	}

	void select_frame(int index) /* Used to select a given frame number. */
	{
		if ( index < 0 || index >= count_ ) return;
		uncache();
		w(frame_[index].width); h(frame_[index].height); d(frame_[index].depth);
		array = frame_[index].data;
		alloc_array = 0;
		current_ = index;
	}

	inline int framecount() { return count_; }
	inline int framecurrent() { return current_; }
	int framedelay_ms(int index) {
		if ( index < 0 || index >= count_ ) return 0;
		return frame_[index].delay_ms;
	}
};

}

#endif
