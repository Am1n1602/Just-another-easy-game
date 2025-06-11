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

# Platform-specific OpenSSL hinting
if(APPLE)
  set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl@1.1")
elseif(WIN32)
  # Windows users must set this path manually if OpenSSL isn't globally available
  set(OPENSSL_ROOT_DIR "C:/OpenSSL-Win64")
elseif(UNIX AND NOT APPLE)
  set(OPENSSL_ROOT_DIR "/usr")
endif()

# Fetch nlohmann_json
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git

)
FetchContent_MakeAvailable(json)

# Fetch CPR with SSL support
set(CPR_USE_SYSTEM_CURL OFF)
set(CPR_ENABLE_SSL ON)
FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git

)
FetchContent_MakeAvailable(cpr)

# Define executable
add_executable(Just_Another_Easy_Game ${SOURCE_FILES})

# Only this target gets these include dirs
target_include_directories(Just_Another_Easy_Game
  PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/libs
)

# Link libraries
target_link_libraries(Just_Another_Easy_Game
  PRIVATE
    raylib
    nlohmann_json::nlohmann_json
    cpr::cpr
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
