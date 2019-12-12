
#include "TutorialGame.h"
#include "../CSC8503Common/GameWorld.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/TextureLoader.h"
#include "../../Common/Assets.h"
#include "../CSC8503Common/PositionConstraint.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include<time.h>
#include<sstream>
#include <ostream>


using namespace NCL;
using namespace CSC8503;

TutorialGame::TutorialGame(Window* w) {
	world = new GameWorld();
	renderer = new GameTechRenderer(*world);
	physics = new PhysicsSystem(*world);

	

	forceMagnitude = 10.0f;
	useGravity = false;
	inSelectionMode = false;
	Screen = w;

	Debug::SetRenderer(renderer);
	

	InitialiseAssets();

	InitServer();
}
void TutorialGame::InitServer() {
	NetworkBase::Initialise();

	serverReceiver = new HaightScoreReciever("Server");
	clientReceiver = new HaightScoreReciever("Client");

	int port = NetworkBase::GetDefaultPort();

	server = new GameServer(port, 1);
	client = new GameClient();

	server->RegisterPacketHandler(String_Message, serverReceiver);
	client->RegisterPacketHandler(String_Message, clientReceiver);

	bool canConnect = client->Connect(127, 0, 0, 1, port);
}

void TutorialGame::InitialiseAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("cube.msh", &cubeMesh);
	loadFunc("sphere.msh", &sphereMesh);
	loadFunc("goose.msh", &gooseMesh);
	loadFunc("CharacterA.msh", &keeperMesh);
	loadFunc("CharacterM.msh", &charA);
	loadFunc("CharacterF.msh", &charB);
	loadFunc("Apple.msh", &appleMesh);

	basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	KeeperTex = (OGLTexture*)TextureLoader::LoadAPITexture("EnemyTex.png");
	GrounTex = (OGLTexture*)TextureLoader::LoadAPITexture("GrounTex.png");
	groundTex = (OGLTexture*)TextureLoader::LoadAPITexture("ground.png");
	BackMenu = (OGLTexture*)TextureLoader::LoadAPITexture("BackMenu.jpg");
	EasyButton = (OGLTexture*)TextureLoader::LoadAPITexture("EasyBut.jpg");
	startbut = (OGLTexture*)TextureLoader::LoadAPITexture("startbut.png");
	ChestTex = (OGLTexture*)TextureLoader::LoadAPITexture("gold-texture.jpg");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

	InitCamera();
	InitWorld();
	InitStateMachine();
}

void TutorialGame::InitStateMachine() {
	statemachine = new StateMachine();

	GenericState* stateA = new GenericState(EnemyGameObject::FolowingEnemy, static_cast<void*>(man));
	GenericState* stateB = new GenericState(EnemyGameObject::OtherThing, static_cast<void*>(man));

	statemachine->AddState(stateA);
	statemachine->AddState(stateB);

	StateTransition* stateTransitionA = new ShouldFollowEnemy(man, stateB, stateA);
	StateTransition* stateTransitionB = new ShouldDoTheOTherThing(man, stateA, stateB);

	statemachine->AddTransition(stateTransitionA);
	statemachine->AddTransition(stateTransitionB);
}

void NCL::CSC8503::TutorialGame::serverTick()
{
	server->SendGlobalPacket(
		StringPacket("Server says hello!"));

	client->SendPacket(
		StringPacket("Client says hello!"));

	server->UpdateServer();
	client->UpdateClient();

	std::this_thread::sleep_for(std::chrono::milliseconds(10));

}

TutorialGame::~TutorialGame() {
	delete cubeMesh;
	delete sphereMesh;
	delete gooseMesh;
	delete basicTex;
	delete basicShader;

	delete physics;
	delete renderer;
	delete world;
}



