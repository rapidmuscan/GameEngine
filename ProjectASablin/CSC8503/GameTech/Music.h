#pragma once

#include <string>
#include <iostream>

using namespace std;

#include "audiere.h"

using namespace audiere;

class Music
{
public:
	Music();

	void play() {
		string filename = "thegrom-kanal-donat-give-me-your-money.mp3";
		AudioDevicePtr device = OpenDevice();
		OutputStreamPtr sound(OpenSound(device, filename.c_str(), false));

		sound->play();
		sound->setRepeat(true);
		sound->setVolume(1.0f);
	}

};

