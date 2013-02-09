# Include this .mk file last in all module.mk files to
# build the list of objects for the top Makefile.

OBJS += $(addprefix $(MODDIR)/, $(MODOBJS))
DEPS += $(addprefix $(MODDIR)/, $(addprefix ., $(MODOBJS:.o=.d)))

# Include the description for each and any sub-module
-include $(patsubst %,$(MODDIR)/%/module.mk,$(MODULES))
