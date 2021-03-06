#include "injectors.h"
#include <errno.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <unistd.h>

#include "fontconfig_wrapper.h"
#include "lua_utils.h"
#include "x11/x_events.h"

#include <injectors/config.h>

#include <cairo/cairo.h>

struct injector_handle_t *injector_handle = NULL;

#define CHECK_AND_RETURN(PATH)                                                 \
    if (access(PATH "/lua/config.lua", R_OK) != -1)                            \
    return PATH

const char *injector_get_script_path() {
    char *env_path = getenv("INJECTOR_PATH");
    if (env_path != 0) {
        injector_handle->use_reloading = true;
        return env_path;
    } else {
        printf("No \"INJECTOR_PATH\" environment variable found. Using "
               "default directories.\n");
    }

    CHECK_AND_RETURN("injector");
    CHECK_AND_RETURN("~/.config/injector");

    return 0;
}

const char *injector_get_program_config() {
    char *env_path = getenv("INJECTOR_PROGRAM_CONFIG");
    if (env_path != 0) {
        return env_path;
    }
    printf("No \"INJECTOR_PROGRAM_CONFIG\" environment variable found. "
           "Using default program config without specific configuration "
           "options.\n");
    return "";
}

struct injector_handle_t *injector_init() {
    if (injector_handle != 0) {
        return injector_handle;
    }

    // Initialize the handle.
    injector_handle = malloc(sizeof(struct injector_handle_t));
    injector_handle->libGL_path = 0;
    injector_handle->libGLX_path = 0;
    injector_handle->libXlib_path = 0;
    injector_handle->vertex_shader = 0;
    injector_handle->fragment_shader = 0;
    injector_handle->window_width = 0;
    injector_handle->window_height = 0;
    injector_handle->status = 0;
    injector_handle->L = 0;
    injector_handle->cairo_cr = 0;
    injector_handle->cairo_surface = 0;
    injector_handle->cairo_data = 0;
    injector_handle->draw_request = false;
    injector_handle->use_reloading = false;
    injector_handle->inotify_ev_buf_size =
        (sizeof(struct inotify_event) + NAME_MAX + 1) * 2;
    injector_handle->inotify_ev_buf =
        calloc(injector_handle->inotify_ev_buf_size, sizeof(char));

    global_injector_handle = injector_handle;

    injector_handle->L = luaL_newstate();
    if (!injector_handle->L) {
        printf("Error while trying to open lua state!\n");
        injector_handle->status |= INJECTOR_INVALID_LUA_STATE;
        return injector_handle;
    }

    luaL_openlibs(injector_handle->L);

    const char *path = injector_get_script_path();
    if (path == 0) {
        printf("No script path found, injector not working.\n");
        injector_handle->status |= INJECTOR_INVALID_SCRIPT_PATH;
        return injector_handle;
    }
    printf("Injector using path \"%s\"\n", path);
    injector_handle->path = path;

    injector_handle->watched_path = injector_combine_path(path, "/lua/");

    // Setup lua global variables.
    lua_pushstring(injector_handle->L, path);
    lua_setglobal(injector_handle->L, "PATH");
    lua_pushstring(injector_handle->L, SYSTEM_ABSOLUTE_LIBDIR_PATH);
    lua_setglobal(injector_handle->L, "SYSTEM_LIBDIR_PATH");

    // Look for program specific options.
    const char *program_specific_options = injector_get_program_config();
    lua_pushstring(injector_handle->L, program_specific_options);
    lua_setglobal(injector_handle->L, "OPTIONS_SCRIPT");

    // Read lua config file.
    char *config_file = injector_combine_path(path, "/lua/config.lua");
    luaL_loadfile(injector_handle->L, config_file);
    int ret = lua_pcall(injector_handle->L, 0, 0, 0);
    if (ret != 0) {
        printf("%s\n", lua_tostring(injector_handle->L, -1));
        injector_handle->status |= INJECTOR_ERROR_IN_CONFIG_LUA;
    }

    // Configuration variables.
    injector_handle->libGL_path =
        get_global_str(injector_handle->L, "libGL_path");
    injector_handle->libGLX_path =
        get_global_str(injector_handle->L, "libGLX_path");
    injector_handle->libXlib_path =
        get_global_str(injector_handle->L, "libXlib_path");

