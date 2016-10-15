UBMO	= UbManager.o UbMTest.o
VUBO	= VirtualUb.o VirtualUbTest.o
UBAO	= UbManager.o ubApp.o
UBMT	= UbMTest
VUBT	= VUbTest
UBAP	= ubApp
FLAGS	= -lpthread -std=c++11
CC	= g++

all:	ubm vub  uba

.cpp.o:
	$(CC) -c $< $(FLAGS)

ubm:	$(UBMO)
	$(CC) -o $(UBMT) $(UBMO) $(FLAGS)
	rm -f UbMTest.o

vub:	$(VUBO)
	$(CC) -o $(VUBT) $(VUBO) $(FLAGS)
	rm -f $(VUBO)

uba:	$(UBAO)
	$(CC) -o $(UBAP) $(UBAO) $(FLAGS)
	rm -f *.o
clean:
	rm $(VUBT) $(UBMT) $(UBAP)
