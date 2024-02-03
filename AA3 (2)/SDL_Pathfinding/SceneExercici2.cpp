#include "SceneExercici2.h"
#include <iostream>
#include <chrono>
using namespace std;

SceneExercici2::SceneExercici2()
{
	draw_grid = false;
	maze = new Grid("../res/maze.csv");
	loadTextures("../res/maze.png", "../res/coin.png");
	currentTime = 0;
	srand((unsigned int)time(NULL));

	for (int i = 0; i < 1; i++)
	{
		Agent* agent = new Agent;
		agent->loadSpriteTexture("../res/soldier.png", 4);
		agent->setBehavior(new PathFollowing);

		Vector2D rand_cell(-1, -1);

		while (!maze->isValidCell(rand_cell))
			rand_cell = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));

		start.push_back(maze->cell2pix(rand_cell));

		Vector2D rand_cell2(-1, -1);

		while (!maze->isValidCell(rand_cell2))
			rand_cell2 = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));

		target.push_back(rand_cell2);


		agents.push_back(agent);



		agent->setPathfinding(new Dijkstra(maze));
	}

	for (int i = 0; i < 4; i++)
	{
		std::vector<Vector2D> enemiePath;
		for (int i = 0; i < 8; i++)
		{
			Vector2D rand_cell(-1, -1);



			while (!maze->isValidCell(rand_cell))
				rand_cell = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));

			for (int e = 0; e < enemiePath.size(); e++)
			{
				if (enemiePath[e] == rand_cell)
				{
					while (!maze->isValidCell(rand_cell))
						rand_cell = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));
					e = -1;
				}
			}

			enemiePath.push_back(rand_cell);
		}
		enemiesPaths.push_back(enemiePath);


		Agent* agent = new Agent;
		agent->loadSpriteTexture("../res/soldier.png", 4);
		agent->setBehavior(new PathFollowing);

		Vector2D rand_cell(-1, -1);

		while (!maze->isValidCell(rand_cell))
			rand_cell = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));

		start.push_back(maze->cell2pix(rand_cell));

		Vector2D rand_cell2(-1, -1);

		while (!maze->isValidCell(rand_cell2))
			rand_cell2 = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));

		target.push_back(rand_cell2);


		agent->setPathfinding(new VariacioAEstrella(maze));
		agent->setEnemy(true);
		agent->setMaxVelocity(agent->getMaxVelocity() / 2);
		enemies.push_back(agent);

	}

	Vector2D rand_cell(-1, -1);

	// set agent position coords to the center of a random cell

	while (!maze->isValidCell(rand_cell))
		rand_cell = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));

	for (int i = 0; i < agents.size(); i++)
	{
		agents[i]->setPosition(maze->cell2pix(rand_cell));
	}

	std::vector<Vector2D> pos;

	for (int i = 0; i < enemies.size(); i++)
	{

		Vector2D rand_cell(-1, -1);

		// set agent position coords to the center of a random cell

		while (!maze->isValidCell(rand_cell))
			rand_cell = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));

		enemies[i]->setPosition(maze->cell2pix(rand_cell));
		pos.push_back(maze->cell2pix(rand_cell));
	}

	maze->setWeight(pos);



	// set the coin in a random cell (but at least 3 cells far from the agent)
	coinPosition = Vector2D(-1, -1);
	while ((!maze->isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, rand_cell) < 3))
		coinPosition = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));

}

SceneExercici2::~SceneExercici2()
{
	if (background_texture)
		SDL_DestroyTexture(background_texture);
	if (coin_texture)
		SDL_DestroyTexture(coin_texture);

	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
}

