CONFIG_SUPPORT_ESP_SERIAL = y
CONFIG_ENABLE_MONITOR_PROCESS = n



# Toolchain Path
CROSS_COMPILE := /usr/bin/arm-linux-gnueabihf-
# Linux Kernel header
KERNEL := /home/dev/Dev/LDD/linux
# Architecture
ARCH := arm

#Default interface is sdio
MODULE_NAME=esp32_spi

ifeq ($(CONFIG_SUPPORT_ESP_SERIAL), y)
	EXTRA_CFLAGS += -DCONFIG_SUPPORT_ESP_SERIAL
endif

ifeq ($(CONFIG_ENABLE_MONITOR_PROCESS), y)
	EXTRA_CFLAGS += -DCONFIG_ENABLE_MONITOR_PROCESS
endif

EXTRA_CFLAGS += -I$(PWD)/../../../../common/include -I$(PWD)


ifeq ($(MODULE_NAME), esp32_spi)
	EXTRA_CFLAGS += -I$(PWD)/spi
	module_objects += spi/esp_spi.o
endif

PWD := $(shell pwd)

obj-m := $(MODULE_NAME).o
$(MODULE_NAME)-y := main.o $(module_objects)

ifeq ($(CONFIG_SUPPORT_ESP_SERIAL), y)
	$(MODULE_NAME)-y += esp_serial.o esp_rb.o
endif

all: clean
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL) M=$(PWD) modules

clean:
	rm -rf *.o sdio/*.o spi/*.o *.ko
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL) M=$(PWD) clean
