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