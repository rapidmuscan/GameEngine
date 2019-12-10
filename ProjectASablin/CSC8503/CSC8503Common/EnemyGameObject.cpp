#include "EnemyGameObject.h"

NCL::CSC8503::EnemyGameObject::EnemyGameObject(string Objectname, int _Layer) : grid{ "TestGrid1.txt" }
{
	name = Objectname;
	Layer = _Layer;
}

void EnemyGameObject::EnemyLogic(float dt, GameObject* target, int maxrange, int speed)
{
	float Difinx = std::abs(target->GetTransform().GetWorldPosition().x - GetTransform().GetWorldPosition().x);
	float Difinz = std::abs(target->GetTransform().GetWorldPosition().y - GetTransform().GetWorldPosition().y);
	if ((Difinx > 4 || Difinz > 4) && (Difinx < maxrange && Difinz < maxrange))
	{
		folowingwithalg(dt, target);
	}
	else if ((Difinx < 4 && Difinz < 4))
	{
		Vector3 direction = (target->GetTransform().GetWorldPosition() - GetTransform().GetWorldPosition()).Normalised();
		GetPhysicsObject()->AddForce(direction * speed);
	}
}

void EnemyGameObject::folowingwithalg(float dt, GameObject* target)
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
	GetPhysicsObject()->AddForce(direction * 50);

	Debug::DrawLine(startPos, targetposition, Vector4(1, 0.55, 0, 1));
	Debug::DrawLine(startPos, startPos + Vector3(0, 100, 0), Vector4(1, 0, 0, 1));
	Debug::DrawLine(targetposition, targetposition + Vector3(0, 100, 0), Vector4(1, 1, 0, 1));
}
