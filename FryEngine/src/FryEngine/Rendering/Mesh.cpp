#include "Mesh.h"
#include "RenderTarget.h"
#include <iostream>

Mesh::Mesh(){}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& vertexIndices) 
: m_vertices(vertices), m_vertexIndices(vertexIndices)
{
}

void Mesh::SetVertices(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& vertexIndices)
{
    m_vertices = vertices;
    m_vertexIndices = vertexIndices;
}

void Mesh::SetModelMatrix(const Matrix<4, 4>& mat)
{
    m_ModelMat = mat;
}

void Mesh::Draw(const Matrix<4,4>& ViewMat, const Matrix<4,4>& PerspectiveMat, RenderTarget& target) const
{
    Vertex tri[3];
    for(size_t i = 0; i < m_vertexIndices.size(); i+=3)
    {
        tri[0] = m_vertices[m_vertexIndices[i]];
        tri[1] = m_vertices[m_vertexIndices[i + 1]];
        tri[2] = m_vertices[m_vertexIndices[i + 2]];

        // Worldspace
        tri[0].coords = m_ModelMat * tri[0].coords;
        tri[1].coords = m_ModelMat * tri[1].coords;
        tri[2].coords = m_ModelMat * tri[2].coords;

        // Viewspace
        tri[0].coords = ViewMat * tri[0].coords;
        tri[1].coords = ViewMat * tri[1].coords;
        tri[2].coords = ViewMat * tri[2].coords;

        // Projection
        tri[0].coords = PerspectiveMat * tri[0].coords;
        tri[1].coords = PerspectiveMat * tri[1].coords;
        tri[2].coords = PerspectiveMat * tri[2].coords;

        // Scale into view
        tri[0].coords[0] += 1;
        tri[0].coords[1] += 1;
        tri[1].coords[0] += 1;
        tri[1].coords[1] += 1;
        tri[2].coords[0] += 1;
        tri[2].coords[1] += 1;
        tri[0].coords[0] *= 0.5 * target.GetWidth();
        tri[0].coords[1] *= 0.5 * target.GetHeight();
        tri[1].coords[0] *= 0.5 * target.GetWidth();
        tri[1].coords[1] *= 0.5 * target.GetHeight();
        tri[2].coords[0] *= 0.5 * target.GetWidth();
        tri[2].coords[1] *= 0.5 * target.GetHeight();


        //target.FillTri
        //(
        //    (int)tri[0].coords.nums[0], (int)tri[0].coords.nums[1], 
        //    (int)tri[1].coords.nums[0], (int)tri[1].coords.nums[1], 
        //    (int)tri[2].coords.nums[0], (int)tri[2].coords.nums[1], 
        //    (255 << 16) | (255 << 8) | 255
        //);
        target.DrawTri
        (
            (int)tri[0].coords.nums[0], (int)tri[0].coords.nums[1],
            (int)tri[1].coords.nums[0], (int)tri[1].coords.nums[1],
            (int)tri[2].coords.nums[0], (int)tri[2].coords.nums[1],
            (255 << 16)
        );
    }
}