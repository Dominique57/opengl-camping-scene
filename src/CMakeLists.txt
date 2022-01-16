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

    player/camera.cc player/camera.hh

    model/objLoader.cc model/objLoader.hh
    model/mesh.cc model/mesh.hh
    model/model.cc model/model.hh
    model/models.hh model/models.cc

    temp/init_gl.cc temp/init_gl.hh
    temp/program.cc temp/program.hh
    temp/vao.cc temp/vao.hh
    temp/stb_image.h temp/stb_image_implementation.cc

    manager/lightManager.cc manager/lightManager.hh
    manager/particleEmitter.cc manager/particleEmitter.hh
    manager/fireworkEmitter.cc manager/fireWorkEmitter.hh
    manager/firePlace.cc manager/firePlace.hh
    manager/gpuParticleEmitter.cc manager/gpuParticleEmitter.hh

    texture/skybox.hh texture/skybox.cc

    wrappers/glWrapper.cc wrappers/glWrapper.hh
    wrappers/glmWrapper.hh
    wrappers/wrapper.cc wrappers/wrapper.hh
    texture/gBuffer.cc texture/gBuffer.hh
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
        assimp
    )
target_include_directories(SOURCE_LIB PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    )