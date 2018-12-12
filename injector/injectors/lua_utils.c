#include "lua_utils.h"
#include <lua.h>
#include <stdio.h>

const char *get_global_str(lua_State *L, const char *varname) {
    lua_getglobal(L, varname);
    if (!lua_isstring(L, -1)) {
        return 0;
    }
    const char *str = lua_tostring(L, -1);
    lua_settop(L, 0);
    return str;
}

void injector_lua_call_void_func(lua_State *L, const char *func) {
    lua_getglobal(L, func);

    if (lua_pcall(L, 0, 0, 0) != 0) {
        printf("error running function `%s': %s\n", func, lua_tostring(L, -1));
    }
}
void injector_lua_call_void_func_light_userdata(lua_State *L, const char *func, void *userdata)
{
    lua_getglobal(L, func);

    lua_pushlightuserdata(L, userdata);
    
    if (lua_pcall(L, 1, 0, 0) != 0) {
        printf("error running function `%s': %s\n", func, lua_tostring(L, -1));
    }
}
bool injector_lua_call_bool_func_light_userdata(lua_State *L, const char *func, void *userdata)
{
    lua_getglobal(L, func);

    lua_pushlightuserdata(L, userdata);
    
    if (lua_pcall(L, 1, 1, 0) != 0) {
        printf("error running function `%s': %s\n", func, lua_tostring(L, -1));
    }

    if (!lua_isboolean(L, -1)) {
        printf("Function `%s' must return a boolean.\n", func);
    }

    bool result = lua_toboolean(L, -1);
    lua_pop(L, 1);

    return result;
}
bool injector_lua_call_bool_func(lua_State *L, const char *func) {
    lua_getglobal(L, func);

    if (lua_pcall(L, 0, 1, 0) != 0) {
        printf("error running function `%s': %s\n", func, lua_tostring(L, -1));
    }

    if (!lua_isboolean(L, -1)) {
        printf("Function `%s' must return a boolean.\n", func);
    }

    bool result = lua_toboolean(L, -1);
    lua_pop(L, 1);

    return result;
}

void injector_lua_set_global_int(lua_State *L, int num, const char *name) {
    lua_pushinteger(L, num);
    lua_setglobal(L, name);
}
