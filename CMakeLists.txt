cmake_minimum_required(VERSION 3.25)
project(GLES3ComputeLib C)

set(CMAKE_C_STANDARD 11)

# Find all required libraries for linking
include(cmake/FindOpenGLES.cmake)
message("[GLES library v" ${GLES_VERSION_STRING} "]")
message("-> Shared object location: " ${GLES_LIBRARIES})
message("-> Include directory location: " ${GLES_INCLUDE_DIRS})

include(cmake/FindEGL.cmake)
message("[EGL library v" ${EGL_VERSION_STRING} "]")
message("-> Shared object location: " ${EGL_LIBRARIES})
message("-> Include directory location: " ${EGL_INCLUDE_DIRS})

include(cmake/FindGBM.cmake)
message("[GBM library]")
message("-> Shared object location: " ${GBM_LIBRARIES})
message("-> Include directory location: " ${GBM_INCLUDE_DIRS})

# Include required library headers
include_directories (inc ${EGL_INCLUDE_DIRS} ${GBM_INCLUDE_DIRS} ${GLES_INCLUDE_DIRS})

# Find all GLSL shader sources and add object targets to be appended to the final library binary
file (GLOB_RECURSE SHADER_FILES_PATHS CONFIGURE_DEPENDS "*.comp" "*.vert" "*.frag" "*.tess" "*.geom" "*.glsl")
set (SHADER_OBJECTS)
foreach(SHADER_FILE_PATH ${SHADER_FILES_PATHS})
    file(RELATIVE_PATH SHADER_FILE ${CMAKE_SOURCE_DIR} ${SHADER_FILE_PATH})
    get_filename_component(SHADER_OBJ ${SHADER_FILE_PATH} NAME_WE)
    set(SHADER_OBJ "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_OBJ}.o")
    add_custom_command(
            OUTPUT ${SHADER_OBJ}
            COMMAND ld -r -b binary -o ${SHADER_OBJ} ${SHADER_FILE}
            DEPENDS ${SHADER_FILE}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            VERBATIM
    )
    list(APPEND SHADER_OBJECTS ${SHADER_OBJ})
endforeach()

# Target: Library archive file
set (THIS_LIBRARY $<TARGET_FILE:GLES3ComputeLib>)
add_library (GLES3ComputeLib src/gl3_utils.c src/queue.c src/compute_lib.c src/utils/lodepng.c ${SHADER_OBJECTS})
add_custom_command(TARGET GLES3ComputeLib POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${THIS_LIBRARY} ${CMAKE_SOURCE_DIR}/out)

# Target: Testing executable for 2D convolution
add_executable (test_conv2d src/tests/test_conv2d.c)
target_link_libraries (test_conv2d ${THIS_LIBRARY} m ${EGL_LIBRARIES} ${GBM_LIBRARIES} ${GLES_LIBRARIES})

