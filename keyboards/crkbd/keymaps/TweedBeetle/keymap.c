/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <math.h>

void ps2_mouse_moved_user(report_mouse_t *mouse_report) {
    double radians = (PS2_MOUSE_ROTATE * M_PI) / 180.0;
    mouse_xy_report_t x = mouse_report->x;
    mouse_xy_report_t y = mouse_report->y;

    mouse_report->x = (int16_t)(x * cos(radians) - y * sin(radians));
    mouse_report->y = (int16_t)(x * sin(radians) + y * cos(radians));
}

enum custom_keycodes {
    F13_SPC = SAFE_RANGE,
    F13_Q,
    F13_E,
    F13_MM,
};


// General combo function that applies active mods
static void combo(uint16_t key1, uint16_t key2, keyrecord_t *record) {
    uint8_t mods = get_mods(); // Store current mods
    if (record->event.pressed) {
        // Apply mods before pressing the keys
        add_mods(mods);
        register_code(key1);
        register_code(key2);
    } else {
        unregister_code(key2);
        unregister_code(key1);
        // Clear mods if they were applied
        del_mods(mods);
    }
}

// f13_combo as a wrapper to call combo with KC_F13
static void f13_combo(uint16_t key, keyrecord_t *record) {
    combo(KC_F13, key, record);
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case F13_SPC:
            f13_combo(KC_SPC, record);
        return false; // Skip further processing of this key
        case F13_Q:
            f13_combo(KC_Q, record);
        return false;
        case F13_E:
            f13_combo(KC_E, record);
        return false;
        case F13_MM:
            f13_combo(KC_MS_BTN3, record); // for some reason, this first clicks f13 and then clicks the mm button. No overlap :/ @todo:0: investigate
        return false;
        default:
            return true; // Process all other keycodes normally
    }
}


// # @todo:0: what is currently simply M(1) could do something when just tapped
// # @todo:0: it would be nice if holdong br would allow scrolling using mouse

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, XXXXXXX, XXXXXXX, XXXXXXX,   MO(1),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, KC_BTN1,  KC_F14,     KC_SPC, KC_MS_BTN3, XXXXXXX
                                      //`--------------------------'  `--------------------------'

  ),

  [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX,   F13_Q,   KC_UP,  F13_E, XXXXXXX,                      XXXXXXX, KC_BSPC,  KC_DEL,  KC_ESC, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, KC_LEFT, KC_DOWN,KC_RIGHT, XXXXXXX,                      KC_ENT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, XXXXXXX, XXXXXXX,    F13_SPC, F13_MM, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  ),
};
