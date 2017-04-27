#include "injectors.h"
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "lua_utils.h"

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
    injector_handle->vertex_shader = 0;
    injector_handle->fragment_shader = 0;
    injector_handle->window_width = 0;
    injector_handle->window_height = 0;
    injector_handle->status = 0;
    injector_handle->L = 0;
    injector_handle->cairo_cr = 0;
    injector_handle->cairo_surface = 0;
    injector_handle->draw_request = false;
 
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

    // Setup lua global variables.
    lua_pushstring(injector_handle->L, path);
    lua_setglobal(injector_handle->L, "PATH");

    // Read lua config file.
    char *config_file = piga_injector_combine_path(path, "/lua/config.lua");
    luaL_loadfile(injector_handle->L, config_file);
    int ret = lua_pcall(injector_handle->L, 0, 0, 0);
    if (ret != 0) {
	printf("%s\n", lua_tostring(injector_handle->L, -1));
	injector_handle->status |= PIGA_INJECTOR_ERROR_IN_CONFIG_LUA;
	return injector_handle;
    }


    // Configuration variables.
    injector_handle->libGL_path = get_global_str(injector_handle->L, "libGL_path");
    injector_handle->libGLX_path = get_global_str(injector_handle->L, "libGLX_path");

    injector_handle->vertex_shader = get_global_str(injector_handle->L, "vertexShaderPath");
    injector_handle->fragment_shader = get_global_str(injector_handle->L, "fragmentShaderPath");

    // Check the variables.
    if(injector_handle->libGL_path == 0) {
        printf("No libGL_path set in %s!\n", config_file);
    }
    if(injector_handle->libGLX_path == 0) {
        printf("No libGLX_path set in %s!\n", config_file);
    }
    if(injector_handle->vertex_shader == 0) {
        printf("No vertexShaderPath set in %s!\n", config_file);
    }
    if(injector_handle->fragment_shader == 0) {
        printf("No fragmentShaderPath set in %s!\n", config_file);
    }

    free(config_file);

    return injector_handle;
}

void piga_injector_draw()
{
    if(injector_handle->cairo_surface == 0) {
        // Allocate the cairo data array.
        injector_handle->cairo_stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, injector_handle->window_width);

        injector_handle->cairo_data = calloc(injector_handle->cairo_stride
                                            * injector_handle->window_width
                                            * injector_handle->window_height,
                                            sizeof(unsigned char));

        injector_handle->cairo_surface = cairo_image_surface_create_for_data(
            injector_handle->cairo_data,
            CAIRO_FORMAT_ARGB32,
            injector_handle->window_width,
            injector_handle->window_height,
            injector_handle->cairo_stride);
        printf("Created cairo_image_surface.\n");
        if(injector_handle->cairo_surface < 0) {
            printf("Cairo surface creation failed!\n");
        }
    }

    if(injector_handle->cairo_cr == 0) {
        injector_handle->cairo_cr = cairo_create(injector_handle->cairo_surface);
        printf("Created cairo_context.\n");

        // Check the cairo object
        if(cairo_status(injector_handle->cairo_cr) != CAIRO_STATUS_SUCCESS) {
            printf("Error in cairo context creation! %s\n",
                   cairo_status_to_string(cairo_status(injector_handle->cairo_cr)));
        }
    }

    // Begin drawing.
    cairo_set_source_rgb(injector_handle->cairo_cr, 0.0, 0.0, 0.0);
    cairo_paint(injector_handle->cairo_cr);

    cairo_surface_flush(injector_handle->cairo_surface);

    injector_handle->draw_request = true;
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
        if(injector_handle->cairo_cr != 0) {
            cairo_surface_destroy(injector_handle->cairo_surface);
            cairo_destroy(injector_handle->cairo_cr);

            injector_handle->cairo_surface = 0;
            injector_handle->cairo_cr = 0;
        }
        if(injector_handle->cairo_data != 0) {
            free(injector_handle->cairo_data);

            injector_handle->cairo_data = 0;
        }
    }
}

char* piga_read_file(const char *path, size_t *length)
{
    // Thanks to http://stackoverflow.com/a/174552 for this handy stub.
    char * buffer = 0;
    FILE * f = fopen (path, "rb");

    if (f)
    {
        fseek (f, 0, SEEK_END);
        *length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = malloc (*length + 1);
        if (buffer)
        {
            fread (buffer, 1, *length, f);
        }
        fclose (f);

        buffer[*length] = '\0';
    }

    return buffer;
}
