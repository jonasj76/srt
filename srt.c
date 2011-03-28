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

#include <stdio.h>
#include <stdint.h>
#include <string.h>   /* memset() */
#include "vector.h"
#include "ray.h"
#include "sphere.h"
#ifdef SSIL
#include "tga.h"
#endif
#ifdef SSGL
#include <SDL/SDL.h>
#include "screen.h"
#include "draw.h"
#endif

/* Screen plane object.
 * A screen plane is a rectangle in the scene representing the visual screen.
 * In this simple ray tracer it is centered at the origin. The z position
 * can though be modified. If the plane is moved away from the camera, the
 * ray tracing beams becomes narrower and the objects will appear bigger on
 * the screen.
 */
typedef struct {
   int width;
   int height;
   int pos_z;
} screen_t;

/* Screen plane and rendered scene dimensions */
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

/* Buffer for the rendered image */
uint8_t  image[SCREEN_WIDTH*SCREEN_HEIGHT*3];

/**
 * render_scence - Creates a rendered scene.
 * @image:    Pointer to buffer which will contain the rendered scene.
 * @image_sz: Size of @image buffer.
 * @screen:   Pointer to a screen plane object.
 * @sphere:   Pointer to a sphere object.
 *
 * This function will create a rendered scene. The output is written to @output
 * and is stored as an array of pixels, starting with the pixel at the lower
 * left corner and then continuing with increasing x value. Each pixel is
 * stored in three bytes starting a value for the the red component followed by
 * the green and then the blue.
 * For each pixel on the screen, @screen, a ray from the camera through that
 * pixel is generated. If the object, @sphere, in the scene is hit by the
 * ray the color of the pixel will be set to white. If the ray doesn't hit the
 * object, no pixel color is set, i.e. the default background color (black)
 * will remain.
 *
 * Returns:
 * POSIX OK (zero) or non-zero on error.
 */
int render_scene (uint8_t* image, int image_sz, screen_t *screen, sphere_t *sphere)
{
   ray_t ray;       /* The ray that will be used to trace through every pixel of the screen plane */
   int x, y;        /* Loop variables for each pixel on the screen plane */
   int image_ofs;   /* Offset in the rendered image, i.e. pointer to next pixel */

  /* Clear whole image buffer to set black as default background color */
  memset (image, 0, image_sz);

  /* Set starting point for the ray to the camera position (0,0,0) */
  ray.origin.x = 0;
  ray.origin.y = 0;
  ray.origin.z = 0;

  /* Reset image offset pointer */
  image_ofs = 0;

  /* Create directions for each ray, i.e. from the camera/ray origin (0,0,0) to
   * each pixel on the screen. Then test if the ray hits this sphere and set the
   * pixel to white if a hit occurs or leave the pixel untouched if no hit was
   * detected. */
  for (y = 0; y < screen->height; y++)
  {
    for (x = 0; x < screen->width; x++)
    {
       float dist;   /* Distance from camera/ray origin (0,0,0) to the sphere */

       /* Set the ray direction. The x,y center of the screen will be at (0,0),
        * i.e. the same x,y coordinates as the center of the camera. The
        * x-direction can therefore be calculated as the different between the
        * x-coordinate for the current pixel and the center of the screen, i.e.
        * half the screen width. The y-direction can be calculated in the same
        * way, i.e. y - height/2. */
       ray.dir.x = x - (screen->width  / 2);
       ray.dir.y = y - (screen->height / 2);
       ray.dir.z = screen->pos_z;

       /* Normalize the direction (a must for the intersection test) */
       vector_normal (&ray.dir);

       /* Get distance from camera/ray origin to sphere, i.e. test if the
        * ray hits the sphere by checking if the returned distance is
        * greater than zero */
      dist = sphere_intersect (sphere, &ray);

      /* If the sphere was intersected by the ray, make the pixel white, else
       * leave the pixel untouched, i.e. keep the background color */
      if (dist > 0.0)
      {
         /* Check that new offset isn't pointing outside the image buffer */
         if (image_ofs >= (image_sz))
            return 1;

         image[image_ofs + 0] = 255;
         image[image_ofs + 1] = 255;
         image[image_ofs + 2] = 255;
      }

      /* Update image offset */
      image_ofs += 3;
    }
  }

  return 0;
}

int main (void)
{
   screen_t screen;   /* Screen plane  */
   sphere_t sphere;   /* Sphere object */

   /* Set screen plane dimension and position */
   screen.width  = SCREEN_WIDTH;
   screen.height = SCREEN_HEIGHT;
   screen.pos_z  = 200;   /* Must be in front of the camera which is at (0,0,0) */

   /* Set sphere position and radius */
   sphere.center.x = 0;
   sphere.center.y = 0;
   sphere.center.z = 600;   /* Must be in front of the camera which is at (0,0,0) */
   sphere.radius   = 100;

   /* Render the scene */
   if (render_scene (image, sizeof(image), &screen, &sphere))
   {
      printf ("error: an error occured when rendering the scene.\n");
   }

#ifdef SSIL   /* Use ssil to save the scene as a .tga image */
   tga_write ("srt.tga", SCREEN_WIDTH, SCREEN_HEIGHT, image);
   printf ("srt.tga was written.\n");

   return 0;
#endif

#ifdef SSGL   /* Use ssgl to draw the scene */
   SDL_Surface *win;   /* Pointer to drawing surface */
   int x, y;           /* Loop variables for drawing surface coordinates */
   int k;              /* Offset in rendered image buffer */

   /* Create a drawing surface */
   win = screen_create (SCREEN_WIDTH, SCREEN_HEIGHT);

   if (!win)
      return 1;

   /* Lock the surface for directly access */
   if (screen_lock (win))
      return 1;

   /* Put the pixels */
   k = 0;
   for (y=0; y<SCREEN_HEIGHT; y++)
      for (x=0; x<SCREEN_WIDTH; x++)
      {
         int r,g,b;
         r = image[k+0];
         g = image[k+1];
         b = image[k+2];
         putpixel(win, x, y, (r<<16) | (g<<8) | b);
         k += 3;
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
               return 0;
         }
      }
   }

   return 0;
#endif

   printf ("note: No visual output was written.\n");
   printf ("      Use ssil, ssgl or write your own code.\n");
   printf ("      See README and Makefile for more information.\n");

   return 0;
}

/**
 * Local Variables:
 *  c-file-style: "ellemtel"
 *  indent-tabs-mode: nil
 * End:
 */
