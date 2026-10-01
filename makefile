PROGINAME = mcrs
IDIR =./include
CC=gcc
CFLAGS=-I$(IDIR) `pkg-config --cflags --libs gsl`
#CFLAGS=-I$(IDIR) `pkg-config --cflags --libs gsl` -ggdb -fexceptions -Wall -pg # for testing

ODIR=./src/obj
LDIR =./lib
SRCDIR=./src

LIBS=-lm

_DEPS = mainheader.h randomgen.h adattipusok.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o szomszed.o torus.o konzolra.o feltoltes.o kimenet.o eszkozok.o metab.o diffuzio.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS) | $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROGINAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR):
	mkdir -p $(ODIR)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(PROGINAME) 

.PHONY: wall
wall: $(OBJ)
	$(CC) -o $(PROGINAME) $^ $(CFLAGS) $(LIBS) -Wall
	
