#include "NavigationPath.h"
#include "Debug.h"


using namespace NCL::CSC8503;

void NavigationPath::DebugDraw() {
	for (int i = 1; i < waypoints.size(); ++i) {
		Vector3 a = waypoints[i - 1];
		Vector3 b = waypoints[i];
		Debug::DrawLine(a, b, Vector4(1, 0.55, 0, 1));
	}
}