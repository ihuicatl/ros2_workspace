cmake_minimum_required(VERSION 3.12)

project(ament_cmake_export_targets NONE)

find_package(ament_cmake_core REQUIRED)

ament_package(
  CONFIG_EXTRAS "ament_cmake_export_targets-extras.cmake"
)

install(
  DIRECTORY cmake
  DESTINATION share/${PROJECT_NAME}
)
