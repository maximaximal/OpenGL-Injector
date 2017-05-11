#include "xkb_parser.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CHECK_AND_RETURN(PATH)                                                 \
    if (access(PATH "/us", R_OK) != -1)                                        \
    return PATH

const char *get_xkb_symbols_path() {
    CHECK_AND_RETURN("/usr/share/X11/xkb/symbols/");
}

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

piga_xkb_layouts_t piga_xkb_get_available_layouts() {
    piga_xkb_layouts_t layouts;
    layouts.layouts = NULL;
    layouts.size = 0;

    // Try to find the correct directory of the layouts.
    const char *symbols_path = get_xkb_symbols_path();

    // How many layouts will there be?
    layouts.size = get_number_of_type_in_dir(symbols_path, DT_REG);
    layouts.layouts = malloc(layouts.size * sizeof(char *));

    DIR *          d;
    struct dirent *dir;
    d = opendir(symbols_path);

    size_t name_length;
    size_t i = 0;

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) {
#ifndef _DIRENT_HAVE_D_NAMLEN
                name_length = strlen(dir->d_name);
#else
                name_length = dir->d_namelen;
#endif
                layouts.layouts[i] = malloc(
                    (name_length + 1) * sizeof(char)); // Add the 0-terminator.
                memcpy(layouts.layouts[i], dir->d_name, name_length);
                layouts.layouts[i][name_length] = '\0';

                ++i;
            }
        }
    }

    closedir(d);

    return layouts;
}

void piga_xkb_free_layouts(piga_xkb_layouts_t *layouts) {
    if (layouts != NULL) {
        for (size_t i = 0; i < layouts->size; ++i) {
            free(layouts->layouts[i]);
        }
    }
}

piga_xkb_keys_t *piga_xkb_get_keys_for_layout(const char *layout) {}
