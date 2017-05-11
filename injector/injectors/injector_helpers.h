#pragma once

#include <piga-injector_export.h>

#define PIGA_OVERRIDE_BACKUP_SYMBOL(NAME, RETURN_TYPE, PARAMS)                 \
    typedef RETURN_TYPE(*__##NAME##_t) PARAMS;                                 \
    PIGA_INJECTOR_NO_EXPORT __##NAME##_t NAME##_ptr;

#define PIGA_OVERRIDE_SYMBOL_DEFINITION(NAME, RETURN_TYPE, PARAMS)             \
    PIGA_INJECTOR_EXPORT RETURN_TYPE NAME PARAMS

#define PIGA_OVERRIDE_SYMBOL(NAME, RETURN_TYPE, PARAMS)                        \
    PIGA_OVERRIDE_BACKUP_SYMBOL(NAME, RETURN_TYPE, PARAMS)                     \
    PIGA_INJECTOR_EXPORT PIGA_OVERRIDE_SYMBOL_DEFINITION(NAME, RETURN_TYPE,    \
                                                         PARAMS)

#define PIGA_CHECK_LOADED_FUNC_PTR(PTR)                                        \
    if (PTR##_ptr == NULL)                                                     \
        printf("The function " #PTR                                            \
               " could not be read from its _ptr variant!\n");

#define PIGA_LOAD_SYMBOL_USING_HANDLE(HANDLE, SYMBOL)                          \
    SYMBOL##_ptr = (__##SYMBOL##_t)dlsym(HANDLE, #SYMBOL);                    \
    PIGA_CHECK_LOADED_FUNC_PTR(SYMBOL);
