local ffi = require( "ffi" )

ffi.cdef [[
typedef struct injector_xkb_layouts_t {
    char ** layouts;
    size_t size;
} injector_xkb_layouts_t;

typedef struct injector_xkb_key_t {
    /**
     * The symbols array follows the XKB specs. There is the base, a shift, an alt key, shift+alt
     */
    uint32_t syms[4];
} injector_xkb_key_t;

typedef struct injector_xkb_skey_t {
    uint32_t sym;
} injector_xkb_skey_t;

typedef struct injector_xkb_keys_t {
    injector_xkb_key_t *AB;
    size_t AB_l;

    injector_xkb_key_t *AC;
    size_t AC_l;

    injector_xkb_key_t *AD;
    size_t AD_l;

    injector_xkb_key_t *AE;
    size_t AE_l;

    injector_xkb_key_t *FK;
    size_t FK_l;

    injector_xkb_key_t ESC;
    injector_xkb_key_t TLDE;
    injector_xkb_skey_t TAB;
    injector_xkb_skey_t CAPS;
    injector_xkb_skey_t LFSH;
    injector_xkb_skey_t LCTL;
    injector_xkb_skey_t LWIN;
    injector_xkb_skey_t LALT;

    injector_xkb_skey_t SPCE;

    injector_xkb_skey_t BKSP;
    injector_xkb_skey_t BKSL;
    injector_xkb_skey_t RTRN;
    injector_xkb_skey_t RTSH;
    injector_xkb_skey_t RALT;
    injector_xkb_skey_t RWIN;
    injector_xkb_skey_t MENU;
    injector_xkb_skey_t RCTL;
} injector_xkb_keys_t;

typedef struct injector_xkb_keyboard_t {
    char *name;
    char *filename;

    struct injector_xkb_keyboard_t *next;
} injector_xkb_keyboard_t;

typedef struct injector_xkb_keyboards_t {
    injector_xkb_keyboard_t *keyboards;
} injector_xkb_keyboards_t;

injector_xkb_keyboards_t *injector_xkb_get_keyboards();

injector_xkb_keys_t *injector_xkb_get_keys_for_keyboard(injector_xkb_keyboard_t *board);

/* EVENTS */
typedef struct injector_key_event_t {
    int  key;
    bool pressed;
} injector_key_event_t;
typedef struct injector_window_event_t {
    int width;
    int height;
} injector_window_event_t;
typedef struct injector_button_event_t {
    int button;
    int x;
    int y;
} injector_button_event_t;
typedef struct injector_motion_event_t {
    int x;
    int y;
} injector_motion_event_t;

]]

return ffi.C
