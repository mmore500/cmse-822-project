EMP_DIR := ../../Empirical/source
CFLAGS_all := -Wall -I$(EMP_DIR)/
CFLAGS_nat := -O3 -DNDEBUG $(CFLAGS_all)

all: main.out

main.out: main.cpp
	mpic++ $(CFLAGS_all) -O3 --std=c++14 -o main.out main.cpp

run: main.out
	mpirun -n 16 main.out

clean:
	rm -f *.out

.PHONY: clean run
