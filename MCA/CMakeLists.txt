project(MCA)

include_directories(../include)

add_definitions(
	-DMCASHARED
)

set(CMAKE_C_FLAGS "/GS-")
if (CMAKE_BUILD_TYPE STREQUAL Debug)
	set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG")
	set(CMAKE_C_FLAGS_DEBUG  "/Od /Zi")
else ()
	set(CMAKE_C_FLAGS_DEBUG  "/O1")
endif ()
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /INCREMENTAL:NO /NOD /NOENTRY")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/CodeView)

aux_source_directory(src SRC)
add_library(MCA SHARED ${SRC})

target_link_options(MCA PUBLIC -INCREMENTAL:NO)