#
# Makefile for Simple Ray Tracer
#
# See comment below to get a visual output.
#

.EXPORT_ALL_VARIABLES:
.PHONY: all clean

ROOTDIR = $(shell pwd)

include $(ROOTDIR)/user.mk

EXEC     = srt

CC       = gcc
CFLAGS   = -std=gnu99 -Wall -Wextra -Werror
CPPFLAGS = -I $(ROOTDIR)/include
RM       = rm -f

OBJS    := srt.o
LIBS     = -lm -lreadline

MODULES  = src
SRCS     = $(OBJS:.o=.c)
DEPS    := $(addprefix .,$(OBJS:.o=.d))

PATH	:= $(PATH):$(ROOTDIR)

# Include the description for each sub-module
include $(patsubst %,%/module.mk,$(MODULES))


# Render output selection (see README for more information)

# Comment the line below to DISABLE visual output from the included ssil
# (Small and Simple Image Library). I.e if you want to write your own code or
# use ssgl (see below) instead.
# note: ssil will create a .tga file which can be viewed by any image viewer.
CFLAGS   += -D SSIL

# Uncomment the following three lines to use the latest ssil version.
# ssil can be cloned from https://github.com/mrjojo/ssil.git
#CFLAGS   += -D SSIL
#CPPFLAGS += -I $(ROOTDIR)/../ssil/include
#LIBS     += ../ssil/ssil.a

# Uncomment the lines below to use ssgl (Small and Simple Graphics Library).
# ssgl can be cloned from https://github.com/mrjojo/ssgl.git
#CFLAGS   += -D SSGL
#CPPFLAGS += -I $(ROOTDIR)/../ssgl/include
#LIBS     += ../ssgl/ssgl.a -lSDL

all: Makefile
	@mkversion
	$(CC) $(CFLAGS) $(CPPFLAGS) $(SRCS) $(LIBS) -o $(EXEC)

clean:
	$(RM) version.h
	$(RM) $(OBJS) $(DEPS) $(EXEC)
