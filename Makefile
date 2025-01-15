CC = gcc
FC = gfortran
AR = ar
CFLAGS = -fopenmp -fPIC -Wall
FFLAGS = -fopenmp -fPIC -Wall

TARGET = libperf_helper.a
SRCS = $(wildcard *.c *.f90)
OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.f90=.o)
MOD = perf_helper_mod.mod

RM = rm -f

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) r $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $< -c $(CFLAGS)

%.o: %.f90
	$(FC) $< -c $(FFLAGS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(MOD)
