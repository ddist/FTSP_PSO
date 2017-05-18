CC=g++
CFLAGS=-Wall -std=c++11
OUT=ftsp
ODIR = .
SDIR = src
INC = -Iinc

_OBJS = main.o instance.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))


$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) -c $(INC) -o $@ $< $(CFLAGS) 

$(OUT): $(OBJS) 
	$(CC) -o $(OUT) $^

.PHONY: clean

clean:
	rm -f $(ODIR)/$(OUT)