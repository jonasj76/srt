/**
 * scene.c - Scene class.
 *
 * Copyright (c) 2011-2013, Jonas Johansson <jonasj76@gmail.com>
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

static scene_t scene;

/**
 * scene_init - Setup scene objects.
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
   /* Clear the scene struct */
   memset (&scene, 0, sizeof(scene));
}

/**
 * scene_get_scene - Get scene object.
 *
 * This function will get a pointer to the scene object.
 *
 * Returns:
 * Pointer to scene object.
 */
scene_t* scene_get_scene (void)
{
   return &scene;
}

/**
 * scene_get_cam - Get camera object.
 * @scene: Pointer to scene_t object
 *
 * This function will get a pointer to the camera object in the scene.
 *
 * Returns:
 * Pointer to camera object.
 */
camera_t* scene_get_camera (scene_t* scene)
{
   return &scene->cam;
}

/**
 * scene_get_sphere - Get sphere object.
 * @scene: Pointer to scene_t object
 *
 * This function will get a pointer to the sphere objects in the scene.
 *
 * Returns:
 * Pointer to sphere objects.
 */
sphere_t* scene_get_sphere (scene_t* scene)
{
   return scene->sphere;
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
