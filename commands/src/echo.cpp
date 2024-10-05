#include <iostream>
#include <cstring>

std::string unescapeSequences(const std::string& input) {
    std::string result;
    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] == '\\' && i + 1 < input.length()) {
            switch (input[i + 1]) {
                case 'n': result += '\n'; ++i; break;
                case 't': result += '\t'; ++i; break;
                case 'r': result += '\r'; ++i; break;
                case 'b': result += '\b'; ++i; break;
                case '\\': result += '\\'; ++i; break;
                case '\'': result += '\''; ++i; break;
                case '\"': result += '\"'; ++i; break;
                default: result += input[i]; break;
            }
        } else {
            result += input[i];
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
    bool line_break = true;
    bool escape = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 && line_break) {
            line_break = false;
            continue;
        }

        if (strcmp(argv[i], "-e") == 0 && !escape) {
            escape = true;
            continue;
        }

        if (strcmp(argv[i], "-E") == 0 && escape) {
            escape = false;
            continue;
        }

        if (escape)
            std::cout << unescapeSequences(argv[i]);
        else
            std::cout << argv[i];

        if (i != argc - 1)
            std::cout << " ";
    }

    if (line_break)
        std::cout << std::endl;
}