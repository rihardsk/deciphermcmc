CC = g++
CFLAGS = -g
# LIB = 

# variables
EXE  = 
SRCS = SubstitutionCipher.cpp
OBJS = SubstitutionCipher.o PairSubstitutionCipher.o


all: $(OBJS)

SubstitutionCipher.o: SubstitutionCipher.cpp SubstitutionCipher.h
	$(CC) $(CFLAGS) -c SubstitutionCipher.cpp

PairSubstitutionCipher.o: PairSubstitutionCipher.cpp PairSubstitutionCipher.h
	$(CC) $(CFLAGS) -c PairSubstitutionCipher.cpp

clean:
	@echo "Cleaning files..."
	rm -f *.o
