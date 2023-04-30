#include QMK_KEYBOARD_H

//layer enumeration
enum layers {
	_COL,
	_QWE,
	_GAM,
	_NUM,
	_SYM,
	_FUN,
	_NAV,
};

enum custom_keycodes {
	GGWPTY = SAFE_RANGE
};

// include the keymap export file
#include "./keymap_.c"

//custom keycode handlers
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case GGWPTY:
        if (record->event.pressed) {
			//pressed
            SEND_STRING(SS_TAP(X_ENT) SS_DELAY(100) "gg wp ty" SS_DELAY(100) SS_TAP(X_ENT));
        } else {
			//released
        }
        break;
    }
    return true;
};

#if RGB_MATRIX_ENABLE

uint8_t prev_layer = _COL;
uint32_t desired_mode = RGB_MATRIX_ALPHAS_MODS;

layer_state_t layer_state_set_user(layer_state_t state) {
	uint8_t current_layer = get_highest_layer(state | default_layer_state);

	//store rgb when setting the rgb from fun layer
	//_GAM layer and its RGB is not set from _FUN layer
	if (prev_layer != _FUN) {
		switch (get_highest_layer(state | default_layer_state)) {
		case _GAM:
			//set game mode rainbow
			rgb_matrix_mode_noeeprom(RGB_MATRIX_RAINBOW_MOVING_CHEVRON);
			break;
		default:
			//for any other layers, revert to desired
			rgb_matrix_mode_noeeprom(desired_mode);
			break;
		}
	} else {
		//when on layer FUN set desired kb rgb mode
		desired_mode = rgb_matrix_get_mode();
	}

	prev_layer = current_layer;
	return state;
}

#endif //RGB_MATRIX_ENABLE

#ifdef OLED_ENABLE

// flip the screen according to which half the display is on.
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    /*if (!is_keyboard_master()) {*/
    /*return OLED_ROTATION_180;*/
    /*}*/
    return OLED_ROTATION_270;
}

void render_logo(void) {
    static const char PROGMEM corne_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0};
    oled_write_P(corne_logo, false);
}

void render_layer_state(void) {
    switch (get_highest_layer(layer_state | default_layer_state)) {
        case _NUM:
            oled_write_P(PSTR(" NUM "), false);
            break;
        case _SYM:
            oled_write_P(PSTR(" SYM "), false);
            break;
        case _FUN:
            oled_write_P(PSTR(" FUN "), false);
            break;
        case _NAV:
            oled_write_P(PSTR(" NAV "), false);
            break;
        case _GAM:
            oled_write_P(PSTR(" GAM "), false);
            break;
        case _QWE:
            oled_write_P(PSTR(" QWE "), false);
            break;
        default:
			oled_write_P(PSTR(" COL "), false);

            break;
    }
}

void render_wpm(int WPM_X, int WPM_Y) {
    /* wpm counter */
    uint8_t n = get_current_wpm();
    oled_set_cursor(WPM_X + 1, WPM_Y);
    oled_write(get_u8_str(n, '0'), false);

    oled_set_cursor(WPM_X, WPM_Y + 1);
}

void render_master_cheat_sheet(int CHEAT_X, int CHEAT_Y) {
    oled_set_cursor(CHEAT_X, CHEAT_Y);

    switch (get_highest_layer(layer_state)) {
        case _NUM:
            oled_write("     ", false);
            oled_write(" 123 ", false);
            oled_write(" 4567", false);
            oled_write("     ", false);
            break;
        case _SYM:
            oled_write("^#$%<", false);
            oled_write("*!+=>", false);
            oled_write("/\\|&@", false);
            oled_write("     ", false);
            break;
        default:
            oled_write("     ", false);
			render_logo();
            break;
    }
}

