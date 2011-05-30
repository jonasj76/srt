/**
 * scene.c - Scene class.
 *
 * Copyright (c) 2011, Jonas Johansson <jonasj76@gmail.com>
 *
 * Description:
 * This class defines the scene.
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

#include <string.h> /* memset */

#include "camera.h"
#include "sphere.h"

#include "scene.h"

/* Number of spheres in scene */
#define NUM_SPHERES 3

static sphere_t sphere[NUM_SPHERES];
static camera_t cam;

/**
 * setup_scene - Setup scene objects.
 *
 * This function will setup default values for the scene objects.
 *
 * Note:
 * All z-coordinates for the spheres must be in front of the camera,
 * which is at z-coordinate 0, i.e. any negative value should be OK.
 *
 * Returns:
 * none.
 */
void scene_init (void)
{
   /* Setup camera */
   cam.pos.x = 0;
   cam.pos.y = 0;
   cam.pos.z = 0;

   /* Clear the sphere struct */
   memset (sphere, 0, sizeof(sphere));

   /* Setup sphere 1 */
   sphere[0].center.x = 0;
   sphere[0].center.y = 0;
   sphere[0].center.z = -600;
   sphere[0].radius   = 100;
   sphere_set_color (&sphere[0], 255, 0, 0);

   /* Setup sphere 2 */
   sphere[1].center.x = -200;
   sphere[1].center.y = 0;
   sphere[1].center.z = -900;
   sphere[1].radius   = 100;
   sphere_set_color (&sphere[1], 0, 255, 0);

   /* Setup sphere 3 */
   sphere[2].center.x = 200;
   sphere[2].center.y = 0;
   sphere[2].center.z = -900;
   sphere[2].radius   = 100;
   sphere_set_color (&sphere[2], 0, 0, 255);
}

/**
 * scene_get_cam - Get camera object.
 *
 * This function will get a pointer to the camera object
 * in the scene.
 *
 * Returns:
 * Pointer to camera object.
 */
camera_t* scene_get_camera (void)
{
   return &cam;
}

/**
 * scene_get_sphere - Get sphere object.
 * @id: ID of sphere object
 *
 * This function will get a pointer to the sphere objects
 * in the scene.
 *
 * Returns:
 * Pointer to sphere objects.
 */
sphere_t* scene_get_sphere (void)
{
   return sphere;
}

/**
 * scene_get_num_sphere - Get number of sphere objects.
 *
 * This function will get the number of spheres in the
 * scene.
 *
 * Returns:
 * Pointer number of sphere objects.
 */
int scene_get_num_spheres (void)
{
   return NUM_SPHERES;
}

/**
 * Local Variables:
 *  c-file-style: "ellemtel"
 *  indent-tabs-mode: nil
 * End:
 */
