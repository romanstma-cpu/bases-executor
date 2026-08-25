cmake_minimum_required(VERSION 3.20)
project(BasesExecutor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/GS- /guard:cf- /O2 /Ob2)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:NONE")
endif()

# Stub Lua implementation for CI
set(LUA_SRC "${CMAKE_SOURCE_DIR}/lua-5.1.5/src")

add_library(lua_stub STATIC
    ${LUA_SRC}/lapi.c ${LUA_SRC}/lauxlib.c ${LUA_SRC}/lbaselib.c
    ${LUA_SRC}/lcode.c ${LUA_SRC}/ldblib.c ${LUA_SRC}/ldebug.c ${LUA_SRC}/ldo.c
    ${LUA_SRC}/ldump.c ${LUA_SRC}/lfunc.c ${LUA_SRC}/lgc.c ${LUA_SRC}/linit.c
    ${LUA_SRC}/liolib.c ${LUA_SRC}/llex.c ${LUA_SRC}/lmathlib.c ${LUA_SRC}/lmem.c
    ${LUA_SRC}/loadlib.c ${LUA_SRC}/lobject.c ${LUA_SRC}/lopcodes.c ${LUA_SRC}/loslib.c
    ${LUA_SRC}/lparser.c ${LUA_SRC}/lstate.c ${LUA_SRC}/lstring.c ${LUA_SRC}/lstrlib.c
    ${LUA_SRC}/ltable.c ${LUA_SRC}/ltablib.c ${LUA_SRC}/ltm.c ${LUA_SRC}/lundump.c
    ${LUA_SRC}/lvm.c ${LUA_SRC}/lzio.c
)

# Ensure C compilation for Lua
set_source_files_properties(
    ${LUA_SRC}/lapi.c ${LUA_SRC}/lauxlib.c ${LUA_SRC}/lbaselib.c
    ${LUA_SRC}/lcode.c ${LUA_SRC}/ldblib.c ${LUA_SRC}/ldebug.c ${LUA_SRC}/ldo.c
    ${LUA_SRC}/ldump.c ${LUA_SRC}/lfunc.c ${LUA_SRC}/lgc.c ${LUA_SRC}/linit.c
    ${LUA_SRC}/liolib.c ${LUA_SRC}/llex.c ${LUA_SRC}/lmathlib.c ${LUA_SRC}/lmem.c
    ${LUA_SRC}/loadlib.c ${LUA_SRC}/lobject.c ${LUA_SRC}/lopcodes.c ${LUA_SRC}/loslib.c
    ${LUA_SRC}/lparser.c ${LUA_SRC}/lstate.c ${LUA_SRC}/lstring.c ${LUA_SRC}/lstrlib.c
    ${LUA_SRC}/ltable.c ${LUA_SRC}/ltablib.c ${LUA_SRC}/ltm.c ${LUA_SRC}/lundump.c
    ${LUA_SRC}/lvm.c ${LUA_SRC}/lzio.c
    PROPERTIES LANGUAGE C)

target_include_directories(lua_stub PUBLIC ${LUA_SRC})
target_compile_definitions(lua_stub PUBLIC LUA_USE_WINDOWS _CRT_SECURE_NO_WARNINGS guardcfw_h)

# Stub main lib
add_library(main_lib STATIC src/main.cpp src/manualmap.cpp src/peb.cpp src/process.cpp src/rbx.cpp src/sunc.cpp src/syscall.cpp)
target_include_directories(main_lib PUBLIC include ${LUA_SRC})
target_link_libraries(main_lib PUBLIC lua_stub)

# Final executable
add_executable(bases_executor EXCLUDE_FROM_ALL)
target_link_libraries(bases_executor PRIVATE main_lib lua_stub)

add_executable(bases_executor_build ${EXEC_SOURCES})
set(EXEC_SOURCES src/main.cpp src/manualmap.cpp src/peb.cpp src/process.cpp src/rbx.cpp src/sunc.cpp src/syscall.cpp)
add_executable(bases_executor ${EXEC_SOURCES})
target_include_directories(bases_executor PRIVATE include ${LUA_SRC})
target_link_libraries(bases_executor PRIVATE lua_stub)
target_compile_definitions(bases_executor PRIVATE UNICODE _UNICODE guardcfw_h)

if(MSVC)
    target_link_options(bases_executor PRIVATE /ENTRY:mainCRTStartup /SUBSYSTEM:CONSOLE)
endif()