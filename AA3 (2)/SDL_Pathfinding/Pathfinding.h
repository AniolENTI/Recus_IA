#pragma once
#include "Grid.h"
#include <queue>
#include <map>
#include <vector>

class Agent;
class Grid;

class Pathfinding
{
protected:
	Grid* grid;

public:
	Pathfinding(Grid* grid);

	//Variables
	std::vector<Vector2D> targets;
	int frontierCount;

	//Calculs
	virtual void FindPath(Agent* agent, Vector2D target, Vector2D start) {};

	//Setters i Getters
	void setGrid(Grid* grid);
	void getPath(Vector2D start, Vector2D goal, std::map<Vector2D, Vector2D> came_from, Agent* agent);
	std::vector<Vector2D> getNeighbors(Vector2D pos);
};

