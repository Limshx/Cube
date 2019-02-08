project(Cube)
set(CMAKE_EXE_LINKER_FLAGS "-lglut -lGL -lGLU -lm -lX11")
add_executable(${PROJECT_NAME} "main.c" "cube.c" "cube.h")
