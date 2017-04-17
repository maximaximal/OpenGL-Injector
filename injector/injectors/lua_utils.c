#include "lua_utils.h"
#include <lua.h>

const char* get_global_str(lua_State *L, const char *varname)
{
    lua_getglobal(L, varname);
    const char* str = lua_tostring(L, -1);
    lua_settop(L, 0);
    return str;
}