void TutorialGame::UpdateGame(float dt) {
	
	
	serverTick();
	
	
	UpdateKeys();
	game(dt);
	

	AppleInitworldobjects("TestGrid1.txt");

	SelectObject();
	MoveSelectedObject();
	

	world->UpdateWorld(dt);
	renderer->Update(dt);
	physics->Update(dt);
	Debug::FlushRenderables();
	renderer->Render();

	HightScores();

	
}

void TutorialGame::HightScores() {
	std::ifstream myfile(Assets::DATADIR + "Hightscores.txt");
	string line;

	while (std::getline(myfile, line))
	{
		std::stringstream ss;
		string temp;
		int number;
		ss << line;
		while (!ss.eof())
		{
			ss >> temp;

			if (std::stringstream(temp) >> number)
				scores.push_back(number);
			else
				names.push_back(temp);

			temp = "";
		}

	}
	myfile.close();

	vector<int> sortedscores = scores;
	std::sort(sortedscores.begin(), sortedscores.end());
	std::reverse(sortedscores.begin(), sortedscores.end());


	for (int i = 0; i < sortedscores.size(); i++)
	{
		int j = 0;
		while (true)
		{
			if (sortedscores[i] == scores[j]) {
				IndexScores.push_back(j);
				break;
			}

			j++;
		}

	}

	std::ofstream outFile;
	outFile.open(Assets::DATADIR + "Hightscores.txt", std::ofstream::trunc);

	for (size_t i = 0; i < IndexScores.size(); i++)
	{
		string string = names[IndexScores[i]] + " " + std::to_string(scores[IndexScores[i]]) + "\n";
		outFile << string;
		renderer->DrawString("HaightScore", Vector2(400, 325));
		renderer->DrawString(string, Vector2(400, 300 - (25 * i)));
	}
	outFile.close();

	IndexScores.clear();
	names.clear();
	scores.clear();
	sortedscores.clear();
}

void TutorialGame::game(float dt) {
	if (MainMenu) {
		Window::GetWindow()->ShowOSPointer(true);
		Window::GetWindow()->LockMouseToWindow(false);

		world->GetMainCamera()->SetPosition(Menu->GetTransform().GetWorldPosition() + Vector3(0, 0, 10));
		world->GetMainCamera()->SetPitch(0);
		world->GetMainCamera()->SetYaw(0);


		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {
			Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());
			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true)) {
				selectionObject = (GameObject*)closestCollision.node;
				if (selectionObject->GetName() == "start") {
					MainMenu = false;
					play = true;
				}
				if (selectionObject->GetName() == "easy") {
					man->seteasy();;
					MainMenu = false;
					play = true;
				}
				if (selectionObject->GetName() == "Load") {
					LoadGame();
					
					MainMenu = false;
					play = true;
				}
			}

		}
	}

	if (play) {
		Window::GetWindow()->ShowOSPointer(false);
		Window::GetWindow()->LockMouseToWindow(true);
		Totaltime += dt;
		renderer->DrawString(std::to_string(Totaltime),Vector2(400,400));
		world->GetMainCamera()->SetPosition(goose->GetTransform().GetWorldPosition() + Vector3(0, 60, 40));
		world->GetMainCamera()->SetPitch(-60);
		world->GetMainCamera()->SetYaw(0);
		statemachine->Update(dt);
		goose->goosebehave(Screen);

		if (firsframecheck < 1) {
			useGravity = true;
			physics->UseGravity(useGravity);
			firsframecheck++;
		}
		if (man->tuched > 0) {
			applesinworld = applesinworld - goose->ApplesEaten;
			man->reset();
			goose->cleenbag();
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::M)) {
			
			names.push_back("Alex");
			scores.push_back(goose->totalApp);

			SaveGame();

			
			play = false;
			MainMenu = true;

			world->GetMainCamera()->SetPosition(Menu->GetTransform().GetWorldPosition() + Vector3(0, 0, 10));
			world->GetMainCamera()->SetPitch(0);
			world->GetMainCamera()->SetYaw(0);
		}

	}

}

