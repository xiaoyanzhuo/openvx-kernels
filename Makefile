CC = gcc 

SRC_DIR=examples_kernels
BIN_DIR=bin_kernels

#CFLAGS += -I$(PULP_SDK_HOME)/install/include -D__OPENRISC__ -D_LITTLE_ENDIAN_ -Wno-incompatible-pointer-types
LDFLAGS += -lopenvx -lopenvx-helper -lvx_debug -lopenvx-debug_k-lib -lopenvx-debug-lib -lopenvx-extras-lib  -lm -Wl,--no-whole-archive


all: $(SRC_DIR)/lut.c
	mkdir -p $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/box  $(SRC_DIR)/box.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/gaussian  $(SRC_DIR)/gaussian.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/median  $(SRC_DIR)/median.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/canny_1  $(SRC_DIR)/canny_1.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/canny_2  $(SRC_DIR)/canny_2.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/scale  $(SRC_DIR)/scale.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/sobel  $(SRC_DIR)/sobel.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/magnitude  $(SRC_DIR)/magnitude.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/thresh  $(SRC_DIR)/thresh.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/phase  $(SRC_DIR)/phase.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/lut  $(SRC_DIR)/lut.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/absdiff  $(SRC_DIR)/absdiff.c  $(CFLAGS)  $(LDFLAGS)
#	$(CC) -o $(BIN_DIR)/absdiff_1  $(SRC_DIR)/absdiff_1.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/integral  $(SRC_DIR)/integral.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/erode  $(SRC_DIR)/erode.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/dilate  $(SRC_DIR)/dilate.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/convdepth_up  $(SRC_DIR)/convdepth_up.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/convdepth_down  $(SRC_DIR)/convdepth_down.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/histogram  $(SRC_DIR)/histogram.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/equal_hist  $(SRC_DIR)/equal_hist.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/remap  $(SRC_DIR)/remap.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/mean_std  $(SRC_DIR)/mean_std.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/minmaxloc  $(SRC_DIR)/minmaxloc.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/add  $(SRC_DIR)/add.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/multiply  $(SRC_DIR)/multiply.c  $(CFLAGS)  $(LDFLAGS)
#	$(CC) -o $(BIN_DIR)/multiply_1  $(SRC_DIR)/multiply_1.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/sub  $(SRC_DIR)/sub.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/colorconvert  $(SRC_DIR)/colorconvert.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/channel_extract  $(SRC_DIR)/channel_extract.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/channel_combine  $(SRC_DIR)/channel_combine.c  $(CFLAGS)  $(LDFLAGS)
#	$(CC) -o $(BIN_DIR)/channel_combine_1  $(SRC_DIR)/channel_combine_1.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/accu  $(SRC_DIR)/accu.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/accu_weight  $(SRC_DIR)/accu_weight.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/accu_square  $(SRC_DIR)/accu_square.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/bitwise_and  $(SRC_DIR)/bitwise_and.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/bitwise_or  $(SRC_DIR)/bitwise_or.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/bitwise_xor  $(SRC_DIR)/bitwise_xor.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/bitwise_not  $(SRC_DIR)/bitwise_not.c  $(CFLAGS)  $(LDFLAGS)
	$(CC) -o $(BIN_DIR)/half_gaussian  $(SRC_DIR)/half_gaussian.c  $(CFLAGS)  $(LDFLAGS)
.PHONY: clean
.PHONY: clean
.PHONY: clean
.PHONY: clean
.PHONY: clean
.PHONY: clean
.PHONY: clean
.PHONY: clean
.PHONY: clean

clean:
	rm -rf $(BIN_DIR)/*
