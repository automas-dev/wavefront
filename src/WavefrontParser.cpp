#include "WavefrontParser.hpp"

#include <sstream>

namespace wavefront {
    using std::stringstream;

    vector<string> splitString(const string & str, char delim) {
        vector<string> parts;
        stringstream ss(str);
        string part;
        for (; std::getline(ss, part, delim);) {
            parts.push_back(part);
        }
        return parts;
    }
}

namespace wavefront {
    vector<string> Parser::Token::params() const {
        return splitString(value, ' ');
    }

    Parser::Parser(istream & is) : is(is) {}

    Parser::operator bool() const {
        return is.operator bool();
    }

    void Parser::read(Parser::Token & token) {
        string line;
        for (; std::getline(is, line);) {
            if (line.empty() || line[0] == '#')
                continue;

            auto split = line.find_first_of(' ');
            if (split == string::npos) {
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
