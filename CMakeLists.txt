cmake_minimum_required(VERSION 3.20)
project(BasesExecutor LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MinGW-specific settings for GitHub Actions
if(MINGW)
    set(CMAKE_SYSTEM_NAME Windows)
    set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
endif()

# Hardening (MinGW equivalent)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG")

# Lua source
set(LUA_SRC "${CMAKE_SOURCE_DIR}/lua-5.1.5/src")

add_library(lua51_lib STATIC
    ${LUA_SRC}/lapi.c ${LUA_SRC}/lauxlib.c ${LUA_SRC}/lbaselib.c
    ${LUA_SRC}/lcode.c ${LUA_SRC}/ldblib.c ${LUA_SRC}/ldebug.c ${LUA_SRC}/ldo.c
    ${LUA_SRC}/ldump.c ${LUA_SRC}/lfunc.c ${LUA_SRC}/lgc.c ${LUA_SRC}/linit.c
    ${LUA_SRC}/liolib.c ${LUA_SRC}/llex.c ${LUA_SRC}/lmathlib.c ${LUA_SRC}/lmem.c
    ${LUA_SRC}/loadlib.c ${LUA_SRC}/lobject.c ${LUA_SRC}/lopcodes.c ${LUA_SRC}/loslib.c
    ${LUA_SRC}/lparser.c ${LUA_SRC}/lstate.c ${LUA_SRC}/lstring.c ${LUA_SRC}/lstrlib.c
    ${LUA_SRC}/ltable.c ${LUA_SRC}/ltablib.c ${LUA_SRC}/ltm.c ${LUA_SRC}/lundump.c
    ${LUA_SRC}/lvm.c ${LUA_SRC}/lzio.c
)
target_include_directories(lua51_lib PUBLIC ${LUA_SRC})
target_compile_options(lua51_lib PRIVATE -fansi-assignment)
set_property(TARGET lua51_lib PROPERTY LINKER_LANGUAGE C)

file(GLOB EXEC_SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(bases_executor ${EXEC_SOURCES})
target_include_directories(bases_executor PRIVATE include ${LUA_SRC})
target_link_libraries(bases_executor PRIVATE lua51_lib -lmsvcrt -lmsvcimp)
target_compile_definitions(bases_executor PRIVATE UNICODE _UNICODE)

# Output as portable PE
set_target_properties(bases_executor PROPERTIES
    WIN32_EXECUTABLE TRUE
    OUTPUT_NAME "bases_executor"
)