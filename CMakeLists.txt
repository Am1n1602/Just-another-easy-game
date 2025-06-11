cmake_minimum_required(VERSION 3.10)
project(Just_Another_Easy_Game VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# For FetchContent
include(FetchContent)

# Collect your sources (re-runs when files change)
file(GLOB_RECURSE SOURCE_FILES
  CONFIGURE_DEPENDS
  ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# Try to find a system install of raylib
find_package(raylib 4.0 QUIET)
if (NOT raylib_FOUND)
  FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
  )
  FetchContent_MakeAvailable(raylib)
endif()

# Try to find or fetch nlohmann_json
find_package(nlohmann_json 3.2.0 QUIET)
if (NOT nlohmann_json_FOUND)
  FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
  )
  FetchContent_MakeAvailable(json)
endif()

# Try to find or fetch CPR
find_package(CPR CONFIG QUIET)
if (NOT CPR_FOUND)
  FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
  )
  FetchContent_MakeAvailable(cpr)
endif()

# Define executable\ nadd_executable(Just_Another_Easy_Game ${SOURCE_FILES})

# Only this target gets these include dirs
target_include_directories(Just_Another_Easy_Game
  PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/libs
)

# Link libraries
# raylib, CPR, and JSON

find_package(raylib REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(CPR REQUIRED)

target_link_libraries(Just_Another_Easy_Game
  PRIVATE
    raylib
    nlohmann_json::nlohmann_json
    CPR::cpr
)

# Warnings as errors or just extra pedantry
if (MSVC)
  target_compile_options(Just_Another_Easy_Game PRIVATE /W4)
else()
  target_compile_options(Just_Another_Easy_Game PRIVATE -Wall -Wextra -pedantic)
endif()

# Platform-specific frameworks / libs
if (APPLE)
  target_link_libraries(Just_Another_Easy_Game
    PRIVATE
      "-framework OpenGL"
      "-framework Cocoa"
      "-framework IOKit"
      "-framework CoreVideo"
  )
elseif (UNIX AND NOT APPLE)
  target_link_libraries(Just_Another_Easy_Game
    PRIVATE
      pthread
      GL
      dl
      m
  )
endif()

# Copy your assets next to the built binary
set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
add_custom_command(TARGET Just_Another_Easy_Game POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${ASSETS_SOURCE_DIR}"
    "$<TARGET_FILE_DIR:Just_Another_Easy_Game>/assets"
)
