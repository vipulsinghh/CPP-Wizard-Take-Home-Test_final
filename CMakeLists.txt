cmake_minimum_required(VERSION 3.10)
project(abx_client VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent
include(FetchContent)

# Fetch nlohmann_json
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Find required packages
find_package(Threads REQUIRED)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source files
set(SOURCES
    src/abx_client.cpp
    main.cpp
)

# Create executable
add_executable(abx_client ${SOURCES})

# Link libraries
target_link_libraries(abx_client
    PRIVATE
    Threads::Threads
    nlohmann_json::nlohmann_json
)

# Install rules
install(TARGETS abx_client
    RUNTIME DESTINATION bin
) 