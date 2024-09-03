add_executable (mincc "mincc.cpp" "mincc.h" "lexer.cpp" "lexer.h" "token.h" "parser.cpp" "parser.h" "ast.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET mincc PROPERTY CXX_STANDARD 20)
endif()