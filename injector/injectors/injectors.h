#pragma once

#include <lua.h>
#include <stddef.h>
#include <stdint.h>
#include <piga-injector_export.h>

PIGA_INJECTOR_NO_EXPORT enum piga_injector_status_t {
    PIGA_INJECTOR_INVALID_LUA_STATE = 1 << 0,
    PIGA_INJECTOR_INVALID_SCRIPT_PATH = 1 << 1,
    PIGA_INJECTOR_ERROR_IN_CONFIG_LUA = 1 << 2,
};

PIGA_INJECTOR_NO_EXPORT struct piga_injector_handle_t {
    char* libGL_path;
    char* libGLX_path;

    int32_t status;

    lua_State *L;
};

PIGA_INJECTOR_NO_EXPORT char* piga_injector_combine_path(const char *p1, const char *p2);

PIGA_INJECTOR_NO_EXPORT const char* piga_injector_get_script_path();

PIGA_INJECTOR_NO_EXPORT struct piga_injector_handle_t* piga_injector_init();

void PIGA_INJECTOR_NO_EXPORT piga_injector_exit();
