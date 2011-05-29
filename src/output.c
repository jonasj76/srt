/**
 * output.c - Render output class.
 *
 * Copyright (c) 2011, Jonas Johansson <jonasj76@gmail.com>
 *
 * Description:
 * This class defines the rendering output.
 *
 * License:
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "output.h"

/* Rendered scene dimensions */
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

/* Buffer for the rendered image */
static uint8_t image[SCREEN_WIDTH*SCREEN_HEIGHT*3];

/* Rendering output callback */
int (*output_render_cb)(uint8_t*, int, int) = NULL;

/**
 * output_render_setup - Setup render output.
 * @cb: Pointer to a render output function.
 *
 * This function will initialize and setup a callback function, @cb, for
 * handling the output of the rendered image, i.e. the function that will
 * be called when the ray tracing has finished to e.g. display the image.
 *
 * Returns:
 * POSIX OK (zero) or non-zero on error.
 */
int output_render_setup (int (*cb)())
{
   if (!cb)
      return 1;

   output_render_cb = cb;

   return 0;
}

/**
 * output_render - Call the output rendering function.
 *
 * This function will call the callback function setup in
 * output_render_setup().
 *
 * Returns:
 * Output from callback function,
 * which should be POSIX OK (zero) or non-zero on error.
 */
int output_render (void)
{
   return output_render_cb (image, SCREEN_WIDTH, SCREEN_HEIGHT);
}

/**
 * output_get_image - Get pointer to rendered image buffer.
 *
 * This function will return a pointer to the output image buffer
 * which should be used rendering the scene.
 *
 * Returns:
 * Pointer to a image buffer.
 */
uint8_t* output_get_image (void)
{
   return image;
}

/**
 * output_get_image_size - Get size of image buffer.
 *
 * This function will return the size of the output image buffer.
 *
 * Returns:
 * Size of image buffer.
 */
size_t output_get_image_size (void)
{
   return sizeof(image);
}

/**
 * output_get_image_width - Get width of image buffer.
 *
 * This function will return the width of the output image buffer.
 *
 * Returns:
 * Width of image buffer.
 */
int output_get_image_width (void)
{
   return SCREEN_WIDTH;
}

/**
 * output_get_image_height - Get height of image buffer.
 *
 * This function will return the height of the output image buffer.
 *
 * Returns:
 * Height of image buffer.
 */
int output_get_image_height (void)
{
   return SCREEN_HEIGHT;
}

/**
 * Local Variables:
 *  c-file-style: "ellemtel"
 *  indent-tabs-mode: nil
 * End:
 */
