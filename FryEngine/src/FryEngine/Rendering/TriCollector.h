#pragma once
#include <vector>
#include "Mesh.h"
#include "../ResourceManagement/ImageLoader.h"
class Mesh;
class Camera;
class RenderTarget;

struct Triangle
{
    Triangle(){};
    Triangle(Vertex v1, UVCoord uv1, Vertex v2, UVCoord uv2, Vertex v3, UVCoord uv3)
    {
        p1 = v1;
        p2 = v2;
        p3 = v3;
        this->uv1 = uv1;
        this->uv2 = uv2;
        this->uv3 = uv3;
    }
    Vertex p1;
    Vertex p2;
    Vertex p3;
    UVCoord uv1;
    UVCoord uv2;
    UVCoord uv3;
};

struct MeshEntry
{
    inline MeshEntry(unsigned char* path, int width, int height, unsigned int lastVertex): image(path), texWidth(width), texHeight(height), endVertex(lastVertex){}
    unsigned char* const image;
    int texWidth;
    int texHeight;
    unsigned int endVertex;
};

class TriCollector
{
    public:
    TriCollector();
    void SubmitMesh(const Mesh& mesh, const Camera* cam);
    void SortTris();
    void Draw(RenderTarget& target);
    void Clear();

    private:
    std::vector<Triangle> m_tris;
    std::vector<MeshEntry> m_entries;
};