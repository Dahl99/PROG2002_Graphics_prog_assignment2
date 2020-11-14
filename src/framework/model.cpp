#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "model.hpp"
#include <iostream>
#include <unordered_map>
#include <tiny_obj_loader.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// Used for caching vertices when loading model
namespace std {
    template<> struct hash<framework::Vertex> {
        size_t operator()(framework::Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ 
                (hash<glm::vec3>()(vertex.nor) << 1)) >> 1) ^ 
                (hash<glm::vec2>()(vertex.tex) << 1);
        }
    };
}

namespace framework
{
	Model::Model(const std::string& filepath)
	{
        // Variables used for lodaing model
        tinyobj::attrib_t attrib{};
        std::vector<tinyobj::shape_t> shapes{};
        std::vector<tinyobj::material_t> mats{};
        std::string warning{}, error{};

        // Used for caching of vertices when loading model to save memory
        std::unordered_map<Vertex, uint32_t> vertex_cache{};

        // Loading model using tinyobjloader
        if (!tinyobj::LoadObj(&attrib, &shapes, &mats, &error, filepath.c_str()))
            std::cout << "WARNING! Couldn't load model!\n";


        for (const auto& shape : shapes)
        {
            for (const auto& index : shape.mesh.indices)
            {
                Vertex vertex{};
                vertex.pos = { attrib.vertices[3 * index.vertex_index + 0],
                                   attrib.vertices[3 * index.vertex_index + 1],
                                   attrib.vertices[3 * index.vertex_index + 2] };

                vertex.nor = { attrib.normals[static_cast<__int64>(3) * index.normal_index + 0],
                                  attrib.normals[3 * index.normal_index + 1],
                                  attrib.normals[3 * index.normal_index + 2] };

                vertex.tex = { attrib.texcoords[static_cast<__int64>(2) * index.texcoord_index + 0],
                                   attrib.texcoords[2 * index.texcoord_index + 1] };

                if (vertex_cache.find(vertex) == vertex_cache.end()) // Checking if vertex isn't in cache
                {
                    vertex_cache[vertex] = m_Vertices.size();
                    m_Vertices.push_back(vertex);
                }

                m_Indices.push_back(vertex_cache[vertex]);
            }
        }
	}
}