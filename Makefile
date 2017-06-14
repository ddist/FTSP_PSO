CC=g++
CFLAGS=-Wall -std=c++11
OUT=pso
ODIR = .
SDIR = src
INC = -Iinc

_OBJS = main.o pso.o h_priority.o n_global.o instance.o particle.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))


$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) -c $(INC) -o $@ $< $(CFLAGS) 

$(OUT): $(OBJS) 
	$(CC) -o $(OUT) $^

.PHONY: clean

clean:
	rm -f $(ODIR)/$(OUT)
	rm -f $(ODIR)/%.o