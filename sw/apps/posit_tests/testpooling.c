#include "miniblas.h"

const unsigned int N = 8;



int main()
{
  printf("Hello World from Posits on Pulpino\n");
  fflush(stdout);
  
  
  posit_t A[N*N];
  posit_t B[N*N];
  posit_t C[N*N];

  for (unsigned int i=0; i<N; i++) {
    for (unsigned int j=0; j<N; j++) {
      A[i*N+j] = (posit_t)(rand());
      B[i*N+j] = (posit_t)(rand());
    }
  }

  // Compute A,B convolution
  printf("Testing convolution...\n");
  avgpool4x4(&A[0],&C[0],N);
  printf("Done\n");
  return 0;
}