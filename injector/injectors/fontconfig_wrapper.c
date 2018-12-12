#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include "fontconfig_wrapper.h"

void *fontconfig_handle = 0;

void injector_load_fontconfig(const char *path) {
    if (path != 0) {
        if (fontconfig_handle == 0) {
            fontconfig_handle = dlopen(path, RTLD_NOW | RTLD_GLOBAL);
            if (fontconfig_handle != 0) {
                printf("Successfully loaded fontconfig library from \"%s\"!",
                       path);
            } else {
                printf("Could not load fontconfig library from \"%s\"!", path);
            }
        }
    }
}
