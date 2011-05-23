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
#include <assert.h>
#include <readline/readline.h>
#include <readline/history.h>
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
#include "version.h"

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

/* Number of spheres in scene */
#define NUM_SPHERES 3

/* Sphere objects */
sphere_t sphere[NUM_SPHERES];

/**
 * render_scene - Creates a rendered scene.
 * @image:       Pointer to buffer which will contain the rendered scene.
 * @image_sz:    Size of @image buffer.
 * @screen:      Pointer to a screen plane object.
 * @sphere_list: Pointer to a list of sphere object.
 * @num_spheres: Num of spheres in @sphere_list.
 *
 * This function will create a rendered scene. The output is written to @output
 * and is stored as an array of pixels, starting with the pixel at the lower
 * left corner and then continuing with increasing x value. Each pixel is
 * stored in three bytes starting a value for the the red component followed by
 * the green and then the blue.
 * For each pixel on the screen, @screen, a ray from the camera through that
 * pixel is generated. For each sphere in @sphere_list, a check is made to
 * see if the object was hit. The closest object to the camera which was hit
 * is recorded the color of the pixel will be set to color of that object. If
 * the ray doesn't hit any object, no pixel color is set, i.e. the default
 * background color (black) will remain.
 *
 * Returns:
 * POSIX OK (zero) or non-zero on error.
 */
int render_scene (uint8_t* image, int image_sz, screen_t *screen, sphere_t *sphere_list, int num_spheres)
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
    * pixel to the color of the sphere object, or leave the pixel untouched if
    * no intersection was detected. */
   for (y = 0; y < screen->height; y++)
   {
      for (x = 0; x < screen->width; x++)
      {
         int i;
         int closest_sphere = -1;   /* Array ID of closests sphere, -1 no sphere was hit by the ray */
         float min_dist = 100000;   /* Distance from camera/ray origin (0,0,0) to the closests sphere. Start value is set so high that if a sphere was hit it should definitely be closer than this value */
         float dist;                /* Distance from camera/ray origin (0,0,0) to the sphere */

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

         /* Loop through all spheres and check which one is the closest to the camera. */
         for (i = 0; i < num_spheres; i++)
         {
            sphere_t *sphere = &sphere_list[i];   /* Current sphere to check */

            /* Get distance from camera/ray origin to sphere, i.e. test if the
             * ray hits the sphere by checking if the returned distance is
             * greater than zero */
            dist = sphere_intersect (sphere, &ray);

            /* Record the current sphere if it was intersected by the ray and closer to the camera than any previous hit sphere. */
            if (dist > 0.0)
            {
               if (dist < min_dist)
               {
                  min_dist       = dist;
                  closest_sphere = i;
               }
            }
         }

         /* If a sphere was intersected by the ray, set the pixel to the color
          * of the sphere object, else leave the pixel untouched, i.e. keep the
          * background color */
         if (closest_sphere != -1)
         {
            sphere_t *sphere = &sphere_list[closest_sphere];
            int r, g, b;

            /* Check that new offset isn't pointing outside the image buffer */
            if (image_ofs >= (image_sz))
               return 1;

            sphere_get_color (sphere, &r, &g, &b);

            image[image_ofs + 0] = r;
            image[image_ofs + 1] = g;
            image[image_ofs + 2] = b;
         }

         /* Update image offset */
         image_ofs += 3;
      }
   }

   return 0;
}

/**
 * setup_scene - Setup scene objects.
 *
 * This function will setup the scene objects, i.e initalize the parameters for
 * each sphere used in the scene.
 *
 * Note:
 * All z-coordinates for the spheres must be in front of the camera, which is
 * at z-coordinate 0, i.e. any positive integer should be OK.
 *
 * Returns:
 * none.
 */
void setup_scene (void)
{
   /* Clear the sphere struct */
   memset (sphere, 0, sizeof(sphere));

   /* Setup sphere 1 */
   sphere[0].center.x = 0;
   sphere[0].center.y = 0;
   sphere[0].center.z = 600;
   sphere[0].radius   = 100;
   sphere_set_color (&sphere[0], 255, 0, 0);

   /* Setup sphere 2 */
   sphere[1].center.x = -200;
   sphere[1].center.y = 0;
   sphere[1].center.z = 900;
   sphere[1].radius   = 100;
   sphere_set_color (&sphere[1], 0, 255, 0);

   /* Setup sphere 3 */
   sphere[2].center.x = 200;
   sphere[2].center.y = 0;
   sphere[2].center.z = 900;
   sphere[2].radius   = 100;
   sphere_set_color (&sphere[2], 0, 0, 255);
}

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
   win = screen_create (SCREEN_WIDTH, SCREEN_HEIGHT);

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

/**
 * render_output_setup - Seyup render output.
 * @cb: Pointer to a render output function.
 *
 * This function will initialize and setup all neccessary data for handling
 * the rendered output image. A callback function, @cb, will be set, which will
 * be called when the rendering is finihed to e.g. display the image.
 *
 * Returns:
 * POSIX OK (zero) or non-zero on error.
 */
int render_output_setup (int (**cb)())
{
#ifdef SSIL
   *cb = output_ssil;

   return 0;
#endif

#ifdef SSGL
   *cb = ssgl;

   return 0;
#endif

   /* error, no method selected */
   *cb = NULL;

   return 1;
}

char* cli_pop_token (char* line)
{
   return strtok (line, " ");
}

