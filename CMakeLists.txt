cmake_minimum_required(VERSION 3.20)

project(DBApp)

set(CMAKE_CXX_STANDARD 17)

# headers 
include_directories(${PROJECT_SOURCE_DIR}/Dependencies/include 
    ${PROJECT_SOURCE_DIR}/DrawingBoardApplication/Imgui
    ${PROJECT_SOURCE_DIR}/DrawingBoardApplication
    ${PROJECT_SOURCE_DIR}/DrawingBoardApplication/Objects
    ${PROJECT_SOURCE_DIR}/Dependencies)


# set(GLFW_LINK /usr/local/lib/libglfw.3.dylib)
link_libraries(/usr/local/lib/libglfw.3.dylib)
# link_libraries(/user/local/lib)

set(GLAD_SRC ${PROJECT_SOURCE_DIR}/Dependencies/src/glad.c)

# sources
file(GLOB IMGUI ${PROJECT_SOURCE_DIR}/DrawingBoardApplication/Imgui/*.cpp)
file(GLOB SRC ${PROJECT_SOURCE_DIR}/DrawingBoardApplication/*.cpp)
file(GLOB OBJECTS_SRC ${PROJECT_SOURCE_DIR}/DrawingBoardApplication/Objects/*.cpp)

add_executable(DBApp ${IMGUI} ${SRC} ${GLAD_SRC} ${OBJECTS_SRC})

if(APPLE)
    target_link_libraries(DBApp "-framework OpenGL")
endif()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)