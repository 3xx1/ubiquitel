UBMO	= UbManager.o UbMTest.o
VUBO	= VirtualUb.o VirtualUbTest.o
UBAO	= UbManager.o ubApp.o
UBLO	= UbManager.o ubLive.o
UBTO	= UbManager.o ubTap.o RhythmPlayer.o Serial.o Tap.o
UBMT	= UbMTest
VUBT	= VUbTest
UBAP	= ubApp
UBLV	= ubLive
UBTP	= ubTap
FLAGS	= -lpthread -std=c++11
CC	= g++

all:	ubm vub uba ubl ubt

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
	rm -f ubApp.o

ubl:	$(UBLO)
	$(CC) -o $(UBLV) $(UBLO) $(FLAGS)
	rm -f ubLive.o

ubt:	$(UBTO)
	$(CC) -o $(UBTP) $(UBTO) $(FLAGS)
	rm -f *.o

clean:
	rm $(VUBT) $(UBMT) $(UBAP) $(UBLV) $(UBTP)
