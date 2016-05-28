PATH		:=	$(DEVKITPPC)/bin:$(PATH)
PREFIX		=	powerpc-eabi-
CC			=	$(PREFIX)gcc
CFLAGS		=	-std=gnu99 -nostdinc -fno-builtin -fno-toplevel-reorder -c
LD			=	$(PREFIX)ld
LDFLAGS		=	-Ttext 1800000 --oformat binary -L$(DEVKITPPC)/lib/gcc/powerpc-eabi/4.8.2 -lgcc
project		:=	src
root		:=	.
build		:=	$(root)/bin
libs		:=	$(root)/../../libwiiu/bin
www			:=	$(root)/../../www
framework	:=	$(root)/../../framework

all: setup main550 main532

setup:
	mkdir -p $(root)/bin/

main550:
	make main FIRMWARE=550

main532:
	make main FIRMWARE=532

main:
	$(CC) $(CFLAGS) -DVER=$(FIRMWARE) $(project)/*.c
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) -o $(build)/code$(FIRMWARE).bin $(build)/loader.o `find $(build) -name "*.o" ! -name "loader.o"` $(LDFLAGS)

clean:
	rm -r $(build)/*
