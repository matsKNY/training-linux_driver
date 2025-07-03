# SPDX-License-Identifier: GPL-2.0

################################################################################
### Parameters and variables:
################################################################################
## Paths:
# Path to the root directory of the modules to build:
MODULE_DIR := $(shell pwd)
# Path to the sources of the kernel to build the modules for:
KERNEL_SRC ?= /lib/modules/$(shell uname -r)/build

################################################################################
### Handle reentrance to build from the context of the kernel:
################################################################################
ifneq ($(KERNELRELEASE),)

# Definition of target modules (and their inner structures) to build:
obj-m += exercise-IV-01.o

################################################################################
### Build context of the module:
################################################################################
else

### Cross-compilation:
ARCH ?= $(shell arch)

ifeq ($(ARCH),i686)
	ARCH := x86

else ifeq ($(ARCH),aarch64)
	ARCH := arm64

else ifeq ($(ARCH),armv7l)
	ARCH := arm

else ifeq ($(ARCH),armv6l)
	ARCH := arm

else ifeq ($(ARCH),riscv64)
	ARCH := riscv
endif

### Rules:
## Build:
all: modules

modules:
	$(MAKE) -C $(KERNEL_SRC) \
		M=$(MODULE_DIR) \
		ARCH=$(ARCH) \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		modules

clean:
	rm -rf .tmp_versions .cache.mk
	rm -f                   \
		*.o                 \
		*.o.cmd             \
		*.o.d               \
		*.ko                \
		*.ko.cmd            \
		*.mod               \
		*.mod.c             \
		*.mod.cmd           \
		*.mod.o.cmd         \
		.Module.symvers     \
		Module.symvers      \
		.Module.symvers.cmd \
		Module.symvers.cmd  \
		.modules.order      \
		modules.order       \
		.modules.order.cmd  \
		modules.order.cmd

## Meta:
.PHONY: all clean modules

endif
