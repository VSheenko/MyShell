#include "UtilsMini.h"

std::vector<std::string> UtilsMini::Split(std::string s, const std::string& separator) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;

    while ((pos = s.find(separator)) != std::string::npos) {
        token = s.substr(0, pos);

        if (!token.empty())
            tokens.push_back(token);

        s.erase(0, pos + separator.length());
    }

    if (!s.empty())
        tokens.push_back(s);

    return tokens;
}
