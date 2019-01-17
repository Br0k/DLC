# change application name here (executable output name)
TARGET=main

# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall

PTHREAD=-pthread

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic -rdynamic -lgmp

OBJS=    main.o

all: $(OBJS)
	$(LD) -rdynamic -o $(TARGET) $(OBJS) $(LDFLAGS)
    
main.o: main.c
	$(CC) -c $(CCFLAGS) main.c $(GTKLIB) -o main.o
    
clean:
	rm -f *.o $(TARGET)

