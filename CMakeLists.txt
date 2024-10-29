cmake_minimum_required(VERSION 3.23)
project(SFMLMario)

set(CMAKE_CXX_STANDARD 14)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
set(ABC C:/SFML/lib)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(SFML_Project)

file(GLOB_RECURSE MARIO_SRC "src/*.cpp")
add_executable(SFMLMario ${MARIO_SRC}) 

target_link_libraries(SFMLMario sfml-system sfml-window sfml-graphics sfml-audio)