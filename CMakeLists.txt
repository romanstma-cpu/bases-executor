cmake_minimum_required(VERSION 3.20)
project(BasesExecutor LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/GS- /guard:cf- /O2 /Ob2 /GL)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:NONE")
endif()

set(LUA_SRC "${CMAKE_SOURCE_DIR}/lua-5.1.5/src")

# Create stub Lua library (pre-compiled would be better but this compiles)
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
target_compile_definitions(lua51_lib PUBLIC LUA_USE_WINDOWS _CRT_SECURE_NO_WARNINGS guardcfw_h)

file(GLOB EXEC_SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(bases_executor ${EXEC_SOURCES})
target_include_directories(bases_executor PRIVATE include ${LUA_SRC})
target_link_libraries(bases_executor PRIVATE lua51_lib)
target_compile_definitions(bases_executor PRIVATE UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS guardcfw_h)
if(MSVC)
    target_link_options(bases_executor PRIVATE /ENTRY:mainCRTStartup)
endif()