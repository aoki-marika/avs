CXX=g++
CXX_FLAGS=-I../include
LD_FLAGS=-Lbin -lavs -Wl,-rpath,"\$ORIGIN"
MKDIR_P=mkdir -p
CP=cp
RM_R=rm -r

SRC_DIR=src
BIN_DIR=bin

SRC=$(wildcard $(SRC_DIR)/*.c)
OUT=$(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%,$(SRC))

all: $(OUT)

$(BIN_DIR):
	$(MKDIR_P) $@

$(OUT): $(SRC) | $(BIN_DIR)
	make -C ../
	$(CP) ../bin/libavs.so bin/
	$(CXX) -o $@ $^ $(CXX_FLAGS) $(LD_FLAGS)

.PHONY: clean
clean:
	$(RM_R) $(BIN_DIR)