    injector_handle->vertex_shader =
        get_global_str(injector_handle->L, "vertexShaderPath");
    injector_handle->fragment_shader =
        get_global_str(injector_handle->L, "fragmentShaderPath");

    const char *fontconfig_path =
        get_global_str(injector_handle->L, "libFontConfig_path");
    injector_load_fontconfig(fontconfig_path);

    // Check the variables.
    if (injector_handle->libGL_path == 0) {
        printf("No libGL_path set in %s!\n", config_file);
    }
    if (injector_handle->libGLX_path == 0) {
        printf("No libGLX_path set in %s!\n", config_file);
    }
    if (injector_handle->libXlib_path == 0) {
        printf("No libXlib_path set in %s!\n", config_file);
    }
    if (injector_handle->vertex_shader == 0) {
        printf("No vertexShaderPath set in %s!\n", config_file);
    }
    if (injector_handle->fragment_shader == 0) {
        printf("No fragmentShaderPath set in %s!\n", config_file);
    }

    free(config_file);

    // Load X-Events.
    printf("Loading X11 from %s\n", injector_handle->libXlib_path);
    injector_load_x_events(injector_handle->libXlib_path);

    // Setup hot-reloading.
    if (injector_handle->use_reloading) {
        injector_handle->inotify_fd = inotify_init1(IN_NONBLOCK);
        injector_handle->inotify_watch_fd = inotify_add_watch(
            injector_handle->inotify_fd, injector_handle->watched_path,
            IN_DELETE | IN_MODIFY | IN_MOVE | IN_CREATE);
        if (injector_handle->inotify_watch_fd == -1) {
            switch (errno) {
            case EACCES:
                printf("No read access to \"%s\"\n",
                       injector_handle->watched_path);
                break;
            case EBADF:
                printf("Given fd for \"%s\" is not valid.\n",
                       injector_handle->watched_path);
                break;
            case EINVAL:
                printf("Given event mask not valid.\n");
                break;
            case ENAMETOOLONG:
                printf("Pathname \"%s\" is too long.\n",
                       injector_handle->watched_path);
                break;
            case ENOENT:
                printf("Directory component in \"%s\" does not exist or is a "
                       "dangling symbolic link.\n",
                       injector_handle->watched_path);
                break;
            case ENOMEM:
                printf("Insufficient kernel memory available.\n");
                break;
            case ENOSPC:
                printf("User limit of total inotify handles was reached.\n");
                break;
            }

            // There was an error, hot reloading can be disabled.
            injector_handle->use_reloading = false;
        }
    }

    return injector_handle;
}

void injector_check_inotify() {
    ssize_t size =
        read(injector_handle->inotify_fd, injector_handle->inotify_ev_buf,
             injector_handle->inotify_ev_buf_size);

    if (size == -1) {
        switch (errno) {
        case EAGAIN:
            // Nothing to do here, this error is expected.
            break;
        case EBADF:
            printf("File descriptor is not valid!\n");
            break;
        case EFAULT:
            printf("Buffer outside of valid variable space!\n");
            break;
        case EINTR:
            printf("Reading inotify event was interrupted by a signal!\n");
            break;
        case EINVAL:
            printf("FD is unsuitable for reading!\n");
            break;
        case EIO:
            printf("IO-Error on inotify fd!\n");
            break;
        case EISDIR:
            printf("Inotify fd refers to a directory!\n");
            break;
        }
    }

    if (size > 0 && size >= (long)sizeof(struct inotify_event)) {
        // Now, a real value should have been read!
        injector_handle->inotify_ev =
            (struct inotify_event *)injector_handle->inotify_ev_buf;

        if (injector_handle->inotify_ev->mask & IN_MODIFY ||
            injector_handle->inotify_ev->mask & IN_CREATE ||
            injector_handle->inotify_ev->mask & IN_DELETE) {
            injector_refresh_lua();
        }
    }
}

