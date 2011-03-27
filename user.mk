#
# Makefile definitions
#
# Include this to make use of its contents.
#

# Autodependency generation via GCC -M.
.%.d: %.c
	$(CC) -MM $(CFLAGS) $(CPPFLAGS) $< | sed 's,.*: ,$*.o $@ : ,g' > $@;
