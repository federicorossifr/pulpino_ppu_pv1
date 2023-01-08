#include "miniblas.h"

const unsigned int N = 2;


int main()
{
  printf("Hello World from Posits on Pulpino\n");
  fflush(stdout);

  posit_t A[N*N];
  posit_t B[N*N];
  posit_t C[N*N];

  // Convert a float to a posit
  int f = 0x3c000000;
  int p = 0x80;
  int f_back = pcvt_f(p);
  posit_t p_back = fcvt_p(f);
  printf("float(0x%x) => posit (0x%x)\n",f,p_back);
  printf("posit(0x%x) => float(0x%x)\n",p,f_back);

  // Initialize matrices A and B with random values in 0,0xFF but discards 0x80
  for (unsigned int i=0; i<N; i++) {
    for (unsigned int j=0; j<N; j++) {
      A[i*N+j] = (posit_t)rand() & 0x7F;
      B[i*N+j] = (posit_t)rand() & 0x7F;
    }
  }
/*
  // Compute A,B dot product
  printf("Testing dot...\n");
  dot(&A[0],&B[0],N*N);
  printf("Done\n");

  // Compute A,B matrix product
  printf("Testing matrix multiplication...\n");
  gemm(&A[0],&B[0],&C[0],N);
  printf("Done\n");

  // Compute A,B convolution
  // printf("Testing convolution...\n");
  // conv3x3(&A[0],&B[0],&C[0],N);
  // printf("Done\n");
*/
  return 0;
}