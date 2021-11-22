#include "Mesh.h"
#include "RenderTarget.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
            if(processTri(triTransformed, &m_vertexIndices[i], ViewMat, PerspectiveMat))
            {
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
        }
        break;
    case E_DrawFlags::WIREFRAME:
        for (size_t i = 0; i < m_vertexIndices.size(); i += 3)
        {
            if(processTri(triTransformed, &m_vertexIndices[i], ViewMat, PerspectiveMat))
            {
                moveAndScaleTriForTarget(triTransformed, triTransformed, target);
                target.DrawTri
                (
                    (int)triTransformed[0].coords.nums[0], (int)triTransformed[0].coords.nums[1],
                    (int)triTransformed[1].coords.nums[0], (int)triTransformed[1].coords.nums[1],
                    (int)triTransformed[2].coords.nums[0], (int)triTransformed[2].coords.nums[1],
                    (255 << 16)
                );
            }
        }
        break;
    case E_DrawFlags::FILL:
        for (size_t i = 0; i < m_vertexIndices.size(); i += 3)
        {
            if(processTri(triTransformed, &m_vertexIndices[i], ViewMat, PerspectiveMat))
            {
                moveAndScaleTriForTarget(triTransformed, triTransformed, target);
                target.FillTri
                (
                    (int)triTransformed[0].coords.nums[0], (int)triTransformed[0].coords.nums[1],
                    (int)triTransformed[1].coords.nums[0], (int)triTransformed[1].coords.nums[1],
                    (int)triTransformed[2].coords.nums[0], (int)triTransformed[2].coords.nums[1],
                    (255 << 16 | 255 << 8 | 255)
                );
            }
        }
        break;
    }
}

const std::vector<Vertex>& Mesh::GetVertices() const
{
    return m_vertices;
}

const std::vector<unsigned int>& Mesh::GetVertexIndices() const
{
    return m_vertexIndices;
}

const Matrix<4,4>& Mesh::GetModelMatrix() const
{
    return m_ModelMat;
}


bool Mesh::processTri(Vertex triOutput[3], const unsigned int triIndices[3], 
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

        //backface culling
        Vector<4> normal = (triOutput[1].coords - triOutput[0].coords).Cross(triOutput[2].coords - triOutput[0].coords);
        if(normal.nums[2] < 0.000001)
        {
            return false;
        }

        // Projection
        triOutput[0].coords = perspectiveMat * triOutput[0].coords;
        triOutput[1].coords = perspectiveMat * triOutput[1].coords;
        triOutput[2].coords = perspectiveMat * triOutput[2].coords;
        return true;
}

void Mesh::moveAndScaleTriForTarget(Vertex triOutput[3], const Vertex triInput[3], const RenderTarget& target) const
{
        // move to the a "rendertarget" to the right and top.
        triOutput[0].coords[0] = triInput[0].coords[0] + 1;
        triOutput[0].coords[1] = triInput[0].coords[1] + 1;
        triOutput[1].coords[0] = triInput[1].coords[0] + 1;
        triOutput[1].coords[1] = triInput[1].coords[1] + 1;
        triOutput[2].coords[0] = triInput[2].coords[0] + 1;
        triOutput[2].coords[1] = triInput[2].coords[1] + 1;

        // Scale triangle to match the size of the rendertarget.
        triOutput[0].coords[0] = triOutput[0].coords[0] * 0.5 * target.GetWidth();
        triOutput[0].coords[1] = triOutput[0].coords[1] * 0.5 * target.GetHeight();
        triOutput[1].coords[0] = triOutput[1].coords[0] * 0.5 * target.GetWidth();
        triOutput[1].coords[1] = triOutput[1].coords[1] * 0.5 * target.GetHeight();
        triOutput[2].coords[0] = triOutput[2].coords[0] * 0.5 * target.GetWidth();
        triOutput[2].coords[1] = triOutput[2].coords[1] * 0.5 * target.GetHeight();
}

void Mesh::ReadFromObj(const std::string& path)
{
    std::ifstream myfile(path);
    std::string line;
    char command;
    char commandTwo;
    float x;
    float y;
    float z;
    float u;
    float v;
    bool isFileFormatCorrect = true;
    unsigned int i;
    std::vector<Vertex> tmpVertices;
    std::vector<UVCoord> tmpUvCoords;
    std::vector<Normal> tmpNormals;
    std::vector<unsigned int> tmpVertexIndices;
    std::vector<unsigned int> tmpUvIndices;
    std::vector<unsigned int> tmpNormalIndices;
    if (myfile.is_open())
    {
        while (std::getline(myfile, line))
        {
            std::stringstream ss(line);
            ss >> command;
            if (command == 'v')
            {
                ss >> commandTwo;
                if (commandTwo == 't')
                {
                    ss >> u >> v;
                    tmpUvCoords.push_back({ u,v });
                }
                else if (commandTwo == 'n')
                {
                    ss >> x >> y >> z;
                    tmpNormals.push_back({ x,y,z });
                }
                else 
                { 
                    ss.unget();
                    ss >> x >> y >> z;
                    tmpVertices.push_back({ x,y,z });
                }
            }
            else if (command == 'f')
            {
                for (int index = 0; index < 3; index++)
                {
                    ss >> i;
                    tmpVertexIndices.push_back(i - 1);
                    ss >> commandTwo;

                    if (commandTwo == '/')
                    {
                        size_t streamPos;
                        streamPos = ss.tellg();
                        ss >> i;

                        if (ss.rdstate() != std::ios_base::badbit && i != 0)
                        {
                            tmpUvIndices.push_back(i - 1);
                        }
                        else
                        {
                            ss.seekg(streamPos);
                            ss.clear();
                        }

                        // Resetting commandTwo because otherwise it'll still remain as '/' even if nothing is read from ss.
                        commandTwo = 0;
                        ss >> commandTwo;

                        if (commandTwo == '/')
                        {
                            ss >> i;
                            tmpNormalIndices.push_back(i - 1);
                        }
                        else
                        {
                            ss.unget();
                            ss.clear();
                        }
                    }
                    else
                    {
                        ss.unget();
                        ss.clear();
                    }
                }
            }
            else if (command == '#' || command == 0)
            {
                // If '#' or '/0' is encountered the file continues to run.
            }
            else 
            {
                isFileFormatCorrect = false;
                break;
            }
            if (ss.rdstate() == std::ios_base::badbit || ss.rdstate() == std::ios_base::failbit)
            {
                isFileFormatCorrect = false;
                break;
            }
            command = 0;
        }
        if (isFileFormatCorrect)
        {
            m_vertices = tmpVertices;
            m_uvCoords = tmpUvCoords;
            m_normals = tmpNormals;
            m_vertexIndices = tmpVertexIndices;
            m_uvIndices = tmpUvIndices;
            m_normalIndices = tmpNormalIndices;
        }
        else
        {
            std::cout << "File could not be read" << std::endl;
        }
        myfile.close();
    }
    else
    {
        std::cout << "File did not open" << std::endl;
    }
}