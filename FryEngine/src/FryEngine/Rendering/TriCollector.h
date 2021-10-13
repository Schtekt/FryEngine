#pragma once
#include <vector>
#include "Mesh.h"
class Mesh;
class Camera;
class RenderTarget;

struct Triangle
{
    Triangle(){};
    Triangle(Vertex v1, Vertex v2, Vertex v3)
    {
        p1 = v1;
        p2 = v2;
        p3 = v3;
    }
    Vertex p1;
    Vertex p2;
    Vertex p3;
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
};