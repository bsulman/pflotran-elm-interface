ifeq ($(USEOPENMP),YES)
F90 ?= xlf2003
else
F90 ?= xlf2003_r
endif

D=-WF,-D
I=-I
M=-I
L=-L

F90FLAGS += -g -O0 -WF,-qfpp -C

ifeq ($(USEOPENMP),YES)
F90FLAGS += -qsmp=omp
endif

FPPFLAGS = $DSTRINGIFY_SIMPLE $DIBM
CPPFLAGS = -WF,-DSTRINGIFY_SIMPLE,-DIBM

ifeq ($(PFUNIT_ABI),64)
  FPPFLAGS += $DLONG_PTR
  CPPFLAGS += -DLONG_PTR
endif
