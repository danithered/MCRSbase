IDIR =./include
CC=gcc
CFLAGS=-I$(IDIR) `pkg-config --cflags --libs gsl`

ODIR=./src/obj
LDIR =./lib
SRCDIR=./src

LIBS=-lm

_DEPS = mainheader.h randomgen.h adattipusok.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o szomszed.o torus.o konzolra.o feltoltes.o kimenet.o eszkozok.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_OBJ2 = test.o function.o feltoltes.o konzolra.o szomszed.o torus.o bitmuveletek.o statisztika.o save.o kimenet.o metab.o load.o
OBJ2 = $(patsubst %,$(ODIR)/%,$(_OBJ2))


$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mcrs: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

	
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~  

.PHONY: test
	
test: $(OBJ2)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

	
.PHONY: wall
wall: $(OBJ)
	$(CC) -o progi $^ $(CFLAGS) $(LIBS) -Wall
	
.PHONY: run
run: 
	rm -f ./OUT/testrun/*.*
	./mcrs 10 100 0 0 0.1 0.2 0.2 0.2 testrun
