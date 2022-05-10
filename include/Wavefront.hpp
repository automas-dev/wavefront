#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace wavefront {
    using std::string;
    using std::vector;
    using std::shared_ptr;
    using glm::vec2;
    using glm::vec3;

    struct Mesh {
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
    };

    class ModelLoadException : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    struct Model {
        vector<shared_ptr<Mesh>> objects;
        vector<shared_ptr<Material>> materials;

        Model();
        ~Model();

        Model(const Model & other) = default;

        Model(Model && other) = default;

        Model & operator=(const Model & other) = default;

        Model & operator=(Model && other) = default;

        void clear();

        void loadMaterialsFrom(const string & path);

        void loadModelFrom(const string & path);
    };
}
