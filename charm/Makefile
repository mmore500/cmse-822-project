EMP_DIR := ../../Empirical/source
CFLAGS_all := -Wall -I$(EMP_DIR)/
CFLAGS_nat := -O3 -DNDEBUG $(CFLAGS_all)

CHARMDIR = ../../charm/
CHARMC = $(CHARMDIR)/bin/charmc $(OPTS) -c++ gcc $(CFLAGS_nat)

default : all
all : tile

tile : main.o tile.o
	$(CHARMC) -language charm++ -o tile main.o tile.o

main.o : main.cpp main.h main.decl.h main.def.h tile.decl.h
	$(CHARMC) -o main.o main.cpp

main.decl.h main.def.h : main.ci
	$(CHARMC) main.ci

tile.o : tile.cpp tile.h tile.decl.h tile.def.h main.decl.h
	$(CHARMC) -o tile.o tile.cpp

tile.decl.h tile.def.h : tile.ci
	$(CHARMC) tile.ci

clean:
	rm -f main.decl.h main.def.h main.o
	rm -f tile.decl.h tile.def.h tile.o
	rm -f tile charmrun

localrun: clean tile
	./charmrun +p4 ./tile

remoterun: clean tile
	./charmrun ++mpiexec +p36 ./tile

send: clean
	cd .. && ./rsync.sh

.PHONY: clean send
