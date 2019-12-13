#pragma once
#include "GameClient.h"
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



class HaightScoreReciever : public PacketReceiver
{
public:
	HaightScoreReciever(std::string name) {
		this->name = name;
	}

	//void ReceivePacket(int type, GamePacket* payload, int source) {
	//	/*if (type == String_Message) {
	//		StringPacket* realPacket = (StringPacket*)payload;
	//		std::string msg = realPacket->GetStringFromData();
	//		std::cout << msg << std::endl;
	//		
	//		vector<int> scores;
	//		vector<string> names;

	//		std::ofstream myfile;

	//		myfile.open(NCL::Assets::DATADIR + "Hightscores.txt");

	//		std::stringstream ss;
	//		ss << msg;
	//		string temp;
	//		int number;
	//		while (!ss.eof()) {
	//			ss >> temp;

	//			if (std::stringstream(temp) >> number)
	//				scores.push_back(number);
	//			else
	//				names.push_back(temp);



	//			temp = "";
	//		}



	//		for (int i = 0; i < scores.size(); ++i) {
	//			string output = names[i] + " " + std::to_string(scores[i]) + "\n";
	//			myfile << output;
	//		}
	//	}*/
	//}
protected:
	std::string name;
};