void TutorialGame::UpdateKeys() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F1)) {
		InitWorld(); //We can reset the simulation at any time with F1
		selectionObject = nullptr;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F2)) {
		InitCamera(); //F2 will reset the camera to a specific default place
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::G)) {
		useGravity = !useGravity; //Toggle gravity!
		physics->UseGravity(useGravity);
	}
	//Running certain physics updates in a consistent order might cause some
	//bias in the calculations - the same objects might keep 'winning' the constraint
	//allowing the other one to stretch too much etc. Shuffling the order so that it
	//is random every frame can help reduce such bias.
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9)) {
		world->ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F10)) {
		world->ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F7)) {
		world->ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F8)) {
		world->ShuffleObjects(false);
	}

	if (lockedObject) {
		LockedObjectMovement();
	}
	else {
		DebugObjectMovement();
	}
}





void TutorialGame::LockedObjectMovement() {
	Matrix4 view = world->GetMainCamera()->BuildViewMatrix();
	Matrix4 camWorld = view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!

	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
		selectionObject->GetPhysicsObject()->AddForce(-rightAxis);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
		selectionObject->GetPhysicsObject()->AddForce(rightAxis);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
		selectionObject->GetPhysicsObject()->AddForce(fwdAxis);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
		selectionObject->GetPhysicsObject()->AddForce(-fwdAxis);
	}
}

void  TutorialGame::LockedCameraMovement() {
	if (lockedObject != nullptr) {
		Vector3 objPos = lockedObject->GetTransform().GetWorldPosition();
		Vector3 camPos = objPos + lockedOffset;

		Matrix4 temp = Matrix4::BuildViewMatrix(camPos, objPos, Vector3(0, 1, 0));

		Matrix4 modelMat = temp.Inverse();

		Quaternion q(modelMat);
		Vector3 angles = q.ToEuler(); //nearly there now!

		world->GetMainCamera()->SetPosition(camPos);
		world->GetMainCamera()->SetPitch(angles.x);
		world->GetMainCamera()->SetYaw(angles.y);
	}
}




void TutorialGame::DebugObjectMovement() {
	//If we've selected an object, we can manipulate it with some key presses
	if (inSelectionMode && selectionObject) {
		//Twist the selected object!
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(-10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM7)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM8)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, -10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, -10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, 10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM5)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
		}
	}
}



bool TutorialGame::SelectObject() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q)) {

		inSelectionMode = !inSelectionMode;

		if (inSelectionMode) {
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else {
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
	if (inSelectionMode) {
		renderer->DrawString("Press Q to change to camera mode!", Vector2(10, 0));
		string text = "Object pos:\n (" + std::to_string(Dvector.x) + "," + std::to_string(Dvector.y) + "," + std::to_string(Dvector.z)
			+ ")";
		renderer->DrawString(text, Vector2(0, 40));
		renderer->DrawString("Layer: (" + std::to_string(DLayer) + ") Name : (" + DName + ")", Vector2(0, 20));

		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {
			if (selectionObject) {	//set colour to deselected;
				selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
				selectionObject = nullptr;
			}
			
			Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());
			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true)) {
				selectionObject = (GameObject*)closestCollision.node;
				selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
				DName = selectionObject->GetName();
				DLayer = selectionObject->GetLayer();
				Dvector = selectionObject->GetTransform().GetWorldPosition();


				return true;
			}
			else {
				return false;
			}
		}
		if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::L)) {
			if (selectionObject) {
				if (lockedObject == selectionObject) {
					lockedObject = nullptr;
				}
				else {
					lockedObject = selectionObject;
				}
			}
		}
	}
	else {
		renderer->DrawString("Press Q to change to select mode!", Vector2(10, 0));
	}
	return false;
}

/*
If an object has been clicked, it can be pushed with the right mouse button, by an amount
determined by the scroll wheel. In the first tutorial this won't do anything, as we haven't
added linear motion into our physics system. After the second tutorial, objects will move in a straight
line - after the third, they'll be able to twist under torque aswell.
*/


