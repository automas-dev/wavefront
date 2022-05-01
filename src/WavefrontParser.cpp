#include "WavefrontParser.hpp"

#include <sstream>

namespace wavefront {
    std::vector<std::string> splitString(const std::string & str, char delim) {
        std::vector<std::string> parts;
        std::stringstream ss(str);
        std::string part;
        for (; std::getline(ss, part, delim);) {
            parts.push_back(part);
        }
        return parts;
    }
}

namespace wavefront {
    std::vector<std::string> Parser::Token::params() const {
        return splitString(value, ' ');
    }

    Parser::Parser(std::istream & is) : is(is) {}

    Parser::operator bool() const {
        return is.operator bool();
    }

    void Parser::read(Parser::Token & token) {
        std::string line;
        for (; std::getline(is, line);) {
            if (line.empty() || line[0] == '#')
                continue;

            auto split = line.find_first_of(' ');
            if (split == std::string::npos) {
                token.key = line;
            }
            else {
                token.key = line.substr(0, split);
                token.value = line.substr(split + 1);
            }
            break;
        }
    }

    Parser::iterator Parser::begin() {
        return iterator(this);
    }

    Parser::iterator Parser::end() {
        return iterator();
    }
}
