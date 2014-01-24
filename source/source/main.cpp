#include "level.h"
#include "Box2DManager.h"

#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-system.lib")

#include <iostream>
#include <random>


Object player;



int main()
{
	srand(time(NULL));

	Level lvl;
	lvl.LoadFromFile("data\/platformer.tmx");

	Box2DManager box2d_manager(0.0f, 1.0f);
		
	b2Body* playerBody;
	std::vector<b2Body*> * coinBody;
	std::vector<b2Body*> * enemyBody;

	sf::Vector2i tileSize = lvl.GetTileSize();

	std::vector<Object> coin = lvl.GetObjects("coin");
	std::vector<Object> enemy = lvl.GetObjects("enemy");

	std::cout << coin.size() << std::endl;

	box2d_manager.createBodiesFromObjectsVector(lvl.GetObjects("block"), tileSize.x, tileSize.y, true);
	coinBody = box2d_manager.createBodiesFromObjectsVector(lvl.GetObjects("coin"), tileSize.x, tileSize.y, false);
	enemyBody = box2d_manager.createBodiesFromObjectsVector(lvl.GetObjects("enemy"), tileSize.x, tileSize.y, false);

	

	playerBody = box2d_manager.createBodyFromObject(lvl.GetObject("player"));
	player = lvl.GetObject("player");

	sf::Vector2i screenSize(800, 600);

	sf::RenderWindow window;
	window.create(sf::VideoMode(screenSize.x, screenSize.y), "Game");


	sf::View view;
	view.reset(sf::FloatRect(0.0f, 0.0f, screenSize.x, screenSize.y));
	view.setViewport(sf::FloatRect(0.0f, 0.0f, 2.0f, 2.0f));

	bool A_pressed = false, D_pressed = false;

    while(window.isOpen())
    {
        sf::Event evt;

		b2Vec2 vel = playerBody->GetLinearVelocity();

        while(window.pollEvent(evt))
        {
			switch(evt.type)
			{
			case sf::Event::Closed:
                window.close();
				break;

			case sf::Event::KeyPressed:
				if(evt.key.code == sf::Keyboard::W && playerBody->GetLinearVelocity().y == 0){
					vel.y = b2Min( vel.y - 15.0f, -15.0f );
					playerBody->SetLinearVelocity(vel);
				}

				if(evt.key.code == sf::Keyboard::D){
					D_pressed = true;
				}

				if(evt.key.code == sf::Keyboard::A){
					A_pressed = true;
				}
				break;
			case sf::Event::KeyReleased:
				if(evt.key.code == sf::Keyboard::D){
					D_pressed = false;
				}

				if(evt.key.code == sf::Keyboard::A){
					A_pressed = false;
				}
				break;
			}
        }

		if(D_pressed){
			vel.x = b2Min( vel.x + 0.1f, 5.0f );
			playerBody->SetLinearVelocity(vel);
		}

		if(A_pressed){
			vel.x = b2Max( vel.x - 0.1f,  -5.0f );
			playerBody->SetLinearVelocity(vel);
		}

		std::cout << vel.x << " : " << vel.y << std::endl;

		box2d_manager.step(1.0f / 60.0f, 1, 1);		

		for(b2ContactEdge* ce = playerBody->GetContactList(); ce; ce = ce->next)
		{
			b2Contact* c = ce->contact;
			
			for(int i = 0; i < coinBody->size(); i++)
				if(c->GetFixtureA() == (*coinBody)[i]->GetFixtureList())
				{
					(*coinBody)[i]->DestroyFixture((*coinBody)[i]->GetFixtureList());
					coin.erase(coin.begin() + i);
					coinBody->erase(coinBody->begin() + i);
				}

			for(int i = 0; i < enemyBody->size(); i++)
				if(c->GetFixtureA() == (*enemyBody)[i]->GetFixtureList())
				{
					if(playerBody->GetPosition().y < (*enemyBody)[i]->GetPosition().y)
					{
						playerBody->SetLinearVelocity(b2Vec2(0.0f, -10.0f));

						(*enemyBody)[i]->DestroyFixture((*enemyBody)[i]->GetFixtureList());
						enemy.erase(enemy.begin() + i);
						enemyBody->erase(enemyBody->begin() + i);
					}
					else
					{
						int tmp = (playerBody->GetPosition().x < (*enemyBody)[i]->GetPosition().x)
							? -1 : 1;
						playerBody->SetLinearVelocity(b2Vec2(10.0f * tmp, 0.0f));
					}
				}
		}

		for(int i = 0; i < enemyBody->size(); i++)
		{
			if((*enemyBody)[i]->GetLinearVelocity() == b2Vec2_zero)
			{
				int tmp = (rand() % 2 == 1) ? 1 : -1;
				(*enemyBody)[i]->SetLinearVelocity(b2Vec2(5.0f * tmp, 0.0f));
			}
		}


		b2Vec2 pos = playerBody->GetPosition();
		view.setCenter(pos.x + screenSize.x / 4, pos.y + screenSize.y / 4);
		window.setView(view);

		player.sprite.setPosition(pos.x, pos.y);

		
		for(int i = 0; i < coin.size(); i++)
			coin[i].sprite.setPosition((*coinBody)[i]->GetPosition().x, (*coinBody)[i]->GetPosition().y);

		for(int i = 0; i < enemy.size(); i++)
			enemy[i].sprite.setPosition((*enemyBody)[i]->GetPosition().x, (*enemyBody)[i]->GetPosition().y);
			
        window.clear();


		lvl.Draw(window);

		window.draw(player.sprite);
		

		for(int i = 0; i < coin.size(); i++)
			window.draw(coin[i].sprite);

		for(int i = 0; i < enemy.size(); i++)
			window.draw(enemy[i].sprite);

		window.display();
    }

    return 0;
}