void TutorialGame::MoveSelectedObject() {
	//renderer->DrawString(" Click Force :" + std::to_string(forceMagnitude),
		//Vector2(10, 20)); // Draw debug text at 10 ,20
	forceMagnitude += Window::GetMouse()->GetWheelMovement() * 100.0f;

	if (!selectionObject) {
		return;// we haven ’t selected anything !

	}
	// Push the selected object !
	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::RIGHT)) {
		Ray ray = CollisionDetection::BuildRayFromMouse(
			*world->GetMainCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {
			if (closestCollision.node == selectionObject) {
				selectionObject->GetPhysicsObject()->AddForceAtPosition(ray.GetDirection() * forceMagnitude, closestCollision.collidedAt);
			}

		}

	}
}

void TutorialGame::InitCamera() {
	world->GetMainCamera()->SetNearPlane(0.5f);
	world->GetMainCamera()->SetFarPlane(500.0f);
	world->GetMainCamera()->SetPitch(-15.0f);
	world->GetMainCamera()->SetYaw(315.0f);
	world->GetMainCamera()->SetPosition(Vector3(-60, 40, 60));
	lockedObject = nullptr;
}

void TutorialGame::InitWorld() {
	world->ClearAndErase();
	physics->Clear();

	generateworld("TestGrid1.txt");
	Initworldobjects("TestGrid1.txt");
}

void TutorialGame::SaveGame()
{
	std::ofstream myfile(Assets::DATADIR + "SaveGame.txt");
	myfile << goose->ApplesEaten;
	myfile << "\n";
	myfile << goose->_lvl;
	myfile << "\n";
	myfile << goose->jumplvl;
	myfile << "\n";
	myfile << goose->sprintlvl;
	myfile << "\n";
	myfile << goose->touch;
	myfile << "\n";
	myfile << goose->Applesatspawn;
	myfile << "\n";
	myfile << goose->totalApp;
	myfile << "\n";
	myfile << goose->musicheck;
	myfile << "\n";
	myfile << man->easy;
	myfile.close();
}
void TutorialGame::LoadGame() {
	std::ifstream infile(Assets::DATADIR + "SaveGame.txt");
	int ApplesEaten;
	float _lvl;
	int jumplvl;
	int sprintlvl;
	bool touch;
	float Applesatspawn;
	int totalApp;
	int musicheck;
	bool easy;
	infile >> ApplesEaten;
	infile >> _lvl;
	infile >> jumplvl;
	infile >> sprintlvl;
	infile >> touch;
	infile >> Applesatspawn;
	infile >> totalApp;
	infile >> musicheck;
	infile >> easy;
	infile.close();
	goose->loadfromfile(ApplesEaten,_lvl,jumplvl,sprintlvl,touch,Applesatspawn,totalApp,musicheck);
	man->loadfromfile(easy);
}

void NCL::CSC8503::TutorialGame::generateworld(const std::string& filename)
{
	std::srand(time(0));
	std::ofstream myfile(Assets::DATADIR + filename);
	int hight = 20;
	int width = 20;
	int numfreespace = 0;


	myfile << 10;
	myfile << "\n";
	myfile << width;
	myfile << "\n";
	myfile << hight;
	myfile << "\n";

	for (int i = 0; i < width; i++)
	{
		myfile << "x";
	}

	for (size_t y = 0; y < hight - 2; y++)
	{
		myfile << "\n";
		myfile << "x";
		for (size_t i = 0; i < width - 2; i++)
		{
			if (i == 0 && y == 0) {
				myfile << "x";
			}
			else
			{


				if ((std::rand() % 4 == 1 && i > 0) && i != 0) {
					myfile << "x";
				}
				else
				{
					myfile << ".";
					numfreespace++;
				}
			}

		}
		myfile << "x";
	}
	myfile << "\n";
	for (int i = 0; i < width; i++)
	{
		myfile << "x";
	}
	myfile << "\n";
	myfile << numfreespace;
	myfile.close();
}



