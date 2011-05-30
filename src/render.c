/**
 * render.h - Render class.
 *
 * Copyright (c) 2011, Jonas Johansson <jonasj76@gmail.com>
 *
 * Description:
 * This class defines the ray tracing rendering.
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
#include <math.h>
#include <stdint.h>
#include <string.h> /* memset */

#include "vector.h"
#include "ray.h"
#include "camera.h"
#include "sphere.h"

#include "render.h"

/**
 * render_scene - Creates a rendered scene.
 * @image:         Pointer to buffer which will contain the rendered scene.
 * @image_sz:      Size of @image buffer.
 * @screen_width:  Width of rendered screen.
 * @screen_height: Height of rendered screen.
 * @cam:           Camera object.
 * @screen:        Pointer to a screen plane object.
 * @sphere_list:   Pointer to a list of sphere object.
 * @num_spheres:   Num of spheres in @sphere_list.
 *
 * This function will create a rendered scene. The output is written to @output
 * and is stored as an array of pixels, starting with the pixel at the lower
 * left corner and then continuing with increasing x value. Each pixel is
 * stored in three bytes starting a value for the the red component followed by
 * the green and then the blue.
 * A ray from the camera, @cam, through each pixel is generated. For each
 * sphere in @sphere_list, a check is made to see if the object was hit. The
 * closest object to the camera which was hit is recorded and the color of the
 * pixel will be set to color of that object. If the ray doesn't hit any object,
 * no pixel color is set, i.e. the default background color (black) will remain.
 *
 * Returns:
 * POSIX OK (zero) or non-zero on error.
 */
int render_scene (uint8_t* image,
                  size_t image_sz,
                  int screen_width,
                  int screen_height,
                  camera_t *cam,
                  sphere_t *sphere_list,
                  int num_spheres)
{
   const float aspect_ratio = (float)screen_height / (float)screen_width;
   const float fov_x = 3.14 / 4.0;             /* Field of view in the x-plane */
   const float fov_y = fov_x * aspect_ratio;   /* Field of view in the y-plane
                                                * with aspect ratio correction */
   ray_t ray;          /* The ray that will be used to trace through every pixel
                        * in the rendered image */
   int x, y;           /* Loop variables for each pixel */
   size_t image_ofs;   /* Offset in the rendered image, i.e. pointer to next pixel */

   /* Clear whole image buffer to set black as default background color */
   memset (image, 0, image_sz);

   /* Set starting point for the ray to the camera position */
   ray.origin.x = cam->pos.x;
   ray.origin.y = cam->pos.y;
   ray.origin.z = cam->pos.z;

   /* Reset image offset pointer */
   image_ofs = 0;

   /* Create directions for each ray, i.e. from the camera to each pixel in the
    * rendered image. Then test if the ray hits any of the spheres and set the
    * pixel to the color of that sphere object, or leave the pixel untouched if
    * no intersection was detected. */
   for (y = 0; y < screen_height; y++)
   {
      for (x = 0; x < screen_width; x++)
      {
         int i;
         int closest_sphere = -1;   /* Array ID of closests sphere,
                                     * -1 no sphere was hit by the ray */
         float min_dist = 100000;   /* Distance from camera to the closests
                                     * sphere. Start value is set to a very
                                     * high value so that if a sphere was hit
                                     * it should definitely be closer than this
                                     * value */
         float dist;                /* Distance from camera to the sphere */

         /* Set the ray direction. The ray will start att the camera position
          * and travel in a angle which at maximum is the field of view (fov)
          * value. I.e. if fov is set to 45 degree, the direction will be from
          * -45 to +45 degree from the camera center. The direction is for both
          * x and y directions, but the y value is corrected by a aspect ratio,
          * which will make a sphere look like a circle instead of an elipse on
          * a not 1:1 screen width to height mapping.
          * The camera will be looking along the negative z-axis. */

         ray.dir.x = tan (fov_x) * (2*x - screen_width)  / screen_width;
         ray.dir.y = tan (fov_y) * (2*y - screen_height) / screen_height;
         ray.dir.z = -1;

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

            /* Record the current sphere if it was intersected by the ray and
             * closer to the camera than any previous hit sphere. */
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
 * Local Variables:
 *  c-file-style: "ellemtel"
 *  indent-tabs-mode: nil
 * End:
 */
