CC = gcc 

SRC_DIR=examples_tiling
BIN_DIR=bin_tiling

#CFLAGS += -I$(PULP_SDK_HOME)/install/include -D__OPENRISC__ -D_LITTLE_ENDIAN_ -Wno-incompatible-pointer-types
LDFLAGS += -lopenvx -lopenvx-helper -lvx_debug -lopenvx-debug_k-lib -lopenvx-debug-lib -lopenvx-extras-lib  -lm -Wl,--no-whole-archive


all: $(SRC_DIR)/tiling_alpha.c
	mkdir -p $(BIN_DIR)
#	$(CC) -o $(BIN_DIR)/vx_tiling_main  $(SRC_DIR)/vx_tiling_main.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/tiling_box  $(SRC_DIR)/tiling_box.c $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/tiling_gaussian  $(SRC_DIR)/tiling_gaussian.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/tiling_alpha  $(SRC_DIR)/tiling_alpha.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/tiling_add  $(SRC_DIR)/tiling_add.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/tiling_erode  $(SRC_DIR)/tiling_erode.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/tiling_dilate  $(SRC_DIR)/tiling_dilate.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/tiling_median  $(SRC_DIR)/tiling_median.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/tiling_sobel  $(SRC_DIR)/tiling_sobel.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/tiling_absdiff  $(SRC_DIR)/tiling_absdiff.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/tiling_sub  $(SRC_DIR)/tiling_sub.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/tiling_magnitude  $(SRC_DIR)/tiling_magnitude.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/tiling_phase  $(SRC_DIR)/tiling_phase.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/tiling_threshold  $(SRC_DIR)/tiling_threshold.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/tiling_accumulateSquare  $(SRC_DIR)/tiling_accumulateSquare.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/tiling_accumulate  $(SRC_DIR)/tiling_accumulate.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/non_tiling_alpha  $(SRC_DIR)/non_tiling_alpha.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/non_tiling_alpha  $(SRC_DIR)/non_tiling_alpha.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/non_tiling_gaussian  $(SRC_DIR)/non_tiling_gaussian.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/non_tiling_box  $(SRC_DIR)/non_tiling_box.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/non_tiling_add  $(SRC_DIR)/non_tiling_add.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/non_tiling_erode  $(SRC_DIR)/non_tiling_erode.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/non_tiling_dilate  $(SRC_DIR)/non_tiling_dilate.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/non_tiling_median  $(SRC_DIR)/non_tiling_median.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/non_tiling_sobel  $(SRC_DIR)/non_tiling_sobel.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/non_tiling_absdiff  $(SRC_DIR)/non_tiling_absdiff.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/non_tiling_sub  $(SRC_DIR)/non_tiling_sub.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/non_tiling_magnitude  $(SRC_DIR)/non_tiling_magnitude.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/non_tiling_phase  $(SRC_DIR)/non_tiling_phase.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/non_tiling_threshold  $(SRC_DIR)/non_tiling_threshold.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/non_tiling_accumulateSquare  $(SRC_DIR)/non_tiling_accumulateSquare.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/non_tiling_accumulate  $(SRC_DIR)/non_tiling_accumulate.c  $(CFLAGS)  $(LDFLAGS)


.PHONY: clean

clean:
	rm -rf $(BIN_DIR)/*
