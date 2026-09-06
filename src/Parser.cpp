#include "wavefront/Parser.hpp"

#include <algorithm>
#include <sstream>

namespace wavefront {
    using std::isspace;
    using std::stringstream;

    // See https://stackoverflow.com/a/217605

    // Trim from the start (in place)
    static inline void ltrim(string & s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                    return !std::isspace(ch);
                }));
    }

    // Trim from the end (in place)
    static inline void rtrim(string & s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
                    return !std::isspace(ch);
                }).base(),
                s.end());
    }

    void trimString(string & str) {
        ltrim(str);
        rtrim(str);
    }

    vector<string> splitString(const string & str, char delim, int maxcount) {
        vector<string> parts;

        size_t start = 0, end = 0;
        while (end < str.size() && parts.size() != maxcount) {
            if (str[end] == delim) {
                parts.push_back(str.substr(start, end - start));
                start = ++end;
            }
            else
                ++end;
        }

        if (parts.empty())
            parts.push_back(str);

        else
            parts.push_back(str.substr(start));

        return parts;
    }

    vector<string> splitStringSpace(const string & str, int maxcount) {
        vector<string> parts;

        size_t start = 0, end = 0;
        while (end < str.size() && parts.size() != maxcount) {
            if (isspace(str[end])) {
                if (end > start)
                    parts.push_back(str.substr(start, end - start));
                start = ++end;
            }
            else
                ++end;
        }

        // Stopped for end of string
        if (start < end)
            parts.push_back(str.substr(start, end - start));

        // Stopped for max count
        if (end < str.size()) {
            string remainder = str.substr(end, str.size() - end);
            trimString(remainder);
            if (!remainder.empty())
                parts.push_back(remainder);
        }

        return parts;
    }
}

namespace wavefront {
    vector<string> Parser::Token::params() const {
        return splitStringSpace(value);
    }
}

namespace wavefront {
    Parser::Parser(istream & is) : is(is) {}

    Parser::operator bool() const {
        return is.operator bool();
    }

    bool Parser::hasNext() {
        if (line.empty()) {
            findNext();
        }
        return !line.empty();
    }

    void Parser::read(Parser::Token & token) {
        // hasNext will populate this->line
        if (hasNext()) {
            auto split = line.find_first_of(' ');
            if (split == string::npos) {
                token.key = line;
                token.value.clear();
            }
            else {
                token.key = line.substr(0, split);
                token.value = line.substr(split + 1);
            }
            line.clear();
        }
    }

    Parser::iterator Parser::begin() {
        return iterator(this);
    }

    Parser::iterator Parser::end() {
        return iterator();
    }

    void Parser::findNext() {
        if (!is) {
            line.clear();
            return;
        }

        for (; std::getline(is, line);) {
            // Remove comment if any exists
            auto split = line.find_first_of('#');
            if (split != string::npos) {
                line.erase(line.begin() + split, line.end());
            }

            trimString(line);

            if (!line.empty())
                break;
        }
    }
}
