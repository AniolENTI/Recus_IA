#include "AEstrella.h"

AEstrella::AEstrella(Grid* grid) :Pathfinding(grid)
{

}

void AEstrella::FindPath(Agent* agent, Vector2D target, Vector2D start)
{
	std::multimap<int, Vector2D> frontier;

	std::map<Vector2D, Vector2D> cameFrom;
	std::map<Vector2D, int> costSoFar;

	cameFrom[grid->pix2cell(start)] = (0, 0);
	costSoFar[grid->pix2cell(start)] = CalculateHeuristic(target, grid->pix2cell(start));
	frontier.emplace(0, grid->pix2cell(start));


	frontierCount = 0;
	while (!frontier.empty())
	{
		Vector2D current = frontier.begin()->second;
		frontierCount++;

		frontier.erase(frontier.begin());

		if (current == target) {

			break;
		}

		for (Vector2D next : getNeighbors(current)) {
			int newCost = (costSoFar[current] + grid->getWeight(next));

			if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) {
				costSoFar[next] = newCost;

				frontier.emplace((newCost + CalculateHeuristic(target, next)), next);

				cameFrom[next] = current;
			}
		}

	}
	std::cout << "Frontera: " << frontierCount << std::endl;


	getPath(grid->pix2cell(start), target, cameFrom, agent);;

}