# Find libraries
find_package(OpenCV REQUIRED)
if (NOT "${OpenCV_opencv_core_FOUND}"
        OR NOT "${OpenCV_opencv_imgcodecs_FOUND}"
        OR NOT "${OpenCV_opencv_highgui_FOUND}")
    message(SEND_ERROR "Missing opencv `core`|`imgcodecs`|`highgui` library !")
endif ()
set(MY_OPENCV_LIBS "opencv_core;opencv_imgcodecs;opencv_highgui")
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# ============================ CXX SOURCE =====================================
add_library(SOURCE_LIB STATIC
    entryPoint.cc entryPoint.hh

    temp/camera.cc temp/camera.hh
    temp/init_gl.cc temp/init_gl.hh
    temp/objLoader.cc temp/objLoader.hh
    temp/program.cc temp/program.hh

    wrappers/glWrapper.cc wrappers/glWrapper.hh
    wrappers/glmWrapper.hh
    wrappers/wrapper.cc wrappers/wrapper.hh
    temp/vao.cc temp/vao.hh
    temp/lightManager.cc temp/lightManager.hh
        temp/stb_image.h temp/stb_image_implementation.cc
        texture/skybox.hh texture/skybox.cc
        )
target_compile_features(SOURCE_LIB PRIVATE
    cxx_std_17
    )
target_compile_options(SOURCE_LIB PRIVATE
    "-Wall"
    "-Wextra"
    $<$<CONFIG:Release>:-Werror -O3>
    $<$<CONFIG:Debug>:-O0 -ggdb>
    "-pedantic"
    )
target_link_libraries(SOURCE_LIB
    ${MY_OPENCV_LIBS}
    glfw
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    )
target_include_directories(SOURCE_LIB PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    )