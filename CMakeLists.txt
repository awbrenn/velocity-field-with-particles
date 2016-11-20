cmake_minimum_required(VERSION 3.3)
project(ParticleSystem)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
add_compile_options(-std=c++11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

include_directories("/usr/include")
find_library(OIIO "OpenImageIO" "/usr/lib")
find_library(GLUT "glut")
find_library(GL "GL")
find_library(GLU "GLU")

set(SOURCE_FILES ${PROJECT_NAME}.cpp Utility.cpp Camera.cpp Matrix.cpp Vector.cpp FGAFile.cpp FGAFile.h VelocityGrid.cpp VelocityGrid.h Particle.cpp Particle.h Emitter.cpp Emitter.h Solver.cpp Solver.h Collider.cpp Collider.h ObjWriter.cpp ObjWriter.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${GLUT} ${GL} ${GLU} ${OIIO})
