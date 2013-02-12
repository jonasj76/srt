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

#ifndef __SCENE_H__
#define __SCENE_H__

#include "camera.h"
#include "sphere.h"

/* Number of spheres in scene */
#define NUM_SPHERES 3

/* Scene object */
typedef struct {
   camera_t cam;                  /* Camera object */
   sphere_t sphere[NUM_SPHERES];  /* Sphere objects */
}  scene_t;

void scene_init (void);
scene_t* scene_get_scene (void);
camera_t* scene_get_camera (scene_t* scene);
sphere_t* scene_get_sphere (scene_t* scene);
int scene_get_num_spheres (void);

#endif /* __SCENE_H__ */

/**
 * Local Variables:
 *  c-file-style: "ellemtel"
 *  indent-tabs-mode: nil
 * End:
 */
