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
	#VFLAGS= $(VF) -ftree-vectorize -ftree-loop-vectorize -fvect-cost-model=unlimited -fsimd-cost-model=unlimited -fprefetch-loop-arrays -std=c99 -unroll0
endif
