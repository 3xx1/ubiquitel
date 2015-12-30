UBMO	= UbManager.o UbMTest.o
VUBO	= VirtualUb.o VirtualUbTest.o
UBMT	= UbMTest
VUBT	= VUbTest
FLAGS	= -std=c++11
CC	= g++

all:	ubm vub

.cpp.o:
	$(CC) -c $< $(FLAGS)

ubm:	$(UBMO)
	$(CC) -o $(UBMT) $(UBMO) $(FLAGS)
	rm -f *.o

vub:	$(VUBO)
	$(CC) -o $(VUBT) $(VUBO) $(FLAGS)
	rm -f *.o
