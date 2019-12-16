CC := g++
CFLAGS := -std=c++11

all : quick

quick : quick.o
	$(CC) $(CFLAGS) -fopenmp -o $@  $< & ./quick 32 4

%.o : %.cpp
	$(CC) $(CFLAGS) -fopenmp -o $@ -c $<

clean:
	rm -f *.o *~ quick
