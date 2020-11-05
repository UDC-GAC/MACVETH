POLYBENCH_FLAGS= -g -DPOLYBENCH_$(TYPE) -I. -L/share/apps/papi/gnu8/6.0.0/lib -lpapi -I ../utilities

ifeq ($(VECT),fast)
	VF= -Ofast
else
	VECT=nofast
	VF= -O3
endif

ifeq ($(COMP),icc)
	CC=icc
	CXX=icpc
	VFLAGS= $(VF) -vec-threshold0 -qoverride-limits -march=native -mfma
	SUFFIX=_icc
else
	COMP=gcc
	CC=gcc
	CXX=g++
	#VFLAGS= $(VF) -march=native -mfma -ftree-vectorize -ftree-loop-vectorize
	#-fvect-cost-model=unlimited -fsimd-cost-model=unlimited
	#-fprefetch-loop-arrays -unroll0
	VFLAGS= $(VF) -march=native -mfma -ftree-vectorize -fvect-cost-model=unlimited -fsimd-cost-model=unlimited -fprefetch-loop-arrays
	VFLAGSMV= $(VF) -march=native -mfma -ftree-vectorize 
endif

NAME=$(COMP)_$(VECT)_$(BINARY_NAME)_$(TYPE)

.PHONY: clean

all: $(BINARY_NAME) run

$(BINARY_NAME):
	$(CC) -O3 -c $(POLYBENCH_FLAGS) ../utilities/polybench.c
	$(CXX) $(VFLAGS) $(MAIN_SRC) $(TARGET).cc polybench.o $(POLYBENCH_FLAGS) -o $(NAME).o
	$(MVPATH)macveth $(MVFLAGS) $(TARGET).cc -o $(BINARY_NAME)/$(TARGET)_macveth$(SUFFIX).cc
	$(CXX) $(VFLAGS) $(MAIN_SRC) $(TARGET)_macveth$(SUFFIX).cc polybench.o $(POLYBENCH_FLAGS) -o $(NAME)_macveth.o

$(BINARY_NAME)_correct:
	$(CC) -O3 -c $(POLYBENCH_FLAGS) -DPOLYBENCH_DUMP_ARRAYS ../utilities/polybench.c
	$(CXX) $(VFLAGS) $(MAIN_SRC) $(TARGET).cc polybench.o $(POLYBENCH_FLAGS) -DPOLYBENCH_DUMP_ARRAYS -o $(NAME)_dump.o
	$(CXX) $(VFLAGSMV) $(MAIN_SRC) $(TARGET)_macveth$(SUFFIX).cc polybench.o $(POLYBENCH_FLAGS) -DPOLYBENCH_DUMP_ARRAYS -o $(NAME)_macveth_dump.o

clean:
	rm *.o *.optrpt 2> /dev/null

run:
	./$(NAME).o > $(NAME).csv
	./$(NAME)_macveth.o > $(NAME)_macveth.csv

$(BINARY_NAME)_check_dump:
	./$(NAME)_dump.o > /dev/null 2> $(NAME).dump
	./$(NAME)_macveth_dump.o > /dev/null 2> $(NAME)_macveth.dump
	diff $(NAME).dump $(NAME)_macveth.dump
