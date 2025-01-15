cmake_minimum_required (VERSION 3.28)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AsciiVideoEncoder")

add_executable (AsciiVideoEncoder "src/filesys.cpp" "src/bmp.cpp" "src/functions.cpp" "src/main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AsciiVideoEncoder PROPERTY CXX_STANDARD 23)
endif()