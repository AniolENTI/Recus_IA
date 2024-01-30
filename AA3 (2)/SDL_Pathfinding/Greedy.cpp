#include "Greedy.h"

Greedy::Greedy(Grid* grid) :Pathfinding(grid)
{
	
}

void Greedy::FindPath(Agent* agent, Vector2D target, Vector2D start)
{

	std::multimap<int, Vector2D> frontier;

	std::map<Vector2D, Vector2D> cameFrom;
	std::map<Vector2D, int> costSoFar;

	cameFrom[grid->pix2cell(start)] = (0, 0);
	costSoFar[grid->pix2cell(start)] = 0;
	frontier.emplace(0, grid->pix2cell(start));

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

			if (costSoFar.find(next) == costSoFar.end()) {

				int newCost = CalculateHeuristic(target, next);
				costSoFar[next] = newCost;
				frontier.emplace(newCost, next);

				cameFrom[next] = current;
			}
		}

	}
	std::cout << "Frontera: " << frontierCount << std::endl;

	getPath(grid->pix2cell(start), target, cameFrom, agent);;

}
