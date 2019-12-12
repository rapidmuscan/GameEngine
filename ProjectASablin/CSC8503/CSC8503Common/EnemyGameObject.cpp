#include "EnemyGameObject.h"

NCL::CSC8503::EnemyGameObject::EnemyGameObject(string Objectname, int _Layer, GameObject* _target, GameObject* _target2, GameObject* _target3) : grid{ "TestGrid1.txt" }
{
	target = _target;
	target2 = _target2;
	target3 = _target3;
	Reftarget = _target;
	Reftarget2 = _target2;
	Reftarget3 = _target3;
	name = Objectname;
	Layer = _Layer;
}


void EnemyGameObject::FolowingEnemy(float dt, void* data)
{
	EnemyGameObject* object = static_cast<EnemyGameObject*>(data);

	if (object->Layer == object->target->GetLayer()) {
		object->buff = object->target->GetLvl();
		int maxrange = 40;
		float Difinx = std::abs(object->target->GetTransform().GetWorldPosition().x - object->GetTransform().GetWorldPosition().x);
		float Difinz = std::abs(object->target->GetTransform().GetWorldPosition().z - object->GetTransform().GetWorldPosition().z);
		float dist = sqrt((Difinz * Difinz) + (Difinx * Difinx));
		if ((dist > 4) && (dist < (maxrange + (maxrange * object->buff / 100))))
		{
			object->folowingwithalg(dt, object->target, 80 + (80 * object->buff / 100));
		}
		else if (dist <= 4)
		{
			Vector3 direction = (object->target->GetTransform().GetWorldPosition() - object->GetTransform().GetWorldPosition()).Normalised();
			object->GetPhysicsObject()->AddForce(direction * 90);
		}
	}
}

void EnemyGameObject::OtherThing(float dt, void* data)
{
	EnemyGameObject* object = static_cast<EnemyGameObject*>(data);

	object->folowingwithalg(dt, object->target2, 40);
	
}

void EnemyGameObject::folowingwithalg(float dt, GameObject* target,int speed )
{
	testTime += dt;
	Vector3 startPos = GetTransform().GetWorldPosition(); startPos.y = 0.0f;

	if (testTime > 3.0f) {
		testTime = 0.0f;


		Vector3 endPos = target->GetTransform().GetWorldPosition();
		//std::cout << "StartPos " << startPos << std::endl;
		//std::cout << "endPos " << endPos << std::endl;
		outPath.Clear();
		bool found = grid.FindPath(startPos, endPos, outPath);
		if (!found) {
			//std::cout << "Path not found" << std::endl;
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

