#include "VariacioAEstrella.h"

VariacioAEstrella::VariacioAEstrella(Grid* grid) :Pathfinding(grid)
{

}

void VariacioAEstrella::FindPath(Agent* agent, Vector2D target, Vector2D start)
{
	std::multimap<int, Vector2D> frontier;

	std::map<Vector2D, Vector2D> cameFrom;
	std::map<Vector2D, int> costSoFar;

	cameFrom[grid->pix2cell(start)] = (0, 0);
	costSoFar[grid->pix2cell(start)] = 0;
	frontier.emplace(0, grid->pix2cell(start));


	frontierCount = 0;
	Vector2D final = (0, 0);
	while (!frontier.empty())
	{
		Vector2D current = frontier.begin()->second;
		frontier.erase(frontier.begin());

		frontierCount++;
		
		for (int i = 0; i < targets.size(); i++)
		{
			if (current == targets[i]) {
				frontier.clear();
				frontier.emplace(0, targets[i]);

				targets.erase(targets.begin() + (i));


				final = current;

				break;
			}
		}
		if (targets.size() == 0)
		{
			final = current;
			break;
		}


		for (Vector2D next : getNeighbors(current)) {
			int newCost = (costSoFar[current] + grid->getWeight(next));

			if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) {
				costSoFar[next] = newCost;

				frontier.emplace((newCost + getSmallerHeuristic(next)), next);

				cameFrom[next] = current;
			}

		}

	}

	system("cls");
	std::cout << "Frontera: " << frontierCount << std::endl;

	getPath(grid->pix2cell(start), target, cameFrom, agent);
}

int VariacioAEstrella::getSmallerHeuristic(Vector2D current)
{
	int lower = 100000;

	for (int i = 0; i < targets.size(); i++)
	{
		int heuristic = CalculateHeuristic(targets[i], current);
		if (heuristic < lower)
		{
			lower = heuristic;
		}
	}

	return lower;
}
