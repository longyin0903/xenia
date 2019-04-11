include $(XENIA_MAKE)

LIB_LEGEND_PROTO=game_config.pbo

liblegend_proto.a: $(LIB_LEGEND_PROTO)
	@$(TEXT_YELLOW)
	@echo "Archiving $@"
	@$(TEXT_RESET)
	@$(AR) $(AR_FLAGS) $@ $(LIB_LEGEND_PROTO)
	@$(MV) $(MV_FLAGS) $@ $(XENIA_LIB)/$(LIB_SUB_PATH)/$@
	@$(RM) $(RM_FLAGS) $(LIB_LEGEND_PROTO)

library: liblegend_proto.a

release: CC_DEBUG_FLAGS=-DNDEBUG
release: LIB_SUB_PATH=release
release: library
release: clean

debug: CC_DEBUG_FLAGS=-g -DDEBUG
debug: LIB_SUB_PATH=debug
debug: library
debug: clean

