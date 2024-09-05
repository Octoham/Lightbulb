cmake_minimum_required (VERSION 3.8)

cmake_policy(SET CMP0004 OLD)

# This is just for hot reload or smth like that idk
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Lightbulb")


# Add LLVM
#find_package(LLVM REQUIRED CONFIG)
#message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
#message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
#include_directories(${LLVM_INCLUDE_DIRS})
#separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
#add_definitions(${LLVM_DEFINITIONS_LIST})
#execute_process(COMMAND llvm-config --components
#                OUTPUT_VARIABLE llvm_libraries)
#llvm_map_components_to_libnames(llvm_libs all core all-targets support irreader)
# Link against LLVM libraries
#target_link_libraries(mincc ${llvm_libs})
#message(STATUS "Current LLVM libs are: ${llvm_libs}")


set(LLVM_DIR "D:/llvm/llvm-18.1.8-windows-amd64-msvc17-msvcrt/lib/cmake/llvm") # set llvm cmake path

# Find LLVM
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Split the definitions properly 
separate_arguments(LLVM_DEFINITIONS)

# Some diagnostics 
message(STATUS "LLVM libraries: ${LLVM_LIBRARIES}")
message(STATUS "LLVM includes: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM definitions: ${LLVM_DEFINITIONS}")
message(STATUS "LLVM tools: ${LLVM_TOOLS_BINARY_DIR}")

add_library(LLVM INTERFACE)
target_include_directories(LLVM SYSTEM INTERFACE ${LLVM_INCLUDE_DIRS})
target_link_libraries(LLVM INTERFACE ${LLVM_AVAILABLE_LIBS})
target_compile_definitions(LLVM INTERFACE ${LLVM_DEFINITIONS} -DNOMINMAX)


# Add subprojects
add_subdirectory ("mincc")
add_subdirectory ("Lightbulb")