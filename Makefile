CXXFLAGS=-fdiagnostics-color -O2 -g $(shell geant4-config --cflags | rev | cut -c 4- | rev) -I${VC__HOME}/include -I${VECGEOM__HOME}/include -I${VECCORE__HOME}/include -DVECCORE_ENABLE_VC 
LDLIBS=-Wl,--copy-dt-needed-entries -fdiagnostics-color -lm $(shell geant4-config --libs-without-gui | tr ' ' '\n' | grep -v G4interface | tr '\n' ' ') -L ${VECGEOM__HOME}/lib64 -l vecgeom -l vgdml -L ${VECGEOM__CORE}/lib64  -L${VC__HOME}/lib -lVc

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
	$(INSTALL) zdab-cat $(INSTALL_BIN)

test: 
	@echo $(LDLIBS)

clean:
	rm -f *.o 

.PHONY: all clean

