POLYBENCH_FLAGS=-DPOLYBENCH_TIME -I. -L/share/apps/papi/gnu8/6.0.0/lib -lpapi -I ../utilities
BINARY_NAME=alexnet

ifeq ($(VECT),fast)
	VF= -Ofast
else
	VECT=nofast
	VF= -O3
endif

ifeq ($(COMP),icc)
	CC=icc
	CXX=icpc
	VFLAGS= $(VF) -vec-threshold0 -unroll0 -qoverride-limits -qopt-report=5 -mavx2 -mfma -Wno-unknown-pragmas

	SUFFIX=_icc
else
	COMP=gcc
	CC=gcc
	CXX=g++
	VFLAGS= $(VF) -ftree-vectorize -ftree-loop-vectorize -fvect-cost-model=unlimited -fsimd-cost-model=unlimited -fprefetch-loop-arrays -march=native -unroll0 -mfma
endif

NAME=$(COMP)_$(VECT)_$(BINARY_NAME)

all: alexnet run

alexnet:
	$(CC) -O3 -c $(POLYBENCH_FLAGS) ../utilities/polybench.c
	$(CXX) $(VFLAGS) alexnet.cc layers.cc polybench.o $(POLYBENCH_FLAGS) -o $(NAME).o
	$(CXX) $(VFLAGS) alexnet.cc layers_macveth$(SUFFIX).cc polybench.o $(POLYBENCH_FLAGS) -o $(NAME)_macveth.o

clean:
	rm *.o *.optrpt

run:
	./$(NAME).o > $(NAME).csv
	./$(NAME)_macveth.o > $(NAME)_macveth.csv
