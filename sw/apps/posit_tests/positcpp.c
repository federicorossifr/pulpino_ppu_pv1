#include "miniblas.h"

const unsigned int N = 8;



int main()
{
  printf("Hello World from Posits on Pulpino\n");
  fflush(stdout);

  posit_t A[N*N];
  posit_t B[N*N];
  posit_t C[N*N];

  int ff[] = {0x40000000, 0x3fc00000};
  //posit_t pp = fcvt_p(ff);
  //printf("pp = %x\n", pp);
  int ffa = fcvt_p(0);
  printf("ffa = %x\n", ffa);
  for (unsigned int i=0; i<N; i++) {
    for (unsigned int j=0; j<N; j++) {
      A[i*N+j] = pdiv((posit_t)rand(),fcvt_p(0x40000000));
      B[i*N+j] = fcvt_p(ff[rand()%2]);
    }
  }

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
  return 0;
}