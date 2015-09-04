
local_DFT: local_DFT.c
#gcc -o -msoft-float $@ $^ -lm
	gcc -o $@ $^
#gcc $@ $^ -lm

complex_practice: complex_practice.c
	gcc -o $@ $^ -lm

getrusage: getrusage.c
	gcc -o $@ $^

clean:
	rm local_DFT
