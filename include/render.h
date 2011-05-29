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

#ifndef __RENDER_H__
#define __RENDER_H__

#include <stdint.h>

#include "vector.h"
#include "sphere.h"

int render_scene (uint8_t* image,
                  size_t image_sz,
                  int screen_width,
                  int screen_height,
                  vector_t *cam,
                  sphere_t *sphere_list,
                  int num_spheres);

#endif /* __RENDER_H__ */

/**
 * Local Variables:
 *  c-file-style: "ellemtel"
 *  indent-tabs-mode: nil
 * End:
 */
