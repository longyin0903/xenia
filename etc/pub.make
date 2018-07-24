ifeq ($(OS), Windows_NT)
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		XENIA_HOME=$(HOME)/git/xenia
    XENIA_LIB=$(HOME)/git/lib
    XENIA_TESTBIN=$(HOME)/git/testbin
		include $(XENIA_HOME)/etc/pub.make.linux
	endif
endif
