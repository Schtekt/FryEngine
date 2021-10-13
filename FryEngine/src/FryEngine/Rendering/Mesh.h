#pragma once
#include "../Math/Matrix.h"
#include <vector>

class RenderTarget;

enum class E_DrawFlags
{
    FILL = 0x01,
    WIREFRAME = 0x02
};

inline constexpr E_DrawFlags operator | (E_DrawFlags lhs, E_DrawFlags rhs)
{
    return (E_DrawFlags)(((int)lhs) | ((int)rhs));
}
    
inline E_DrawFlags& operator |= (E_DrawFlags& lhs, E_DrawFlags rhs)
{
    lhs = lhs | rhs;
    return lhs;
}

struct Vertex
{
    Vector<4> coords{0};
};

class Mesh
{
    public:
        Mesh();
        Mesh(const std::vector<Vertex>& m_vertices, const std::vector<unsigned int>& m_vertexIndices);
        void Draw(const Matrix<4,4>& View, const Matrix<4,4>& perspective, RenderTarget& target, E_DrawFlags flag = E_DrawFlags::FILL) const;
        void SetVertices(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& vertexIndices);
        void SetModelMatrix(const Matrix<4,4>& mat);

        const std::vector<Vertex>& GetVertices() const;
        const std::vector<unsigned int>& GetVertexIndices() const;
        const Matrix<4,4>& GetModelMatrix() const;
    private:
        bool processTri(Vertex triOutput[3], const unsigned int triIndices[3], 
                        const Matrix<4,4>& viewMat, const Matrix<4,4> perspectiveMat) const;
        void moveAndScaleTriForTarget(Vertex triOuput[3], const Vertex triInput[3], const RenderTarget& target) const;
    private:
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_vertexIndices;
        Matrix<4,4> m_ModelMat = Matrix<4,4>::IdentityMatrix();
};