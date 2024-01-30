#pragma once
#include "Pathfinding.h"

class AEstrella : public Pathfinding
{
private:

public:
	AEstrella(Grid* grid);

	void FindPath(Agent* agent, Vector2D target, Vector2D start) override;
};

