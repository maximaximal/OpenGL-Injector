#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct piga_xkb_key_t {
    /**
     * The symbols array follows the XKB specs. There is the base, a shift, an
     * alt key, shift+alt
     */
    uint32_t syms[4];
} piga_xkb_key_t;

typedef struct piga_xkb_skey_t { uint32_t sym; } piga_xkb_skey_t;

typedef struct piga_xkb_keys_t {
    piga_xkb_key_t *AB;
    size_t          AB_l;

    piga_xkb_key_t *AC;
    size_t          AC_l;

    piga_xkb_key_t *AD;
    size_t          AD_l;

    piga_xkb_key_t *AE;
    size_t          AE_l;

    piga_xkb_key_t *FK;
    size_t          FK_l;

    piga_xkb_key_t  ESC;
    piga_xkb_key_t  TLDE;
    piga_xkb_skey_t TAB;
    piga_xkb_skey_t CAPS;
    piga_xkb_skey_t LFSH;
    piga_xkb_skey_t LCTL;
    piga_xkb_skey_t LWIN;
    piga_xkb_skey_t LALT;

    piga_xkb_skey_t SPCE;

    piga_xkb_skey_t BKSP;
    piga_xkb_skey_t BKSL;
    piga_xkb_skey_t RTRN;
    piga_xkb_skey_t RTSH;
    piga_xkb_skey_t RALT;
    piga_xkb_skey_t RWIN;
    piga_xkb_skey_t MENU;
    piga_xkb_skey_t RCTL;
} piga_xkb_keys_t;

typedef struct piga_xkb_keyboard_t {
    char *name;
    char *filename;

    struct piga_xkb_keyboard_t *next;
} piga_xkb_keyboard_t;

typedef struct piga_xkb_keyboards_t {
    piga_xkb_keyboard_t *keyboards;
} piga_xkb_keyboards_t;

piga_xkb_keyboards_t *piga_xkb_get_keyboards();

piga_xkb_keys_t *piga_xkb_get_keys_for_keyboard(piga_xkb_keyboard_t *board);
