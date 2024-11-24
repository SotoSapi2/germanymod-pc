#include "GameBootstrap.hpp"
#include "ClassFinder.hpp"
#include "PointerWrapper.hpp"

#include "classes/MouseFix.hpp"
#include "classes/WeaponSounds.hpp"
#include "classes/Websocket.hpp"
#include "classes/WeaponManager.hpp"
#include "classes/ProgressUpdater.hpp"

namespace GameBootstrap
{
	void INIT()
	{
		ClassFinder::INIT();
		PointerWrapperManager::INIT();

		MouseFix::INIT();
		WeaponManager::INIT();
		WeaponSounds::INIT();

		ProgressUpdater::INIT();
		Websocket::INIT();
	}
}
