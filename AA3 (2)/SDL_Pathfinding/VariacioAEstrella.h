#pragma once
#include "Pathfinding.h"

class VariacioAEstrella : public Pathfinding
{
private:

public:
	VariacioAEstrella(Grid* grid);

	void FindPath(Agent* agent, Vector2D target, Vector2D start) override;

	int getSmallerHeuristic(Vector2D current);
};