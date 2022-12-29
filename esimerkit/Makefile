
-include config.inc
LIBPATH=..

CC=cc
LD=$(CC)
RM=rm -f

CFLAGS=-Og -g -Wall -Werror -Wpedantic -ansi -pedantic
LFLAGS=

HEADERS=../koskut.h ../koskutio.h

DEFINES=-DKOSKUT_CONVERT=1

PROGS=sana veikkaa

.PHONY: all clean

all: $(PROGS) 

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(DEFINES) -I$(LIBPATH) -c -o $@ $<

sana: $(LIBFILES) ../k_cu8.o ../k_aste.o ../k_nvart.o ../k_ntaiv.o sana.o $(HEADERS)
	$(LD) -o $@ $^ $(LFLAGS)

veikkaa: $(LIBFILES) ../k_cu8.o ../k_aste.o ../k_nvart.o ../k_ntaiv.o ../k_nveik.o veikkaa.o $(HEADERS)
	$(LD) -o $@ $^ $(LFLAGS)

clean:
	$(RM) ../*.o *.o $(PROGS)
