
-include config.inc
LIBPATH=..

CC=cc
LD=$(CC)
RM=rm -f

CFLAGS=-Og -g -Wall -Werror -Wpedantic -ansi -pedantic
LFLAGS=

HEADERS=../koskut.h ../koskutio.h

DEFINES=-DKOSKUT_CONVERT=1

PROGS=tnomini veikkaa tverbi

.PHONY: all clean

all: $(PROGS) 

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(DEFINES) -I$(LIBPATH) -c -o $@ $<

tnomini: $(LIBFILES) ../k_cu8.o ../k_aste.o ../k_nvart.o ../k_ntaiv.o tnomini.o
	$(LD) -o $@ $^ $(LFLAGS)

veikkaa: $(LIBFILES) ../k_cu8.o ../k_aste.o ../k_nvart.o ../k_ntaiv.o ../k_nveik.o veikkaa.o
	$(LD) -o $@  $^ $(LFLAGS)

tverbi: $(LIBFILES) ../k_cu8.o ../k_aste.o ../k_vvart.o ../k_vtaiv.o tverbi.o
	$(LD) -o $@ $^ $(LFLAGS)


clean:
	$(RM) ../*.o *.o $(PROGS)