void SceneExercici2::update(float dtime, SDL_Event* event)
{
	currentTime += dtime;
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->getPathSize() == 0)
		{
			for (int e = 0; e < enemiesPaths[i].size(); e++)
			{
				enemies[i]->AddTargets(enemiesPaths[i][e]);
			}
			enemies[i]->FindPath((0, 0));
		}

	}
	std::vector<Vector2D> pos;

	for (int i = 0; i < enemies.size(); i++)
	{

		pos.push_back(maze->pix2cell(enemies[i]->getPosition()));
	}
	maze->setWeight(pos);



	if (agents[0]->getPathSize() != 0 && (currentTime - delayTimeComprovation) > 0.5f)
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			if (DistanceToEnemy(maze->pix2cell(agents[0]->getPosition()), maze->pix2cell(enemies[i]->getPosition())) < 6)
			{
				delayTimeComprovation = currentTime;

				// Convierte la duración a un valor float


				agents[0]->clearPath();
				agents[0]->FindPath(agents[0]->targetSaved);
				break;
			}
		}
	}

	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_grid = !draw_grid;


		break;
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			Vector2D cell = maze->pix2cell(Vector2D((float)(event->button.x), (float)(event->button.y)));
			if (maze->isValidCell(cell)) {

				agents[0]->targetSaved = cell;
				agents[0]->FindPath(cell);

				//agents[0]->addPathPoint(maze->cell2pix(cell));

			}

		}
		break;
	default:
		break;
	}
	for (int i = 0; i < agents.size(); i++)
	{
		agents[i]->update(dtime, event);

		// if we have arrived to the coin, replace it in a random cell!
		if ((agents[i]->getCurrentTargetIndex() == -1) && (maze->pix2cell(agents[i]->getPosition()) == coinPosition))
		{
			coinPosition = Vector2D(-1, -1);
			while ((!maze->isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, maze->pix2cell(agents[i]->getPosition())) < 3))
				coinPosition = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));
		}
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->update(dtime, event);

		// if we have arrived to the coin, replace it in a random cell!

	}
}

void SceneExercici2::draw()
{
	drawMaze();
	drawCoin();

	if (draw_grid)
	{
		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 255, 255, 255, 127);
		for (int i = 0; i < SRC_WIDTH; i += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), i, 0, i, SRC_HEIGHT);
		}
		for (int j = 0; j < SRC_HEIGHT; j = j += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), 0, j, SRC_WIDTH, j);
		}
	}
	for (int i = 0; i < agents.size(); i++)
	{
		agents[i]->draw();
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->draw();
	}
}

const char* SceneExercici2::getTitle()
{
	return "SDL Path Finding :: PathFinding Mouse Demo";
}

void SceneExercici2::drawMaze()
{
	SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
	SDL_Rect rect;
	Vector2D coords;
	for (int j = 0; j < maze->getNumCellY(); j++)
	{
		for (int i = 0; i < maze->getNumCellX(); i++)
		{
			if (!maze->isValidCell(Vector2D((float)i, (float)j)))
			{
				SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
				coords = maze->cell2pix(Vector2D((float)i, (float)j)) - Vector2D((float)CELL_SIZE / 2, (float)CELL_SIZE / 2);
				rect = { (int)coords.x, (int)coords.y, CELL_SIZE, CELL_SIZE };
				SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &rect);
			}
			else {
				// Do not draw if it is not necessary (bg is already black)
			}


		}
	}
	//Alternative: render a backgroud texture:
	//SDL_RenderCopy(TheApp::Instance()->getRenderer(), background_texture, NULL, NULL );
}

void SceneExercici2::drawCoin()
{
	Vector2D coin_coords = maze->cell2pix(coinPosition);
	int offset = CELL_SIZE / 2;
	SDL_Rect dstrect = { (int)coin_coords.x - offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE };
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}


bool SceneExercici2::loadTextures(char* filename_bg, char* filename_coin)
{
	SDL_Surface* image = IMG_Load(filename_bg);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	background_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	image = IMG_Load(filename_coin);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	coin_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	return true;
}

int SceneExercici2::DistanceToEnemy(Vector2D player, Vector2D enemy)
{
	return std::abs(enemy.x - player.x) + std::abs(enemy.y - player.y);
}
