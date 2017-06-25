#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <x86intrin.h>

int main() {
  __m256d e = (__m256d){12.1, 54.3, 6.1, 7.8};
  __m256d f = (__m256d){1.3, 4.5, 3.4, 7.6};
  __m256d result = _mm256_mul_pd(e, f);
  printf("%f %f %f %f\n", result[0], result[1], result[2], result[3]);
}