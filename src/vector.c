/**
 * vector.c - Vector class API.
 *
 * Copyright (c) 2011, Jonas Johansson <jonasj76@gmail.com>
 *
 * Description:
 * This class defines a 3D vector.
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

#include <assert.h>
#include <math.h>
#include "vector.h"

/**
 * vector_sub - Subtract two vectors.
 * @vr: Pointer to resulting vector.
 * @v1: Pointer to vector 1.
 * @v2: Pointer to vector 2.
 *
 * This function will subtract vector @v2 from vector @v1 and place the
 * result in vector @vr.
 *
 * Returns:
 * none.
 */
void vector_sub (vector_t *vr, vector_t *v1, vector_t *v2)
{
   assert (vr);
   assert (v1);
   assert (v2);

   vr->x = v1->x - v2->x;
   vr->y = v1->y - v2->y;
   vr->z = v1->z - v2->z;
}

/**
 * vector_length - Get vector length.
 * @v: Pointer to vector.
 *
 * This function returns the length of vector @v.
 *
 * Returns:
 * Length of vector @v.
 */
float vector_length (vector_t *v)
{
   assert (v);

   return sqrt (v->x * v->x + v->y * v->y + v->z * v->z);
}

/**
 * vector_normal - Make unit vector.
 * @v: Pointer to vector.
 *
 * This function will make vector @v a unit vector.
 *
 * Returns:
 * none.
 */
void vector_normal (vector_t *v)
{
   float len;

   assert (v);

   len = vector_length (v);
   if (len == 0.0f)
      len = 1.0f;

   v->x = v->x / len;
   v->y = v->y / len;
   v->z = v->z / len;
}

/**
 * vector_dot - Calculate the dot product.
 * @v1: Pointer to vector 1.
 * @v2: Pointer to vector 2.
 *
 * This function will calculate the dot product of vector @v1 and vector @v2.
 *
 * Returns:
 * Dot product of @v1 and @v2.
 */
float vector_dot (vector_t *v1, vector_t *v2)
{
   return (v1->x * v2->x + v1->y * v2->y + v1->z * v2->z);
}

/**
 * Local Variables:
 *  c-file-style: "ellemtel"
 *  indent-tabs-mode: nil
 * End:
 */
