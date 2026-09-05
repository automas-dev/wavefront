#pragma once

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>

namespace wavefront_test {
    namespace fs = std::filesystem;

    /**
     * RAII helper that creates a unique temporary directory and removes it
     * (recursively) when the guard goes out of scope.
     *
     * Used to exercise Material::fromFile / Model::fromFile, which only
     * accept real filesystem paths.
     */
    struct TempDir {
        fs::path path;

        TempDir() {
            path = fs::temp_directory_path() / fs::path("wavefront_test_" + std::to_string(std::rand()));
            fs::create_directories(path);
        }

        ~TempDir() {
            std::error_code ec;
            fs::remove_all(path, ec);
        }

        /**
         * Write contents to a file named name inside this temp directory and
         * return the full path to the created file.
         */
        fs::path writeFile(const std::string & name, const std::string & contents) const {
            fs::path filePath = path / name;
            std::ofstream os(filePath);
            os << contents;
            os.close();
            return filePath;
        }
    };
}
