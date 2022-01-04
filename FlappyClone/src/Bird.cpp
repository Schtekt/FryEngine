#include "Bird.h"
#include "FryEngine/Rendering/Mesh.h"
#include <algorithm>

Matrix<4, 4> rotBase
{
	cos(-3.14159265358979323846 / 2), -sin(-3.14159265358979323846 / 2), 0, 0,
	sin(-3.14159265358979323846 / 2), cos(-3.14159265358979323846 / 2), 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

BirdSystem::BirdSystem()
{
	addComponentType<Bird>();
	addComponentType<Mesh>();
}

void BirdSystem::UpdateComponents(const long long delta, std::vector<void*>& components)
{
	// Tilt the bird with regards to its vertical velocity.
	Bird* birdStats = (Bird*)components[0];
	Mesh* birdMesh = (Mesh*)components[1];

	double timeInSeconds = (double)delta / 1000;

	// TODO: Place this in a separate function, maybe define in Bird struct?
	birdStats->heightPosition += timeInSeconds * timeInSeconds * birdStats->heightAccelration / 2 + timeInSeconds * birdStats->heightVelocity;
	birdStats->heightVelocity += timeInSeconds * birdStats->heightAccelration;

	float rotVal = std::min(3.14159265358979323846f / 2, std::max(-3.14159265358979323846f / 2, birdStats->heightVelocity / 10));
	birdStats->rotation += abs(birdStats->heightVelocity/20);
	Matrix<4, 4> rotX
	{
		1, 0, 0, 0,
		0, cos(birdStats->rotation), sin(birdStats->rotation), 0,
		0, -sin(birdStats->rotation), cos(birdStats->rotation), 0,
		0, 0, 0, 1
	};

	Matrix<4, 4> rotZ
	{
		cos(rotVal), -sin(rotVal), 0, 0,
		sin(rotVal), cos(rotVal), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	Matrix<4, 4> transMat
	{
		0.4, 0, 0, -0.5,
		0, 0.4, 0, birdStats->heightPosition,
		0, 0, 0.4, 0,
		0, 0, 0, 0.4
	};
	birdMesh->SetModelMatrix(transMat * rotZ * rotX * rotBase);
}