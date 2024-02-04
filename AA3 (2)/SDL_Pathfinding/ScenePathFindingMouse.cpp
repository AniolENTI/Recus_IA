#include "ScenePathFindingMouse.h"

using namespace std;

ScenePathFindingMouse::ScenePathFindingMouse()
{
	draw_grid = false;
	maze = new Grid("../res/maze.csv");

	loadTextures("../res/maze.png", "../res/coin.png");

	srand((unsigned int)time(NULL));

	agentNumber = 20;

	for (int i = 0; i < agentNumber; i++)
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

		agent->setPathfinding(new BFS(maze));
	}

	// set agent position coords to the center of a random cell
	Vector2D rand_cell(-1,-1);
	while (!maze->isValidCell(rand_cell))
		rand_cell = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));

	for (int i = 0; i < agentNumber; i++)
	{
		agents[i]->setPosition(maze->cell2pix(rand_cell));
	}

	// set the coin in a random cell (but at least 3 cells far from the agent)
	coinPosition = Vector2D(-1,-1);
	while ((!maze->isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, rand_cell)<3))
		coinPosition = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));

}

ScenePathFindingMouse::~ScenePathFindingMouse()
{
	if (background_texture)
		SDL_DestroyTexture(background_texture);
	if (coin_texture)
		SDL_DestroyTexture(coin_texture);

	for (int i = 0; i < agentNumber; i++)
	{
		delete agents[i];
	}
}

void ScenePathFindingMouse::update(float dtime, SDL_Event *event)
{
	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_grid = !draw_grid;
		//Canviar a BFS
		if (event->key.keysym.sym == SDLK_b)
		{
			system("cls");
			for (int i = 0; i < agentNumber; i++)
			{
				agents[i]->setPathfinding(new BFS(maze));
			}
		}
		//Canviar a Dijkstra
		if (event->key.keysym.sym == SDLK_d)
		{
			system("cls");
			for (int i = 0; i < agentNumber; i++)
			{
				agents[i]->setPathfinding(new Dijkstra(maze));
			}
		}
		//Canviar a Greedy
		if (event->key.keysym.sym == SDLK_g)
		{
			system("cls");
			for (int i = 0; i < agentNumber; i++)
			{
				agents[i]->setPathfinding(new Greedy(maze));
			}
		}
		//Canviar a A*
		if (event->key.keysym.sym == SDLK_a)
		{
			system("cls");
			for (int i = 0; i < agentNumber; i++)
			{
				agents[i]->setPathfinding(new AEstrella(maze));
			}
		}
		break;
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			for (int i = 0; i < agentNumber; i++)
			{
				agents[i]->setPosition(start[i]);
				agents[i]->FindPath(target[i]);
			}
			int minim = 10000;
			for (int i = 0; i < agentNumber; i++)
			{
				if (agents[i]->getFrontierCount() < minim)
				{
					minim = agents[i]->getFrontierCount();
				}
			}
			int maxim = 0;
			for (int i = 0; i < agentNumber; i++)
			{
				if (agents[i]->getFrontierCount() > maxim)
				{
					maxim = agents[i]->getFrontierCount();
				}
			}
			float mitjana = 0;

			for (int i = 0; i < agentNumber; i++)
			{
				mitjana += agents[i]->getFrontierCount();
			}
			mitjana /= agentNumber;

			std::cout << "Minim: " << std::to_string(minim) << " Maxim: " << std::to_string(maxim) << " Mitjana: " << std::to_string(mitjana) << std::endl;
		}
		break;
	default:
		break;
	}

	for (int i = 0; i < agentNumber; i++)
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
}

void ScenePathFindingMouse::draw()
{
	drawMaze();
	drawCoin();

	if (draw_grid)
	{
		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 255, 255, 255, 127);
		for (int i = 0; i < SRC_WIDTH; i+=CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), i, 0, i, SRC_HEIGHT);
		}
		for (int j = 0; j < SRC_HEIGHT; j = j += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), 0, j, SRC_WIDTH, j);
		}
	}

	for (int i = 0; i < (int)agents.size(); i++)
	{
		agents[i]->draw();
	}
}

const char* ScenePathFindingMouse::getTitle()
{
	return "SDL Path Finding :: PathFinding Mouse Demo";
}

void ScenePathFindingMouse::drawMaze()
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
			} else {
				// Do not draw if it is not necessary (bg is already black)
			}
					
			
		}
	}
	//Alternative: render a backgroud texture:
	//SDL_RenderCopy(TheApp::Instance()->getRenderer(), background_texture, NULL, NULL );
}

void ScenePathFindingMouse::drawCoin()
{
	Vector2D coin_coords = maze->cell2pix(coinPosition);
	int offset = CELL_SIZE / 2;
	SDL_Rect dstrect = {(int)coin_coords.x-offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE};
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}


bool ScenePathFindingMouse::loadTextures(char* filename_bg, char* filename_coin)
{
	SDL_Surface *image = IMG_Load(filename_bg);
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
