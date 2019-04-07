CXX=g++
CXX_FLAGS=-Iinclude/avs -Ibin -Wno-psabi
LD_FLAGS=-licuuc
MKDIR=mkdir
RM_R=rm -r
SHD_INC=utils/shader_header.sh

# get the platform name
# check for an rpi by checking for a bcm2708 cpu
ifeq ($(grep -m 1 -o BCM2708 /proc/cpuinfo),BCM2708)
	PLATFORM=Raspberry
else
	PLATFORM=Linux
endif

SRC_DIR=src
BIN_DIR=bin
OBJ_DIR=$(BIN_DIR)/obj
SHD_DIR=shaders

SRC=$(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/$(PLATFORM)/*.cpp)

# handle platform specific functionality
ifeq ($(PLATFORM),Raspberry)
	CXX_FLAGS:=-I/opt/vc/include $(CXX_FLAGS)
	LD_FLAGS:=-L/opt/vc/lib -lbrcmGLESv2 -lbrcmEGL -lbcm_host $(LD_FLAGS)
else ifeq ($(PLATFORM),Linux)
	LD_FLAGS:=-lGLESv2 -lX11 -lEGL $(LD_FLAGS)
endif

OBJ=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
SHD=$(wildcard $(SHD_DIR)/*.vert $(SHD_DIR)/*.frag)
SHD_OUT=$(BIN_DIR)/ShaderSource.hpp
OUT=$(BIN_DIR)/libavs.so

all: $(OUT)

$(BIN_DIR):
	$(MKDIR) $@

$(OBJ_DIR): | $(BIN_DIR)
	$(MKDIR) $@
	$(MKDIR) $@/$(PLATFORM)

$(SHD_OUT): $(SHD) | $(BIN_DIR)
	$(SHD_INC) $(SHD_DIR) $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR) $(SHD_OUT)
	$(CXX) -fPIC -c -o $@ $< $(CXX_FLAGS) $(LD_FLAGS)

$(OUT): $(OBJ)
	$(CXX) -shared -Wl,-soname,$@ -o $@ $^ $(CXX_FLAGS) $(LD_FLAGS)

.PHONY: clean
clean:
	$(RM_R) $(BIN_DIR)
