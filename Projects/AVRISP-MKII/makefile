#
#             LUFA Library
#     Copyright (C) Dean Camera, 2012.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

MCU          = atmega32u2
ARCH         = AVR8
BOARD        = MINIMUS
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = AVRISP-MKII
SRC          = $(TARGET).c minimus_hack.c AVRISPDescriptors.c Lib/V2Protocol.c Lib/V2ProtocolParams.c Lib/ISP/ISPProtocol.c Lib/ISP/ISPTarget.c Lib/XPROG/XPROGProtocol.c \
               Lib/XPROG/XPROGTarget.c Lib/XPROG/XMEGANVM.c Lib/XPROG/TINYNVM.c $(LUFA_SRC_USB) \
	       $(LUFA_SRC_SERIAL)
LUFA_PATH    = ../../LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/
LD_FLAGS     =

AVRDUDE_PROGRAMMER = avr109
AVRDUDE_PORT = /dev/ttyACM1
AVRDUDE_FLAGS = 

# Default target
all:

minimus_hack.o: minimus_rom.h

minimus_rom.h: minimus.rom build_rom.py
	./build_rom.py

clean: cleanrom

cleanrom:
	rm -f minimus_rom.h

.phony: cleanrom

# Include LUFA build script makefiles
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
include $(LUFA_PATH)/Build/lufa_cppcheck.mk
include $(LUFA_PATH)/Build/lufa_doxygen.mk
include $(LUFA_PATH)/Build/lufa_dfu.mk
include $(LUFA_PATH)/Build/lufa_hid.mk
include $(LUFA_PATH)/Build/lufa_avrdude.mk
include $(LUFA_PATH)/Build/lufa_atprogram.mk
