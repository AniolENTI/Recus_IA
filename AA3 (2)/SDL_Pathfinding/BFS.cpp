#include "BFS.h"

BFS::BFS(Grid* grid) :Pathfinding(grid)
{

}

void BFS::FindPath(Agent* agent, Vector2D target, Vector2D start)
{
	std::queue<Vector2D> frontier;

	std::map<Vector2D, Vector2D> cameFrom;

	frontier.push(grid->pix2cell(start));

	frontierCount = 0;
	while (!frontier.empty())
	{
		Vector2D current = frontier.front();
		frontier.pop();
		frontierCount++;
		if (current == target) {

			break;
		}

		for (Vector2D next : getNeighbors(current)) {
			if (cameFrom[next] == Vector2D(0, 0)) {
				frontier.push(next);
				cameFrom[next] = current;
			}
		}

	}

	system("cls");
	std::cout << "Frontera: " << frontierCount << std::endl;

	getPath(grid->pix2cell(start), target, cameFrom, agent);
}

