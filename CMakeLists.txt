### Project Info ###

cmake_minimum_required(VERSION 3.1)
project(ControllerTester)

# Build type
if(NOT CMAKE_BUILD_TYPE)
   message(STATUS "No build type selected, defaulting to Debug")
   set(CMAKE_BUILD_TYPE "Debug")
endif(NOT CMAKE_BUILD_TYPE)

# Compiler requirements
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directories
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(DEP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies")

# Source content
include(sources.cmake)
foreach(SOURCE_NAME ${SOURCE_NAMES})
   list(APPEND SOURCES "${SRC_DIR}/${SOURCE_NAME}")
endforeach(SOURCE_NAME)

include(headers.cmake)
foreach(HEADER_NAME ${HEADER_NAMES})
   list(APPEND HEADERS "${INCLUDE_DIR}/${HEADER_NAME}")
endforeach(HEADER_NAME)

set(INCLUDES)
list(APPEND INCLUDES
   ${INCLUDE_DIR}
)

source_group("Dependencies" "dependencies/*")

### Libraries ###

function(attach_lib include source lib)
   if(include)
      list(APPEND INCLUDES ${include})
      set(INCLUDES ${INCLUDES} PARENT_SCOPE)
   endif(include)

   if(source)
      list(APPEND SOURCES ${source})
      set(SOURCES ${SOURCES} PARENT_SCOPE)
   endif(source)

   if(lib)
      list(APPEND LIBS ${lib})
      set(LIBS ${LIBS} PARENT_SCOPE)

      if(BUILD_SHARED_LIBS)
         list(APPEND COPY_LIBS ${lib})
         set(COPY_LIBS ${COPY_LIBS} PARENT_SCOPE)
      endif(BUILD_SHARED_LIBS)
   endif(lib)
endfunction(attach_lib)

function(set_import_locations target lib_release lib_debug imp_release imp_debug)
   if(lib_release)
      set_target_properties(${target} PROPERTIES IMPORTED_LOCATION_RELEASE ${lib_release})
   endif(lib_release)

   if(lib_debug)
      set_target_properties(${target} PROPERTIES IMPORTED_LOCATION_DEBUG ${lib_debug})
   endif(lib_debug)

   if(imp_release)
      set_target_properties(${target} PROPERTIES IMPORTED_IMPLIB_RELEASE ${imp_release})
   endif(imp_release)

   if(imp_debug)
      set_target_properties(${target} PROPERTIES IMPORTED_IMPLIB_DEBUG ${imp_debug})
   endif(imp_debug)
endfunction(set_import_locations)

macro(list_subdirs result curdir)
   file(GLOB children RELATIVE ${curdir} ${curdir}/*)
   set(dirlist "")
   foreach(child ${children})
      if(IS_DIRECTORY ${curdir}/${child})
         list(APPEND dirlist ${child})
      endif(IS_DIRECTORY ${curdir}/${child})
   endforeach(child)
   set(${result} ${dirlist})
endmacro(list_subdirs)

function(copy_headers source dest recurse)
   file(MAKE_DIRECTORY "${dest}")

   file(GLOB HEADERS "${source}/*.h" "${source}/*.hpp" "${source}/*.inl")
   file(COPY ${HEADERS} DESTINATION "${dest}")

   if(recurse)
      list_subdirs(SUBDIRS ${source})
      foreach(subdir ${SUBDIRS})
         copy_headers("${source}/${subdir}" "${dest}/${subdir}" recurse)
      endforeach()
   endif(recurse)
endfunction(copy_headers)

## Integrated ##

## Static ##

set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Build shared libraries")

## Shared ##

set(BUILD_SHARED_LIBS ON CACHE INTERNAL "Build shared libraries")

# Shiny
set(SHINY_DIR "${DEP_DIR}/Shiny")
add_subdirectory(${SHINY_DIR})
attach_lib("${SHINY_INCLUDE_DIRS}" "" "${SHINY_TARGET}")
add_definitions(${SHINY_DEFINITIONS})
list(APPEND COPY_LIBS ${SHINY_SHARED_LIBS})

### Building ###

# Includes
include_directories(${INCLUDES})

# Binary
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link
target_link_libraries(${PROJECT_NAME} ${LIBS})

### Post-Build ###

# Copy DLLs
if(WIN32)
   foreach(COPY_LIB ${COPY_LIBS})
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${COPY_LIB}>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
      )
   endforeach(COPY_LIB)
endif(WIN32)
