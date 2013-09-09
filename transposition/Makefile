CC = g++
CFLAGS = -g
# LIB = 

# variables
EXE  = 
SRCS = TranspositionCipher.cpp
OBJS = TranspositionCipher.o


all: $(OBJS)

TranspositionCipher.o: TranspositionCipher.cpp TranspositionCipher.h
	$(CC) $(CFLAGS) -c TranspositionCipher.cpp

clean:
	@echo "Cleaning files..."
	rm -f *.o
