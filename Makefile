CXX=g++
CXX_FLAGS=-Iinclude/avs
MKDIR_P=mkdir -p
RM_R=rm -r

SRC_DIR=src
BIN_DIR=bin
OBJ_DIR=$(BIN_DIR)/obj

SRC=$(wildcard $(SRC_DIR)/*.cpp)
OBJ=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
OUT=$(BIN_DIR)/libavs.a

all: $(OUT)

$(OBJ_DIR):
	$(MKDIR_P) $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) -c -o $@ $< $(CXX_FLAGS)

$(OUT): $(OBJ)
	ar -rcs $@ $^

.PHONY: clean
clean:
	$(RM_R) $(BIN_DIR)
