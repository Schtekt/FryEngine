#include "RenderSystem.h"
#include <stdexcept>

RenderSystem::RenderSystem(Camera& pCamera) : m_pCamera(pCamera)
{
	addComponentType<Mesh>();
}

void RenderSystem::UpdateComponents(const long long delta, std::vector<void*>& components)
{
    Mesh* mesh = (Mesh*)components[0];
    m_triCollector.SubmitMesh(*mesh, &m_pCamera);
}

void RenderSystem::Draw(RenderTarget& target)
{
    m_triCollector.SortTris();
    m_triCollector.Draw(target);
    m_triCollector.Clear();
}
