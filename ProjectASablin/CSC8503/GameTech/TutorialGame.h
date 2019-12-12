#pragma once
#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/State.h"
#include "../CSC8503Common/HaightScoreReciever.h"
#include "../CSC8503Common/StateTransition.h"
#include "../../Common/Window.h"
#include "../CSC8503Common/GameServer.h"
#include "../CSC8503Common/GameClient.h"
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../CSC8503Common/AppleGameObject.h"
#include "../CSC8503Common/NavigationGrid.h"
#include "../../Common/Assets.h"
#include "../CSC8503Common/GooseGameObject.h"
#include "../CSC8503Common/EnemyGameObject.h"

namespace NCL {
	namespace CSC8503 {
		class TutorialGame {
		public:
			TutorialGame(Window* w);
			~TutorialGame();

			virtual void UpdateGame(float dt);

			void HightScores();

			void game(float dt);

			
		protected:
			void InitServer();
			void InitialiseAssets();

			void InitStateMachine();
			void serverTick();
			void InitCamera();
			void UpdateKeys();

			void Initworldobjects(const std::string& filename);
			void MenuInit();
			
			void AppleInitworldobjects(const std::string& filename);
			void InitWorld();
			void SaveGame();
			void LoadGame();
			void generateworld(const std::string& filename);
			/*
			These are some of the world/object creation functions I created when testing the functionality
			in the module. Feel free to mess around with them to see different objects being created in different
			test scenarios (constraints, collision types, and so on).
			*/
			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			//void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);
			void BridgeConstraintTest();
			void SimpleGJKTest();
			bool SelectObject();
			void MoveSelectedObject();
			
			void DebugObjectMovement();
			void LockedObjectMovement();
			void LockedCameraMovement();
			
			int applesinworld = 0;
			int oldvalspawn = 0;
			Window* Screen;
			int nimappleseated = 0;
			int numofblank = 2;
			
			GameObject* AddFloorToWorld(const Vector3& position, int nodesize);
			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, string name, int Layer= 0);

			GameObject* AddStartButToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, int Layer);

			GameObject* AddEasyButToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, int Layer);


			GameObject* AddBackMenuToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, string name, int Layer);

			GameObject* AddChestToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, string name);
			
			//IT'S HAPPENING
			GooseGameObject* AddGooseToWorld(const Vector3& position);
			EnemyGameObject* AddParkKeeperToWorld(const Vector3& position);
			GameObject* AddCharacterToWorld(const Vector3& position);
			AppleGameObject* AddAppleToWorld(const Vector3& position);
			GooseGameObject* goose = nullptr;
			EnemyGameObject* man = nullptr;
			
			AppleGameObject* Apple;
			GameObject* Chest = nullptr;
			GameObject* Menu = nullptr;
			GameObject* easytarg = nullptr;

			bool MainMenu = true;
			bool play = false;
			bool gameover = false;

			GameTechRenderer* renderer;
			PhysicsSystem* physics;
			GameWorld* world;

			StateMachine* statemachine;

			bool useGravity;
			bool inSelectionMode;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;

			OGLMesh* cubeMesh = nullptr;
			OGLMesh* sphereMesh = nullptr;
			OGLTexture* basicTex = nullptr;
			OGLTexture* groundTex = nullptr;
			OGLTexture* BackMenu = nullptr;
			OGLTexture* EasyButton = nullptr;
			OGLTexture* startbut = nullptr;
			OGLTexture* ChestTex = nullptr;
			OGLTexture* KeeperTex = nullptr;
			OGLTexture* GrounTex = nullptr;
			OGLShader* basicShader = nullptr;

			//Coursework Meshes
			OGLMesh* gooseMesh = nullptr;
			OGLMesh* keeperMesh = nullptr;
			OGLMesh* appleMesh = nullptr;
			OGLMesh* charA = nullptr;
			OGLMesh* charB = nullptr;

			int DLayer = 0;
			string DName = "";
			Vector3 Dvector = Vector3(0, 0, 0);

			int firsframecheck = 0;
			vector<Vector3> testNodes;
			vector<int> scores;
			vector<string> names;
			vector<int> IndexScores;

			//Coursework Additional functionality	
			GameObject* lockedObject = nullptr;
			Vector3 lockedOffset = Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}

			float Totaltime = 0;
			HaightScoreReciever*  serverReceiver;
			HaightScoreReciever*  clientReceiver;
			GameServer* server;
			GameClient* client;

		};
	}
}

