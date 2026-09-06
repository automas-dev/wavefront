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

    /**
     * A single named mesh made up of vertices, texture coordinates and
     * normals, along with a reference to the material used to render it.
     */
    struct Mesh {
        using Ptr = shared_ptr<Mesh>;
        using ConstPtr = const shared_ptr<Mesh>;

        /// The name of this mesh (from the `o` command)
        string name;
        /// Index into the containing Model's materials, or -1 if unset
        int matId;
        /// The vertex positions
        vector<vec3> vertices;
        /// The texture coordinates
        vector<vec2> texcoords;
        /// The vertex normals
        vector<vec3> normals;

        /**
         * Create a new empty Mesh with no name and matId set to -1.
         */
        Mesh();

        /**
         * Get the number of vertices in this mesh.
         *
         * @return the number of vertices
         */
        size_t size() const;
    };

    /**
     * Thrown when a Material fails to load from a file or stream.
     */
    class MaterialLoadException : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    /**
     * A single named material parsed from a Wavefront .mtl file.
     */
    struct Material {
        using Ptr = shared_ptr<Material>;
        using ConstPtr = const shared_ptr<Material>;

        /// The name of this material (from the `newmtl` command)
        string name;
        /// The specular exponent (`Ni`)
        float specExp;
        /// The alpha / dissolve value (`d`)
        float alpha;
        /// The ambient color (`Ka`)
        vec3 colAmbient;
        /// The diffuse color (`Kd`)
        vec3 colDiffuse;
        /// The specular color (`Ks`)
        vec3 colSpecular;
        /// The path to the albedo / diffuse texture (`map_Kd`)
        string texAlbedo;
        /// The path to the normal / bump map texture (`map_Bump` or `map_bump`)
        string texNormal;
        /// The path to the specular texture (`map_Ks`)
        string texSpecular;

        /**
         * Create a new Material with default values (specExp and alpha set
         * to 1.0, colors set to zero, texture paths empty).
         */
        Material();

        /**
         * Load one or more materials from a .mtl file.
         *
         * @param path path to the .mtl file to load
         *
         * @return the materials parsed from the file
         *
         * @throws MaterialLoadException if the file could not be opened or
         * the contents could not be parsed
         */
        static vector<Ptr> fromFile(const fs::path & path);

        /**
         * Load one or more materials from a stream of .mtl formatted text.
         *
         * @param is the stream to read from
         *
         * @return the materials parsed from the stream
         *
         * @throws MaterialLoadException if the stream contents could not be
         * parsed
         */
        static vector<Ptr> fromStream(istream & is);
    };

    /**
     * Thrown when a Model fails to load from a file or stream.
     */
    class ModelLoadException : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    /**
     * A full Wavefront model made up of one or more meshes and the
     * materials they reference.
     */
    struct Model {
        using Ptr = shared_ptr<Model>;
        using ConstPtr = const shared_ptr<Model>;

        /// The meshes that make up this model
        vector<Mesh::Ptr> objects;
        /// The materials referenced by this model's meshes
        vector<Material::Ptr> materials;

        /**
         * Create a new empty Model with no objects or materials.
         */
        Model();

        Model(const Model & other) = default;

        Model(Model && other) = default;

        Model & operator=(const Model & other) = default;

        Model & operator=(Model && other) = default;

        /**
         * Load a model from a .obj file. Any referenced .mtl file is
         * resolved relative to the directory containing path.
         *
         * @param path path to the .obj file to load
         *
         * @return the loaded model
         *
         * @throws ModelLoadException if the file could not be opened or the
         * contents could not be parsed
         */
        static Ptr fromFile(const fs::path & path);

        /**
         * Load a model from a stream of .obj formatted text. Any referenced
         * .mtl file is resolved relative to basePath.
         *
         * @param is the stream to read from
         * @param basePath directory used to resolve referenced .mtl files
         *
         * @return the loaded model
         *
         * @throws ModelLoadException if the stream contents could not be
         * parsed
         */
        static Ptr fromStream(istream & is, const fs::path & basePath = "");
    };
}
