# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\MusicManager_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MusicManager_autogen.dir\\ParseCache.txt"
  "MusicManager_autogen"
  )
endif()
