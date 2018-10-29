CHARMDIR = ../../charm/
CHARMC = $(CHARMDIR)/bin/charmc $(OPTS)

default : all
all : hello

hello : main.o hello.o
	$(CHARMC) -language charm++ -o hello main.o hello.o

main.o : main.cpp main.h main.decl.h main.def.h hello.decl.h
	$(CHARMC) -o main.o main.cpp

main.decl.h main.def.h : main.ci
	$(CHARMC) main.ci

hello.o : hello.cpp hello.h hello.decl.h hello.def.h main.decl.h
	$(CHARMC) -o hello.o hello.cpp

hello.decl.h hello.def.h : hello.ci
	$(CHARMC) hello.ci

clean:
	rm -f main.decl.h main.def.h main.o
	rm -f hello.decl.h hello.def.h hello.o
	rm -f hello charmrun

localrun: clean hello
	./charmrun +p4 ./hello 5 5

remoterun: clean hello
	./charmrun ++mpiexec +p10 ./hello 5 5

send: clean
	cd .. && ./rsync.sh

.PHONY: clean send
