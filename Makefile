all: BROWSE build-binaries run-tests

BINARIES := kfl-test kfl-pprint

SRCDIR := src

LIBMODULES := core spirit-infix-parser type
BINMODULES := bin

ifndef BUILD
	BUILD := dbgopt
endif

OSNAME := $(shell uname -s)
MHNAME := $(shell uname -m)

BUILDDIR := build/$(BUILD)/$(OSNAME)/$(MHNAME)

CC := cc
CXX := c++
EBROWSE := ebrowse

GMOCK_ROOT := /Users/$(USER)/gmock
GTEST_ROOT := $(GMOCK_ROOT)/gtest
GMOCK_LIB := /Users/$(USER)/gmock/make/gmock_main.a

INCFLAGS := $(patsubst %,-I$(SRCDIR)/%,$(LIBMODULES))
INCFLAGS += -I/sw/include -I/opt/local/include -I$(GTEST_ROOT)/include -I$(GMOCK_ROOT)/include

LIBFLAGS := 

ifeq ($(BUILD),release)
	OPTFLAGS := -O2
	DBGFLAGS :=
else ifeq ($(BUILD),debug)
	OPTFLAGS :=
	DBGFLAGS := -g
else
	OPTFLAGS := -O2
	DBGFLAGS := -g
endif

C11FLAGS := -std=c++11 -stdlib=libc++ -DGTEST_HAS_PTHREAD=0
C11LDFLAGS := -stdlib=libc++

CFLAGS += $(DBGFLAGS) $(OPTFLAGS) $(INCFLAGS)
CXXFLAGS += $(DBGFLAGS) $(OPTFLAGS) $(C11FLAGS) $(INCFLAGS)
LDFLAGS += $(DBGFLAGS) $(C11LDFLAGS) $(LIBFLAGS)

ALLSRC := $(foreach mod, $(LIBMODULES), $(wildcard $(SRCDIR)/$(mod)/*.cc))
ALLHDR := $(foreach mod, $(LIBMODULES), $(wildcard $(SRCDIR)/$(mod)/*.h))

LIBSRC := $(filter-out %_flymake.cc,$(ALLSRC))
LIBHDR := $(filter-out %_flymake.h,$(ALLHDR))
LIBOBJ := $(patsubst $(SRCDIR)/%.cc,$(BUILDDIR)/%.o,$(filter %.cc,$(LIBSRC)))
-include $(LIBOBJ:.o=.d)

TSTSRC := $(filter-out %_flymake.cc, $(foreach mod, $(LIBMODULES), $(wildcard $(SRCDIR)/$(mod)/test/*.cc)))
TSTOBJ := $(patsubst $(SRCDIR)/%.cc,$(BUILDDIR)/%.o,$(filter %.cc,$(TSTSRC)))
-include $(TSTOBJ:.o=.d)

BINSRC := $(filter-out %_flymake.cc, $(foreach mod, $(BINMODULES), $(wildcard $(SRCDIR)/$(mod)/*.cc)))
BINOBJ := $(patsubst $(SRCDIR)/%.cc,$(BUILDDIR)/%.o,$(filter %.cc,$(BINSRC)))
-include $(BINOBJ:.o=.d)

LIBS := $(GMOCK_LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cc
	mkdir -p $(BUILDDIR)/`dirname $*`
	$(CXX) -MMD -MT $@ $(CXXFLAGS) -c $< -o $@

check-syntax:
	$(CXX) -o nul $(CXXFLAGS) -fsyntax-only $(CHK_SOURCES)

build-binaries: $(patsubst %,bin/%,$(BINARIES))

bin/%: $(BUILDDIR)/bin/%
	rm -f $@
	ln -s ../$< $@

$(BUILDDIR)/bin/kfl-test: $(LIBS) $(LIBOBJ) $(TSTOBJ)
	mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) -o $@ $^

$(BUILDDIR)/bin/kfl-pprint: $(LIBOBJ) $(BUILDDIR)/bin/kfl-pprint.o
	mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) -o $@ $^

BROWSE: $(LIBHDR) $(LIBSRC) $(TSTSRC)
	$(EBROWSE) $(LIBHDR) $(LIBSRC) $(TSTSRC)

.PHONEY run-tests: bin/kfl-test
	bin/kfl-test

clean:
	rm -rf BROWSE bin/kfl-test bin/kfl-pprint $(BUILDDIR)
