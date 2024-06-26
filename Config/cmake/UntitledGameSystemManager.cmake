function(custom_setup_step)
endfunction()

function(custom_compile_step)
    if (NOT WIN32)
        target_link_libraries(${APP_LIB_TARGET} UGM_Incus_InternalFuncs)
    endif()
endfunction()

set(USE_OS_MODULE ON)
set(ENABLE_XDG_BASEDIR ON)

set(USE_THEME_MODULE ON)
set(USE_I18N_MODULE ON)

file(GLOB_RECURSE UIMGUI_CUSTOM_APP_HEADERS "IncusBindings/libUGM_Incus_InternalFuncs.h")
link_directories(IncusBindings/)
