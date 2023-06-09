
BUILD_DIR_DEBUG = ../build_debug
BUILD_DIR_RELEASE = ../build_release
EXE = NetFlux
EXE_DEBUG = $(BUILD_DIR_DEBUG)/$(EXE)
EXE_RELEASE = $(BUILD_DIR_RELEASE)/$(EXE)

CXX = g++
INC_DIR = ../include

CXX_FLAGS_DEBUG = -g -Wall -Wno-unused-function -fmax-errors=1 -I$(INC_DIR)
CXX_FLAGS_RELEASE = -DNDEBUG -Wall -Wno-unused-function -fmax-errors=1 -I$(INC_DIR) -O3
 
#external libraries
LDLIBS = -lyaml-cpp

#add source files here
SRC = NetFlux.cpp \
	  Config.cpp \
	  ConfigParser.cpp \
	  Grid.cpp \
	  Solver.cpp \
	  Utilities.cpp \
	  Output.cpp \
	  Driver.cpp \
	  SolverData.cpp \
	  Numerics.cpp

OBJ_DEBUG = $(patsubst %.cpp, $(BUILD_DIR_DEBUG)/%.o, $(SRC)) 
OBJ_RELEASE = $(patsubst %.cpp, $(BUILD_DIR_RELEASE)/%.o, $(SRC)) 


all: prep debug 

debug: $(EXE_DEBUG) 

release: $(EXE_RELEASE) 


$(EXE_DEBUG): $(OBJ_DEBUG)
	$(CXX) -o $(EXE_DEBUG) $^ $(LDLIBS) $(CXX_FLAGS_DEBUG)

$(EXE_RELEASE): $(OBJ_RELEASE)
	$(CXX) -o $(EXE_RELEASE) $^ $(LDLIBS) $(CXX_FLAGS_RELEASE)


#The -MMD option creates a list of depenendencies, and it's included later
$(BUILD_DIR_DEBUG)/%.o: %.cpp 
	$(CXX) -MMD -c -o $@ $< $(CXX_FLAGS_DEBUG)

$(BUILD_DIR_RELEASE)/%.o: %.cpp 
	$(CXX) -MMD -c -o $@ $< $(CXX_FLAGS_RELEASE)


-include $(OBJ_DEBUG:.o=.d)
-include $(OBJ_RELEASE:.o=.d)



prep:
	mkdir -p $(BUILD_DIR_DEBUG) $(BUILD_DIR_RELEASE)  

.PHONY: clean
clean:
	-rm -f $(BUILD_DIR_DEBUG)/*
	-rm -f $(BUILD_DIR_RELEASE)/*













