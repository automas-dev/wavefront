#pragma once

#include <istream>
#include <iterator>
#include <string>
#include <vector>

namespace wavefront {
    /**
     * Split a string based on a delimeter.
     *
     * @param str the string to split
     * @param delim the delimiter
     *
     * @return a vector of strings
     */
    std::vector<std::string> splitString(const std::string & str, char delim);

    /**
     * Parser for Wavefront formatted .obj and .mtl files.
     */
    class Parser {
    public:
        /**
         * A single token with key and value.
         */
        struct Token {
            /// The token command / key
            std::string key;
            /// Everything after the first space
            std::string value;

            /**
             * Split value on spaces and return the results.
             *
             * @return value split by spaces
             */
            std::vector<std::string> params() const;
        };

        struct iterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = Token;
            using pointer = value_type *;
            using reference = value_type &;

            iterator() : parser(nullptr) {}

            iterator(Parser * parser) : parser(parser) {}

            reference operator*() const {
                return token;
            }

            pointer operator->() {
                return &token;
            }

            iterator & operator++() {
                parser->read(token);
                if (!parser->operator bool())
                    parser = nullptr;
                return *this;
            }

            iterator operator++(int) {
                iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const iterator & a, const iterator & b) {
                return a.parser == b.parser;
            }

            friend bool operator!=(const iterator & a, const iterator & b) {
                return a.parser != b.parser;
            }

        private:
            mutable Parser::Token token;
            Parser * parser;
        };

    private:
        std::istream & is;

    public:
        /**
         * Create a new WavefrontParser that reads from is.
         *
         * @param is the input stream to read from
         */
        Parser(std::istream & is);

        /**
         * Return true if the last read was successful.
         *
         * @return was the read successful
         */
        explicit operator bool() const;

        /**
         * Read the next token into token.
         *
         * @param token the Token to read into
         */
        void read(Token & token);

        /**
         * Get an iterator that points to the start of the parsed tokens.
         *
         * @return an iterator over the tokens
         */
        iterator begin();

        /**
         * Get an iterator that points to the end of the parsed tokens.
         *
         * @return an iterator to the end of all tokens
         */
        iterator end();
    };
}
