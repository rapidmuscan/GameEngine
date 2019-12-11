#pragma once
#include "GameObject.h"
#include "../CSC8503Common/NavigationGrid.h"
#include "../../Common/Assets.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../../Common/Assets.h"
#include "../CSC8503Common/StateTransition.h"

namespace NCL {
	namespace CSC8503 {
		class EnemyGameObject : public GameObject
		{
		public:
			EnemyGameObject(string name = "", int _Layer = 0, GameObject * _target = nullptr);

			int tuched = 0;
			float buff = 0;
			GameObject* target = nullptr;


			void EnemyLogic(float dt);

			static void FolowingEnemy(float dt, void* data);
			static void OtherThing(float dt, void* data);

			void folowingwithalg(float dt, GameObject* target, int speed);




			void reset() {
				tuched = 0;
			}

			float testTime = 0.0f;
			NavigationPath outPath;
			NavigationGrid grid;
			Vector3 targetposition;

			virtual void OnCollisionBegin(GameObject* otherObject) {
				if (otherObject->GetName() == "goose") {
					PlaySound(TEXT("lole.WAV"), NULL, SND_FILENAME | SND_ASYNC);

					tuched++;
				}
				
			}



			virtual void OnCollisionEnd(GameObject* otherObject) {
				//std::cout << "OnCollisionEnd event occured!\n";
			}
			
		};

		class ShouldFollowEnemy : public StateTransition
		{
		public:
			ShouldFollowEnemy(EnemyGameObject* enemy, State* srcState, State* destState) : enemy(enemy)
			{
				sourceState = srcState;
				destinationState = destState;
			}

			virtual bool CanTransition() const override
			{
				//enemy->
				return true;
			}

		protected:
			EnemyGameObject* enemy;
		};

		class ShouldDoTheOTherThing : public StateTransition
		{
		public:
			ShouldDoTheOTherThing(EnemyGameObject* enemy, State* srcState, State* destState) : enemy(enemy)
			{
				sourceState = srcState;
				destinationState = destState;
			}

			virtual bool CanTransition() const override
			{
				//enemy->
				return false;
			}

		protected:
			EnemyGameObject* enemy;
		};
	}
}
