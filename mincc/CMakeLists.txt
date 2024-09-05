add_executable (mincc "mincc.cpp" "mincc.h" "lexer.cpp" "lexer.h" "token.h" "parser.cpp" "parser.h" "ast.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET mincc PROPERTY CXX_STANDARD 20)
endif()

# Link against LLVM libraries ${LLVM_LIBRARIES}
target_link_libraries(mincc PUBLIC LLVM)
#message(STATUS "Current LLVM libs are: ${llvm_libs}")
#message(STATUS "Current LLVM libs are: ${llvm_libraries}")