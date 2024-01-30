#pragma once
#include "Pathfinding.h"

class BFS : public Pathfinding
{
public:
	BFS(Grid* grid);

	void FindPath(Agent* agent, Vector2D target, Vector2D start) override;
};