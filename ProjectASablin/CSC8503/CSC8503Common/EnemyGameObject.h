#pragma once
#include "GameObject.h"
#include "../CSC8503Common/NavigationGrid.h"
#include "../../Common/Assets.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../../Common/Assets.h"

namespace NCL {
	namespace CSC8503 {
		class EnemyGameObject : public GameObject
		{
		public:
			EnemyGameObject(string name = "", int _Layer = 0);



			void EnemyLogic(float dt, GameObject* target, int maxrange = 40, int speed = 100);

			void folowingwithalg(float dt, GameObject* target);





			float testTime = 0.0f;
			NavigationPath outPath;
			NavigationGrid grid;
			Vector3 targetposition;

			virtual void OnCollisionBegin(GameObject* otherObject) {

			}



			virtual void OnCollisionEnd(GameObject* otherObject) {
				//std::cout << "OnCollisionEnd event occured!\n";
			}
		};
	}
}
