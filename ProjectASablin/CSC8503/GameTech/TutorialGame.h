#pragma once
#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/State.h"
#include "../CSC8503Common/StateTransition.h"
#include "../../Common/Window.h"
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

		protected:
			void InitialiseAssets();

			void InitStateMachine();

			void InitCamera();
			void UpdateKeys();

			void Initworldobjects(const std::string& filename);
			void AppleInitworldobjects(const std::string& filename);
			void InitWorld();
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
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, string name);

			GameObject* AddChestToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, string name);
			
			//IT'S HAPPENING
			GooseGameObject* AddGooseToWorld(const Vector3& position);
			EnemyGameObject* AddParkKeeperToWorld(const Vector3& position);
			GameObject* AddCharacterToWorld(const Vector3& position);
			AppleGameObject* AddAppleToWorld(const Vector3& position);
			GooseGameObject* goose = nullptr;
			EnemyGameObject* man = nullptr;
			
			AppleGameObject* Apple;

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


			int firsframecheck = 0;
			vector<Vector3> testNodes;


			//Coursework Additional functionality	
			GameObject* lockedObject = nullptr;
			Vector3 lockedOffset = Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}
		};
	}
}

