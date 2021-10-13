#pragma once
#include "System.h"
#include "../Rendering/TriCollector.h"

class RenderSystem : public BaseSystem
{
public:
	RenderSystem(Camera& pCamera);
	void UpdateComponents(const long long delta, std::vector<void*>& components);
	void Draw(RenderTarget& target);
private:
	TriCollector m_triCollector;
	Camera& m_pCamera;
};