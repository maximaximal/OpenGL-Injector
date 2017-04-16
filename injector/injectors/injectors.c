#include "injectors.h"
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include <cairo/cairo.h>

struct piga_injector_handle_t *injector_handle = 0;

#define CHECK_AND_RETURN(PATH) if(access( PATH "/lua/config.lua", R_OK) != -1) return PATH

const char* piga_injector_get_script_path()
{
    CHECK_AND_RETURN("piga-injector");
    CHECK_AND_RETURN("~/.config/piga-injector");

    return 0;
}

struct piga_injector_handle_t*  piga_injector_init()
{
    if(injector_handle != 0) {
	return injector_handle;
    }

    // Initialize the handle.
    injector_handle = malloc(sizeof(struct piga_injector_handle_t));
    injector_handle->libGL_path = 0;
    injector_handle->libGLX_path = 0;
    injector_handle->status = 0;
    injector_handle->L = 0;
    
    injector_handle->L = luaL_newstate();
    if(!injector_handle->L) {
	printf("Error while trying to open lua state!\n");
	injector_handle->status |= PIGA_INJECTOR_INVALID_LUA_STATE;
	return injector_handle;
    }

    luaL_openlibs(injector_handle->L);

    const char* path = piga_injector_get_script_path();
    if(path == 0) {
	printf("No script path found, injector not working.\n");
	injector_handle->status |= PIGA_INJECTOR_INVALID_SCRIPT_PATH;
	return injector_handle;
    }
    printf("Injector using path %s\n", path);

    char *config_file = piga_injector_combine_path(path, "/lua/config.lua");
    luaL_loadfile(injector_handle->L, config_file);
    int ret = lua_pcall(injector_handle->L, 0, 0, 0);
    if (ret != 0) {
	fprintf(stderr, "%s\n", lua_tostring(injector_handle->L, -1));
	injector_handle->status |= PIGA_INJECTOR_ERROR_IN_CONFIG_LUA;
	return injector_handle;
    }
    free(config_file);

    return injector_handle;
}

char* piga_injector_combine_path(const char *p1, const char *p2)
{
    const size_t len_p1 = strlen(p1);
    const size_t len_p2 = strlen(p2);
    char *str = malloc(len_p1 + len_p2 + 1);
    memcpy(str, p1, len_p1);
    memcpy(str + len_p1, p2, len_p2 + 1);
    return str;
}

void piga_injector_exit()
{
    if(injector_handle != 0) {
	if(injector_handle->L != 0) {
	    lua_close(injector_handle->L);
	    injector_handle->L = 0;
	}
    }
}
