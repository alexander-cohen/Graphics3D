#include <x86intrin.h>
#include <stdio.h>
#include <stdlib.h>


int main() {
  __m256i a,b,c;
  a = _mm256_setr_epi32(1,2,3,4,5,6,7,8);
  b = _mm256_setr_epi32(9,10,11,12,13,14,15,16);
  c = a + b;
  int *p = (int *)&c;
  printf("%d %d %d %d %d %d %d %d", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
 
}