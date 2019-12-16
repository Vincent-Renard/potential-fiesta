CC := g++
CFLAGS := -std=c++11

all : quick

quick : quick.o fonctions.o
	$(CC) $(CFLAGS) -fopenmp -o $@ $^

%.o : %.cpp
	$(CC) $(CFLAGS) -fopenmp -o $@ -c $<

clean:
	rm -f *.o *~ quick
