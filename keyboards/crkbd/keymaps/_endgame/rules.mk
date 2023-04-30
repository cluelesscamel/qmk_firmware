#EXTRAFLAGS += -flto

SPLIT_KEYBOARD = yes

BOOTLOADER = atmel-dfu

RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = yes
#BACKLIGHT_ENABLE = yes

OLED_ENABLE = yes
OLED_DRIVER = SSD1306

WPM_ENABLE = yes

#n key rollover
NKRO_ENABLE = yes

#tap dance
#TAP_DANCE_ENABLE = yes

#optimisations to save space
LTO_ENABLE = yes
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
MOUSEKEY_ENABLE = no
