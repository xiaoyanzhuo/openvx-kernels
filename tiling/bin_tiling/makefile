CC = gcc
#CFLAGS += -I$(PULP_SDK_HOME)/install/include -D__OPENRISC__ -D_LITTLE_ENDIAN_ -Wno-incompatible-pointer-types
LDFLAGS += -lopenvx -lopenvx-helper -lvx_debug -lopenvx-debug_k-lib -lopenvx-debug-lib -lopenvx-extras-lib  -lm -Wl,--no-whole-archive

all: ./scale.c
	$(CC) -o scale.out  scale.c $(CFLAGS)  $(LDFLAGS)
	
.PHONY: clean
clean:
	rm -f scale.out
