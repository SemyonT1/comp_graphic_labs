cmake_minimum_required(VERSION 3.10)
project(Lab4)

set(CMAKE_CXX_STANDARD 17)


find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)


include_directories(${CMAKE_SOURCE_DIR}/lib/glad/include)
set(GLAD_SOURCE ${CMAKE_SOURCE_DIR}/lib/glad/src/glad.c)


add_executable(lab_4 main.cpp ${GLAD_SOURCE})


target_link_libraries(lab_4
    OpenGL::GL
    GLUT::GLUT
    OpenGL::GLU
)


target_include_directories(lab_4 PRIVATE 
    /usr/include
)