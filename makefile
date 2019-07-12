PROGINAME = mcrs
IDIR =./include
CC=gcc
CFLAGS=-I$(IDIR) `pkg-config --cflags --libs gsl`

ODIR=./src/obj
LDIR =./lib
SRCDIR=./src

LIBS=-lm

_DEPS = mainheader.h randomgen.h adattipusok.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o szomszed.o torus.o konzolra.o feltoltes.o kimenet.o eszkozok.o metab.o diffuzio.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_OBJ2 = test.o
OBJ2 = $(patsubst %,$(ODIR)/%,$(_OBJ2))


$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROGINAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

	
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~  

.PHONY: test
	
test: $(OBJ2)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

	
.PHONY: wall
wall: $(OBJ)
	$(CC) -o $(PROGINAME) $^ $(CFLAGS) $(LIBS) -Wall
	
.PHONY: run
run: 
	rm -f ./OUT/testrun/*.*
	./$(PROGINAME) 10 30 0 0 0.1 0.1 0 1 0 1 0.2 0.15 0.15 0.15 2 1.5 1.5 1.6 testrun
