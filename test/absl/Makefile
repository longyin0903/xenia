include $(XENIA_MAKE)

string_view_test: string_view_test.o
	@$(TEXT_RED)
	@echo "Createing $@ ..."
	@$(TEXT_RESET)
	@$(CC) $(CC_FLAGS) $(CC_LIB_DEBUG_FLAGS) -o $@ string_view_test.o \
		$(CC_TEST_LIBS) -labsl
	@${MV} ${MV_FLAGS} $@ $(XENIA_TESTBIN)/absl/$@
	@${RM} ${RM_FLAGS} string_view_test.o

all: clean string_view_test
