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
			float Applesatspawn = 15;
			int totalApp = 0;
			int musicheck = 0;

			void wipeall() {
				ApplesEaten = 0;
				_lvl = 0;
				jumplvl = 0;
				sprintlvl = 0;
				touch = false;
				Applesatspawn = 0;
				totalApp = 0;
				musicheck = 0;
			}

			void loadfromfile(int _ApplesEaten, float lvl_, int _jumplvl,int _sprintlvl,bool _touch,float _Applesatspawn, int _totalApp, int _musicheck)
			{
				ApplesEaten = _ApplesEaten;
				_lvl = lvl_;
				jumplvl = _jumplvl;
				sprintlvl = _sprintlvl;
				touch = _touch;
				Applesatspawn = _Applesatspawn;
				totalApp = _totalApp;
				musicheck = _musicheck;
			}

			void cleenbag() { ApplesEaten = 0; }
			void lvlup() {
				Applesatspawn = Applesatspawn - (3.0f + (3.0f * _lvl / 5.0f));
				_lvl++;
				lvl = _lvl;
			}
			void Music();
			void GooseIndecators(Window* w);
			void goosebehave(Window* w);
			void SetBack() {
				touch = false;
			}
			
			virtual void OnCollisionBegin(GameObject* otherObject) {
				if (otherObject->GetName() == "apple") {
					ApplesEaten++;
					PlaySound(TEXT("eda.WAV"), NULL, SND_FILENAME | SND_ASYNC);
				}

				if (otherObject->GetName() == "chest") {
					if (ApplesEaten > 0)
					{
						ApplesEaten--;
						totalApp++;
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

