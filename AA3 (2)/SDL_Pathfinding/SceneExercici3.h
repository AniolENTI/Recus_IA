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
#include "BFS.h"
#include "Dijkstra.h"
#include "Greedy.h"
#include "AEstrella.h"
#include "VariacioAEstrella.h"

class SceneExercici3 :
	public Scene
{
public:
	SceneExercici3();
	~SceneExercici3();
	void update(float dtime, SDL_Event* event);
	void draw();
	const char* getTitle();
private:
	std::vector<Agent*> agents;
	Vector2D coinPosition;

	int agentNumber;

	std::vector<Vector2D> start;
	std::vector<Vector2D> target;

	Grid* maze;
	bool draw_grid;

	void drawMaze();
	void drawCoin();
	SDL_Texture* background_texture;
	SDL_Texture* coin_texture;
	bool loadTextures(char* filename_bg, char* filename_coin);

};
