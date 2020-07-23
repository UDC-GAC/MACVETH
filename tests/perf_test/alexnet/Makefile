BINARY_NAME=alexnet
MAIN_SRC=alexnet.cc
TARGET=layers

include ../MakeFlags.mk

NAME=$(COMP)_$(VECT)_$(BINARY_NAME)

.PHONY: clean

all: alexnet run

alexnet:
	$(CC) -O3 -c $(POLYBENCH_FLAGS) ../utilities/polybench.c
	$(CXX) $(VFLAGS) $(MAIN_SRC) $(TARGET).cc polybench.o $(POLYBENCH_FLAGS) -o $(NAME).o
	$(CXX) $(VFLAGS) $(MAIN_SRC) $(TARGET)_macveth$(SUFFIX).cc polybench.o $(POLYBENCH_FLAGS) -o $(NAME)_macveth.o

correct:
	$(CC) -O3 -c $(POLYBENCH_FLAGS) -DPOLYBENCH_DUMP_ARRAYS ../utilities/polybench.c
	$(CXX) $(VFLAGS) $(MAIN_SRC) $(TARGET).cc polybench.o $(POLYBENCH_FLAGS) -DPOLYBENCH_DUMP_ARRAYS -o $(NAME)_dump.o
	$(CXX) $(VFLAGSMV) $(MAIN_SRC) $(TARGET)_macveth$(SUFFIX).cc polybench.o $(POLYBENCH_FLAGS) -DPOLYBENCH_DUMP_ARRAYS -o $(NAME)_macveth_dump.o

clean:
	rm *.o *.optrpt

run:
	./$(NAME).o > $(NAME).csv
	./$(NAME)_macveth.o > $(NAME)_macveth.csv

check_dump:
	./$(NAME)_dump.o 2> $(NAME).dump
	./$(NAME)_macveth_dump.o 2> $(NAME)_macveth.dump
	diff $(NAME).dump $(NAME)_macveth.dump
