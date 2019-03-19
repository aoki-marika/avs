CXX=g++
CXX_FLAGS=-Iinclude/avs -I/opt/vc/include
LD_FLAGS=-L/opt/vc/lib -lbrcmGLESv2 -lbrcmEGL -lbcm_host
MKDIR_P=mkdir -p
RM_R=rm -r

SRC_DIR=src
BIN_DIR=bin
OBJ_DIR=$(BIN_DIR)/obj

SRC=$(wildcard $(SRC_DIR)/*.cpp)
OBJ=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
OUT=$(BIN_DIR)/libavs.so

all: $(OUT)

$(OBJ_DIR):
	$(MKDIR_P) $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) -fPIC -c -o $@ $< $(CXX_FLAGS) $(LD_FLAGS)

$(OUT): $(OBJ)
	$(CXX) -shared -Wl,-soname,$@ -o $@ $^ $(CXX_FLAGS) $(LD_FLAGS)

.PHONY: clean
clean:
	$(RM_R) $(BIN_DIR)
