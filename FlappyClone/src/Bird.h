#pragma once
#include "FryEngine/ECS/System.h"

struct Bird
{
	float heightPosition = 0.0f;
	float heightVelocity = 0.0f;
	float heightAccelration = 0.0f;
	float rotation = 0.0f;
};

class BirdSystem : public BaseSystem
{
public:
	BirdSystem();
	void UpdateComponents(const long long delta, std::vector<void*>& components);
};