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
			EnemyGameObject(string name = "", int _Layer = 0, GameObject * _target = nullptr, GameObject* _target2 = nullptr, GameObject* _target3 = nullptr);

			int tuched = 0;
			float buff = 0;
			GameObject* target = nullptr;
			GameObject* target2 = nullptr;
			GameObject* target3 = nullptr;

			static void FolowingEnemy(float dt, void* data);
			static void OtherThing(float dt, void* data);

			void folowingwithalg(float dt, GameObject* target, int speed);


			void seteasy() {
				GameObject* a = target3;
				target3 = target2;
				target2 = a;
			}
			void sethard() {
				GameObject* a = target2;
				target2 = target3;
				target3 = a;
			}

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
				enemy->buff = enemy->target->GetLvl();
				int maxrange = 40;
				float Difinx = std::abs(enemy->target->GetTransform().GetWorldPosition().x - enemy->GetTransform().GetWorldPosition().x);
				float Difinz = std::abs(enemy->target->GetTransform().GetWorldPosition().z - enemy->GetTransform().GetWorldPosition().z);
				float dist = sqrt((Difinz * Difinz) + (Difinx * Difinx));
				return  (dist < (maxrange + (maxrange * enemy->buff / 100)));
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
				enemy->buff = enemy->target->GetLvl();
				int maxrange = 40;
				float Difinx = std::abs(enemy->target->GetTransform().GetWorldPosition().x - enemy->GetTransform().GetWorldPosition().x);
				float Difinz = std::abs(enemy->target->GetTransform().GetWorldPosition().z - enemy->GetTransform().GetWorldPosition().z);
				float dist = sqrt((Difinz * Difinz) + (Difinx * Difinx));
				return (dist > (maxrange + (maxrange * enemy->buff / 100)));
			}

		protected:
			EnemyGameObject* enemy;
		};
	}
}
