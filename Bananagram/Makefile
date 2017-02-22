BUILD_DIR = .
OBJ_DIR = .
ARCHFLAGS = -march=native
CXX       = clang++ -std=c++11 -O3
#CXX       = g++ --std=c++11 -O3
EXTOPT    = -Wall
EXTOPT    = -fvectorize -funroll-loops -fno-lax-vector-conversions -stdlib=libc++ -fno-limit-debug-info -fmessage-length=0 -fdiagnostics-show-note-include-stack -fmacro-backtrace-limit=0 -fno-asm -fasm-blocks  -fstrict-aliasing  -Wno-missing-field-initializers -Wno-missing-prototypes -Werror=return-type -Werror=deprecated-objc-isa-usage -Werror=objc-root-class -Wno-non-virtual-dtor -Wno-overloaded-virtual -Wno-exit-time-destructors -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wunused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wno-unknown-pragmas -pedantic -Wno-shadow -Wno-four-char-constants -Wbool-conversion -Wenum-conversion -Wsign-compare -Wshorten-64-to-32 -Wno-newline-eof

CXXFLAGS  = $(ARCHFLAGS) $(EXTOPT) -Wall -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -DDEBUG=$(DEBUG)

HEADERS = Board.h Dictionary.h Place.h defs.h simple_utils.h

DOTCPP = Board.cpp Dictionary.cpp Place.cpp simple_utils.cpp main.cpp

BINS =  banana

DOTO    =       $(DOTCPP:%.cpp=$(OBJ_DIR)/%.o)

all:    $(DOTO) $(BINS) $(test_targets)

banana:	$(DOTO)
	$(CXX) $(CXXFLAGS) -o banana main.o Board.o Dictionary.o Place.o simple_utils.o

run:	banana ../TestData/words.5letters
	./banana -D 96 -n 21 -F ../TestData/words.5letters

.PHONY: doto
doto:   $(DOTO)

#$(DOTO):        $(BUILD_DIR)/Env.mk

$(DOTO): $(OBJ_DIR)/%.o:        %.cpp $(HEADERS)
	$(CXX) -c $(CXXFLAGS) $< -o $@


.PHONY : clean
clean :
	rm -f $(DOTO) $(BINS) $(test_objects) $(test_targets)

clobber:
	rm -f *.o $(DOTO) $(BINS) $(test_objects) $(test_targets)


