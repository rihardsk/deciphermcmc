CC = g++
CFLAGS = -g
# LIB = 

# variables
EXE  = 
SRCS = SubstitutionTranspositionCipher.cpp
OBJS = SubstitutionTranspositionCipher.o


all: $(OBJS)

SubstitutionTranspositionCipher.o: SubstitutionTranspositionCipher.cpp SubstitutionTranspositionCipher.h
	$(CC) $(CFLAGS) -c SubstitutionTranspositionCipher.cpp

clean:
	@echo "Cleaning files..."
	rm -f *.o
