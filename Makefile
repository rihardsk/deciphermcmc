CC = g++
CFLAGS = -g
# LIB = 

# variables
EXE  = deciphermcmc
SRCS = deciphermcmc.cpp
OBJS = deciphermcmc.o common/ArrayUtil.o common/ConvUtil.o common/MathUtil.o common/RandUtil.o common/SortUtil.o common/SysUtil.o common/TextFileUtil.o substitution/PairSubstitutionCipher.o substitution/SubstitutionCipher.o transposition/TranspositionCipher.o substitution-transpotition/SubstitutionTranspositionCipher.o
DIRS = common substitution transposition ciphers

all: $(EXE)


$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $(EXE)

deciphermcmc.o: deciphermcmc.cpp
	$(CC) $(CFLAGS) -c deciphermcmc.cpp

clean:
	@echo "Cleaning files..."
	rm -Rf $(OBJS) $(EXE)
