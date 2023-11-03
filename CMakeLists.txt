cmake_minimum_required(VERSION 3.25)
project(CodeView C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(MCA)

include_directories(
	include
	MCA/src
)

message("CMAKE: " ${CMAKE_BUILD_TYPE})
set(CMAKE_C_FLAGS "/GS-")
if (CMAKE_BUILD_TYPE STREQUAL Debug)
	set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG /SUBSYSTEM:CONSOLE")
	set(CMAKE_C_FLAGS_DEBUG  "/Od /Zi")
else ()
	set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
	set(CMAKE_C_FLAGS_DEBUG  "/O1")
endif ()
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NOD /LIBPATH:${CMAKE_HOME_DIRECTORY}/.compile")
message("COMPILER: " ${CMAKE_C_FLAGS})
message("LINKER: " ${CMAKE_EXE_LINKER_FLAGS})
message("" ${CMAKE_C_FLAGS_DEBUG})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/CodeView)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


aux_source_directory(src SRC)
add_executable(CodeView ${SRC} res/CodeView.rc)
target_link_libraries(CodeView MCA Memory)

target_link_options(CodeView PUBLIC -INCREMENTAL:NO)