#pragma once

#include <lua.h>
#include <stdbool.h>

const char *get_global_str(lua_State *L, const char *varname);

void piga_lua_call_void_func(lua_State *L, const char *func);
bool piga_lua_call_bool_func(lua_State *L, const char *func);

void piga_lua_set_global_int(lua_State *L, int num, const char *name);
