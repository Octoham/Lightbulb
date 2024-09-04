#include "mincc.h"

int main(int argc, char* argv[]) {
    // Handle arguments
    std::cout << "Number of arguments: " << argc << std::endl;

    for (int i = 0; i < argc; i++) {
        std::cout << "Argument " << i << ": " << argv[i] << std::endl;
    }

    // TODO Call lexer, then parser, then IRgen

    return 0;
}

std::string ReadFile(std::string path)
{
    // Simple function to easily open a file
    std::ifstream file(path);
    if (file.is_open()) {
        std::string contents((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
        std::cout << contents << std::endl; // Print the contents for now
        file.close();
        return contents;
    }
    else {
        std::cerr << "Unable to open file";
        return 0;
    }
}
