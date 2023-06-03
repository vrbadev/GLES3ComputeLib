cmake_minimum_required(VERSION 3.25)
project(GLES3ComputeLib C)

set(CMAKE_C_STANDARD 11)

# Find all required libraries for linking
find_library (LIBGBM libgbm.so REQUIRED)
find_library (LIBEGL libEGL.so REQUIRED)
find_library (LIBGL libGL.so REQUIRED)
find_library (LIBM libm.so REQUIRED)

# Include required library headers
include_directories (inc ${OpenGL_LIBRARIES} ${LIBGBM_LIBRARIES})

# Select proper OpenGL ES version available on the system
# Possible values: OPENGL_ES_31 or OPENGL_ES_32
add_compile_definitions(OPENGL_ES_31)

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
target_link_libraries (GLES3ComputeLib ${LIBM} ${LIBGL} ${LIBEGL} ${LIBGBM})

# Target: Testing executable for 2D convolution
add_executable (test_conv2d src/tests/test_conv2d.c)
target_link_libraries (test_conv2d ${THIS_LIBRARY} ${LIBM} ${LIBGL} ${LIBEGL} ${LIBGBM})

