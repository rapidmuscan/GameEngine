#include "EnemyGameObject.h"

NCL::CSC8503::EnemyGameObject::EnemyGameObject(string Objectname, int _Layer, GameObject* _target) : grid{ "TestGrid1.txt" }
{
	target = _target;
	name = Objectname;
	Layer = _Layer;
}

void EnemyGameObject::EnemyLogic(float dt)
{
	if (Layer == target->GetLayer()) {
		buff = target->GetLvl();
		int maxrange = 40;
		float Difinx = std::abs(target->GetTransform().GetWorldPosition().x - GetTransform().GetWorldPosition().x);
		float Difinz = std::abs(target->GetTransform().GetWorldPosition().z - GetTransform().GetWorldPosition().z);
		if ((Difinx > 4 || Difinz > 4) && (Difinx < (maxrange + (maxrange * buff / 100)) && Difinz < (maxrange + (maxrange * buff / 100))))
		{
			folowingwithalg(dt, target, 80 + (80 * buff / 100));
		}
		else if ((Difinx < 4 && Difinz < 4))
		{
			Vector3 direction = (target->GetTransform().GetWorldPosition() - GetTransform().GetWorldPosition()).Normalised();
			GetPhysicsObject()->AddForce(direction * 90);
		}
	}
}

void EnemyGameObject::FolowingEnemy(float dt, void* data)
{
	EnemyGameObject* object = static_cast<EnemyGameObject*>(data);
	object->buff = object->target->GetLvl();
	object->folowingwithalg(dt, object->target, 80 + (80 * object->buff / 100));
}

void EnemyGameObject::OtherThing(float dt, void* data)
{
}

void EnemyGameObject::folowingwithalg(float dt, GameObject* target,int speed )
{
	testTime += dt;
	Vector3 startPos = GetTransform().GetWorldPosition(); startPos.y = 0.0f;

	if (testTime > 3.0f) {
		testTime = 0.0f;


		Vector3 endPos = target->GetTransform().GetWorldPosition();
		std::cout << "StartPos " << startPos << std::endl;
		std::cout << "endPos " << endPos << std::endl;
		outPath.Clear();
		bool found = grid.FindPath(startPos, endPos, outPath);
		if (!found) {
			std::cout << "Path not found" << std::endl;
		}
		else {
			outPath.DebugDraw();
			outPath.PopWaypoint(targetposition);

			if ((startPos - targetposition).Length() < 2) {
				outPath.PopWaypoint(targetposition);
			}
		}
	}
	outPath.DebugDraw();




	if ((startPos - targetposition).Length() < 2) {
		outPath.PopWaypoint(targetposition);
		testTime = 1000;
	}
	Vector3 direction = (targetposition - startPos).Normalised();
	GetPhysicsObject()->AddForce(direction * speed);

	Debug::DrawLine(startPos, targetposition, Vector4(1, 0.55, 0, 1));
	Debug::DrawLine(startPos, startPos + Vector3(0, 100, 0), Vector4(1, 0, 0, 1));
	Debug::DrawLine(targetposition, targetposition + Vector3(0, 100, 0), Vector4(1, 1, 0, 1));
}
