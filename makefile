CC_avr      := avr-gcc
CFLAGS_avr  := -Wall -mmcu=atmega8535 -O2 -std=c99 -c
LDFLAGS_avr :=       -mmcu=atmega8535

BUILD_DIR  := build

all: main utest_led_matrix utest_morse utest_timer_event

-include $(BUILD_DIR)/*.d

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC_avr) $(CFLAGS_avr) $< -o $@

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)


MAIN_SRC := main.c morse.c timer_event.c led_matrix.c
MAIN_OBJ := $(addprefix $(BUILD_DIR)/,$(MAIN_SRC:.c=.o))
.PHONY: main
main: $(BUILD_DIR)/main.hex
$(BUILD_DIR)/main.elf: $(MAIN_OBJ)
	$(CC_avr) $(LDFLAGS_avr) $(MAIN_OBJ) -o $@


UTEST_LED_MATRIX_SRC := utest_led_matrix.c led_matrix.c
UTEST_LED_MATRIX_OBJ := $(addprefix $(BUILD_DIR)/,$(UTEST_LED_MATRIX_SRC:.c=.o))
.PHONY: utest_led_matrix
utest_led_matrix: $(BUILD_DIR)/utest_led_matrix.hex
$(BUILD_DIR)/utest_led_matrix.elf: $(UTEST_LED_MATRIX_OBJ)
	$(CC_avr) $(LDFLAGS_avr) $(UTEST_LED_MATRIX_OBJ) -o $@


UTEST_MORSE_SRC := utest_morse.c morse.c led_matrix.c
UTEST_MORSE_OBJ := $(addprefix $(BUILD_DIR)/,$(UTEST_MORSE_SRC:.c=.o))
.PHONY: utest_morse
utest_morse: $(BUILD_DIR)/utest_morse.hex
$(BUILD_DIR)/utest_morse.elf: $(UTEST_MORSE_OBJ)
	$(CC_avr) $(LDFLAGS_avr) $(UTEST_MORSE_OBJ) -o $@


UTEST_TIMER_EVENT_SRC := utest_timer_event.c timer_event.c led_matrix.c
UTEST_TIMER_EVENT_OBJ := $(addprefix $(BUILD_DIR)/,$(UTEST_TIMER_EVENT_SRC:.c=.o))
.PHONY: utest_timer_event
utest_timer_event: $(BUILD_DIR)/utest_timer_event.hex
$(BUILD_DIR)/utest_timer_event.elf: $(UTEST_TIMER_EVENT_OBJ)
	$(CC_avr) $(LDFLAGS_avr) $(UTEST_TIMER_EVENT_OBJ) -o $@

UTEST_BUTTON_SRC := utest_button.c led_matrix.c
UTEST_BUTTON_OBJ := $(addprefix $(BUILD_DIR)/,$(UTEST_BUTTON_SRC:.c=.o))
.PHONY: utest_button
utest_button: $(BUILD_DIR)/utest_button.hex
$(BUILD_DIR)/utest_button.elf: $(UTEST_BUTTON_OBJ)
	$(CC_avr) $(LDFLAGS_avr) $(UTEST_BUTTON_OBJ) -o $@
