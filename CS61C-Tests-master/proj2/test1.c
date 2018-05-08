#include <stdio.h>
#include "types.h"
#include "mips.h"

int main() {
  // Read newline-separated hex strings from STDIN
  // Prints disassembled mips code to STDOUT
  Instruction ins;
  unsigned int i;
  while (scanf("%x", &i) != EOF) {
    ins.bits = i;
    decode_instruction(ins);
  }
  return 0;
}
