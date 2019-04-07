CXX=g++
CXX_FLAGS=-Iinclude/avs -Ibin -I/opt/vc/include -Wno-psabi
LD_FLAGS=-lGLESv2 -licuuc -lX11 -lEGL
MKDIR=mkdir
RM_R=rm -r
SHD_INC=utils/shader_header.sh

SRC_DIR=src
BIN_DIR=bin
OBJ_DIR=$(BIN_DIR)/obj
SHD_DIR=shaders

SRC=$(wildcard $(SRC_DIR)/*.cpp)
OBJ=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
SHD=$(wildcard $(SHD_DIR)/*.vert $(SHD_DIR)/*.frag)
SHD_OUT=$(BIN_DIR)/ShaderSource.hpp
OUT=$(BIN_DIR)/libavs.so

all: $(OUT)

$(BIN_DIR):
	$(MKDIR) $@

$(OBJ_DIR): | $(BIN_DIR)
	$(MKDIR) $@

$(SHD_OUT): $(SHD) | $(BIN_DIR)
	$(SHD_INC) $(SHD_DIR) $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR) $(SHD_OUT)
	$(CXX) -fPIC -c -o $@ $< $(CXX_FLAGS) $(LD_FLAGS)

$(OUT): $(OBJ)
	$(CXX) -shared -Wl,-soname,$@ -o $@ $^ $(CXX_FLAGS) $(LD_FLAGS)

.PHONY: clean
clean:
	$(RM_R) $(BIN_DIR)