void render_offhand_cheat_sheet(int CHEAT_X, int CHEAT_Y) {
    oled_set_cursor(CHEAT_X, CHEAT_Y);

    switch (get_highest_layer(layer_state)) {
        case _NUM:
            oled_write("-789/", false);
            oled_write("+456*", false);
            oled_write("=123 ", false);
            oled_write("S0.  ", false);
            break;
        case _SYM:
            oled_write("?()\"'", false);
            oled_write("-{}:;", false);
            oled_write("_[]~`", false);
            oled_write("     ", false);
            break;
        default:
            oled_write("     ", false);
			render_logo();
            break;
    }
}

void render_space(void) {
    oled_write_P(PSTR("     "), false);
}

void render_mod_status_gui_alt(uint8_t modifiers) {
    static const char PROGMEM gui_off_1[] = {0x85, 0x86, 0};
    static const char PROGMEM gui_off_2[] = {0xa5, 0xa6, 0};
    static const char PROGMEM gui_on_1[] = {0x8d, 0x8e, 0};
    static const char PROGMEM gui_on_2[] = {0xad, 0xae, 0};

    static const char PROGMEM alt_off_1[] = {0x87, 0x88, 0};
    static const char PROGMEM alt_off_2[] = {0xa7, 0xa8, 0};
    static const char PROGMEM alt_on_1[] = {0x8f, 0x90, 0};
    static const char PROGMEM alt_on_2[] = {0xaf, 0xb0, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_1[] = {0xc5, 0};
    static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_1[] = {0xc7, 0};
    static const char PROGMEM on_off_2[] = {0xc8, 0};
    static const char PROGMEM off_on_1[] = {0xc9, 0};
    static const char PROGMEM off_on_2[] = {0xca, 0};
    static const char PROGMEM on_on_1[] = {0xcb, 0};
    static const char PROGMEM on_on_2[] = {0xcc, 0};

    if(modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_1, false);
    } else {
        oled_write_P(gui_off_1, false);
    }

    if ((modifiers & MOD_MASK_GUI) && (modifiers & MOD_MASK_ALT)) {
        oled_write_P(on_on_1, false);
    } else if(modifiers & MOD_MASK_GUI) {
        oled_write_P(on_off_1, false);
    } else if(modifiers & MOD_MASK_ALT) {
        oled_write_P(off_on_1, false);
    } else {
        oled_write_P(off_off_1, false);
    }

    if(modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_1, false);
    } else {
        oled_write_P(alt_off_1, false);
    }

    if(modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_2, false);
    } else {
        oled_write_P(gui_off_2, false);
    }

    if (modifiers & MOD_MASK_GUI & MOD_MASK_ALT) {
        oled_write_P(on_on_2, false);
    } else if(modifiers & MOD_MASK_GUI) {
        oled_write_P(on_off_2, false);
    } else if(modifiers & MOD_MASK_ALT) {
        oled_write_P(off_on_2, false);
    } else {
        oled_write_P(off_off_2, false);
    }

    if(modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_2, false);
    } else {
        oled_write_P(alt_off_2, false);
    }
}

void render_mod_status_ctrl_shift(uint8_t modifiers) {
    static const char PROGMEM ctrl_off_1[] = {0x89, 0x8a, 0};
    static const char PROGMEM ctrl_off_2[] = {0xa9, 0xaa, 0};
    static const char PROGMEM ctrl_on_1[] = {0x91, 0x92, 0};
    static const char PROGMEM ctrl_on_2[] = {0xb1, 0xb2, 0};

    static const char PROGMEM shift_off_1[] = {0x8b, 0x8c, 0};
    static const char PROGMEM shift_off_2[] = {0xab, 0xac, 0};
    static const char PROGMEM shift_on_1[] = {0xcd, 0xce, 0};
    static const char PROGMEM shift_on_2[] = {0xcf, 0xd0, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_1[] = {0xc5, 0};
    static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_1[] = {0xc7, 0};
    static const char PROGMEM on_off_2[] = {0xc8, 0};
    static const char PROGMEM off_on_1[] = {0xc9, 0};
    static const char PROGMEM off_on_2[] = {0xca, 0};
    static const char PROGMEM on_on_1[] = {0xcb, 0};
    static const char PROGMEM on_on_2[] = {0xcc, 0};

    if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_1, false);
    } else {
        oled_write_P(ctrl_off_1, false);
    }

    if ((modifiers & MOD_MASK_CTRL) && (modifiers & MOD_MASK_SHIFT)) {
        oled_write_P(on_on_1, false);
    } else if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(on_off_1, false);
    } else if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(off_on_1, false);
    } else {
        oled_write_P(off_off_1, false);
    }

    if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_1, false);
    } else {
        oled_write_P(shift_off_1, false);
    }

    if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_2, false);
    } else {
        oled_write_P(ctrl_off_2, false);
    }

    if (modifiers & MOD_MASK_CTRL & MOD_MASK_SHIFT) {
        oled_write_P(on_on_2, false);
    } else if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(on_off_2, false);
    } else if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(off_on_2, false);
    } else {
        oled_write_P(off_off_2, false);
    }

    if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_2, false);
    } else {
        oled_write_P(shift_off_2, false);
    }
}

