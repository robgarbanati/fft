

local_DFT: local_DFT.c exponent_table.h
	gcc -o $@ $^ -lm

complex_practice: complex_practice.c
	gcc -o $@ $^ -lm

clean:
	rm local_DFT
