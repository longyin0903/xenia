include $(XENIA_MAKE)

LIB_ABSL=string_view.o

libabsl.a: $(LIB_ABSL)
	@$(TEXT_YELLOW)
	@echo "Archiving $@"
	@$(TEXT_RESET)
	@$(AR) $(AR_FLAGS) $@ $(LIB_ABSL)
	@$(MV) $(MV_FLAGS) $@ $(XENIA_LIB)/$(LIB_SUB_PATH)/$@
	@$(RM) $(RM_FLAGS) $(LIB_ABSL)

library: libabsl.a

release: CC_DEBUG_FLAGS=-DNDEBUG
release: LIB_SUB_PATH=release
release: library
release: clean

debug: CC_DEBUG_FLAGS=-g -DDEBUG
debug: LIB_SUB_PATH=debug
debug: library
debug: clean

