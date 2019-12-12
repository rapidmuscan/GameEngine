#include "GooseGameObject.h"


NCL::CSC8503::GooseGameObject::GooseGameObject(string objectName, int _Layer)
{

	name = objectName;
	Layer = _Layer;
}

void NCL::CSC8503::GooseGameObject::GooseIndecators(Window* w)
{
	

	float width = w->GetScreenSize().x;
	float hight = w->GetScreenSize().y;
	int step = 25;
	string text2 = "Applemoney: " + std::to_string(Applesatspawn);
	Debug::Print(text2, Vector2(0, hight - step * 7), Vector4(1, 0, 0, 1));
	string text3 = "In your Bag:" + std::to_string(ApplesEaten);
	Debug::Print(text3, Vector2(0, hight - step * 2), Vector4(0, 0, 0, 1));
	string text4 = "Your current lvl: " + std::to_string(lvl);
	Debug::Print(text4, Vector2(0, hight - step * 3), Vector4(0, 0, 0, 1));
	string text5 = "Jump lvl:" + std::to_string(jumplvl);
	Debug::Print(text5, Vector2(0, hight - step * 4), Vector4(0, 0, 0, 1));
	string text6 = "Sprint lvl:" + std::to_string(sprintlvl);
	Debug::Print(text6, Vector2(0, hight - step * 5), Vector4(0, 0, 0, 1));
	string text10 = "Total Amount of Apples taken:" + std::to_string(totalApp);
	Debug::Print(text10, Vector2(0, hight - step), Vector4(0, 0, 0, 1));
	string text11 = "Music you heard:" + std::to_string(musicheck);
	Debug::Print(text11, Vector2(0, hight - step * 6), Vector4(0, 0, 0, 1));


	if (Applesatspawn >= (3.0f + (3.0f * _lvl / 10.0f))) {
		string text7 = "Lvlup is avalable!!!:";
		Debug::Print(text7, Vector2(0, hight - step * 15), Vector4(0, 0, 0, 1));
		string text8 = "(P) for jump upgrade";
		Debug::Print(text8, Vector2(0, hight - step * 16), Vector4(0, 0, 0, 1));
		string text9 = "(I) for sprint upgrade";
		Debug::Print(text9, Vector2(0, hight - step * 17), Vector4(1, 0, 0, 1));
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::P)) {
			lvlup();
			jumplvl++;
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::I)) {
			lvlup();
			sprintlvl++;
		}
	}
}
void GooseGameObject::Music() {
	int a = 2;
	if (totalApp == a * 1 && musicheck != 1)
	{
		musicheck++;
		PlaySound(TEXT("1.WAV"), NULL, SND_FILENAME | SND_ASYNC);
	}
	if (totalApp == a * 2 && musicheck != 2) {
		PlaySound(TEXT("2.WAV"), NULL, SND_FILENAME | SND_ASYNC);
		musicheck++;
	}
	if (totalApp == a * 3 && musicheck != 3) {
		PlaySound(TEXT("3.WAV"), NULL, SND_FILENAME | SND_ASYNC);
		musicheck++;
	}
	if (totalApp == a * 4 && musicheck != 4) {
		PlaySound(TEXT("4.WAV"), NULL, SND_FILENAME | SND_ASYNC);
		musicheck++;
	}
	if (totalApp == a * 5 && musicheck != 5) {
		PlaySound(TEXT("5.WAV"), NULL, SND_FILENAME | SND_ASYNC);
		musicheck++;
	}
	if (totalApp == a * 6 && musicheck != 6) {
		PlaySound(TEXT("6.WAV"), NULL, SND_FILENAME | SND_ASYNC);
		musicheck++;
	}
	if (totalApp == a * 7 && musicheck != 7) {
		PlaySound(TEXT("7.WAV"), NULL, SND_FILENAME | SND_ASYNC);
		musicheck++;
	}
	if (totalApp == a * 8 && musicheck != 8) {
		PlaySound(TEXT("8.WAV"), NULL, SND_FILENAME | SND_ASYNC);
		musicheck++;
	}
	if (totalApp == a * 9 && musicheck != 9) {
		PlaySound(TEXT("9.WAV"), NULL, SND_FILENAME | SND_ASYNC);
		musicheck++;
	}
	if (totalApp == a * 10 && musicheck != 10) {
		PlaySound(TEXT("10.WAV"), NULL, SND_FILENAME | SND_ASYNC);
		musicheck++;
	}
	if (totalApp == a * 11 && musicheck != 11) {
		PlaySound(TEXT("11.WAV"), NULL, SND_FILENAME | SND_ASYNC);
		musicheck++;
	}

}
void NCL::CSC8503::GooseGameObject::goosebehave(Window* w)
{
	GooseIndecators(w);
	Music();
	Vector3 p_w_r = GetTransform().GetLocalOrientation().ToEuler();

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
		Vector3 p_w_r = GetTransform().GetLocalOrientation().ToEuler();
		GetTransform().SetLocalOrientation(Quaternion::EulerAnglesToQuaternion(p_w_r.x, -90, p_w_r.z));
		GetPhysicsObject()->AddForce(Vector3(-40 + (40 * (sprintlvl / 6)), 0, 0));
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {

		Vector3 p_w_r = GetTransform().GetLocalOrientation().ToEuler();
		GetTransform().SetLocalOrientation(Quaternion::EulerAnglesToQuaternion(p_w_r.x, 90, p_w_r.z));
		GetPhysicsObject()->AddForce(Vector3(40 + (40 * (sprintlvl / 6)), 0, 0));
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {

		Vector3 p_w_r = GetTransform().GetLocalOrientation().ToEuler();
		GetTransform().SetLocalOrientation(Quaternion::EulerAnglesToQuaternion(p_w_r.x, 180, p_w_r.z));
		GetPhysicsObject()->AddForce(-Vector3(0, 0, 40 + (40 * (sprintlvl / 6))));
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
		Vector3 p_w_r = GetTransform().GetLocalOrientation().ToEuler();
		GetTransform().SetLocalOrientation(Quaternion::EulerAnglesToQuaternion(p_w_r.x, 0, p_w_r.z));
		GetPhysicsObject()->AddForce(Vector3(0, 0, 40 + (40 * (sprintlvl / 6))));
	}
	if (GetTransform().GetWorldPosition().y < 2) {
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::SPACE))
		{
			GetPhysicsObject()->AddForce(Vector3(0, 300 + (300 * (jumplvl / 4)), 0));
		}
	}
}
