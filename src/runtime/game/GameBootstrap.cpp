#include "GameBootstrap.hpp"
#include "ClassFinder.hpp"
#include "PointerWrapper.hpp"

#include "classes/MouseFix.hpp"
#include "classes/WeaponSounds.hpp"
#include "classes/Websocket.hpp"

namespace GameBootstrap
{
	void INIT()
	{
		ClassFinder::INIT();
		PointerWrapperManager::INIT();

		MouseFix::INIT();
		WeaponSounds::INIT();
		Websocket::INIT();
	}
}