GameObject* TutorialGame::AddFloorToWorld(const Vector3& position, int nodesize) {
	GameObject* floor = new GameObject("floor");

	Vector3 floorSize = Vector3(nodesize* nodesize, 2, nodesize* nodesize);
	AABBVolume* volume = new AABBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform().SetWorldScale(floorSize);
	floor->GetTransform().SetWorldPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, groundTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(floor);

	return floor;
}


GameObject* TutorialGame::AddSphereToWorld(const Vector3& position, float radius, float inverseMass) {
	GameObject* sphere = new GameObject("", 1);

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);
	sphere->GetTransform().SetWorldScale(sphereSize);
	sphere->GetTransform().SetWorldPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(sphere);

	return sphere;
}

GameObject* TutorialGame::AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, string name = "",int Layer) {
	GameObject* cube = new GameObject(name, Layer);

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetWorldPosition(position);
	cube->GetTransform().SetWorldScale(dimensions);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, GrounTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

GameObject* TutorialGame::AddStartButToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, int Layer = 0) {
	GameObject* cube = new GameObject("start", Layer);

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetWorldPosition(position);
	cube->GetTransform().SetWorldScale(dimensions);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, startbut, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}
GameObject* TutorialGame::AddEasyButToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, int Layer = 0) {
	GameObject* cube = new GameObject("easy", Layer);

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetWorldPosition(position);
	cube->GetTransform().SetWorldScale(dimensions);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, EasyButton, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

GameObject* TutorialGame::AddBackMenuToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, string name = "", int Layer = 0) {
	GameObject* cube = new GameObject(name, Layer);

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetWorldPosition(position);
	cube->GetTransform().SetWorldScale(dimensions);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, BackMenu, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

GameObject* TutorialGame::AddChestToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, string name = "") {
	GameObject* cube = new GameObject(name, 0);

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetWorldPosition(position);
	cube->GetTransform().SetWorldScale(dimensions);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, ChestTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

GooseGameObject* TutorialGame::AddGooseToWorld(const Vector3& position)
{
	float size = 1.0f;
	float inverseMass = 1.0f;

	GooseGameObject* goose = new GooseGameObject("goose");


	SphereVolume* volume = new SphereVolume(size);
	goose->SetBoundingVolume((CollisionVolume*)volume);

	goose->GetTransform().SetWorldScale(Vector3(size, size, size));
	goose->GetTransform().SetWorldPosition(position);

	goose->SetRenderObject(new RenderObject(&goose->GetTransform(), gooseMesh, nullptr, basicShader));
	goose->SetPhysicsObject(new PhysicsObject(&goose->GetTransform(), goose->GetBoundingVolume()));

	goose->GetPhysicsObject()->SetInverseMass(inverseMass);
	goose->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(goose);

	return goose;
}

EnemyGameObject* TutorialGame::AddParkKeeperToWorld(const Vector3& position)
{
	float meshSize = 4.0f;
	float inverseMass = 0.5f;

	EnemyGameObject* keeper = new EnemyGameObject("",0,goose, Chest,easytarg);

	AABBVolume* volume = new AABBVolume(Vector3(0.3, 0.9f, 0.3) * meshSize);
	keeper->SetBoundingVolume((CollisionVolume*)volume);

	keeper->GetTransform().SetWorldScale(Vector3(meshSize, meshSize, meshSize));
	keeper->GetTransform().SetWorldPosition(position);

	keeper->SetRenderObject(new RenderObject(&keeper->GetTransform(), keeperMesh, KeeperTex, basicShader));
	keeper->SetPhysicsObject(new PhysicsObject(&keeper->GetTransform(), keeper->GetBoundingVolume()));

	keeper->GetPhysicsObject()->SetInverseMass(inverseMass);
	keeper->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(keeper);

	return keeper;
}

GameObject* TutorialGame::AddCharacterToWorld(const Vector3& position) {
	float meshSize = 4.0f;
	float inverseMass = 0.5f;

	auto pos = keeperMesh->GetPositionData();

	Vector3 minVal = pos[0];
	Vector3 maxVal = pos[0];

	for (auto& i : pos) {
		maxVal.y = max(maxVal.y, i.y);
		minVal.y = min(minVal.y, i.y);
	}

	GameObject* character = new GameObject();

	float r = rand() / (float)RAND_MAX;


	AABBVolume* volume = new AABBVolume(Vector3(0.3, 0.9f, 0.3) * meshSize);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform().SetWorldScale(Vector3(meshSize, meshSize, meshSize));
	character->GetTransform().SetWorldPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), r > 0.5f ? charA : charB, nullptr, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(character);

	return character;
}

