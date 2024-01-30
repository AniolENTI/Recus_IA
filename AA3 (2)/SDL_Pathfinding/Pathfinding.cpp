#include "Pathfinding.h"

Pathfinding::Pathfinding(Grid * grid)
{
	this->grid = grid;

}

void Pathfinding::setGrid(Grid* grid)
{
	this->grid = grid;
}

std::vector<Vector2D> Pathfinding::getNeighbors(Vector2D pos)
{
	int dr[] = { -1, 1, 0, 0 }; // Movimientos arriba, abajo
	int dc[] = { 0, 0, -1, 1 }; // Movimientos izquierda, derecha
	std::vector<Vector2D> neighbors;

	for (int i = 0; i < 4; ++i) {
		float newRow = pos.x + dr[i];
		float newCol = pos.y + dc[i];

		if (grid->isValidCell({ (float)newRow ,(float)newCol })) {
			neighbors.push_back({ (float)newRow ,(float)newCol });
		}
	}

	return neighbors;
}
void Pathfinding::getPath(Vector2D start, Vector2D goal, std::map<Vector2D, Vector2D> came_from, Agent* agent) {
	std::vector<Vector2D> path;
	Vector2D current = goal;

	while (current != start) {
		path.push_back(current);
		current = came_from[current];
	}

	path.push_back(start);

	while (!path.empty())
	{
		agent->addPathPoint(grid->cell2pix(path.back()));
		path.pop_back();
	}
}

int Pathfinding::CalculateHeuristic(Vector2D target, Vector2D current)
{
	return std::abs(target.x - current.x) + std::abs(target.y - current.y);
}