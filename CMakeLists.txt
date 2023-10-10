project(ARK)

include_directories(lib/include)

file(GLOB SOURCES "src/*.cpp")

add_executable(ARK ${SOURCES})

target_link_libraries(ARK ${lib})

