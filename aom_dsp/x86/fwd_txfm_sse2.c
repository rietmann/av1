/*
 * Copyright (c) 2016, Alliance for Open Media. All rights reserved
 *
 * This source code is subject to the terms of the BSD 2 Clause License and
 * the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
 * was not distributed with this source code in the LICENSE file, you can
 * obtain it at www.aomedia.org/license/software. If the Alliance for Open
 * Media Patent License 1.0 was not distributed with this source code in the
 * PATENTS file, you can obtain it at www.aomedia.org/license/patent.
 */

#include <emmintrin.h>  // SSE2

#include "./aom_config.h"
#include "./aom_dsp_rtcd.h"
#include "aom_dsp/aom_dsp_common.h"
#include "aom_dsp/x86/fwd_txfm_sse2.h"

void aom_fdct4x4_1_sse2(const int16_t *input, tran_low_t *output, int stride) {
  __m128i in0, in1;
  __m128i tmp;
  const __m128i zero = _mm_setzero_si128();
  in0 = _mm_loadl_epi64((const __m128i *)(input + 0 * stride));
  in1 = _mm_loadl_epi64((const __m128i *)(input + 1 * stride));
  in1 = _mm_unpacklo_epi64(
      in1, _mm_loadl_epi64((const __m128i *)(input + 2 * stride)));
  in0 = _mm_unpacklo_epi64(
      in0, _mm_loadl_epi64((const __m128i *)(input + 3 * stride)));

  tmp = _mm_add_epi16(in0, in1);
  in0 = _mm_unpacklo_epi16(zero, tmp);
  in1 = _mm_unpackhi_epi16(zero, tmp);
  in0 = _mm_srai_epi32(in0, 16);
  in1 = _mm_srai_epi32(in1, 16);

  tmp = _mm_add_epi32(in0, in1);
  in0 = _mm_unpacklo_epi32(tmp, zero);
  in1 = _mm_unpackhi_epi32(tmp, zero);

  tmp = _mm_add_epi32(in0, in1);
  in0 = _mm_srli_si128(tmp, 8);

  in1 = _mm_add_epi32(tmp, in0);
  in0 = _mm_slli_epi32(in1, 1);
  output[0] = (tran_low_t)_mm_cvtsi128_si32(in0);
}

void aom_fdct8x8_1_sse2(const int16_t *input, tran_low_t *output, int stride) {
  __m128i in0 = _mm_load_si128((const __m128i *)(input + 0 * stride));
  __m128i in1 = _mm_load_si128((const __m128i *)(input + 1 * stride));
  __m128i in2 = _mm_load_si128((const __m128i *)(input + 2 * stride));
  __m128i in3 = _mm_load_si128((const __m128i *)(input + 3 * stride));
  __m128i u0, u1, sum;

  u0 = _mm_add_epi16(in0, in1);
  u1 = _mm_add_epi16(in2, in3);

  in0 = _mm_load_si128((const __m128i *)(input + 4 * stride));
  in1 = _mm_load_si128((const __m128i *)(input + 5 * stride));
  in2 = _mm_load_si128((const __m128i *)(input + 6 * stride));
  in3 = _mm_load_si128((const __m128i *)(input + 7 * stride));

  sum = _mm_add_epi16(u0, u1);

  in0 = _mm_add_epi16(in0, in1);
  in2 = _mm_add_epi16(in2, in3);
  sum = _mm_add_epi16(sum, in0);

  u0 = _mm_setzero_si128();
  sum = _mm_add_epi16(sum, in2);

  in0 = _mm_unpacklo_epi16(u0, sum);
  in1 = _mm_unpackhi_epi16(u0, sum);
  in0 = _mm_srai_epi32(in0, 16);
  in1 = _mm_srai_epi32(in1, 16);

  sum = _mm_add_epi32(in0, in1);
  in0 = _mm_unpacklo_epi32(sum, u0);
  in1 = _mm_unpackhi_epi32(sum, u0);

  sum = _mm_add_epi32(in0, in1);
  in0 = _mm_srli_si128(sum, 8);

  in1 = _mm_add_epi32(sum, in0);
  output[0] = (tran_low_t)_mm_cvtsi128_si32(in1);
}

#define DCT_HIGH_BIT_DEPTH 0
#define FDCT4x4_2D aom_fdct4x4_sse2
#define FDCT8x8_2D aom_fdct8x8_sse2
#include "aom_dsp/x86/fwd_txfm_impl_sse2.h"
#undef FDCT4x4_2D
#undef FDCT8x8_2D

#undef DCT_HIGH_BIT_DEPTH
#define DCT_HIGH_BIT_DEPTH 1
#define FDCT4x4_2D aom_highbd_fdct4x4_sse2
#define FDCT8x8_2D aom_highbd_fdct8x8_sse2
#include "aom_dsp/x86/fwd_txfm_impl_sse2.h"  // NOLINT
#undef FDCT4x4_2D
#undef FDCT8x8_2D
