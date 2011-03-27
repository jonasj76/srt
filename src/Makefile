OBJS = vector.o sphere.o

SRCS = $(OBJS:.o=.c)
DEPS = $(addprefix .,$(OBJS:.o=.d))

include $(ROOTDIR)/user.mk

all: Makefile $(OBJS)

clean:
	$(RM) $(OBJS) $(DEPS)

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif
