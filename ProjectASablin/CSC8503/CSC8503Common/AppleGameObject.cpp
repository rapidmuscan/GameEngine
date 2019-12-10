#include "AppleGameObject.h"

NCL::CSC8503::AppleGameObject::AppleGameObject(string objectName, int _Layer) : GameObject()
{
	name = objectName;
	Layer = _Layer;
}
