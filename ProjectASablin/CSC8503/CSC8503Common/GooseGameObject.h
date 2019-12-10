#pragma once
#include "GameObject.h"

namespace NCL {
	namespace CSC8503 {
		class GooseGameObject : public GameObject
		{
		public:
			GooseGameObject(string name = "", int _Layer = 0);
			int ApplesEaten = 0;

			int Applesatspawn = 0;
			

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
					}
				}
			}

			virtual void OnCollisionEnd(GameObject* otherObject) {
				//std::cout << "OnCollisionEnd event occured!\n";
			}
		};
	}
};

