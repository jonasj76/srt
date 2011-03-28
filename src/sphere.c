/**
 * sphere.c - Sphere object class.
 *
 * Copyright (c) 2011, Jonas Johansson <jonasj76@gmail.com>
 *
 * Description:
 * This class defines a 3D sphere.
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
#include "sphere.h"

/**
 * sphere_intersect - Check ray intersection with a sphere object.
 * @sphere: Pointer to sphere object.
 * @ray: Pointer to normalized ray object.
 *
 * This function will test if a ray, @ray, will intersect a sphere, @sphere.
 * Imaging a ray R with origin at E and direction V intersecting a sphere with
 * center O and radius r. The intersection point will be detnoted P. A triangle
 * E-O-A, where A is a right angle can be drawn. The E-O side has length c, E-A
 * has length v and O-A has length b. See figure 1.
 *
 *                       ...---o O                               ...---o O
 *              c  ...---      |                        r  ...---      |
 *           ...---            | b                   ...---            | b
 *     ...---                  |               ...---                  |
 *  E o------------------------o A          P o------------------------o A
 *                 v                                       d
 *  fig 1.                                  fig 2.
 *
 * The v side will then have the same direction as V and i.e. will represent a
 * bit of the ray R.
 * Pythagorean theorem gives:
 *    v² + b² = c²   (1)
 *
 * A triangle P-O-A, where A is a right angle can be drawn. The P-O side has
 * length r (the sphere radius; remember point P is the sphere intersection
 * point), P-A has length d and O-A has length b (same side as previous drawn
 * triangle). See figure 2.
 *
 * Pythagorean theorem gives:
 *    d² + b² = r²   (2)
 *
 * (1) and (2) gives:
 *    (1): b² = c² - v²
 *    (2): d² = r² - b²
 *     =>  d² = r² - (c² - v²)   (3)
 *
 * For a vector a and a vector b, the dot product
 *    a * b = |a| |b| cos(p)
 * where |a| and |b| denote the the length of a and b, and p is the angle
 * between them. If both a and b have length one (i.e. they are unit vectors),
 * their dot product simply gives the cosine of the angle between them. If only
 * b is a unit vector, then the dot product a * b gives |a| cos(p), i.e. the
 * orthogonal projection of the vector a onto the vector b, with a minus sign
 * if the direction is opposite. This is called the scalar projection of a onto
 * b. For an intuitive understanding of this formula, recall from trigonometry
 * that
 *    cos(p) = a * b / |a|
 * where b is the unit vector
 *
 * If V is normalized (a unit vector), v is equal to the dot product of E-O
 * and V, i.e.
 *    v = E-O * V
 *
 * To determine whether an intersection occurs, we compute the value of d. That
 * is, if r² - (c² - v²) in (3) is less than 0, d can't be computed and the ray
 * does not intersect the sphere. If a intersection occurs, the distance from E
 * to the intersection point P is v - d.
 *
 * Returns:
 * Distance from @ray origin to the @sphere intersection point or 0 if no
 * intersection was found.
 */
float sphere_intersect (sphere_t *sphere, ray_t *ray)
{
   vector_t oe;   /* O-E vector in fig 1. */
   float    c;    /* Length of O-E vector, i.e. c in fig 1. */
   float    v;    /* Length of E-A vector, i.e. v in fig 1. */
   float    d2;   /* Computed d² value from formula (3). */

   /* Get the direction from the ray origin to the sphere center (O-E) */
   vector_sub (&oe, &sphere->center, &ray->origin);

   /* Get the length from the ray origin to the sphere center (c) */
   c = vector_length (&oe);

   /* Get the orthogonal projection of O-E vector onto the V vector,
    * i.e. the length of v. */
   v = vector_dot (&oe, &ray->dir);

   /* If the length of v is less than zero then the ray is going the opposite
    * direction and will therefore not intersect the sphere */
   if (v < 0 )
      return 0.0;

   /* Use formula (3) to check for sphere intersection */
   d2 = (sphere->radius * sphere->radius) - (c * c) + (v * v);

   /* If d2 is less than zero then d can not be computed, i.e. the ray does
    * not intersect the sphere */
   if (d2 < 0)
      return 0.0;

   /* The ray hit the sphere, return the distance from the ray origin to
    * the intersection point (P) */
   return v - sqrt(d2);
}

/**
 * sphere_set_color - Set color of sphere object.
 * @sphere: Pointer to sphere object.
 * @r:      Red component color value.
 * @g:      Green component color value.
 * @b:      Blue component color value.
 *
 * This function will set the color of a sphere object.
 *
 * Returns:
 * none.
 */
void sphere_set_color (sphere_t *sphere, int r, int g, int b)
{
   sphere->r = r;
   sphere->g = g;
   sphere->b = b;
}

/**
 * sphere_get_color - Get color of sphere object.
 * @sphere: Pointer to sphere object.
 * @r:      Pointer to integer where to return red component.
 * @g:      Pointer to integer where to return green component.
 * @b:      Pointer to integer where to return blue component.
 *
 * This function will get the color of a sphere object.
 *
 * Returns:
 * none.
 */
void sphere_get_color (sphere_t *sphere, int *r, int *g, int *b)
{
   *r = sphere->r;
   *g = sphere->g;
   *b = sphere->b;
}

/**
 * Local Variables:
 *  c-file-style: "ellemtel"
 *  indent-tabs-mode: nil
 * End:
 */
