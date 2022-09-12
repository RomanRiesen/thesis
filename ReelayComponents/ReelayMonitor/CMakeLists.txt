#execute_process(COMMAND "./configure && make --enable-shared --enable-obj --with-system-qsort && make check" WORKING_DIRECTORY "../cudd")

#execute_process(COMMAND "make" WORKING_DIRECTORY "../cudd")
#execute_process(COMMAND "make check" WORKING_DIRECTORY "../cudd")
#if(NOT BUILD_SHARED_LIBS)
set(CMAKE_FIND_LIBRARY_SUFFIXES .a .lib)
#endif()

#set(RM_TARGET "REELAY_MONITOR")

add_library(CUDD_LIB STATIC IMPORTED GLOBAL)
find_library(CUDD_LIB NAMES libcudd cudd CUDD)

set(SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/ReelayMonitor.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/ReelayMonitor.fpp"
)
set(MOD_DEPS
    "${CUDD_LIB}")

register_fprime_module()

set(UT_NAME "MONITOR_UT_EXE")

if (BUILD_TESTING AND NOT __FPRIME_NO_UT_GEN__)

    set(UT_SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/ReelayMonitor.fpp"
        "${CMAKE_CURRENT_LIST_DIR}/test/ut/Tester.cpp" 
        "${CMAKE_CURRENT_LIST_DIR}/test/ut/TestMain.cpp"
    )

    register_fprime_ut("${UT_NAME}")

    target_link_libraries("${UT_NAME}" PUBLIC "${CUDD_LIB}")

endif()