void injector_refresh_lua() {
    // Load the overlay lua file.
    char *main_file =
        injector_combine_path(injector_handle->path, "/lua/overlay.lua");
    luaL_loadfile(injector_handle->L, main_file);
    int ret = lua_pcall(injector_handle->L, 0, 0, 0);
    if (ret != 0) {
        printf("Error loading overlay.lua: %s\n",
               lua_tostring(injector_handle->L, -1));
        injector_handle->status |= INJECTOR_ERROR_IN_OVERLAY_LUA;
    } else {
        printf("Successfully loaded overlay.lua.\n");
    }
    free(main_file);

    // Set other globals.
    injector_lua_set_global_int(injector_handle->L,
                                injector_handle->window_width, "WIDTH");
    injector_lua_set_global_int(injector_handle->L,
                                injector_handle->window_height, "HEIGHT");

    // Set the global Lua cairo context variable.
    lua_pushlightuserdata(injector_handle->L, injector_handle->cairo_cr);
    lua_setglobal(injector_handle->L, "ctx");

    // Run the GC
    lua_gc(injector_handle->L, LUA_GCCOLLECT, 0);
}

void injector_draw() {
    if (injector_handle->cairo_surface == 0) {
        // Allocate the cairo data array.
        injector_handle->cairo_stride = cairo_format_stride_for_width(
            CAIRO_FORMAT_ARGB32, injector_handle->window_width);

        injector_handle->cairo_data = calloc(injector_handle->cairo_stride *
                                                 injector_handle->window_width *
                                                 injector_handle->window_height,
                                             sizeof(unsigned char));

        injector_handle->cairo_surface = cairo_image_surface_create_for_data(
            injector_handle->cairo_data, CAIRO_FORMAT_ARGB32,
            injector_handle->window_width, injector_handle->window_height,
            injector_handle->cairo_stride);
        printf("Created cairo_image_surface.\n");
        if (injector_handle->cairo_surface < 0) {
            printf("Cairo surface creation failed!\n");
        }
    }

    if (injector_handle->cairo_cr == 0) {
        injector_handle->cairo_cr =
            cairo_create(injector_handle->cairo_surface);
        printf("Created cairo_context.\n");

        // Check the cairo object
        if (cairo_status(injector_handle->cairo_cr) != CAIRO_STATUS_SUCCESS) {
            printf("Error in cairo context creation! %s\n",
                   cairo_status_to_string(
                       cairo_status(injector_handle->cairo_cr)));
        }

        // Set the global Lua cairo context variable.
        lua_pushlightuserdata(injector_handle->L, injector_handle->cairo_cr);
        lua_setglobal(injector_handle->L, "ctx");
    }

    // Begin drawing.

    // Call Lua to draw the overlay according to the current state of the
    // console.
    if (injector_lua_call_bool_func(injector_handle->L, "needsRedraw")) {
        // Make the background transparent
        cairo_save(injector_handle->cairo_cr);
        cairo_set_operator(injector_handle->cairo_cr, CAIRO_OPERATOR_SOURCE);
        cairo_set_source_rgba(injector_handle->cairo_cr, 0.0, 0.0, 0.0, 0);
        cairo_paint(injector_handle->cairo_cr);
        cairo_restore(injector_handle->cairo_cr);

        injector_lua_call_void_func(injector_handle->L, "draw");

        cairo_surface_flush(injector_handle->cairo_surface);

        injector_handle->draw_request = true;
    }
}

char *injector_combine_path(const char *p1, const char *p2) {
    const size_t len_p1 = strlen(p1);
    const size_t len_p2 = strlen(p2);
    char *       str = malloc(len_p1 + len_p2 + 1);
    memcpy(str, p1, len_p1);
    memcpy(str + len_p1, p2, len_p2 + 1);
    return str;
}

void injector_exit() {
    if (injector_handle != 0) {
        if (injector_handle->L != 0) {
            lua_close(injector_handle->L);

            injector_handle->L = 0;
        }
        if (injector_handle->cairo_cr != 0) {
            cairo_surface_destroy(injector_handle->cairo_surface);
            cairo_destroy(injector_handle->cairo_cr);

            injector_handle->cairo_surface = 0;
            injector_handle->cairo_cr = 0;
        }
        if (injector_handle->cairo_data != 0) {
            free(injector_handle->cairo_data);

            injector_handle->cairo_data = 0;
        }
    }
}

char *injector_read_file(const char *path, size_t *length) {
    // Thanks to http://stackoverflow.com/a/174552 for this handy stub.
    char *buffer = 0;
    FILE *f = fopen(path, "rb");

    if (f) {
        fseek(f, 0, SEEK_END);
        *length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(*length + 1);
        if (buffer) {
            fread(buffer, 1, *length, f);
        }
        fclose(f);

        buffer[*length] = '\0';
    }

    return buffer;
}
