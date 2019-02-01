# change application name here (executable output name)
TARGET=RSA_CRT

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
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic -rdynamic -lgmp -lcrypto

OBJS=    RSA_CRT.o

all: $(OBJS)
	$(LD) -rdynamic -o $(TARGET) $(OBJS) $(LDFLAGS)
    
RSA_CRT.o: RSA_CRT.c
	$(CC) -c $(CCFLAGS) RSA_CRT.c $(GTKLIB) -o RSA_CRT.o
    
clean:
	rm -f *.o $(TARGET)

