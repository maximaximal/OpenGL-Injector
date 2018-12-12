#pragma once

#include <injector_export.h>

#define INJECTOR_OVERRIDE_BACKUP_SYMBOL(NAME, RETURN_TYPE, PARAMS)             \
    typedef RETURN_TYPE(*__##NAME##_t) PARAMS;                                 \
    INJECTOR_NO_EXPORT __##NAME##_t NAME##_ptr;

#define INJECTOR_OVERRIDE_SYMBOL_DEFINITION(NAME, RETURN_TYPE, PARAMS)         \
    INJECTOR_EXPORT RETURN_TYPE NAME PARAMS

#define INJECTOR_OVERRIDE_SYMBOL(NAME, RETURN_TYPE, PARAMS)                    \
    INJECTOR_OVERRIDE_BACKUP_SYMBOL(NAME, RETURN_TYPE, PARAMS)                 \
    INJECTOR_EXPORT INJECTOR_OVERRIDE_SYMBOL_DEFINITION(NAME, RETURN_TYPE,     \
                                                        PARAMS)

#define INJECTOR_CHECK_LOADED_FUNC_PTR(PTR)                                    \
    if (PTR##_ptr == NULL)                                                     \
        printf("The function " #PTR                                            \
               " could not be read from its _ptr variant!\n");

#define INJECTOR_LOAD_SYMBOL_USING_HANDLE(HANDLE, SYMBOL)                      \
    SYMBOL##_ptr = (__##SYMBOL##_t)dlsym(HANDLE, #SYMBOL);                     \
    INJECTOR_CHECK_LOADED_FUNC_PTR(SYMBOL);
