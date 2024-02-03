#pragma once
#include <iostream>
#include <minmax.h>
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_SimpleApp.h"
#include "Path.h"
#include "Vector2D.h"
#include "utils.h"
#include "Pathfinding.h"

class Pathfinding;

class Agent
{
public:
	class SteeringBehavior
	{
	public:
		SteeringBehavior() {};
		virtual ~SteeringBehavior() {};
		virtual void applySteeringForce(Agent *agent, float dtime) {};
	};
private:
	SteeringBehavior *steering_behaviour;
	Vector2D position;
	Vector2D velocity;
	Vector2D target;

	// Pathfinding
	Path path;
	Pathfinding* pathfinding;
	int currentTargetIndex;

	float mass;
	float orientation;
	float max_force;
	float max_velocity;

	SDL_Texture *sprite_texture;
	bool draw_sprite;
	int sprite_num_frames;
	int sprite_w;
	int sprite_h;

	bool isEnemy;
	Vector2D savedTarget;

public:
	Agent();
	~Agent();

	Vector2D getPosition();
	Vector2D getTarget();
	Vector2D getVelocity();
	float getMaxVelocity();
	float getMaxForce();
	float getMass();
	bool getEnemy();
	int getCurrentTargetIndex();
	int getPathSize();
	Vector2D getPathPoint(int idx);
	int getFrontierCount();
	std::vector<Vector2D> getTargets();
	Vector2D getSavedTarget();

	void setBehavior(SteeringBehavior *behavior);
	void setPosition(Vector2D position);
	void setTarget(Vector2D target);
	void setVelocity(Vector2D velocity);
	void setMaxVelocity(float maximum);
	void setEnemy(bool isEnemy);
	void addPathPoint(Vector2D point);
	void setCurrentTargetIndex(int idx);
	void setPathfinding(Pathfinding* pathfinding);
	
	void clearPath();
	void update(float dtime, SDL_Event *event);
	void draw();
	bool loadSpriteTexture(char* filename, int num_frames=1);
	void FindPath(Vector2D pos);
	void AddTargets(Vector2D pos);
	void saveTarget(Vector2D target);
};
