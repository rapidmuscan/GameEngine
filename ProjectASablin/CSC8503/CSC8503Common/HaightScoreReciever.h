#pragma once
#include "GameClient.h"
#include<iostream>

class HaightScoreReciever : public PacketReceiver
{
public:
	HaightScoreReciever(std::string name) {
		this->name = name;
	}

	void ReceivePacket(int type, GamePacket* payload, int source) {
		if (type == String_Message) {
			StringPacket* realPacket = (StringPacket*)payload;
			std::string msg = realPacket->GetStringFromData();
			std::cout << name << " received message : " << msg << std::endl;
		}
	}
protected:
	std::string name;
};

