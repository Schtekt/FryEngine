#pragma once
#include "../Math/Matrix.h"
#include <vector>
#include <string>

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
    Vector<4> coords{0,0,0,1};
};

struct UVCoord
{
    float u;
    float v;
};

struct Normal
{
    Vector<4> coords{0,0,0,0};
};

class Mesh
{
    public:
        Mesh();
        Mesh(const std::vector<Vertex>& m_vertices, const std::vector<unsigned int>& m_vertexIndices);
        void Draw(const Matrix<4,4>& View, const Matrix<4,4>& perspective, RenderTarget& target, E_DrawFlags flag = E_DrawFlags::FILL) const;
        void SetVertices(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& vertexIndices);
        void SetModelMatrix(const Matrix<4,4>& mat);
        void ReadFromObj(const std::string& path);
        void ReadFromMTL(const std::string& path);

        const std::vector<Vertex>& GetVertices() const;
        const std::vector<unsigned int>& GetVertexIndices() const;
        const std::vector<UVCoord>& GetUVCoordinates() const;
        const std::vector<unsigned int>& GetUVIndices() const;
        const Matrix<4,4>& GetModelMatrix() const;
        inline const std::string& GetdiffusePath() const {return m_materialDiffuseTexture;};
    private:
        bool processTri(Vertex triOutput[3], const unsigned int triIndices[3], 
                        const Matrix<4,4>& viewMat, const Matrix<4,4> perspectiveMat) const;
        void moveAndScaleTriForTarget(Vertex triOuput[3], const Vertex triInput[3], const RenderTarget& target) const;
    private:
        std::vector<Vertex> m_vertices;
        std::vector<UVCoord> m_uvCoords;
        std::vector<Normal> m_normals;
        std::vector<unsigned int> m_vertexIndices;
        std::vector<unsigned int> m_uvIndices;
        std::vector<unsigned int> m_normalIndices;
        Matrix<4,4> m_ModelMat = Matrix<4,4>::IdentityMatrix();
        float m_materialSpecularHighlights;
        Vector<3> m_materialAmbientColour;
        Vector<3> m_materialDiffuseColour;
        Vector<3> m_materialSpecularColour;
        Vector<3> m_materialEmissiveColour;
        float m_materialOpticalDensity;
        float m_materialDissolve;
        int m_materialIlluminationModel;
        std::string m_materialAmbientTexture;
        std::string m_materialDiffuseTexture;
        std::string m_materialSpecularTexture;
        std::string m_materialEmissiveTexture;
};