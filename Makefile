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

PATH	:= $(PATH):$(ROOTDIR)

# Uncomment the lines below for ssil (Small and Simple Image Library) or
# ssgl (Small and Simple Graphics Library) to use one of the libraries
# when building. Without ssil or ssgl you won't get any visual output.
# If you don't wont to use any of the libraries you can instead write your
# own code to handle the visual output, see README for more information.
# ssil can be cloned from https://github.com/mrjojo/ssil.git
# ssgl can be cloned from https://github.com/mrjojo/ssgl.git

# Uncomment the following three lines to use ssil
#CFLAGS   += -D SSIL
#CPPFLAGS += -I $(ROOTDIR)/../ssil/include
#LIBS     += ../ssil/ssil.a

# Uncomment the following three lines to use ssgl
#CFLAGS   += -D SSGL
#CPPFLAGS += -I $(ROOTDIR)/../ssgl/include
#LIBS     += ../ssgl/ssgl.a -lSDL

all:
	@mkversion
	$(MAKE) -C src
	$(CC) $(CFLAGS) $(CPPFLAGS) srt.c $(OBJS) $(LIBS) -o $(EXEC)

clean:
	$(MAKE) -C src clean
	$(RM) version.h
	$(RM) $(EXEC)
