/*****************************************************************************
 * Copyright (C) 2013 x265 project
 *
 * Authors: Praveen Tiwari <praveen@multicorewareinc.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at licensing@multicorewareinc.com.
 *****************************************************************************/

#include "TLibCommon/TComTrQuant.h"
#include "vectorclass.h"


#ifdef ENABLE_VECTOR
void fastInverseDst (Short *tmp,Short *block,Int shift)  // input tmp, output block
{ 
  Int rnd_factor = 1<<(shift-1);
 
  Vec8s tmp0, tmp1;
  tmp0.load (tmp);
  tmp1.load (tmp + 8);
  
  Vec4i c0 = extend_low (tmp0);
  Vec4i c1 = extend_high (tmp0);
  Vec4i c2 = extend_low (tmp1);
  Vec4i c3 = extend_high (tmp1);

  Vec4i c0_total = c0 + c2;
  Vec4i c1_total = c2 + c3;
  Vec4i c2_total = c0 - c3;
  Vec4i c3_total = 74 * c1;

  Vec4i c4 = (c0 - c2 + c3);

  Vec4i c0_final = ( 29 * c0_total + 55 * c1_total + c3_total + rnd_factor ) >> shift;
  Vec4i c1_final = ( 55 * c2_total - 29 * c1_total + c3_total + rnd_factor ) >> shift;
  Vec4i c2_final = (74 * c4 + rnd_factor) >> shift;
  Vec4i c3_final = (55 * c0_total + 29 * c2_total - c3_total + rnd_factor) >> shift;

  Vec4i first_arg (-32768);
  Vec4i second_arg (32767);

  Vec4i max_number1 = max (first_arg, c0_final);
  Vec4i mid_number1 = min (max_number1, second_arg);
  Vec4i max_number2 = max (first_arg, c1_final);
  Vec4i mid_number2 = min (max_number2, second_arg);
  Vec8s half = compress (mid_number1, mid_number2);

  block[0] = half[0];
  block[4] = half[1];
  block[8] = half[2];
  block[12] = half[3];
  block[1] = half[4];
  block[5] = half[5];
  block[9] = half[6];
  block[13] = half[7];

  max_number1 = max (first_arg, c2_final);
  mid_number1 = min (max_number1, second_arg);
  max_number2 = max (first_arg, c3_final);
  mid_number2 = min (max_number2, second_arg);
  half = compress (mid_number1, mid_number2);

  block[2] = half[0];
  block[6] = half[1];
  block[10] = half[2];
  block[14] = half[3];
  block[3] = half[4];
  block[7] = half[5];
  block[11] = half[6];
  block[15] = half[7];
}
#endif


