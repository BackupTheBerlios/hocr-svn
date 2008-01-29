
/***************************************************************************
 *            hocr.c
 *
 *  Fri Aug 12 20:13:33 2005
 *  Copyright  2005-2008  Yaacov Zamir
 *  <kzamir@walla.co.il>
 ****************************************************************************/

/*  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <math.h>

#include "hocr.h"

#ifndef TRUE
#define TRUE -1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL ((void*)0)
#endif

/**
 convert a gray pixbuf to bitmap 

 @param pix_in the input ho_pixbuf
 @param scale the scale to use
 @param no_auto_scale do not try to autoscale
 @param rotate the rotation angle to use
 @param no_auto_rotate do not try to autorotate
 @param adaptive what type of thresholding to use
 @param threshold the threshold to use 0..100
 @param a_threshold the threshold to use for adaptive thresholding 0..100
 @return newly allocated gray ho_bitmap
 */
ho_bitmap *
hocr_image_processing (const ho_pixbuf * pix_in,
  const unsigned char scale,
  const unsigned char no_auto_scale,
  double rotate,
  const unsigned char no_auto_rotate,
  const unsigned char adaptive,
  const unsigned char threshold, const unsigned char a_threshold)
{

  ho_bitmap *bitmap_out = NULL;
  ho_bitmap *bitmap_temp = NULL;
  double angle = 0.0;
  int scale_by = 0;

  /* get the raw b/w bitmap from the pixbuf */
  bitmap_temp = ho_pixbuf_to_bitmap_wrapper (pix_in,
    scale, adaptive, threshold, a_threshold);
  if (!bitmap_temp)
    return NULL;

  /* do we want to auto scale ? */
  if (!scale && !no_auto_scale)
  {
    /* get fonts size for autoscale */
    if (ho_dimentions_font_width_height_nikud (bitmap_temp, 6, 200, 6, 200))
      return NULL;

    /* if fonts are too small, re-scale image */
    if (bitmap_temp->font_height < 10)
      scale_by = 4;
    else if (bitmap_temp->font_height < 25)
      scale_by = 2;
    else
      scale_by = 1;

    if (scale_by > 1)
    {
      /* re-create bitmap */
      ho_bitmap_free (bitmap_temp);
      bitmap_temp = ho_pixbuf_to_bitmap_wrapper (pix_in,
        scale, adaptive, threshold, a_threshold);
      if (!bitmap_temp)
        return NULL;
    }
  }

  /* remove very small and very large things */
  bitmap_out =
    ho_bitmap_filter_by_size (bitmap_temp, 3, 3 * bitmap_temp->height / 4, 3,
    3 * bitmap_temp->width / 4);
  ho_bitmap_free (bitmap_temp);
  if (!bitmap_out)
    return NULL;
  
  /* rotate image */
  if (rotate)
  {
    bitmap_temp = ho_bitmap_rotate (bitmap_out, rotate);
    ho_bitmap_free (bitmap_out);
    if (!bitmap_temp)
      return NULL;
    
    bitmap_out = bitmap_temp;
  }
  else if (!no_auto_rotate)
  {
    /* get fonts size for auto angle */
    if (ho_dimentions_font_width_height_nikud (bitmap_out, 6, 200, 6, 200))
      return NULL;

    angle = ho_dimentions_get_lines_angle (bitmap_out);
    if (angle)
    {
      bitmap_temp = ho_bitmap_rotate (bitmap_out, angle);
      ho_bitmap_free (bitmap_out);
      if (!bitmap_temp)
        return NULL;

      bitmap_out = bitmap_temp;
    }
  }

  return bitmap_out;
}
