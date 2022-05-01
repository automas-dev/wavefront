#include "Wavefront.hpp"

#include <array>
#include <fstream>
#include <stdexcept>

#include "WavefrontParser.hpp"

namespace wavefront {
    Mesh::Mesh() : matId(0) {}

    size_t Mesh::size() const {
        return vertices.size();
    }
}

namespace wavefront {
    Material::Material() : specExp(1.0), alpha(1.0) {}
}

namespace wavefront {
    Model::Model() {}

    Model::~Model() {
        clear();
    }

    void Model::clear() {
        for (auto * ptr : objects) {
            delete ptr;
        }
        objects.clear();
        for (auto * ptr : materials) {
            delete ptr;
        }
        materials.clear();
    }

    void Model::loadMaterialsFrom(const std::string & path) {
        std::ifstream is(path);
        if (!is.is_open()) {
            throw ModelLoadException("Failed to open file " + path);
        }
        Parser parser(is);

        Material * material = nullptr;

        for (auto & token : parser) {
            switch (token.key[0]) {
                case 'n': { // newmtl
                    if (token.key != "newmtl")
                        break;
                    material = new Material();
                    material->name = token.value;
                    materials.push_back(material);
                } break;
                case 'K': {
                    if (token.key.size() < 2)
                        break;
                    switch (token.key[1]) {
                        case 'a': { // Ka
                            if (!material)
                                throw MaterialLoadException(
                                    "Got a Color ambient (Ka) before starign an material (newmtl)");
                            auto params = token.params();
                            if (params.size() != material->colAmbient.length())
                                throw MaterialLoadException(
                                    "Color ambient (Ka) must have 3 values");
                            for (int i = 0; i < material->colAmbient.length(); i++)
                                material->colAmbient[i] = std::stof(params[i]);
                        } break;
                        case 'd': { // Kd
                            if (!material)
                                throw MaterialLoadException(
                                    "Got a Color diffuse (Kd) before starign an material (newmtl)");
                            auto params = token.params();
                            if (params.size() != material->colDiffuse.length())
                                throw MaterialLoadException(
                                    "Color diffuse (Kd) must have 3 values");
                            for (int i = 0; i < material->colDiffuse.length(); i++)
                                material->colDiffuse[i] = std::stof(params[i]);
                        } break;
                        case 's': { // Ks
                            if (!material)
                                throw MaterialLoadException(
                                    "Got a Color specular (Ks) before starign an material (newmtl)");
                            auto params = token.params();
                            if (params.size() != material->colSpecular.length())
                                throw MaterialLoadException(
                                    "Color specular (Ks) must have 3 values");
                            for (int i = 0; i < material->colSpecular.length(); i++)
                                material->colSpecular[i] = std::stof(params[i]);
                        } break;
                        default:
                            break;
                    }
                } break;
                case 'N': {
                    if (token.key.size() < 2)
                        break;
                    switch (token.key[1]) {
                        case 'i': { // Ni
                            material->specExp = std::stof(token.value);
                        } break;
                        default:
                            break;
                    }
                } break;
                case 'd': { // d
                    material->alpha = std::stof(token.value);
                } break;
                case 'm': {
                    if (token.key.size() < 5 || token.key.substr(0, 4) != "map_")
                        break;
                    auto lastSlash = path.find_last_of('/');
                    std::string texPath = path.substr(0, lastSlash + 1);
                    switch (token.key[4]) {
                        case 'K': {
                            if (token.key == "map_Kd") {
                                texPath += token.value;
                                material->texAlbedo = texPath;
                            }
                            else if (token.key == "map_Ks") {
                                texPath += token.value;
                                material->texSpecular = texPath;
                            }
                        } break;
                        case 'B': // map_Bump
                        case 'b': { // map_bump
                            texPath += token.value;
                            material->texNormal = texPath;
                        } break;
                        default:
                            break;
                    }
                } break;
                default:
                    break;
            }
        }
    }

    void Model::loadModelFrom(const std::string & path) {
        std::ifstream is(path);
        if (!is.is_open()) {
            throw ModelLoadException("Failed to open file " + path);
        }
        Parser parser(is);

        std::vector<glm::vec3> av;
        std::vector<glm::vec2> avt;
        std::vector<glm::vec3> avn;

        Mesh * mesh = nullptr;

        for (auto & token : parser) {
            switch (token.key[0]) {
                case 'o': {
                    mesh = new Mesh();
                    mesh->name = token.value;
                    objects.push_back(mesh);
                } break;
                case 'v': {
                    if (token.key.size() == 1) { // v
                        auto & v = av.emplace_back();
                        auto params = token.params();
                        if (params.size() != v.length())
                            throw ModelLoadException(
                                "Vertex (v) must have 3 values");
                        for (int i = 0; i < v.length(); i++) //
                            v[i] = std::stof(params[i]);
                    }
                    else { // token.key.size() > 1
                        switch (token.key[1]) {
                            case 't': { // vt
                                auto & vt = avt.emplace_back();
                                auto params = token.params();
                                if (params.size() != vt.length())
                                    throw ModelLoadException(
                                        "TexCoord (vt) must have 2 values");
                                for (int i = 0; i < vt.length(); i++)
                                    vt[i] = std::stof(params[i]);
                            } break;
                            case 'n': { // vn
                                auto & vn = avn.emplace_back();
                                auto params = token.params();
                                if (params.size() != vn.length())
                                    throw ModelLoadException(
                                        "Normal (vn) must have 3 values");
                                for (int i = 0; i < vn.length(); i++)
                                    vn[i] = std::stof(params[i]);
                            } break;
                            default:
                                break;
                        }
                    }
                } break;
                case 'f': {
                    if (!mesh)
                        throw ModelLoadException(
                            "Got a face (f) before starign an object (o)");
                    auto params = token.params();
                    if (params.size() != 3)
                        throw ModelLoadException("Face (f) must have 3 components");
                    for (int i = 0; i < 3; i++) {
                        auto subParams = splitString(params[i], '/');
                        if (subParams.size() != 3)
                            throw ModelLoadException(
                                "Face (c) component must have 3 values");
                        size_t iv = std::stoul(subParams[0]);
                        iv -= 1; // index at 1
                        if (iv >= av.size())
                            throw ModelLoadException("Vertex out of bounds");
                        mesh->vertices.push_back(av[iv]);

                        size_t it = std::stoul(subParams[1]);
                        it -= 1; // index at 1
                        if (it >= avt.size())
                            throw ModelLoadException("TexCoord out of bounds");
                        mesh->texcoords.push_back(avt[it]);

                        size_t in = std::stoul(subParams[2]);
                        in -= 1; // index at 1
                        if (in >= avn.size())
                            throw ModelLoadException("Normal out of bounds");
                        mesh->normals.push_back(avn[in]);
                    }
                } break;
                case 'm': { // mtllib
                    if (token.key != "mtllib")
                        break;
                    auto lastSlash = path.find_last_of('/');
                    std::string mtlPath = path.substr(0, lastSlash + 1);
                    mtlPath += token.value;
                    loadMaterialsFrom(mtlPath);
                } break;
                case 'u': { // usemtl
                    if (token.key != "usemtl")
                        break;
                    if (!mesh)
                        throw ModelLoadException(
                            "Got a (usemtl) before starign an object (o)");
                    bool found = false;
                    for (int i = 0; i < materials.size(); i++) {
                        if (materials[i]->name == token.value) {
                            mesh->matId = i;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        throw ModelLoadException("Unknown material name "
                                                 + token.value);
                    }
                } break;
                default:
                    break;
            }
        }
    }
}
