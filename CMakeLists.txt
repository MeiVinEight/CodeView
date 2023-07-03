cmake_minimum_required(VERSION 3.25)
project(CodeView C)
set(CMAKE_C_STANDARD 11)
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
add_definitions(
	-DCMAKE_BUILD
)
include_directories(include)
aux_source_directory(src SRC)
add_executable(CodeView ${SRC})
