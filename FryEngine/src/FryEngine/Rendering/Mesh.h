#pragma once
#include "../Math/Matrix.h"
#include <vector>

class RenderTarget;

struct Vertex
{
    Vector<4> coords{0};
};

class Mesh
{
    public:
        Mesh();
        Mesh(const std::vector<Vertex>& m_vertices, const std::vector<unsigned int>& m_vertexIndices);
        void Draw(const Matrix<4,4>& View, const Matrix<4,4>& perspective, RenderTarget& target) const;
        void SetVertices(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& vertexIndices);
        void SetModelMatrix(const Matrix<4,4>& mat);
    private:
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_vertexIndices;
        Matrix<4,4> m_ModelMat = Matrix<4,4>::IdentityMatrix();
};