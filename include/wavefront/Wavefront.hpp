#pragma once

#include <filesystem>
#include <glm/glm.hpp>
#include <istream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace wavefront {
    using glm::vec2;
    using glm::vec3;
    using std::istream;
    using std::shared_ptr;
    using std::string;
    using std::vector;
    namespace fs = std::filesystem;

    struct Mesh {
        using Ptr = shared_ptr<Mesh>;
        using ConstPtr = const shared_ptr<Mesh>;

        string name;
        size_t matId;
        vector<vec3> vertices;
        vector<vec2> texcoords;
        vector<vec3> normals;

        Mesh();

        size_t size() const;
    };

    class MaterialLoadException : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    struct Material {
        using Ptr = shared_ptr<Material>;
        using ConstPtr = const shared_ptr<Material>;

        string name;
        float specExp;
        float alpha;
        vec3 colAmbient;
        vec3 colDiffuse;
        vec3 colSpecular;
        string texAlbedo;
        string texNormal;
        string texSpecular;

        Material();
        static vector<Ptr> fromFile(const fs::path & path);
        static vector<Ptr> fromStream(istream & is);
    };

    class ModelLoadException : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    struct Model {
        using Ptr = shared_ptr<Model>;
        using ConstPtr = const shared_ptr<Model>;

        vector<Mesh::Ptr> objects;
        vector<Material::Ptr> materials;

        Model();

        Model(const Model & other) = default;

        Model(Model && other) = default;

        Model & operator=(const Model & other) = default;

        Model & operator=(Model && other) = default;

        static Ptr fromFile(const fs::path & path);
        static Ptr fromStream(istream & is, const fs::path & basePath = "");
    };
}
