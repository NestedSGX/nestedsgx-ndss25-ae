# Name of the module
MODULE_NAME := svsm_guest

SRC_PATH := ./src
# Kernel build directory

# Kernel Flag for choosing kernel in Host or Guest.
# We will always choose guest os, but in Host Os we got a version to check compilering mistakes.
KERN ?= 1

# Debug Flag.
# in fact we will provide some info during the driver handling process.
# but you can use 'DEBUG=y' in Makefile to see more info about our driver running process. 
EXTRA_CFLAGS += -DDEBUG=$(DEBUG)
# GHCB Flag.
# this flag will change our protocols interacting with the svsm into ghcb version.
# Basically, due to our initial test, these two protocols didn't have very big difference in speed.
EXTRA_CFLAGS += -DGHCB=$(GHCB)


# Test in Host OS.
# The Host OS has already compiled once for the guest. so if we simply want to check
# whether the compiler will work right, we can set KERNEL_DIR as follow.

ifeq ($(KERN), 0)
	KERNEL_DIR := /home/yourname_and_path/linux-svsm/scripts/linux/guest
else
# Work in Guest OS.
# Transferred the Guest OS into the Guest Machine, and compile the kernel here.
# And now the Guest Machine can build kernel module as usual.
	KERNEL_DIR := /lib/modules/$(shell uname -r)/build
endif

# Build target
obj-m := $(MODULE_NAME).o

$(MODULE_NAME)-objs := $(SRC_PATH)/main.o $(SRC_PATH)/handlers.o $(SRC_PATH)/protocols.o $(SRC_PATH)/pgwalk.o $(SRC_PATH)/utils.o $(SRC_PATH)/backtrace.o

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules

test:
	make clean
	make all
	insmod $(MODULE_NAME).ko
	cd ../svsm-apps && make
	cd ../svsm-apps && ./user_app

clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
