CC_avr      := avr-gcc
CFLAGS_avr  := -Wall -mmcu=atmega8535 -O2 -std=c99 -c
LDFLAGS_avr :=       -mmcu=atmega8535

BUILD_DIR  := build

all: main utest_draw utest_decode

-include $(BUILD_DIR)/*.d

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC_avr) $(CFLAGS_avr) $< -o $@

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)


MAIN_SRC := main.c morse.c timer_event.c
MAIN_OBJ := $(addprefix $(BUILD_DIR)/,$(MAIN_SRC:.c=.o))
.PHONY: main
main: $(BUILD_DIR)/main.hex
$(BUILD_DIR)/main.elf: $(MAIN_OBJ)
	$(CC_avr) $(LDFLAGS_avr) $(MAIN_OBJ) -o $@


UTEST_DRAW_SRC := utest_draw.c morse.c
UTEST_DRAW_OBJ := $(addprefix $(BUILD_DIR)/,$(UTEST_DRAW_SRC:.c=.o))
.PHONY: utest_draw
utest_draw: $(BUILD_DIR)/utest_draw.hex
$(BUILD_DIR)/utest_draw.elf: $(UTEST_DRAW_OBJ)
	$(CC_avr) $(LDFLAGS_avr) $(UTEST_DRAW_OBJ) -o $@

UTEST_DECODE_SRC := utest_decode.c morse.c
UTEST_DECODE_OBJ := $(addprefix $(BUILD_DIR)/,$(UTEST_DECODE_SRC:.c=.o))
.PHONY: utest_decode
utest_decode: $(BUILD_DIR)/utest_decode.hex
$(BUILD_DIR)/utest_decode.elf: $(UTEST_DECODE_OBJ)
	$(CC_avr) $(LDFLAGS_avr) $(UTEST_DECODE_OBJ) -o $@
