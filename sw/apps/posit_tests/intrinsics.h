#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

typedef int posit_t;

typedef struct {
    int8_t p1;
    int8_t p2;
    int8_t p3;
    int8_t p4;
} packed_p4;

typedef struct {
    int16_t p1;
    int16_t p2;
} packed_p2;

typedef union {
    int32_t raw;
    packed_p4 p4;
    packed_p2 p2;
} packed_posit;


// Posit addition via asm
// Format is identical to integer addition
// We change opcode accordingly
posit_t padd(long a, long b) {
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

// Posit addition via asm
// Format is identical to integer addition
// We change opcode accordingly
posit_t psub(long a, long b) {
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
      ".set opf1,1\n"
      ".set opf2,0x6A\n"
      // "padd %[result],%1,%2\n"
      ".byte op|((r%[result]&1) <<7),  ((r%[result]>>1)&0xF)|(opf1<<4)|((r%1 "
      "&1) <<7),  ((r%2&0xF) << 4) | ((r%1>>1)&0xF),  ((r%2>>4)&0x1)|(opf2 << 1)\n"
      : [result] "=r"(result)
      : "r"(p1), "r"(p2), "[result]"(result));
  return result;
}

// Posit addition via asm
// Format is identical to integer addition
// We change opcode accordingly
posit_t pmul(long a, long b) {
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
      ".set opf1,2\n"
      ".set opf2,0x6A\n"
      // "padd %[result],%1,%2\n"
      ".byte op|((r%[result]&1) <<7),  ((r%[result]>>1)&0xF)|(opf1<<4)|((r%1 "
      "&1) <<7),  ((r%2&0xF) << 4) | ((r%1>>1)&0xF),  ((r%2>>4)&0x1)|(opf2 << 1)\n"
      : [result] "=r"(result)
      : "r"(p1), "r"(p2), "[result]"(result));
  return result;
}

// Posit addition via asm
// Format is identical to integer addition
// We change opcode accordingly
posit_t pdiv(long a, long b) {
  register int p1 asm("a1") = a;  // asm ("a0")
  register int p2 asm("a2") = b;  // asm ("a1")
  register int result asm("a0");
  __asm__(
      ""
      ".set op,0xb\n"
      ".set opf1,4\n"
      ".set opf2,0x6A\n"
      // "padd %[result],%1,%2\n"
      ".byte op|((r%[result]&1) <<7),  ((r%[result]>>1)&0xF)|(opf1<<4)|((r%1 "
      "&1) <<7),  ((r%2&0xF) << 4) | ((r%1>>1)&0xF),  ((r%2>>4)&0x1)|(opf2 << 1)\n"
      : [result] "=r"(result)
      : "r"(p1), "r"(p2), "[result]"(result));
  return result;
}

posit_t fcvt_p(int f) {
  register int p1 asm("a1") = f; 
  register int result asm("a0");
  __asm__(
      ""
      ".set op,0xb\n"
      ".set opf1,0\n"
      ".set opf2,0x68\n"
      // "pcvt_f %[result],%1\n"
      ".byte op|((r%[result]&1) <<7),  ((r%[result]>>1)&0xF)|(opf1<<4)|((r%1 "
      "&1) <<7),  ((r%1>>1)&0xF),  opf2 << 1\n"
      : [result] "=r"(result)
      : "r"(p1), "[result]"(result));
  return result;
}

posit_t pcvt_f(int p) {
  register int p1 asm("a1") = p; 
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
      ".set opf2,0x69\n"
      // "pcvt_f %[result],%1\n"
      ".byte op|((r%[result]&1) <<7),  ((r%[result]>>1)&0xF)|(opf1<<4)|((r%1 "
      "&1) <<7),  ((r%1>>1)&0xF),  opf2 << 1\n"
      : [result] "=r"(result)
      : "r"(p1), "[result]"(result));
  return result;
}