SOURCES := part1.c part2.c mips.c
HEADERS := elf.h types.h mips.h

ASM_TESTS := simple insts rt3 rt13 rt25 tests

all: mips $(addprefix mipscode/, $(ASM_TESTS))

.PHONY: disasmtest runtest %_test

mipscode/%.o: mipscode/%.s
	mips-gcc -c -o $@ $<

$(addprefix mipscode/, $(ASM_TESTS)): %: %.s mipscode/mips.ld
	mips-gcc -o $(patsubst %.s, %.o, $<) -c $<
	mips-ld -o $@ $(patsubst %.s, %.o, $<) -T mipscode/mips.ld

mips: $(SOURCES) $(HEADERS)
	gcc -g -Wall -Werror -Wfatal-errors -O2 -o $@ $(SOURCES)

disasmtest: mips mipscode/insts
	./mips -d mipscode/insts > insts.dump
	@diff insts.dump mipscode/insts.dump \
          && echo "DISASSEMBLY TEST PASSED!" || echo "DISASSEMBLY TEST FAILED!"

%_test: mipscode/% mipscode/%.trace
	./mips -r $< > test.trace
	@diff test.trace $(addsuffix .trace, $(basename $<)) \
          && echo "$@ PASSED" || echo "$@ FAILED"

runtest: mips insts_test rt3_test rt13_test rt25_test tests_test
	@echo "Tests Complete"

clean:
	rm -f mips $(addsuffix .o, $(addprefix mipscode/,  $(ASM_TESTS))) \
              *.dump *.trace *.out test1 test2 mipscode/test-disasm.dump
	rm -rf mips.dSYM

cleanall: clean
	rm -f $(addprefix mipscode/,  $(ASM_TESTS))

test1: part1.c test1.c
	gcc -o test1 part1.c test1.c

test2: test2.c
	gcc -o test2 test2.c

mipscode/test-disasm.dump: mipscode/test-disasm.s
	java -jar Mars.jar a dump .text HexText mipscode/test-disasm.dump \
                           mipscode/test-disasm.s

extradisasmtest: mips test1 mipscode/test-disasm.dump mipscode/test-disasm.cmp
	cat mipscode/test-disasm.dump | ./test1 > test-disasm.out
	@diff test-disasm.out mipscode/test-disasm.cmp \
           && echo "EXTRA DISASSEMBLY TEST PASSED" \
           || echo "EXTRA DISASSEMBLY TEST FAILED"
