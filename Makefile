CXXFLAGS=-fdiagnostics-color -O2 -g $(shell geant4-config --cflags) -I${VC__HOME}/include
LDLIBS=-Wl,--copy-dt-needed-entries -fdiagnostics-color -lm $(shell geant4-config --libs)

PREFIX?=$(HOME)/local
INSTALL_BIN=$(PREFIX)/build
INSTALL=install

all: sim 

Makefile.dep:
	-$(CXX) $(CXXFLAGS) -MM *.cc > Makefile.dep

-include Makefile.dep

sim: action.o construction.o detector.o event.o generator.o physics.o run.o sim.o stepping.o tracking.o

install:
	@mkdir -p $(INSTALL_BIN)
	$(INSTALL) sim $(INSTALL_BIN)

test: 
	@echo $(LDLIBS)

clean:
	rm -f *.o 

.PHONY: all clean

