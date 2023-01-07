// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.


#include <stdio.h>

// Example intrinsic OP REG, REG FOR ADD
int testiadd(long a, long b) {
  register int p1 asm("a1") = a;  // asm ("a0")
  register int p2 asm("a2") = b;  // asm ("a1")
  register int result asm("a0");
  __asm__(
      ""
      ".set rs0,8\n"
      ".set rs1,9\n"
      ".set rs2,18\n"
      ".set ra0,10\n"
      ".set ra1,11\n"
      ".set ra2,12\n"
      ".set op,0x33\n"
      ".set opf1,0\n"
      ".set opf2,0\n"
      // "add %[result],%1,%2\n"
      ".byte op|((r%[result]&1) <<7),  ((r%[result]>>1)&0xF)|(opf1<<4)|((r%1 "
      "&1) <<7),  ((r%2&0xF) << 4) | ((r%1>>1)&0xF),  ((r%2>>4)&0x1)|opf2\n"
      : [result] "=r"(result)
      : "r"(p1), "r"(p2), "[result]"(result));
  return result;
}

// Posit addition via asm
// Format is identical to integer addition
// We change opcode accordingly
int testpadd(long a, long b) {
  register int p1 asm("a1") = a;  // asm ("a0")
  register int p2 asm("a2") = b;  // asm ("a1")
  register int result asm("a0");
  __asm__(
      ""
      ".set rs0,8\n"
      ".set rs1,9\n"
      ".set rs2,18\n"
      ".set ra0,10\n"
      ".set ra1,11\n"
      ".set ra2,12\n"
      ".set op,0xb\n"
      ".set opf1,0\n"
      ".set opf2,0x6A\n"
      // "padd %[result],%1,%2\n"
      ".byte op|((r%[result]&1) <<7),  ((r%[result]>>1)&0xF)|(opf1<<4)|((r%1 "
      "&1) <<7),  ((r%2&0xF) << 4) | ((r%1>>1)&0xF),  ((r%2>>4)&0x1)|(opf2 << 1)\n"
      : [result] "=r"(result)
      : "r"(p1), "r"(p2), "[result]"(result));
  return result;
}


int main()
{
  printf("Hello World!!!!!\n\n\n");
  fflush(stdout);
  int a=0b00010000,b=0b00100000;
  __asm__ __volatile__ ("nop");
  __asm__ __volatile__ ("nop");
    int d = 0;
    d = testpadd(a,b);
    d = testpadd(a,d);
    d = testpadd(a,d);
    d = testpadd(a,d);

 
  __asm__ __volatile__ ("nop");
  __asm__ __volatile__ ("nop");

 // d = testpadd(a,d);
 // d = testpadd(a,d);*/
  printf("%x + %x = %x\n",a,b,d);
  return 0;
}
