MPICXX = mpic++
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)
EXEC= quick

%.o :%.cpp
	$(MPICXX) -o $@ -c $<

quick : $(OBJ)
	$(MPICXX) -o $@ $^

run: quick
	OMPI_MCA_btl=self,tcp mpirun --oversubscribe -np 4 ./quick 64 && rm -rf quick *.o

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)
