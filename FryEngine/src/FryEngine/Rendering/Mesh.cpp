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

void Mesh::Draw(const Matrix<4,4>& ViewMat, const Matrix<4,4>& PerspectiveMat, RenderTarget& target, E_DrawFlags flags) const
{
    Vertex triTransformed[3];

    switch(flags)
    {
    case ((E_DrawFlags::WIREFRAME | E_DrawFlags::FILL)):
        for(size_t i = 0; i < m_vertexIndices.size(); i+=3)
        {
            processTris(triTransformed, &m_vertexIndices[i], ViewMat, PerspectiveMat);
            moveAndScaleTriForTarget(triTransformed, triTransformed, target);
            target.FillTri
            (
                (int)triTransformed[0].coords.nums[0], (int)triTransformed[0].coords.nums[1], 
                (int)triTransformed[1].coords.nums[0], (int)triTransformed[1].coords.nums[1], 
                (int)triTransformed[2].coords.nums[0], (int)triTransformed[2].coords.nums[1], 
                (255 << 16) | (255 << 8) | 255
            );
            target.DrawTri
            (
                (int)triTransformed[0].coords.nums[0], (int)triTransformed[0].coords.nums[1],
                (int)triTransformed[1].coords.nums[0], (int)triTransformed[1].coords.nums[1],
                (int)triTransformed[2].coords.nums[0], (int)triTransformed[2].coords.nums[1],
                (255 << 16)
            );
        }
        break;
    case E_DrawFlags::WIREFRAME:
        for (size_t i = 0; i < m_vertexIndices.size(); i += 3)
        {
            processTris(triTransformed, &m_vertexIndices[i], ViewMat, PerspectiveMat);
            moveAndScaleTriForTarget(triTransformed, triTransformed, target);
            target.DrawTri
            (
                (int)triTransformed[0].coords.nums[0], (int)triTransformed[0].coords.nums[1],
                (int)triTransformed[1].coords.nums[0], (int)triTransformed[1].coords.nums[1],
                (int)triTransformed[2].coords.nums[0], (int)triTransformed[2].coords.nums[1],
                (255 << 16)
            );
        }
        break;
    case E_DrawFlags::FILL:
        for (size_t i = 0; i < m_vertexIndices.size(); i += 3)
        {
            processTris(triTransformed, &m_vertexIndices[i], ViewMat, PerspectiveMat);
            moveAndScaleTriForTarget(triTransformed, triTransformed, target);
            target.FillTri
            (
                (int)triTransformed[0].coords.nums[0], (int)triTransformed[0].coords.nums[1],
                (int)triTransformed[1].coords.nums[0], (int)triTransformed[1].coords.nums[1],
                (int)triTransformed[2].coords.nums[0], (int)triTransformed[2].coords.nums[1],
                (255 << 16 | 255 << 8 | 255)
            );
        }
        break;
    }
}

void Mesh::processTris(Vertex triOutput[3], const unsigned int triIndices[3], 
                       const Matrix<4,4>& viewMat, const Matrix<4,4> perspectiveMat) const
{
        triOutput[0] = m_vertices[triIndices[0]];
        triOutput[1] = m_vertices[triIndices[1]];
        triOutput[2] = m_vertices[triIndices[2]];

        // Worldspace
        triOutput[0].coords = m_ModelMat * triOutput[0].coords;
        triOutput[1].coords = m_ModelMat * triOutput[1].coords;
        triOutput[2].coords = m_ModelMat * triOutput[2].coords;

        // Viewspace
        triOutput[0].coords = viewMat * triOutput[0].coords;
        triOutput[1].coords = viewMat * triOutput[1].coords;
        triOutput[2].coords = viewMat * triOutput[2].coords;

        // Projection
        triOutput[0].coords = perspectiveMat * triOutput[0].coords;
        triOutput[1].coords = perspectiveMat * triOutput[1].coords;
        triOutput[2].coords = perspectiveMat * triOutput[2].coords;
}

void Mesh::moveAndScaleTriForTarget(Vertex triOuput[3], const Vertex triInput[3], const RenderTarget& target) const
{
        // move to the a "rendertarget" to the right and top.
        triOuput[0].coords[0] = triInput[0].coords[0] + 1;
        triOuput[0].coords[1] = triInput[0].coords[1] + 1;
        triOuput[1].coords[0] = triInput[1].coords[0] + 1;
        triOuput[1].coords[1] = triInput[1].coords[1] + 1;
        triOuput[2].coords[0] = triInput[2].coords[0] + 1;
        triOuput[2].coords[1] = triInput[2].coords[1] + 1;

        // Scale triangle to match the size of the rendertarget.
        triOuput[0].coords[0] = triInput[0].coords[0] * 0.5 * target.GetWidth();
        triOuput[0].coords[1] = triInput[0].coords[1] * 0.5 * target.GetHeight();
        triOuput[1].coords[0] = triInput[1].coords[0] * 0.5 * target.GetWidth();
        triOuput[1].coords[1] = triInput[1].coords[1] * 0.5 * target.GetHeight();
        triOuput[2].coords[0] = triInput[2].coords[0] * 0.5 * target.GetWidth();
        triOuput[2].coords[1] = triInput[2].coords[1] * 0.5 * target.GetHeight();
}