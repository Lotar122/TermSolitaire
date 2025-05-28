cmake_minimum_required (VERSION 3.12)

project ("TermSolitaire")

if(CMAKE_BUILD_TYPE STREQUAL "")
	set(CMAKE_BUILD_TYPE "Release")
endif()

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/bin/${OUTPUTCONFIG})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/lib/${OUTPUTCONFIG})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/lib_static/${OUTPUTCONFIG})
endforeach()


file(GLOB_RECURSE SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(TermSolitaire ${SOURCES} "${CMAKE_SOURCE_DIR}/src/main.cpp")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	# do not define the macro
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(TermSolitaire PRIVATE DEBUG)
endif()

if(WIN32)
	target_compile_definitions(TermSolitaire PRIVATE Windows)
endif()
if (MSVC)
    add_compile_options(/utf-8)
endif()

target_include_directories(TermSolitaire
	PUBLIC
	${CMAKE_SOURCE_DIR},
	${CMAKE_SOURCE_DIR}/src
)

target_compile_features(TermSolitaire PRIVATE cxx_std_20)
