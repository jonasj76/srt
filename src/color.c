/**
 * color.c - Color object class.
 *
 * Copyright (c) 2013, Jonas Johansson <jonasj76@gmail.com>
 *
 * Description:
 * This class defines the color of an object.
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

#include "color.h"

/**
 * color_set - Set color of an object.
 * @obj: Pointer to object color
 * @r:   Red component color value or -1 to not modify
 * @g:   Green component color value or -1 to not modify
 * @b:   Blue component color value or -1 to not modify
 *
 * This function will set the color of an object.
 * If the color value is set to -1 that component will not be changed. This can
 * be useful if only a specific component shall be changed and the other
 * untouched.
 *
 * Returns:
 * none.
 */
void color_set (color_t* obj, int r, int g, int b)
{
   if (r>0) obj->r = r;
   if (g>0) obj->g = g;
   if (b>0) obj->b = b;
}

/**
 * color_get - Get color of an object.
 * @obj: Pointer to object color
 * @r:   Pointer to integer where to return red component
 * @g:   Pointer to integer where to return green component
 * @b:   Pointer to integer where to return blue component
 *
 * This function will get the color of an object.
 *
 * Returns:
 * none.
 */
void color_get (color_t* obj, int *r, int *g, int *b)
{
   *r = obj->r;
   *g = obj->g;
   *b = obj->b;
}

/**
 * Local Variables:
 *  c-file-style: "ellemtel"
 *  indent-tabs-mode: nil
 * End:
 */
