#include "intrinsics.h"
#include <stdio.h>


int main()
{
  printf("Hello World from Posits on Pulpino\n");
  fflush(stdout);
  int a=0x10,b=0x20;
  __asm__ __volatile__ ("nop");
  __asm__ __volatile__ ("nop");
    int d = 0;
    d = testpadd(a,b);

  __asm__ __volatile__ ("nop");
  __asm__ __volatile__ ("nop");

  printf("Posit(0x%x) + Posit(0x%x) = Posit(0x%x)\n",a,b,d);

}