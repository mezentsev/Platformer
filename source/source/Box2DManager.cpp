#include "Box2DManager.h"


Box2DManager::Box2DManager(float gravity_x, float gravity_y)
{
	b2Vec2 gravity(gravity_x, gravity_y);
	_world = new b2World(gravity);
}


Box2DManager::~Box2DManager(void)
{
	delete _world;
}

std::vector<b2Body*> * Box2DManager::createBodiesFromObjectsVector(std::vector<Object>  vector_objects, int object_width, int object_height, bool is_static)
{
	std::vector<b2Body*> * vector_bodies = new std::vector<b2Body*>();

	for(int i = 0; i < vector_objects.size(); i++)
	{
		Object obj = vector_objects[i];
		
		b2BodyDef bodyDef;
		
		if (is_static)
			bodyDef.type = b2_staticBody;
		else 
			bodyDef.type = b2_dynamicBody;
		
		bodyDef.position.Set(	obj.rect.left + object_width / 2 * (obj.rect.width / object_width - 1),
								obj.rect.top + object_height / 2 * (obj.rect.height / object_height - 1));

		b2PolygonShape shape;
		shape.SetAsBox(obj.rect.width / 2, obj.rect.height / 2);

		b2Body* body = _world->CreateBody(&bodyDef);
		body->CreateFixture(&shape,1.0f);

		vector_bodies->push_back(body);
	}

	return vector_bodies;
}

b2Body * Box2DManager::createBodyFromObject(Object object)
{
	b2Body* obj_body;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(object.rect.left, object.rect.top);
	bodyDef.fixedRotation = true;
	
	obj_body = _world->CreateBody(&bodyDef);
	
	b2PolygonShape shape; 
	shape.SetAsBox(object.rect.width / 2, object.rect.height / 2);
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f; fixtureDef.friction = 0.3f;
	obj_body->CreateFixture(&fixtureDef);

	return obj_body;
}

void Box2DManager::step(float timeStep, int velocityIterations, int positionIterations)
{
	_world->Step(timeStep, velocityIterations, positionIterations);
}
