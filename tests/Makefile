CXX=g++
CXX_FLAGS=-I../include -I/opt/vc/include
LD_FLAGS=-Lbin -lavs -Wl,-rpath,"\$ORIGIN" -L/opt/vc/lib -lbrcmGLESv2
MKDIR_P=mkdir -p
CP=cp
RM_R=rm -r

SRC_DIR=src
BIN_DIR=bin
AVS_LIB=$(BIN_DIR)/libavs.so

OUT=$(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%,$(wildcard $(SRC_DIR)/*.c))

all: $(AVS_LIB) $(OUT)

$(BIN_DIR):
	$(MKDIR_P) $@

$(AVS_LIB): FORCE | $(BIN_DIR)
	make -C ../
	$(CP) ../bin/libavs.so $(BIN_DIR)/
FORCE:

bin/%: src/%.c | $(BIN_DIR) $(AVS_LIB)
	$(CXX) -o $@ $^ $(CXX_FLAGS) $(LD_FLAGS)

.PHONY: clean
clean:
	$(RM_R) $(BIN_DIR)
