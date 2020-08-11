#include QMK_KEYBOARD_H
#include "keymap_jp.h"
// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_number {
  _QWERTY = 0,
  _ADJUST,
};

enum custom_keycodes {
  RGB_RST = SAFE_RANGE,
  IN_BRC,  // ※日本語入力時、すみかっこ"【】"を入力してかっこ内にカーソルを移動する
  MGDL,  // mg/dlを入力する
};

// Macスリープ
#define KC_SLP LGUI(LALT(KC_POWER))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT(
  //,-----------------------------------------------------|   |--------------------------------------------------------------------------------.
       KC_ESC,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,       KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,  KC_INS, KC_PSCR,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------+--------|
LT(_ADJUST,KC_ZKHK),KC_1,  KC_2,    KC_3,    KC_4,    KC_5,        KC_6,    KC_7,    KC_8,    KC_9,    KC_0, JP_MINS,  JP_EQL,  JP_YEN, KC_BSPC,  KC_HOME,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------+--------|
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,   JP_AT, JP_LBRC,  KC_ENT, KC_PGUP,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------+--------|
      KC_CAPS,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L, JP_SCLN, JP_QUOT, JP_RBRC,           KC_PGDN,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, JP_BSLS, KC_DEL,   KC_UP, KC_END,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------+--------|
     KC_LCTRL, KC_LGUI, KC_LALT, KC_MHEN,  KC_SPC,  KC_SPC,      KC_SPC,  KC_SPC, KC_HENK, KC_KANA,  KC_APP,          KC_LEFT, KC_DOWN,KC_RIGHT
  //`-----------------------------------------------------|   |--------------------------------------------------------------------------------'
  ),
  [_ADJUST] = LAYOUT( /* Base */
  //,-----------------------------------------------------|   |--------------------------------------------------------------------------------.
       KC_SLP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_VOLD, KC_VOLU, KC_MUTE, KC_BRID, KC_BRIU, XXXXXXX, XXXXXXX, XXXXXXX, RESET,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------+--------|
  MO(_ADJUST), RGB_TOG, XXXXXXX, RGB_VAI, RGB_HUI, RGB_SAI,     XXXXXXX, XXXXXXX, IN_BRC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------+--------|
      RGB_RST, RGB_MOD, XXXXXXX, RGB_VAD, RGB_HUD, RGB_SAD,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX,    MGDL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX, XXXXXXX
  //`-----------------------------------------------------|   |--------------------------------------------------------------------------------'
  )
};

int RGB_current_mode;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  bool result = false;
  switch (keycode) {
    #ifdef RGBLIGHT_ENABLE
      case RGB_MOD:
          if (record->event.pressed) {
            rgblight_mode(RGB_current_mode);
            rgblight_step();
            RGB_current_mode = rgblight_get_mode();
          }
        break;
      case RGB_RST:
          if (record->event.pressed) {
            eeconfig_update_rgblight_default();
            rgblight_enable();
            RGB_current_mode = rgblight_get_mode();
          }
        break;
    #endif
    // すみかっこのマクロ
    case IN_BRC:
        if (record->event.pressed) {
            register_code(KC_LSFT);
            tap_code(KC_8);
            tap_code(KC_9);
            unregister_code(KC_LSFT);
            tap_code(KC_SPC);
            tap_code(KC_ENT);
            tap_code(KC_LEFT);
        }
        break;
    case MGDL:
        if (record->event.pressed) {
            SEND_STRING("mg/dl");
        }
    default:
        result = true;
        break;
}

  return result;
}
