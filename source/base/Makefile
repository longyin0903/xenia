include $(XENIA_MAKE)

LIB_BASE=file_location.o init_xenia.o logging.o

libbase.a: $(LIB_BASE)
	@$(TEXT_YELLOW)
	@echo "Archiving $@"
	@$(TEXT_RESET)
	@$(AR) $(AR_FLAGS) $@ $(LIB_BASE)
	@$(MV) $(MV_FLAGS) $@ $(XENIA_LIB)/$(LIB_SUB_PATH)/$@
	@$(RM) $(RM_FLAGS) $(LIB_BASE)

library: libbase.a

release: CC_DEBUG_FLAGS=-DNDEBUG
release: LIB_SUB_PATH=release
release: library
release: clean

debug: CC_DEBUG_FLAGS=-g -DDEBUG
debug: LIB_SUB_PATH=debug
debug: library
debug: clean

