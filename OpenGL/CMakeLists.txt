include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable (glfwExample
  glfwImplement.cpp 
  GLSL.cpp GLSL.h ../src/SceneParser_JSON.cpp
)

target_link_libraries (glfwExample PRIVATE GLEW::GLEW)
target_link_libraries (glfwExample PRIVATE glfw)
target_link_libraries(glfwExample PRIVATE tinyobjloader)

if(APPLE)
    target_link_libraries(glfwExample PRIVATE
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
else()
    target_link_libraries(glfwExample PRIVATE OpenGL::GL)
endif()

target_link_libraries (glfwExample PRIVATE PNG::PNG)
target_link_libraries (glfwExample PRIVATE ZLIB::ZLIB)

