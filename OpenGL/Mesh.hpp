#pragma once

#include <vector>
#include "../renderLib/Vertex.hpp"
#include "../renderLib/Triangle.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstddef>

namespace ES{

    class Mesh{
        GLuint VBO_id, VAO_id, EBO_id;
    public:
        std::size_t indexCount;

        Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned int>& inds){

            glGenVertexArrays(1, &VAO_id);

            glGenBuffers(1, &VBO_id);
            glGenBuffers(1, &EBO_id);

            glBindVertexArray(VAO_id);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_id);
            glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);
    
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_id);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds.size() * sizeof(unsigned int), inds.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, sizeof(Vertex),(void*)offsetof(Vertex,normal));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,uv));

            glBindVertexArray(0);

            indexCount = inds.size();
        }

        ~Mesh() {
            glDeleteVertexArrays(1, &VAO_id);
            glDeleteBuffers(1, &VBO_id);
            glDeleteBuffers(1, &EBO_id);
        }

        void draw() const {
            glBindVertexArray(VAO_id);
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        }


        Mesh(const Mesh&) = delete;     
        Mesh& operator=(const Mesh&) = delete;

        Mesh(Mesh&& other) noexcept: VBO_id(other.VBO_id), VAO_id(other.VAO_id), EBO_id(other.EBO_id), indexCount(other.indexCount){
            other.VBO_id = 0;
            other.VAO_id = 0;
            other.EBO_id = 0;
            other.indexCount = 0;
        }

        Mesh& operator=(Mesh&& other) noexcept {
            if (this != &other) {
                if (VAO_id) glDeleteVertexArrays(1, &VAO_id);
                if (VBO_id) glDeleteBuffers(1, &VBO_id);
                if (EBO_id) glDeleteBuffers(1, &EBO_id);

                VBO_id = other.VBO_id;
                VAO_id = other.VAO_id;
                EBO_id = other.EBO_id;
                indexCount = other.indexCount;

                other.VBO_id = 0;
                other.VAO_id = 0;
                other.EBO_id = 0;
                other.indexCount = 0;
            }
            return *this;
        }

    };


}