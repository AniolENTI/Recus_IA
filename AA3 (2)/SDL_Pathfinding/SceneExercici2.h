#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include "Scene.h"
#include "Agent.h"
#include "Seek.h"
#include "PathFollowing.h"
#include "Grid.h"
#include "Pathfinding.h"
#include "BFS.h"
#include "Dijkstra.h"
#include "Greedy.h"
#include "AEstrella.h"
#include "VariacioAEstrella.h"

class SceneExercici2 : public Scene
{
public:
	SceneExercici2();
	~SceneExercici2();
	void update(float dtime, SDL_Event* event);
	void draw();
	const char* getTitle();
private:
	std::vector<Agent*> agents;
	Vector2D coinPosition;

	int agentNumber;
	int enemyNumber;

	std::vector<Agent*> enemies;
	std::vector < std::vector<Vector2D>> enemyPaths;

	std::vector<Vector2D> start;
	std::vector<Vector2D> target;


	Grid* maze;
	bool draw_grid;

	SDL_Texture* background_texture;
	SDL_Texture* coin_texture;

	float currentTime;
	float delayTime;

	void drawMaze();
	void drawCoin();

	bool loadTextures(char* filename_bg, char* filename_coin);
	int CalculateDistance(Vector2D player, Vector2D enemy);
};