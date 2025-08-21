cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("nazimod")

set(STATIC_LINKS "${CMAKE_CURRENT_SOURCE_DIR}/libs")
set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INJECTOR_NAME "${PROJECT_NAME}-loader")
set(RUNTIME_NAME "${PROJECT_NAME}-runtime")
set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_DEST_DIR "${CMAKE_BINARY_DIR}/assets")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_CONSOLE FALSE)
option(DISABLE_CHEAT_FEATURE FALSE)
option(EXPERIMENTAL_CHEAT_FEATURE FALSE)

add_compile_definitions(
	RUNTIME_DLL_FILENAME="${RUNTIME_NAME}.dll" 
	PROJECT_NAME="${PROJECT_NAME}"
	LOGGER_DEFINE_MACRO
	LOGGER_ERROR_DEBUG_BREAK
	LOGGER_TRACE_ERROR
	LOGGER_TRACE_WARNING
	_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR 
	$<$<CONFIG:Debug>:OE_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:OE_DEBUG>
    $<$<CONFIG:Release>:OE_RELEASE>
    $<$<CONFIG:MinSizeRel>:OE_RELEASE>
)

if(DISABLE_CHEAT_FEATURE)
	message("Cheat feature disabled")
	add_compile_definitions(NO_FEATURE)
endif()

if(EXPERIMENTAL_CHEAT_FEATURE)
	message("Experimental cheat enabled")
	add_compile_definitions(EXPERIMENTAL)
endif()

if(ENABLE_CONSOLE OR ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	message("Console enabled")
	add_compile_definitions(CONSOLE_ENABLED)
endif()

add_custom_target(
	assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ASSETS_SOURCE_DIR}
    ${ASSETS_DEST_DIR}
    COMMENT "Copying assets into build directory"
)

file(GLOB dependencies LIST_DIRECTORIES true "dependencies/*")
foreach(item ${dependencies})
	if(IS_DIRECTORY ${item})
		add_subdirectory(${item})
	endif()
endforeach()

include_directories("header-dependencies")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/static-libs/")
add_compile_definitions(RUNTIME_DLL_FILENAME="${RUNTIME_NAME}.dll" PROJECT_NAME="${PROJECT_NAME}")

file(
	GLOB_RECURSE
	INJECTOR_GLOB
	CONFIGURE_DEPENDS
	"${SRC}/loader/*.cpp"
	"${SRC}/loader/*.c"
)

add_executable(${INJECTOR_NAME} ${INJECTOR_GLOB})
target_link_libraries(${INJECTOR_NAME} PRIVATE 
	logger TulipHook
)
add_dependencies(${INJECTOR_NAME}
	assets
)

file(
	GLOB_RECURSE
	RUNTIME_GLOB
	CONFIGURE_DEPENDS
	"${SRC}/runtime/*.cpp"
	"${SRC}/runtime/*.c"
)

add_library(${RUNTIME_NAME} SHARED ${RUNTIME_GLOB})
target_link_libraries(${RUNTIME_NAME} PRIVATE
	logger imgui Unity TulipHook detours-x64
	base64
)