#include "WavefrontParser.hpp"

#include <sstream>

namespace wavefront {
    using std::stringstream;

    vector<string> splitString(const string & str, char delim, size_t maxcount) {
        vector<string> parts;

        size_t start = 0, end = 0;
        while (end < str.size() && (maxcount-- > 0)) {
            while (end < str.size()) {
                if (str[end] == delim) {
                    parts.push_back(str.substr(start, end - start));
                    start = ++end;
                    break;
                }
                else
                    ++end;
            }
        }

        if (parts.empty())
            parts.push_back(str);

        else if (start <= str.size())
            parts.push_back(str.substr(start));

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
                token.value.clear();
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
