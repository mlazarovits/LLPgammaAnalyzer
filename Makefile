#include ROOT cflags and libraries
ROOTCFLAGS  = $(shell root-config --cflags)
ROOTGLIBS   = $(shell root-config --glibs)




#set c(xx)flags and libraries
CXXFLAGS    = $(ROOTCFLAGS)

GLIBS       = $(ROOTGLIBS)
#specify compiler
CXX         = g++

#specify local paths
INCLUDEDIR  = ./include/
SRCDIR      = ./src/
#make sure compiler knows where local include files are
CXX	   += -I$(INCLUDEDIR) -I.
OUTOBJ	    = ./obj/

#specify local source, include, and object files
CC_FILES    = $(wildcard src/*.cc)
HH_FILES    = $(wildcard include/*.hh)
OBJ_FILES   = $(addprefix $(OUTOBJ),$(notdir $(CC_FILES:.cc=.o)))


#specify what to make
pv_time = pv-time-skimmer
pv-time: $(pv_time)
alltargets: $(pv_time)

#executables
$(pv_time): $(SRCDIR)$(pv_time).C $(OBJ_FILES) $(HH_FILES)
	$(CXX) $(CXXFLAGS) -o $(pv_time).x $(GLIBS) $ $<
	touch $(pv_time).x

#where to put object files
$(OUTOBJ)%.o: src/%.cc include/%.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@

#clean options
clean:
	rm -f $(OUTOBJ)*.o
	rm -f *.x
	rm -f AutoDict*
	rm -f *.d



