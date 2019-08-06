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
	rm -f ./OUT/testrun/save/*.*
	#./$(PROGINAME) 6 5 0 0 0.1 0.1 0.03 1 1 1 0.15 0.2 0.2 0.2 2 1.5 1.5 1.6 testrun
	./$(PROGINAME) 300 10000 1 0 0.2 2.0 100 1 100 1 0.125 0.125 0.125 0.125 8.0 2.0 4.0 6.0 testrun
