CC_avr      := avr-gcc
CFLAGS_avr  := -Wall -mmcu=atmega8535 -O2 -c
LDFLAGS_avr :=       -mmcu=atmega8535

BUILD_DIR  := build

all: main

-include $(BUILD_DIR)/*.d

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC_avr) $(CFLAGS_avr) $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)


MAIN_SRC := main.c
MAIN_OBJ := $(addprefix $(BUILD_DIR)/,$(MAIN_SRC:.c=.o))
.PHONY: main
main: $(BUILD_DIR)/main
$(BUILD_DIR)/main: $(MAIN_OBJ)
	$(CC_avr) $(LDFLAGS_avr) $(MAIN_OBJ) -o $@
