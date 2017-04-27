#pragma once

#include <lua.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <cairo/cairo.h>

#include <piga-injector_export.h>

PIGA_INJECTOR_NO_EXPORT enum piga_injector_status_t {
    PIGA_INJECTOR_INVALID_LUA_STATE = 1 << 0,
    PIGA_INJECTOR_INVALID_SCRIPT_PATH = 1 << 1,
    PIGA_INJECTOR_ERROR_IN_CONFIG_LUA = 1 << 2,
};

PIGA_INJECTOR_NO_EXPORT struct piga_injector_handle_t {
    const char* libGL_path;
    const char* libGLX_path;

    const char* vertex_shader;
    const char* fragment_shader;

    unsigned int window_width;
    unsigned int window_height;

    
    int32_t status;

    lua_State *L;
    cairo_surface_t *cairo_surface;
    cairo_t *cairo_cr;
    int cairo_stride;
    unsigned char *cairo_data;
    bool draw_request;
};

PIGA_INJECTOR_NO_EXPORT char* piga_injector_combine_path(const char *p1, const char *p2);

PIGA_INJECTOR_NO_EXPORT const char* piga_injector_get_script_path();

PIGA_INJECTOR_NO_EXPORT struct piga_injector_handle_t* piga_injector_init();

PIGA_INJECTOR_NO_EXPORT void piga_injector_draw();

void PIGA_INJECTOR_NO_EXPORT piga_injector_exit();

PIGA_INJECTOR_NO_EXPORT char* piga_read_file(const char *path, size_t *length);
