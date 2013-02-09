/**
 * tga.c - TGA image class.
 *
 * Copyright (c) 2011, Jonas Johansson <jonasj76@gmail.com>
 *
 * Description:
 * TGA image class.
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
#include <assert.h>
#include <errno.h>
#include <err.h>

/**
 * tga_write - Creates a TGA image file.
 * @fname:  Filname for image file.
 * @width:  Image width.
 * @height: Image height.
 * @image:  Pointer to image buffer.
 *
 * This funciton will create a TGA image file named @fname. The image data is
 * read from @image which should be an array of @width x @height pixel
 * elements, where each pixel is represented by a a 24 bit color value,
 * described in three bytes for the red, green and blue component.
 *
 * Returns:
 * POSIX OK (zero) or non-zero on error. %errno is set on error.
 */
int tga_write (const char *fname, int width, int height, uint8_t *image)
{
   FILE *fp = NULL;

   assert (image);

   /* Open file for writing */
   fp = fopen (fname, "w");

   /* Check that open was successful */
   if (!fp)
   {
      warn ("%s:%d", __FUNCTION__, __LINE__);
      return 1;
   }
   
   /* Image ID length */
   fputc (0, fp);
   /* Color map type */
   fputc (0, fp);              /* 0:       No color map included with this image.
                                * 1:       A color map is included with this image.
                                * 2-127:   Reserved by Truevision.
                                * 128-255: Available for developer use. */
   /* Image type */
   fputc (2, fp);              /* 0:  No image date present.
                                * 1:  Uncompressed, color-mapped image.
                                * 2:  Uncompressed, true-color image.
                                * 3:  Uncompressed, black-and-white image.
                                * 9:  Run-length encoded, color-mapped image.
                                * 10: Run-length encoded, true-color image.
                                * 11: Run-length encoded, black-and-white image. */
   /* Color map specification */
   fputc (0, fp);              /* First entry index (lo): Offset into the color map table */
   fputc (0, fp);              /*                   (hi)                                  */
   fputc (0, fp);              /* Color map length (lo): Number of entries                */
   fputc (0, fp);              /*                  (hi)                                   */
   fputc (0, fp);              /* Color map entry size: Number of bits per pixel          */
   /* Image specification */
   fputc (0, fp);              /* X-origin (lo): Absolute coordinate of lower-left corner */
   fputc (0, fp);              /*          (hi)                                           */
   fputc (0, fp);              /* Y-origin (lo): As for X-origin                          */
   fputc (0, fp);              /*          (hi)                                           */
   fputc (width % 256, fp);    /* Image width (lo): Width in pixels                       */
   fputc (width / 256, fp);    /*             (hi)                                        */
   fputc (height % 256, fp);   /* Image height (lo): Height in pixels                     */
   fputc (height / 256, fp);   /*              (hi)                                       */
   fputc (24, fp);             /* Pixel depth: Bits per pixel                             */
   fputc (0, fp);              /* Image descriptor: Bits 0-3: Alpha channel depth
                                *                   Bits 4-5: Pixel transfer order
                                *                   Bits 6-7: Unused, must be zero        */

   /* RGB to BGR */
   for (int i=0; i < (width * height * 3); i += 3)
   {
      uint8_t c;

      c          = image[i];
      image[i]   = image[i+2];
      image[i+2] = c;
   }

   /* Write the image */
   fwrite (image, width * height * 3, 1, fp);

   /* Close file */
   fclose (fp);

   return 0;
}

/**
 * Local Variables:
 *  c-file-style: "ellemtel"
 *  indent-tabs-mode: nil
 * End:
 */
