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
            tinyobj::ObjReaderConfig reader_config;
            reader_config.mtl_search_path = "./";
            reader_config.triangulate = true;
            tinyobj::ObjReader reader;


            if (!reader.ParseFromFile(path, reader_config)) {
                if (!reader.Error().empty()) {
                    std::cerr << "TinyOBJReader Error: " << reader.Error() << "\n";
                }
                return {};
            }

            auto& attrib = reader.GetAttrib();
            auto& shapes = reader.GetShapes();
            auto& materials = reader.GetMaterials();

            OBJModel outModel;

            for (const auto& shape : shapes) {
                OBJObject objPart;
                
                std::unordered_map<size_t, unsigned int> uniqueVertices;

                for (const auto& index : shape.mesh.indices) {
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
                    }

                    if (index.texcoord_index >= 0) {
                        vertex.uv = {
                            attrib.texcoords[2 * index.texcoord_index + 0],
                            attrib.texcoords[2 * index.texcoord_index + 1]
                        };
                    }

                    size_t hash = index.vertex_index;
                    hash ^= std::hash<int>{}(index.normal_index) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
                    hash ^= std::hash<int>{}(index.texcoord_index) + 0x9e3779b9 + (hash << 6) + (hash >> 2);

                    if (uniqueVertices.find(hash) == uniqueVertices.end()) {
                        uniqueVertices[hash] = static_cast<unsigned int>(objPart.vertices.size());
                        objPart.vertices.push_back(vertex);
                    }
                    
                    objPart.indices.push_back(uniqueVertices[hash]);
                }

                if (!shape.mesh.material_ids.empty() && shape.mesh.material_ids[0] >= 0) {
                    auto& mtl = materials[shape.mesh.material_ids[0]];
                    
                    objPart.material_data = {
                        mtl.diffuse[0], mtl.diffuse[1], mtl.diffuse[2],   // 0, 1, 2: Diffuse Color
                        mtl.specular[0], mtl.specular[1], mtl.specular[2],// 3, 4, 5: Specular Color
                        mtl.shininess,                                    // 6: Shininess
                        mtl.dissolve                                      // 7: Alpha
                    };
                    objPart.tex_path = mtl.diffuse_texname;
                } 
                else {
                    // FALLBACK: If no material info exists, provide a default color (e.g., Light Grey)
                    // This ensures material_data is NEVER empty.
                    objPart.material_data = {
                        0.7f, 0.7f, 0.7f, // Default Diffuse (Kd)
                        0.1f, 0.1f, 0.1f, // Default Specular
                        32.0f,            // Default Shininess
                        1.0f              // Default Alpha
                    };
                    objPart.tex_path = ""; 
                }
                                
                outModel.model.push_back(objPart);
            }

            return outModel;
        }
    };

}


