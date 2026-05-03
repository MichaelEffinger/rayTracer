#pragma once

#include <tiny_obj_loader.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include "../renderLib/Vertex.hpp"

namespace ES{

    struct OBJObject {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<float> material_data;
        std::string tex_path;
    };

    struct OBJModel{
        std::vector<OBJObject> model;
    };

    class OBJLoader {
    public:
       static OBJModel load(const std::string& path) {
            std::string directory = "";
            size_t last_slash = path.find_last_of("/\\");
            if (last_slash != std::string::npos) {
                directory = path.substr(0, last_slash + 1);
            }

            tinyobj::ObjReaderConfig reader_config;
            reader_config.mtl_search_path = directory.empty() ? "./" : directory;
            reader_config.triangulate = true;
            tinyobj::ObjReader reader;

            if (!reader.ParseFromFile(path, reader_config)) {
                if (!reader.Error().empty()) {
                    std::cerr << "TinyOBJReader Error: " << reader.Error() << "\n";
                }
                return {};
            }

            auto& attrib    = reader.GetAttrib();
            auto& shapes    = reader.GetShapes();
            auto& materials = reader.GetMaterials();

            OBJModel outModel;

            for (const auto& shape : shapes) {

                std::unordered_map<int, OBJObject> materialGroups;
                std::unordered_map<int, std::unordered_map<size_t, unsigned int>> uniqueVerticesPerGroup;

                for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {

                    int mat_id = shape.mesh.material_ids[f];
                    if (mat_id < 0) mat_id = 0;

                    OBJObject& objPart = materialGroups[mat_id];
                    auto& uniqueVertices = uniqueVerticesPerGroup[mat_id];

                    for (size_t v = 0; v < 3; v++) {
                        tinyobj::index_t index = shape.mesh.indices[3 * f + v];

                        Vertex vertex{};

                        vertex.position = {
                            attrib.vertices[3 * index.vertex_index + 0],
                            attrib.vertices[3 * index.vertex_index + 1],
                            attrib.vertices[3 * index.vertex_index + 2]
                        };

                        if (index.normal_index >= 0) {
                            vertex.normal = {
                                attrib.normals[3 * index.normal_index + 0],
                                attrib.normals[3 * index.normal_index + 1],
                                attrib.normals[3 * index.normal_index + 2]
                            };
                        } else {
                            vertex.normal = {0.0f, 1.0f, 0.0f};
                        }

                        if (index.texcoord_index >= 0) {
                            vertex.uv = {
                                attrib.texcoords[2 * index.texcoord_index + 0],
                                attrib.texcoords[2 * index.texcoord_index + 1]
                            };
                        }

                        size_t hash = index.vertex_index;
                        hash ^= std::hash<int>{}(index.normal_index)    + 0x9e3779b9 + (hash << 6) + (hash >> 2);
                        hash ^= std::hash<int>{}(index.texcoord_index)  + 0x9e3779b9 + (hash << 6) + (hash >> 2);

                        if (uniqueVertices.find(hash) == uniqueVertices.end()) {
                            uniqueVertices[hash] = static_cast<unsigned int>(objPart.vertices.size());
                            objPart.vertices.push_back(vertex);
                        }

                        objPart.indices.push_back(uniqueVertices[hash]);
                    }
                }

                for (auto& [mat_id, objPart] : materialGroups) {
                    if (mat_id >= 0 && mat_id < static_cast<int>(materials.size())) {
                        auto& mtl = materials[mat_id];

                        float safe_shininess = (mtl.shininess < 1.0f) ? 32.0f : mtl.shininess;

                        float dR = mtl.diffuse[0];
                        float dG = mtl.diffuse[1];
                        float dB = mtl.diffuse[2];

                        if (dR <= 0.001f && dG <= 0.001f && dB <= 0.001f) {
                            if (!mtl.diffuse_texname.empty()) {
                                // If there is a texture, default to pure white so the texture isn't darkened
                                dR = 1.0f; dG = 1.0f; dB = 1.0f; 
                            } else {
                                // If no texture, default to a visible grey instead of pitch black
                                dR = 0.7f; dG = 0.7f; dB = 0.7f; 
                            }
                        }

                        objPart.material_data = {
                            dR, dG, dB,
                            mtl.specular[0], mtl.specular[1], mtl.specular[2],
                            safe_shininess,
                            mtl.dissolve
                        };

                        if (!mtl.diffuse_texname.empty()) {
                            objPart.tex_path = directory + mtl.diffuse_texname;
                        } else {
                            objPart.tex_path = "";
                        }
                    } else {
                        // No valid material found - use a safe grey default
                        objPart.material_data = {0.7f, 0.7f, 0.7f, 0.1f, 0.1f, 0.1f, 32.0f, 1.0f};
                        objPart.tex_path = "";
                        std::cerr << "Warning: no valid material for mat_id " << mat_id 
                                  << " in shape '" << shape.name << "' of '" << path << "'\n";
                    }

                    outModel.model.push_back(objPart);
                }
            }

            return outModel;
        }
    };
}