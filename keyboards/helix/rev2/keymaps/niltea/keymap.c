#include QMK_KEYBOARD_H
#include "bootloader.h"
#ifdef PROTOCOL_LUFA
#include "lufa.h"
#include "split_util.h"
#endif
#ifdef AUDIO_ENABLE
  #include "audio.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif
#ifdef CONSOLE_ENABLE
  #include <print.h>
#endif

extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_number {
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _PADFUNC,
    _ADJUST,
};

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  RGBRST
};

//Macros
#define KC_LOWER  MO(_LOWER)
#define KC_RAISE  MO(_RAISE)
#define KC_RABS   LT(_RAISE,KC_BSPC)
#define KC_RAEN   LT(_RAISE,KC_ENT)
#define KC_FF12   LT(_PADFUNC,KC_F12)
#define KC_____   _______
#define KC_XXXX   XXXXXXX
#define KC_ADJ    MO(_ADJUST)
#define KC_LSMI   LSFT(KC_MINS)
#define KC_LSEQ   LSFT(KC_EQL)
#define KC_LSRB   LSFT(KC_RBRC)
#define KC_LSLB   LSFT(KC_LBRC)
#define ___       _______

#if MATRIX_ROWS == 10 // HELIX_ROWS == 5
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  /* Qwerty
   * ,-----------------------------------------.             ,-----------------------------------------.
   * | ESC  |   1  |   2  |   3  |   4  |   5  |             |   6  |   7  |   8  |   9  |   0  |  BS  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Tab  |   Q  |   W  |   E  |   R  |   T  |             |   Y  |   U  |   I  |   O  |   P  |   \  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | CAPS |   A  |   S  |   D  |   F  |   G  |             |   H  |   J  |   K  |   L  |   ;  |   '  |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |   Z  |   X  |   C  |   V  |   B  |   [  |   ]  |   N  |   M  |   ,  |   .  |   /  |   -  |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Shift|      | Ctrl |  Alt | LOWER|  GUI |Space | Enter| GUI　| RAISE|      |      |      | Shift|
   * `-------------------------------------------------------------------------------------------------'
   */
  [_QWERTY] = LAYOUT_kc( \
      ESC,  1,      2,     3,      4,     5,                  6,    7,     8,      9,     0,     BSPC, \
      TAB,  Q,      W,     E,      R,     T,                  Y,    U,     I,      O,     P,     BSLS, \
      CAPS, A,      S,     D,      F,     G,                  H,    J,     K,      L,     SCLN,  MINS, \
      XXXX, Z,      X,     C,      V,     B,    LBRC,  RBRC,  N,    M,     COMM,   DOT,   SLSH,  QUOT, \
      LSFT, XXXX,   LCTL,  LALT,   LOWER, LGUI, SPC,   RAEN,  RGUI, RAISE, BSPC,   ____,  ____,  RSFT \
      ),

  /* Lower
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |   `  |  F1  |  F2  |  F3  |  F4  |  F5  |             |  F6  |  F7  |  F8  |  F9  |  F10 | F11  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |      |      |      |      |      | F12  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      | Home | PgUp | Left | Up   | Right|             | Left | Up   | Right| PgUp | Home |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      | End  | PgDn |      | Down |      |      |      | Left | Down | Right| PgDn |  End |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |Adjust|Adjust|      |      | DEL  |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_LOWER] = LAYOUT_kc( \
      GRV,  F1,    F2,    F3,    F4,    F5,                  F6,    F7,    F8,    F9,    F10,   F11, \
      XXXX, XXXX,  XXXX,  XXXX,  XXXX,  XXXX,                XXXX,  XXXX,  XXXX,  XXXX,  XXXX,  F12, \
      XXXX, HOME,  PGUP,  LEFT,  UP,    RGHT,                LEFT,  LEFT,  UP,    RGHT,  ____,  ____, \
      ____, END,   PGDN,  XXXX,  DOWN,  XXXX,  XXXX,  XXXX,  LEFT,  LEFT,  DOWN,  RGHT,  END,   ____, \
      ____, ____,  ____,  ____,  ____,  ____,  ADJ,   ADJ,   ____,  ____,  DEL,   ____,  ____,  ____ \
      ),

  /* Raise
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |      |      |      |      |      |      |             | Play | Next | Vol- | Vol+ | Mute |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      |  _   |      |             |  =   |  +   |      |      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      |  {   |  [   |             | Left | Up   | Right| PgUp | Home |  +   |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |  -   |      | Left | Down | Right| PgDn |  End |  =   |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      | DEL  |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_RAISE] = LAYOUT_kc( \
      XXXX, XXXX, XXXX, XXXX, XXXX, XXXX,             MPLY, MNXT, VOLD, VOLU, MUTE, XXXX, \
      XXXX, XXXX, XXXX, XXXX, LSMI, XXXX,             EQL,  LSEQ, XXXX, XXXX, XXXX, LSEQ, \
      ____, XXXX, XXXX, XXXX, LSLB, LBRC,             LEFT, LEFT, UP,   RGHT,  EQL, PLUS, \
      ____, XXXX, XXXX, XXXX, XXXX, ____, MINS, XXXX, LEFT, LEFT, DOWN, RGHT, ____, ____, \
      ____, ____, XXXX, ____, ____, XXXX,  ADJ,  ADJ, XXXX, ____, ____, DEL,  ____, ____ \
      ),

  /*  Keypad function layer
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |      |      |      | Pause| ScrLk| PtrSc|             | PtrSc| ScrLk| Pause|      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      | Home |  Up  | PgUp |             | PgUp |  Up  | Home |      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |Delete|Insert| Left | Down | Right|             | Left | Down | Right|Insert|Delete|      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      | End  |      | PgDn |Adjust|Adjust| PgDn |      | End  |      |      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_PADFUNC] = LAYOUT_kc( \
      XXXX, XXXX, XXXX, PAUS, SLCK, PSCR,             PSCR, SLCK, PAUS, XXXX, XXXX, XXXX, \
      XXXX, XXXX, XXXX, HOME, UP,   PGUP,             PGUP, UP,   HOME, XXXX, XXXX, XXXX, \
      XXXX,  DEL,  INS, LEFT, DOWN, RGHT,             LEFT, DOWN, RGHT, INS,  DEL,  XXXX, \
      XXXX, XXXX, XXXX, END,  XXXX, PGDN,  ADJ,  ADJ, PGDN, XXXX, END,  XXXX, XXXX, XXXX, \
      XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, ____, ____, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX \
   ),

  /* Adjust (Lower + Raise)
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |      |      |      |      |      |Qwerty|             |Qwerty|      |      |      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      | Reset|RGBRST|RGB ON|Aud on| Win  |             | Win  |Aud on|RGB ON|RGBRST|      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | HUE+ | SAT+ | VAL+ |RGB md|Audoff| Mac  |             | Mac  |Audoff|RGB md| VAL+ | SAT+ | HUE+ |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | HUE- | SAT- | VAL- |      |      |      |      |      |      |      |      | VAL- | SAT- | HUE- |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_ADJUST] =  LAYOUT( \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  QWERTY,          QWERTY,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
      XXXXXXX, RESET,   RGBRST,  RGB_TOG, XXXXXXX, AG_SWAP,          AG_SWAP,   AU_ON, RGB_TOG,  RGBRST, XXXXXXX, XXXXXXX, \
      RGB_HUI, RGB_SAI, RGB_VAI, RGB_MOD,  AU_OFF, AG_NORM,          AG_NORM,  AU_OFF, RGB_MOD, RGB_VAI, RGB_SAI, RGB_HUI, \
      RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX, XXXXXXX, XXXXXXX, ___,___, XXXXXXX, XXXXXXX, XXXXXXX, RGB_VAD, RGB_SAD, RGB_HUD, \
      _______, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, ___,___, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, _______ \
   )
};

#else
#error "undefined keymaps"
#endif


#ifdef AUDIO_ENABLE

float tone_qwerty[][2]     = SONG(QWERTY_SOUND);
float tone_dvorak[][2]     = SONG(DVORAK_SOUND);
float tone_colemak[][2]    = SONG(COLEMAK_SOUND);
float tone_plover[][2]     = SONG(PLOVER_SOUND);
float tone_plover_gb[][2]  = SONG(PLOVER_GOODBYE_SOUND);
float music_scale[][2]     = SONG(MUSIC_SCALE_SOUND);
#endif

static int current_default_layer;

uint32_t default_layer_state_set_kb(uint32_t state) {
    return state;
}

void update_base_layer(int base)
{
  eeconfig_update_default_layer(1UL<<base);
  default_layer_set(1UL<<base);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(tone_qwerty);
        #endif
        update_base_layer(_QWERTY);
      }
      return false;
      break;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
        }
      #endif
      break;
  }
  return true;
}

void matrix_init_user(void) {
    #ifdef AUDIO_ENABLE
        startup_user();
    #endif
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
        iota_gfx_init(!has_usb());   // turns on the display
    #endif
}


#ifdef AUDIO_ENABLE

void startup_user()
{
    _delay_ms(20); // gets rid of tick
}

void shutdown_user()
{
    _delay_ms(150);
    stop_all_notes();
}

void music_on_user(void)
{
    music_scale_user();
}

void music_scale_user(void)
{
    PLAY_SONG(music_scale);
}

#endif


//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

void matrix_scan_user(void) {
     iota_gfx_task();  // this is what updates the display continuously
}

void matrix_update(struct CharacterMatrix *dest,
                          const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

static void render_logo(struct CharacterMatrix *matrix) {

  static char logo[]={
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,
    0};
  matrix_write(matrix, logo);
#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_ANIMATIONS)
  char buf[30];
  if(rgblight_config.enable) {
      snprintf(buf, sizeof(buf), " LED %2d: %d,%d,%d ",
               rgblight_config.mode,
               rgblight_config.hue/RGBLIGHT_HUE_STEP,
               rgblight_config.sat/RGBLIGHT_SAT_STEP,
               rgblight_config.val/RGBLIGHT_VAL_STEP);
      matrix_write(matrix, buf);
  }
#endif
  //matrix_write_P(&matrix, PSTR(" Split keyboard kit"));
}

static const char Qwerty_name[]  PROGMEM = " Qwerty";

static const char Padfunc_name[] PROGMEM = ":PadFunc";
static const char Lower_name[]   PROGMEM = ":Lower";
static const char Raise_name[]   PROGMEM = ":Raise";
static const char Adjust_name[]  PROGMEM = ":Adjust";

static const char *layer_names[] = {
    [_QWERTY] = Qwerty_name,
    [_LOWER]  = Lower_name,
    [_RAISE]  = Raise_name,
    [_PADFUNC]= Padfunc_name,
    [_ADJUST] = Adjust_name
};

void render_status(struct CharacterMatrix *matrix) {

  // Render to mode icon
  static char logo[][2][3]={{{0x95,0x96,0},{0xb5,0xb6,0}},{{0x97,0x98,0},{0xb7,0xb8,0}}};
  if(keymap_config.swap_lalt_lgui==false){
    matrix_write(matrix, logo[0][0]);
    matrix_write_P(matrix, PSTR("\n"));
    matrix_write(matrix, logo[0][1]);
  }else{
    matrix_write(matrix, logo[1][0]);
    matrix_write_P(matrix, PSTR("\n"));
    matrix_write(matrix, logo[1][1]);
  }

  // Define layers here, Have not worked out how to have text displayed for each layer. Copy down the number you see and add a case for it below
  int name_num;
  uint32_t lstate;
  matrix_write_P(matrix, layer_names[current_default_layer]);
  matrix_write_P(matrix, PSTR("\n"));
  for( lstate = layer_state, name_num = 0;
       lstate && name_num < sizeof(layer_names)/sizeof(char *);
       lstate >>=1, name_num++ ) {
      if( (lstate & 1) != 0 ) {
          if( layer_names[name_num] ) {
              matrix_write_P(matrix, layer_names[name_num]);
          }
      }
  }

  // Host Keyboard LED Status
  char led[40];
  snprintf(led, sizeof(led), "\n%s", (host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK)) ? "CAPS" : "");
  matrix_write(matrix, led);
}


void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;

#if DEBUG_TO_SCREEN
  if (debug_enable) {
    return;
  }
#endif

  matrix_clear(&matrix);
  if(is_master){
    render_status(&matrix);
  }else{
    render_logo(&matrix);
  }
  matrix_update(&display, &matrix);
}

#endif
