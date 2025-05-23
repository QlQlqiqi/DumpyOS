#include <x86intrin.h>
#define PORTABLE_ALIGN32 __attribute__((aligned(32)))
#define PORTABLE_ALIGN64 __attribute__((aligned(64)))

float L2SqrSIMD16ExtAVX512Float(const void *pVect1v, const void *pVect2v, const void *qty_ptr) {
  float *pVect1 = (float *)pVect1v;
  float *pVect2 = (float *)pVect2v;
  size_t qty = *((size_t *)qty_ptr);
  float PORTABLE_ALIGN64 TmpRes[16];
  size_t qty16 = qty >> 4;

  const float *pEnd1 = pVect1 + (qty16 << 4);

  __m512 diff, v1, v2;
  __m512 sum = _mm512_set1_ps(0);

  while (pVect1 < pEnd1) {
    v1 = _mm512_loadu_ps(pVect1);
    pVect1 += 16;
    v2 = _mm512_loadu_ps(pVect2);
    pVect2 += 16;
    diff = _mm512_sub_ps(v1, v2);
    // sum = _mm512_fmadd_ps(diff, diff, sum);
    sum = _mm512_add_ps(sum, _mm512_mul_ps(diff, diff));
  }

  _mm512_store_ps(TmpRes, sum);
  float res = TmpRes[0] + TmpRes[1] + TmpRes[2] + TmpRes[3] + TmpRes[4] + TmpRes[5] + TmpRes[6] +
              TmpRes[7] + TmpRes[8] + TmpRes[9] + TmpRes[10] + TmpRes[11] + TmpRes[12] +
              TmpRes[13] + TmpRes[14] + TmpRes[15];

  return (res);
}