AppleGameObject* TutorialGame::AddAppleToWorld(const Vector3& position) {
	AppleGameObject* apple = new AppleGameObject("apple");

	SphereVolume* volume = new SphereVolume(0.7f);
	apple->SetBoundingVolume((CollisionVolume*)volume);
	apple->GetTransform().SetWorldScale(Vector3(4, 4, 4));
	apple->GetTransform().SetWorldPosition(position);

	apple->SetRenderObject(new RenderObject(&apple->GetTransform(), appleMesh, nullptr, basicShader));
	apple->SetPhysicsObject(new PhysicsObject(&apple->GetTransform(), apple->GetBoundingVolume()));

	apple->GetPhysicsObject()->SetInverseMass(1.0f);
	apple->GetPhysicsObject()->InitSphereInertia();

	apple->GetRenderObject()->SetColour(Vector4(1, 0.2, 0.2, 1));

	world->AddGameObject(apple);

	return apple;
}

void TutorialGame::InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddSphereToWorld(position, radius, 1.0f);
		}
	}
	
}

void TutorialGame::Initworldobjects(const std::string& filename) {

	std::ifstream infile(Assets::DATADIR + filename);
	int nodeSize;
	int gridWidth;
	int gridHeight;

	infile >> nodeSize;
	infile >> gridWidth;
	infile >> gridHeight;

	for (int y = 0; y < gridHeight; ++y) {
		for (int x = 0; x < gridWidth; ++x) {
			Vector3 position = Vector3(x * nodeSize, nodeSize / 2, y * nodeSize);
			char type = 0;
			infile >> type;
			if ((x == 1) && (y == 1)) {		
			}
			else {
				if (type == 'x') {
					AddCubeToWorld(position, Vector3(nodeSize / 2, nodeSize / 2, nodeSize / 2), 0.0f);
				}
			}
			}
	}

	MenuInit();
	easytarg = AddCubeToWorld(Vector3(1 * nodeSize, nodeSize * 102, (gridHeight - 2) * nodeSize), Vector3(nodeSize / 2, nodeSize / 2, nodeSize / 2), 0.0f);
	AddChestToWorld(Vector3(nodeSize + ((nodeSize) / 4), nodeSize / 4, nodeSize + ((nodeSize) / 4)), Vector3((nodeSize / 2)/3, (nodeSize / 2)/3, (nodeSize / 2)/3), 0.0f, "chest");
	Chest = AddCubeToWorld(Vector3(nodeSize,nodeSize*102,nodeSize*2), Vector3(nodeSize / 2, nodeSize / 2, nodeSize / 2), 0.0f);
	Vector3 position = Vector3(nodeSize, nodeSize, nodeSize);
	Apple = AddAppleToWorld(Vector3(0, -19999, 0));
	goose = AddGooseToWorld(Vector3(1 * nodeSize, nodeSize / 2, 2 * nodeSize));
	man = AddParkKeeperToWorld(Vector3(1 * nodeSize, nodeSize / 2, (gridHeight - 2)* nodeSize));
	
	AddFloorToWorld(Vector3((nodeSize*nodeSize)-(nodeSize/2), -2, (nodeSize*nodeSize)-(nodeSize/2)), nodeSize);
	oldvalspawn = goose->Applesatspawn;
}

