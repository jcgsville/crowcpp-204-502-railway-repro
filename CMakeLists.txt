cmake_minimum_required(VERSION 3.28)
project(crowcpp-204-502-railway-repro)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DCROW_BUILD_EXAMPLES OFF CACHE BOOL "Disable building crow examples")
set(DCROW_BUILD_TESTS OFF CACHE BOOL "Disable building crow tests")

include(FetchContent)
include(GNUInstallDirs) # Provides standard install directory variables like CMAKE_INSTALL_LIBDIR, CMAKE_INSTALL_INCLUDEDIR

FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  # Crow says it can use up to 1.30 https://crowcpp.org/master/getting_started/setup/linux/#getting-crow
  GIT_TAG asio-1-30-0
)

# Set the include directory hint for Crow's Find asio.cmake
# Need to FetchContent_MakeAvailable(asio) first so that asio_SOURCE_DIR is set
FetchContent_MakeAvailable(asio)
set(ASIO_INCLUDE_DIR "${asio_SOURCE_DIR}/asio/include" CACHE PATH "Path to ASIO include directory")

FetchContent_Declare(
  crow
  GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
  GIT_TAG v1.2.1
)

FetchContent_MakeAvailable(crow)

install(DIRECTORY ${asio_SOURCE_DIR}/asio/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_executable(server server.cpp)

target_link_libraries(
  server
  PRIVATE
  Crow::Crow
)
