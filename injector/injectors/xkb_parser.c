#include "xkb_parser.h"

#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "xkbcomp/xkbcommon.h"

#define CHECK_AND_RETURN(PATH)                                                 \
    if (access(PATH "/us", R_OK) != -1)                                        \
    return PATH

const char *get_xkb_symbols_path() {
    CHECK_AND_RETURN("/usr/share/X11/xkb/symbols/");
    return "";
}

injector_xkb_keyboards_t *injector_keyboards = NULL;

size_t get_number_of_type_in_dir(const char *path, unsigned char type) {
    size_t         counter = 0;
    DIR *          d;
    struct dirent *dir;
    d = opendir(path);

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == type) {
                ++counter;
            }
        }
    }

    closedir(d);

    return counter;
}

injector_xkb_keyboards_t *injector_xkb_get_keyboards() {
    if (injector_keyboards == NULL) {
        injector_keyboards = malloc(sizeof(injector_xkb_keyboards_t));
    } else {
        return injector_keyboards;
    }

    injector_keyboards->keyboards = NULL;

    // Try to find the correct directory of the layouts.
    const char *symbols_path = get_xkb_symbols_path();

    DIR *          d;
    struct dirent *dir;
    d = opendir(symbols_path);

    size_t name_length;

    injector_xkb_keyboard_t *keyboard = NULL;

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) {
#ifndef _DIRENT_HAVE_D_NAMLEN
                name_length = strlen(dir->d_name);
#else
                name_length = dir->d_namelen;
#endif
                injector_xkb_keyboard_t *next_keyboard =
                    malloc(sizeof(injector_xkb_keyboard_t));
                if (injector_keyboards->keyboards == NULL)
                    injector_keyboards->keyboards = next_keyboard;
                else
                    keyboard->next = next_keyboard;
                keyboard = next_keyboard;

                // Name
                keyboard->name = malloc((name_length + 1) *
                                        sizeof(char)); // Add the 0-terminator.
                memcpy(keyboard->name, dir->d_name, name_length);
                keyboard->name[name_length] = '\0';

                // Filename
                size_t filename_length = strlen(symbols_path) + name_length;
                keyboard->filename = malloc(filename_length + 1);
                memcpy(keyboard->filename, symbols_path, strlen(symbols_path));
                strcat(keyboard->filename, keyboard->name);

                // Next keyboard.
                keyboard->next = NULL;
            }
        }
    }

    closedir(d);

    return injector_keyboards;
}

injector_xkb_keys_t *
injector_xkb_get_keys_for_keyboard(injector_xkb_keyboard_t *board) {
    injector_xkb_keys_t *keys = malloc(sizeof(injector_xkb_keys_t));
    struct xkb_context * ctx = NULL;
    struct xkb_keymap *  keymap = NULL;
    ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    if (!ctx) {
        printf("Could not create libxkbcommon context!\n");
        goto CLEANUP;
    }

    FILE *file = fopen(board->filename, "r");
    if (!file) {
        printf("Could not open symbols file \"%s\"!\n", board->filename);
        goto CLEANUP;
    }

    keymap = xkb_keymap_new_from_file(ctx, file, XKB_KEYMAP_FORMAT_TEXT_V1, 0);
    if (!keymap) {
        printf("Could not compile symbols file \"%s\"!\n", board->filename);
    }

    fclose(file);

CLEANUP:
    if (keymap)
        xkb_keymap_unref(keymap);
    if (ctx)
        xkb_context_unref(ctx);

    return keys;
}
