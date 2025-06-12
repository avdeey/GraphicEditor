# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\GraphicEditor_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\GraphicEditor_autogen.dir\\ParseCache.txt"
  "GraphicEditor_autogen"
  )
endif()
