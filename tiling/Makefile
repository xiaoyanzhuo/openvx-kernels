CC = gcc
  
SRC_DIR=examples_tiling
BIN_DIR=bin_tiling

#CFLAGS += -I$(PULP_SDK_HOME)/install/include -D__OPENRISC__ -D_LITTLE_ENDIAN_ -Wno-incompatible-pointer-types
LDFLAGS += -lopenvx -lopenvx-helper -lvx_debug -lopenvx-debug_k-lib -lopenvx-debug-lib -lopenvx-extras-lib  -lm -Wl,--no-whole-archive


all: $(SRC_DIR)/vx_tiling_main.c
        mkdir -p $(BIN_DIR)
      # $(CC) -o $(BIN_DIR)/vx_tiling_main  $(SRC_DIR)/vx_tiling_main.c  $(CFLAGS)  $(LDFLAGS)
      $(CC) -o $(BIN_DIR)/tiling_box  $(SRC_DIR)/tiling_box.c $(CFLAGS)  $(LDFLAGS)
      $(CC) -o $(BIN_DIR)/tiling_gaussian  $(SRC_DIR)/tiling_gaussian.c  $(CFLAGS)  $(LDFLAGS)
      $(CC) -o $(BIN_DIR)/tiling_alpha  $(SRC_DIR)/tiling_alpha.c  $(CFLAGS)  $(LDFLAGS)
      $(CC) -o $(BIN_DIR)/tiling_add  $(SRC_DIR)/tiling_add.c  $(CFLAGS)  $(LDFLAGS)
      $(CC) -o $(BIN_DIR)/non_tiling_alpha  $(SRC_DIR)/non_tiling_alpha.c  $(CFLAGS)  $(LDFLAGS)
      $(CC) -o $(BIN_DIR)/non_tiling_gaussian  $(SRC_DIR)/non_tiling_gaussian.c  $(CFLAGS)  $(LDFLAGS)
      $(CC) -o $(BIN_DIR)/non_tiling_box  $(SRC_DIR)/non_tiling_box.c  $(CFLAGS)  $(LDFLAGS)
      $(CC) -o $(BIN_DIR)/non_tiling_add  $(SRC_DIR)/non_tiling_add.c  $(CFLAGS)  $(LDFLAGS)
.PHONY: clean

clean:
        rm -rf $(BIN_DIR)/*