void render_layer_icon(void) {
    static const char PROGMEM std_layer[] = {
        0x20, 0x94, 0x95, 0x96, 0x20,
        0x20, 0xb4, 0xb5, 0xb6, 0x20,
        0x20, 0xd4, 0xd5, 0xd6, 0x20, 0};
    static const char PROGMEM gam_layer[] = {
        0x20, 0x9d, 0x9e, 0x9f, 0x20,
        0x20, 0xbd, 0xbe, 0xbf, 0x20,
        0x20, 0xdd, 0xde, 0xdf, 0x20, 0};
    static const char PROGMEM num_layer[] = {
        0x20, 0x09, 0x0a, 0x0b, 0x20,
        0x20, 0x0c, 0x0d, 0x0e, 0x20,
        0x20, 0x0f, 0x10, 0x11, 0x20, 0};
    static const char PROGMEM sym_layer[] = {
        0x20, 0x97, 0x98, 0x99, 0x20,
        0x20, 0xb7, 0xb8, 0xb9, 0x20,
        0x20, 0xd7, 0xd8, 0xd9, 0x20, 0};
    static const char PROGMEM fun_layer[] = {
        0x20, 0x00, 0x01, 0x02, 0x20,
        0x20, 0x03, 0x04, 0x05, 0x20,
        0x20, 0x06, 0x07, 0x08, 0x20, 0};
    static const char PROGMEM nav_layer[] = {
        0x20, 0x9a, 0x9b, 0x9c, 0x20,
        0x20, 0xba, 0xbb, 0xbc, 0x20,
        0x20, 0xda, 0xdb, 0xdc, 0x20, 0};

	switch (get_highest_layer(layer_state | default_layer_state)) {
	case _NUM:
		oled_write_P(num_layer, false);
		break;
	case _SYM:
		oled_write_P(sym_layer, false);
		break;
	case _FUN:
		oled_write_P(fun_layer, false);
		break;
	case _NAV:
		oled_write_P(nav_layer, false);
		break;
	case _GAM:
		oled_write_P(gam_layer, false);
		break;
	default:
		oled_write_P(std_layer, false);
		break;
	}
}

// event loop
bool oled_task_user(void) {
	if (is_keyboard_master()) {
		render_layer_state();
		render_space();
		render_master_cheat_sheet(0, 3);
		render_space();
		render_space();
		render_mod_status_gui_alt(get_mods()|get_oneshot_mods());
		render_mod_status_ctrl_shift(get_mods()|get_oneshot_mods());
	} else {
		render_wpm(0, 0);
		render_space();
		render_offhand_cheat_sheet(0, 3);
		render_space();
		render_space();
		render_layer_icon();
	}

	return false;
}

// turn oled off when computer sleeps
void suspend_power_down_user(void) {
    oled_off();
    rgb_matrix_set_suspend_state(true);
    /*rgb_matrix_disable();*/  // ends up disabling and thus losing what was active before
}

void suspend_wakeup_init_user(void) {
    oled_on();
    rgb_matrix_set_suspend_state(false);
}

#endif  // OLED_ENABLE
