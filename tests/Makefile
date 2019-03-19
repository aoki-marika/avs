CXX=g++
CXX_FLAGS=-I../include
LD_FLAGS=-Lbin -lavs -Wl,-rpath,"\$ORIGIN"
MKDIR_P=mkdir -p
CP=cp
RM_R=rm -r

SRC_DIR=src
BIN_DIR=bin

OUT=$(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%,$(wildcard $(SRC_DIR)/*.c))

all: $(OUT)

$(BIN_DIR):
	$(MKDIR_P) $@
	make -C ../
	$(CP) ../bin/libavs.so $(BIN_DIR)/

bin/%: src/%.c | $(BIN_DIR)
	$(CXX) -o $@ $^ $(CXX_FLAGS) $(LD_FLAGS)

.PHONY: clean
clean:
	$(RM_R) $(BIN_DIR)
