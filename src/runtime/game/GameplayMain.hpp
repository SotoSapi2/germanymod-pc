#pragma once
#include <IL2CPP.hpp>

namespace GameplayMain
{
	extern IL2CPP::Object* gMyPlayerMoveC;
	extern IL2CPP::List<IL2CPP::Object*>* gPlayerMoveCList;

	void INIT();
}