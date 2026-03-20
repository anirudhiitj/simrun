cmake_minimum_required(VERSION 3.15)
project(simrun)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------
# Find packages (vcpkg CONFIG mode)
# ---------------------------
find_package(nlohmann_json CONFIG REQUIRED)

# ---------------------------
# Source files
# ---------------------------
set(SOURCES
    main.cpp
    core/event_queue.cpp
    core/simulator.cpp
    entities/service.cpp
    entities/database.cpp
    entities/cache.cpp
    events/api_events.cpp
    events/database_events.cpp
    events/network_link_event.cpp
    factory/factory.cpp
    src/ir_parser.cpp
    src/event_initializer.cpp
)

# ---------------------------
# Create executable
# ---------------------------
add_executable(simrun ${SOURCES})

# Put the final .exe in the project source/sim folder
set_target_properties(simrun PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_SOURCE_DIR}"
)

# ---------------------------
# Link libraries
# ---------------------------
target_link_libraries(simrun
    PRIVATE
    nlohmann_json::nlohmann_json
)
