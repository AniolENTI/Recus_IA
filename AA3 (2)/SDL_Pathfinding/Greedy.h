#pragma once
#include "Pathfinding.h"
class Greedy : public Pathfinding
{

private:

public:
	Greedy(Grid* grid);

	void FindPath(Agent* agent, Vector2D target, Vector2D start) override;
};

