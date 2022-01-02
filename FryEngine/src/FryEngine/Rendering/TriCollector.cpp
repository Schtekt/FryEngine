#include "TriCollector.h"
#include <algorithm>
#include "RenderTarget.h"
#include "Camera.h"
#include <iostream>

TriCollector::TriCollector()
{

}

void TriCollector::SubmitMesh(const Mesh& mesh, const Camera* cam)
{
    const std::vector<Vertex>& verticesToSubmit = mesh.GetVertices();
    const std::vector<unsigned int>& vertexIndicesToSubmit = mesh.GetVertexIndices();
    const std::vector<UVCoord>& uvCoordinatesToSubmit = mesh.GetUVCoordinates();
    const std::vector<unsigned int>& uvCoordinatesIndicesToSubmit = mesh.GetUVIndices();
    const Matrix<4,4>& modelMatrix = mesh.GetModelMatrix();

    size_t currSize = m_tris.size();
    m_tris.reserve(currSize + vertexIndicesToSubmit.size() / 3);
    for(size_t i = 0; i < vertexIndicesToSubmit.size(); i += 3)
    {
        Triangle& tri = m_tris.emplace_back(verticesToSubmit[vertexIndicesToSubmit[i]], uvCoordinatesToSubmit[uvCoordinatesIndicesToSubmit[i]],
        verticesToSubmit[vertexIndicesToSubmit[i + 1]], uvCoordinatesToSubmit[uvCoordinatesIndicesToSubmit[i + 1]],
        verticesToSubmit[vertexIndicesToSubmit[i + 2]], uvCoordinatesToSubmit[uvCoordinatesIndicesToSubmit[i + 2]]);

        // Worldspace
        tri.p1.coords = modelMatrix * tri.p1.coords;
        tri.p2.coords = modelMatrix * tri.p2.coords;
        tri.p3.coords = modelMatrix * tri.p3.coords;

        // Viewspace
        tri.p1.coords = cam->GetViewMat() * tri.p1.coords;
        tri.p2.coords = cam->GetViewMat() * tri.p2.coords;
        tri.p3.coords = cam->GetViewMat() * tri.p3.coords;

        // ClipSpace
        tri.p1.coords = cam->GetProjectionMatrix() * tri.p1.coords;
        tri.p2.coords = cam->GetProjectionMatrix() * tri.p2.coords;
        tri.p3.coords = cam->GetProjectionMatrix() * tri.p3.coords;
    }
    m_entries.emplace_back(mesh.GetDiffuseTexture(), mesh.GetDiffuseTextureWidth(), mesh.GetDiffuseTextureHeight(), m_tris.size() - 1);
}

void TriCollector::SortTris()
{
    std::sort(m_tris.begin(), m_tris.end(), [](Triangle& tri1, Triangle& tri2){
        double depthFirst = tri1.p1.coords[2] + tri1.p2.coords[2] + tri1.p3.coords[2];
        double depthSec = tri2.p1.coords[2] + tri2.p2.coords[2] + tri2.p3.coords[2];
        return depthFirst > depthSec;
    });
}

void TriCollector::Draw(RenderTarget& target)
{
    Vertex triOutput[3];
    unsigned int entryCount = 0;
    for(unsigned int i = 0; i < m_tris.size(); ++i)
    {
        if(i > m_entries[entryCount].endVertex)
        {
            entryCount++;
        }

        // Projection
        triOutput[0].coords = m_tris[i].p1.coords;
        triOutput[1].coords = m_tris[i].p2.coords;
        triOutput[2].coords = m_tris[i].p3.coords;

        // move to the a "rendertarget" to the right and top.
        triOutput[0].coords[0] = triOutput[0].coords[0] + 1;
        triOutput[0].coords[1] = triOutput[0].coords[1] + 1;
        triOutput[1].coords[0] = triOutput[1].coords[0] + 1;
        triOutput[1].coords[1] = triOutput[1].coords[1] + 1;
        triOutput[2].coords[0] = triOutput[2].coords[0] + 1;
        triOutput[2].coords[1] = triOutput[2].coords[1] + 1;

        // Scale triangle to match the size of the rendertarget.
        triOutput[0].coords[0] = triOutput[0].coords[0] * 0.5 * target.GetWidth();
        triOutput[0].coords[1] = triOutput[0].coords[1] * 0.5 * target.GetHeight();
        triOutput[1].coords[0] = triOutput[1].coords[0] * 0.5 * target.GetWidth();
        triOutput[1].coords[1] = triOutput[1].coords[1] * 0.5 * target.GetHeight();
        triOutput[2].coords[0] = triOutput[2].coords[0] * 0.5 * target.GetWidth();
        triOutput[2].coords[1] = triOutput[2].coords[1] * 0.5 * target.GetHeight();
        target.FillTexturedTri
        (
            (int)triOutput[0].coords.nums[0], (int)triOutput[0].coords.nums[1], m_tris[i].uv1.u, m_tris[i].uv1.v,
            (int)triOutput[1].coords.nums[0], (int)triOutput[1].coords.nums[1], m_tris[i].uv2.u, m_tris[i].uv2.v,
            (int)triOutput[2].coords.nums[0], (int)triOutput[2].coords.nums[1], m_tris[i].uv3.u, m_tris[i].uv3.v,
            m_entries[entryCount].image, m_entries[entryCount].texWidth, m_entries[entryCount].texHeight
        );
        target.DrawTri
        (
            (int)triOutput[0].coords.nums[0], (int)triOutput[0].coords.nums[1],
            (int)triOutput[1].coords.nums[0], (int)triOutput[1].coords.nums[1],
            (int)triOutput[2].coords.nums[0], (int)triOutput[2].coords.nums[1],
            (255 << 16)
        );
    }
}

void TriCollector::Clear()
{
    m_tris.clear();
    m_entries.clear();
}