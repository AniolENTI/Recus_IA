#pragma once
#include "Pathfinding.h"

class Dijkstra : public Pathfinding
{
public:
	Dijkstra(Grid* grid);

	void FindPath(Agent* agent, Vector2D target, Vector2D start) override;
};