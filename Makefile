#
# Makefile for Simple Ray Tracer
#
# See comment below to get a visual output.
#

.EXPORT_ALL_VARIABLES:
.PHONY: all clean

EXEC     = srt

ROOTDIR  = $(shell pwd)

CC       = gcc
CFLAGS   = -std=gnu99 -Wall -Wextra -Werror
CPPFLAGS = -I $(ROOTDIR)/include
RM       = rm -f

OBJS     = src/vector.o src/sphere.o
LIBS     = -lm

# Uncomment and correct the paths in the following lines to use
# ssil (Small and Simle Image Library) when building. Without ssil
# you don't get any visual output. If you don't wont to use ssil
# you can write your own visual output routine.
# ssil can be cloned from https://github.com/mrjojo/ssil.git
#CFLAGS   += -D SSIL
#CPPFLAGS += -I $(ROOTDIR)/../ssil/include
#LIBS     += ../ssil/ssil.a

all:
	$(MAKE) -C src
	$(CC) $(CFLAGS) $(CPPFLAGS) srt.c $(OBJS) $(LIBS) -o $(EXEC)

clean:
	$(MAKE) -C src clean
	$(RM) $(EXEC)
