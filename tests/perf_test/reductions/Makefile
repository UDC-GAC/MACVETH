BINARY_NAME=reductions
MAIN_SRC=reductions.cc
TARGET=reductions

include ../MakeFlags.mk

NAME=$(COMP)_$(VECT)_$(BINARY_NAME)

.PHONY: clean

all: $(TARGET)

$(TARGET):
	$(CC) -O3 -c $(POLYBENCH_FLAGS) ../utilities/polybench.c
	$(CXX) $(VFLAGS) $(MAIN_SRC) redux1d.cc polybench.o $(POLYBENCH_FLAGS) -o $(NAME).o
	$(CXX) $(VFLAGSMV) $(MAIN_SRC) redux1d_macveth$(SUFFIX).cc polybench.o $(POLYBENCH_FLAGS) -o $(NAME)_macveth.o

clean:
	rm *.o *.optrpt

run:
	./$(NAME).o > $(NAME).csv
	./$(NAME)_macveth.o > $(NAME)_macveth.csv