void cli_enter_sphere (int id)
{
   char prompt[32];
   char* line;
   char* token;
   int end = 0;
   int i;
   int param[3]; /* x, y, z or r, g, b */

   snprintf (prompt, sizeof(prompt), "scene/sphere-%d> ", id);

   while (!end)
   {
      line = readline(prompt);

      token = cli_pop_token (line);

      if (!token)
         continue;

      if (!strcmp (token, "center"))
      {
         for (i=0; i<3; i++)
         {
            token = cli_pop_token (NULL);
            if (!token)
               continue;
            param[i] = strtol (token, NULL, 10);
         }
         sphere[id].center.x = param[0];
         sphere[id].center.y = param[1];
         sphere[id].center.z = param[2];
      }
      else
      if (!strcmp (token, "radius"))
      {
         token = cli_pop_token (NULL);
         if (!token)
            continue;
         sphere[id].radius = strtol (token, NULL, 10);
      }
      else
      if (!strcmp (token, "color"))
      {
         for (i=0; i<3; i++)
         {
            token = cli_pop_token (NULL);
            if (!token)
               continue;
            param[i] = strtol (token, NULL, 10);
         }
         sphere_set_color (&sphere[id], param[0], param[1], param[2]);
      }
      else
      if (!strcmp (token, "show"))
      {
         printf ("x:%.0f, y:%.0f, z:%.0f\n", sphere[id].center.x, sphere[id].center.y, sphere[id].center.z);
         printf ("radius: %.0f\n", sphere[id].radius);
         printf ("r:%d, g:%d, b:%d\n", sphere[id].r, sphere[id].g, sphere[id].b);
      }
      else
      if (!strcmp (token, "help"))
      {
         printf ("center <X> <Y> <Z>"  "\tSphere center coordinates.\n");
         printf ("radius <R>"          "\t\tSphere radius\n");
         printf ("color <R> <G> <B>"   "\tSphere color\n");
         printf ("show"                "\t\t\tShow sphere settings.\n");
         printf ("help"                "\t\t\tShow this help text.\n");
         printf ("end"                 "\t\t\tExit context.\n");
      }
      else
      if (!strcmp (token, "end"))
      {
         return;
      }
      else
      {
         if (strlen (token))
            printf ("Unknown command\n");
      }
   }
}

void cli_enter_scene (void)
{
   char* line;
   char* token;
   int end = 0;

   while (!end)
   {
      line = readline("scene> ");

      token = cli_pop_token (line);

      if (!token)
         continue;

      if (!strcmp (token, "sphere"))
      {
         int id;

         token = cli_pop_token (NULL);
         if (!token)
         {
            printf ("Missing sphere ID.\n");
            continue;
         }

         id = strtol (token, NULL, 10);
         if (id >=0  && id < NUM_SPHERES)
         {
            printf ("Entering sphere context\n");
            cli_enter_sphere (id);
         }
         else
         {
            printf ("Invalid ID, must be between 0 and %d\n", NUM_SPHERES);
         }
      }
      else
      if (!strcmp (token, "help"))
      {
         printf ("sphere <ID>"  "\tSetup sphere object.\n");
         printf (               "\t\t<ID> sphere identity, 0-%d.\n", NUM_SPHERES);
         printf ("help"         "\t\tShow this help text.\n");
         printf ("end"          "\t\tExit context.\n");
      }
      else
      if (!strcmp (token, "end"))
      {
         return;
      }
      else
      {
         if (strlen (token))
            printf ("Unknown command\n");
      }
   }
}

int main (void)
{
   screen_t screen;                                      /* Screen plane */
   uint8_t  image[SCREEN_WIDTH*SCREEN_HEIGHT*3];         /* Buffer for the rendered image */
   int (*render_output_cb)(uint8_t*, int, int) = NULL;   /* Rendering putput callback */
   char* line;
   char* token;
   int quit = 0;

   /* Print version */
   printf ("srt %s\n", VERSION);

   /* Clear rendered image buffer */
   memset (image, 0, sizeof(image));

   /* Set screen plane dimension and position */
   screen.width  = SCREEN_WIDTH;
   screen.height = SCREEN_HEIGHT;
   screen.pos_z  = 200;   /* Must be in front of the camera which is at (0,0,0) */

   /* Setup scene */
   setup_scene ();

   /* Init render output function */
   render_output_setup (&render_output_cb);
   if (!render_output_cb)
   {
      printf ("error: No rendering output method was selected.\n");
      printf ("       Use ssil, ssgl or write your own code.\n");
      printf ("       See README and Makefile for more information.\n");

      return 1;
   }

   /* Enter CLI */
   printf ("Enter 'help' for available commands.\n");
   while (!quit)
   {
      line = readline("> ");

      token = cli_pop_token (line);

      if (!token)
         continue;

      if (!strcmp (token, "scene"))
      {
         printf ("Entering scene context\n");
         cli_enter_scene ();
      }
      else
      if (!strcmp (token, "render"))
      {
         printf ("Rendering scene\n");
         /* Render the scene */
         if (render_scene (image, sizeof(image), &screen, sphere, NUM_SPHERES))
            printf ("An error occured when rendering the scene.\n");
      }
      else
      if (!strcmp (token, "output"))
      {
         printf ("Calling rendering output function\n");
         if (render_output_cb (image, SCREEN_WIDTH, SCREEN_HEIGHT))
            printf ("An error occured when calling the rendering output function.\n");
      }
      else
      if (!strcmp (token, "help"))
      {
         printf ("scene"   "\tEnter scene context.\n");
         printf ("render"  "\tRender scene.\n");
         printf ("output"  "\tSend the rendered scene to output function.\n");
         printf ("help"    "\tShow this help text.\n");
         printf ("quit"    "\tQuit.\n");
      }
      else
      if (!strcmp (token, "quit"))
      {
         quit = 1;
      }
      else
      {
         if (strlen (token))
            printf ("Unknown command\n");
      }
   }

   return 0;
}

/**
 * Local Variables:
 *  c-file-style: "ellemtel"
 *  indent-tabs-mode: nil
 * End:
 */
