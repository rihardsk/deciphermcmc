CC = g++
CFLAGS = -g
# LIB = 

# variables
EXE  = 
SRCS = ArrayUtil.cpp ConvUtil.cpp
OBJS = ArrayUtil.o ConvUtil.o MathUtil.o RandUtil.o SortUtil.o SysUtil.o TextFileUtil.o


all: $(OBJS)

ArrayUtil.o: ArrayUtil.cpp ArrayUtil.h
	$(CC) $(CFLAGS) -c ArrayUtil.cpp

ConvUtil.o: ConvUtil.cpp ConvUtil.h
	$(CC) $(CFLAGS) -c ConvUtil.cpp

MathUtil.o: MathUtil.cpp MathUtil.h
	$(CC) $(CFLAGS) -c MathUtil.cpp

RandUtil.o: RandUtil.cpp RandUtil.h
	$(CC) $(CFLAGS) -c RandUtil.cpp


SortUtil.o: SortUtil.cpp SortUtil.h
	$(CC) $(CFLAGS) -c SortUtil.cpp

SysUtil.o: SysUtil.cpp SysUtil.h
	$(CC) $(CFLAGS) -c SysUtil.cpp

TextFileUtil.o: TextFileUtil.cpp TextFileUtil.h
	$(CC) $(CFLAGS) -c TextFileUtil.cpp

clean:
	@echo "Cleaning files..."
	rm -f *.o
