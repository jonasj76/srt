/**
 * Simple Ray Tracer
 *
 * Copyright (c) 2011, Jonas Johansson <jonasj76@gmail.com>
 *
 * Description:
 * This program will render a scene containing a sphere object.
 * Ray tracing is the process of generating an image by tracing the path of
 * light through pixels in an screen plane and simulating the effects of its
 * encounters with virtual objects.
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

#include <stdint.h>

#include "output.h"
#include "cli.h"
#include "version.h"

#ifdef SSIL
#include "tga.h"
#endif

#ifdef SSGL
#include <SDL/SDL.h>
#include "screen.h"
#include "draw.h"
#endif

#ifdef SSIL
/**
 * output_ssil - Rendered image output callback when using ssil.
 * @image:  Pointer to rendered image buffer.
 * @width:  Width of renderd image.
 * @height: Height of rendered image.
 *
 * This function will be used is ssil is used.
 * The function will save the rendered image as a .tga image.

 * Returns:
 * POSIX OK (zero) or non-zero on error.
 */
int output_ssil (uint8_t *image, int width, int height)
{
   tga_write ("srt.tga", width, height, image);
   printf ("srt.tga was written.\n");

   return 0;
}
#endif

#ifdef SSGL
/**
 * output_ssgl - Rendered image output callback when using ssgl.
 * @image:  Pointer to rendered image buffer.
 * @width:  Width of renderd image.
 * @height: Height of rendered image.
 *
 * This function will be used is ssgl is used.
 * The function will use ssgl to draw the image in a window.
 *
 * Returns:
 * POSIX OK (zero) or non-zero on error.
 */
int ssgl (uint8_t *image, int width, int height)
{
   SDL_Surface *win; /* Pointer to the drawing surface */
   int x, y;         /* Loop variables for drawing surface coordinates */
   int k;            /* Offset in rendered image buffer */

   /* Open a window and create a drawing surface */
   win = screen_create (width, height);

   if (!win)
      return 1;

   /* Lock the surface for directly access */
   if (screen_lock (win))
      return 1;

   /* Put the pixels */
   k = 0;
   for (y = 0; y < height; y++)
   {
      for (x = 0; x < width; x++)
      {
         int r,g,b;
         r = image[k+0];
         g = image[k+1];
         b = image[k+2];
         putpixel (win, x, y, (r<<16) | (g<<8) | b);
         k += 3;
      }
   }

   /* Unlock surface */
   screen_unlock (win);

   /* Update screen  */
   screen_update (win);

   /* Poll for events; wait until window is closed */
   while (1)
   {
      SDL_Event event;

      while (SDL_PollEvent (&event))
      {
         switch (event.type)
         {
            case SDL_QUIT:
               SDL_Quit ();
               return 0;
         }
      }
   }

   SDL_Quit ();
   return 0;
}
#endif

int render_output_setup (void)
{
#ifdef SSIL
   return output_render_setup (output_ssil);
#endif

#ifdef SSGL
   return output_render_setup (ssgl);
#endif

   /* error, no method selected */
   return 1;
}

int main (void)
{
   /* Print version */
   printf ("srt %s\n", VERSION);

   /* Init render output function */
   if (render_output_setup ())
   {
      printf ("error: No rendering output method was selected.\n");
      printf ("       Use ssil, ssgl or write your own code.\n");
      printf ("       See README and Makefile for more information.\n");

      return 1;
   }

   /* Enter CLI */
   cli_enter ();

   return 0;
}

/**
 * Local Variables:
 *  c-file-style: "ellemtel"
 *  indent-tabs-mode: nil
 * End:
 */
