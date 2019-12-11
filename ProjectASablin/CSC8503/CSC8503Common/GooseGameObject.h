#pragma once
#include "GameObject.h"
#include "../../Common/Assets.h"
#include "../CSC8503Common/GameWorld.h"
namespace NCL {
	namespace CSC8503 {
		class GooseGameObject : public GameObject
		{
		public:
			GooseGameObject(string name = "", int _Layer = 0);
			int ApplesEaten = 0;
			float _lvl = 0;
			int jumplvl = 0;
			int sprintlvl = 0;
			bool touch = false;
			float Applesatspawn = 0;
			void cleenbag() { ApplesEaten = 0; }
			void lvlup() {
				Applesatspawn = Applesatspawn - (3.0f + (3.0f * _lvl / 10.0f));
				_lvl++;
				lvl = _lvl;
			}
			
			void GooseIndecators(Window* w);
			void goosebehave(Window* w);
			void SetBack() {
				touch = false;
			}
			virtual void OnCollisionBegin(GameObject* otherObject) {
				//std::cout << "OnCollisionBegin event occured!\n";
				if (otherObject->GetName() == "apple") {
					ApplesEaten++;
				}

				if (otherObject->GetName() == "tower") {
					if (ApplesEaten > 0)
					{
						ApplesEaten--;
						Applesatspawn++;
						touch = true;
					}
				}
			}

			virtual void OnCollisionEnd(GameObject* otherObject) {
				
			}
		};
	}
};

