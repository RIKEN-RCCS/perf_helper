ifeq ($(COMPILER), gcc)
  include config.gcc
else ifeq ($(COMPILER), acfl)
  include config.acfl
else ifeq ($(COMPILER), fj)
  include config.fj
else ifeq ($(COMPILER), llvm)
  include config.llvm
endif

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
