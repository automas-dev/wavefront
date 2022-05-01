#pragma once

#include <glm/glm.hpp>
#include <stdexcept>
#include <string>
#include <vector>

namespace wavefront {
    struct Mesh {
        std::string name;
        size_t matId;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> texcoords;
        std::vector<glm::vec3> normals;

        Mesh();

        size_t size() const;
    };

    class MaterialLoadException : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    struct Material {
        std::string name;
        float specExp;
        float alpha;
        glm::vec3 colAmbient;
        glm::vec3 colDiffuse;
        glm::vec3 colSpecular;
        std::string texAlbedo;
        std::string texNormal;
        std::string texSpecular;
        Material();
    };

    class ModelLoadException : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    struct Model {
        std::vector<Mesh *> objects;
        std::vector<Material *> materials;

        Model();
        ~Model();

        Model(const Model & other) = default;

        Model(Model && other) = default;

        Model & operator=(const Model & other) = default;

        Model & operator=(Model && other) = default;

        void clear();

        void loadMaterialsFrom(const std::string & path);

        void loadModelFrom(const std::string & path);
    };
}