void TutorialGame::MenuInit() {
	
	Menu = AddCubeToWorld(Vector3(0, 10000, 0), Vector3(0.00000000001, 0.00000000001, 0), 0.0f);
	Vector3 a = Menu->GetTransform().GetWorldPosition();

	AddBackMenuToWorld(a + Vector3(0, 0, -5), Vector3(12, 7, 0), 0.0f,"",4);

	AddStartButToWorld(a + Vector3(0, -2, 0), Vector3(2, 1, 0), 0.0f);
	AddEasyButToWorld(a + Vector3(-4, -2, 0), Vector3(1, 0.5, 0), 0.0f);
	AddCubeToWorld(a + Vector3(4, -2, 0), Vector3(1, 0.5, 0), 0.0f,"Load");
}

void TutorialGame::AppleInitworldobjects(const std::string& filename) {
		std::ifstream infile(Assets::DATADIR + filename);
	int nodeSize;
	int gridWidth;
	int gridHeight;

	int rundnumber = std::rand() % numofblank;
	int curnum = 0;

	infile >> nodeSize;
	infile >> gridWidth;
	infile >> gridHeight;

	if (goose->touch) {
		applesinworld--;
		goose->SetBack();
	}

	for (int y = 0; y < gridHeight; ++y) {
		for (int x = 0; x < gridWidth; ++x) {
			Vector3 position = Vector3(x * nodeSize, nodeSize / 2, y * nodeSize);
			char type = 0;
			infile >> type;

			if ((type == '.' && (goose->ApplesEaten + applesinworld) < 7) && (x != 1) &&(y != 1) ){
				if (curnum == rundnumber) {
					Apple = AddAppleToWorld(position);
					applesinworld++;
				}
				curnum++;
			}

		}
	}
	infile >> numofblank;
}


void TutorialGame::InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims) {
	for (int x = 1; x < numCols + 1; ++x) {
		for (int z = 1; z < numRows + 1; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddCubeToWorld(position, cubeDims, 1.0f);
		}
	}

}

void TutorialGame::BridgeConstraintTest() {
	Vector3 cubeSize = Vector3(8, 8, 8);

	float	invCubeMass = 5;
	int		numLinks = 25;
	float	maxDistance = 30;
	float	cubeDistance = 20;

	Vector3 startPos = Vector3(500, 1000, 500);

	GameObject* start = AddCubeToWorld(startPos + Vector3(0, 0, 0), cubeSize, 0);

	GameObject* end = AddCubeToWorld(startPos + Vector3((numLinks + 2) * cubeDistance, 0, 0), cubeSize, 0);

	GameObject* previous = start;

	for (int i = 0; i < numLinks; ++i) {
		GameObject* block = AddCubeToWorld(startPos + Vector3((i + 1) * cubeDistance, 0, 0), cubeSize, invCubeMass);
		PositionConstraint* constraint = new PositionConstraint(previous, block, maxDistance);
		world->AddConstraint(constraint);
		previous = block;
	}

	PositionConstraint* constraint = new PositionConstraint(previous, end, maxDistance);
	world->AddConstraint(constraint);
}

void TutorialGame::SimpleGJKTest() {
	Vector3 dimensions = Vector3(5, 5, 5);
	Vector3 floorDimensions = Vector3(100, 2, 100);

	GameObject* fallingCube = AddCubeToWorld(Vector3(0, 20, 0), dimensions, 10.0f);
	GameObject* newFloor = AddCubeToWorld(Vector3(0, 0, 0), floorDimensions, 0.0f);

	delete fallingCube->GetBoundingVolume();
	delete newFloor->GetBoundingVolume();

	fallingCube->SetBoundingVolume((CollisionVolume*)new OBBVolume(dimensions));
	newFloor->SetBoundingVolume((CollisionVolume*)new OBBVolume(floorDimensions));

}

