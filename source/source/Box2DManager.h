#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <Box2D\Box2D.h>

#include "level.h"

#pragma comment(lib,"Box2D.lib")

class Box2DManager
{
	b2World* _world; 
	

public:
	Box2DManager(float gravity_x, float gravity_y);
	~Box2DManager(void);

	std::vector<b2Body*> * createBodiesFromObjectsVector(std::vector<Object>  vector_objects, int object_width, int object_height, bool is_static);
	b2Body * createBodyFromObject(Object object);
	void step(float timeStep, int velocityIterations, int positionIterations);
};
