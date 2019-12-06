#pragma once
#include "GameObject.h"

namespace NCL {
	namespace CSC8503 {
		class AppleGameObject : public GameObject
		{
		public:
			AppleGameObject();

			virtual void OnCollisionBegin(GameObject* otherObject) {
				std::cout << "OnCollisionBegin event occured!\n";
				if (otherObject->GetName() == "goose") {
					GetTransform().SetLocalPosition(Vector3(0, -19999, 0));
				}
			}

			virtual void OnCollisionEnd(GameObject* otherObject) {
				//std::cout << "OnCollisionEnd event occured!\n";
			}
		};
	}
}
