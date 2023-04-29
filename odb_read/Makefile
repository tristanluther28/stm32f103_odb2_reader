PROJECT = odb_read
BINARY = odb_read
BUILD_DIR = bin

SHARED_DIR = ../lib
CFILES = odb_read.c
CFILES += pwm.c
CFILES += spi.c
CFILES += ltc4627.c
CFILES += odb2.c

# TODO - you will need to edit these two lines!
DEVICE=stm32f103c4t6a
OOCD_FILE = board/stm32f1x.cfg

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
