# CMAKE generated file: DO NOT EDIT!
# Generated by CMake Version 3.28
cmake_policy(SET CMP0009 NEW)

# sol2-headers at libs/sol2/CMakeLists.txt:83 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/dregos/uni/grapevine/libs/sol2/include/sol*.hpp")
set(OLD_GLOB
  "/home/dregos/uni/grapevine/libs/sol2/include/sol/sol.hpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/dregos/uni/grapevine/build/CMakeFiles/cmake.verify_globs")
endif()

# sol2-sources at libs/sol2/CMakeLists.txt:89 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/dregos/uni/grapevine/libs/sol2/source/**")
set(OLD_GLOB
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/dregos/uni/grapevine/build/CMakeFiles/cmake.verify_globs")
endif()
