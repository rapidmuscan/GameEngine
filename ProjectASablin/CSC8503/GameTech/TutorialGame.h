#pragma once
#include "../../Common/Window.h"
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../CSC8503Common/AppleGameObject.h"
#include "../CSC8503Common/NavigationGrid.h"
#include "../../Common/Assets.h"
#include "../CSC8503Common/GooseGameObject.h"

namespace NCL {
	namespace CSC8503 {
		class TutorialGame		{
		public:
			TutorialGame(Window* w);
			~TutorialGame();

			virtual void UpdateGame(float dt);

		protected:
			void InitialiseAssets();

			void InitCamera();
			void UpdateKeys();
			void ManWalk(float dt);
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
			int applesinworld = 0;
			bool SelectObject();
			void MoveSelectedObject();
			void MoveGoose();
			void DebugObjectMovement();
			void LockedObjectMovement();
			void LockedCameraMovement();
			void CalculatePathCharacter();
			float testTime = 0.0f;
			NavigationPath outPath;
			Window* Screen;
			int nimappleseated = 0;

			GameObject* AddFloorToWorld(const Vector3& position);
			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f);
			//IT'S HAPPENING
			GooseGameObject* AddGooseToWorld(const Vector3& position);
			GameObject* AddParkKeeperToWorld(const Vector3& position);
			GameObject* AddCharacterToWorld(const Vector3& position);
			AppleGameObject* AddAppleToWorld(const Vector3& position);
			GooseGameObject* goose =nullptr;
			GameObject* man = nullptr;
			AppleGameObject* Apple;

			GameTechRenderer*	renderer;
			PhysicsSystem*		physics;
			GameWorld*			world;

			bool useGravity;
			bool inSelectionMode;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;

			OGLMesh*	cubeMesh	= nullptr;
			OGLMesh*	sphereMesh	= nullptr;
			OGLTexture* basicTex	= nullptr;
			OGLShader*	basicShader = nullptr;

			//Coursework Meshes
			OGLMesh*	gooseMesh	= nullptr;
			OGLMesh*	keeperMesh	= nullptr;
			OGLMesh*	appleMesh	= nullptr;
			OGLMesh*	charA		= nullptr;
			OGLMesh*	charB		= nullptr;

			Vector3 targetposition;
			int firsframecheck = 0;
			vector<Vector3> testNodes;
			NavigationGrid grid;

			//Coursework Additional functionality	
			GameObject* lockedObject	= nullptr;
			Vector3 lockedOffset		= Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}
		};
	}
}

