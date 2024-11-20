cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("skidding-tool")

set(STATIC_LINKS "${CMAKE_CURRENT_SOURCE_DIR}/libs")
set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INJECTOR_NAME "${PROJECT_NAME}-loader")
set(RUNTIME_NAME "${PROJECT_NAME}-runtime")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB dependencies LIST_DIRECTORIES true "dependencies/*")
foreach(item ${dependencies})
	if(IS_DIRECTORY ${item})
		add_subdirectory(${item})
	endif()
endforeach()

include_directories("header-dependencies")
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
	logger
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
	logger imgui fileutils
)