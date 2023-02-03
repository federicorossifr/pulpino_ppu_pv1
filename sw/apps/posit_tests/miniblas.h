#include "intrinsics.h"

posit_t dot(posit_t *a, posit_t *b, int n) {
  posit_t result = 0;
  for (int i = 0; i < n; i++) {
    result = padd(result, pmul(a[i], b[i]));
  }
  return result;
}

void gemm(posit_t *a, posit_t *b, posit_t *c, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      posit_t sum = 0;
      for (int k = 0; k < n; k++) {
        sum = padd(sum, pmul(a[i * n + k], b[k * n + j]));
      }
      c[i * n + j] = sum;
    }
  }
}

void conv3x3(posit_t *a, posit_t *f, posit_t *c, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      posit_t sum = 0;
      for (int k = 0; k < 3; k++) {
        for (int l = 0; l < 3; l++) {
          sum = padd(sum, pmul(a[(i + k) * n + j + l], f[k * 3 + l]));
        }
      }
      c[i * n + j] = sum;
    }
  }
}

void avgpool4x4(posit_t *a, posit_t *c, int n) {
  for (int i = 0; i < n - 4; i++) {
    for (int j = 0; j < n - 4; j++) {
      posit_t sum = 0;
      for (int k = 0; k < 4; k++) {
        for (int l = 0; l < 4; l++) {
          sum = padd(sum, a[(i + k) * n + j + l]);
        }
      }
      c[i * n + j] = pdiv(sum, fcvt_p(0x41800000));
    }
  }
}