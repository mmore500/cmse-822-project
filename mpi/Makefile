all: main.out

main.out: main.cpp
	mpic++ -o main.out main.cpp

run: main.out
	mpirun -n 4 main.out

clean:
	rm -f *.out

.PHONY: clean run
