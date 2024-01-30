#include "Greedy.h"

Greedy::Greedy(Grid* grid) :Pathfinding(grid)
{
	
}

void Greedy::FindPath(Agent* agent, Vector2D target, Vector2D start)
{

	std::multimap<int, Vector2D> frontier;

	std::map<Vector2D, Vector2D> came_from;
	std::map<Vector2D, int> cost_so_far;

	came_from[grid->pix2cell(start)] = (0, 0);
	cost_so_far[grid->pix2cell(start)] = 0;
	frontier.emplace(0, grid->pix2cell(start));


	system("cls");

	frontierCount = 0;
	while (!frontier.empty())
	{
		Vector2D current = frontier.begin()->second;
		frontier.erase(frontier.begin());
		frontierCount++;
		if (current == target) {

			break;
		}

		for (Vector2D next : getNeighbors(current)) {

			if (cost_so_far.find(next) == cost_so_far.end()) {

				int newCost = CalculateHeuristic(target, next);
				cost_so_far[next] = newCost;
				frontier.emplace(newCost, next);

				came_from[next] = current;
			}
		}

	}
	std::cout << "Frontera: " << frontierCount << std::endl;

	getPath(grid->pix2cell(start), target, came_from, agent);;

}

int Greedy::CalculateHeuristic(Vector2D target, Vector2D current)
{
	return std::abs(target.x - current.x) + std::abs(target.y - current.y);
}
