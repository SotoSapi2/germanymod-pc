#pragma once
#include "../Main.hpp"

namespace IL2CPP
{
	namespace DefaultTypeClass
	{
		extern Class* Void;
		extern Class* Type;
		extern Class* Boolean;
		extern Class* Byte;
		extern Class* Int16;
		extern Class* Int32;
		extern Class* Int64;
		extern Class* Float;
		extern Class* Double;
		extern Class* Decimal;
		extern Class* Char;
		extern Class* String;
		extern Class* Object;
		extern Class* Array;
	}

	namespace DefaultImage
	{
		extern const Image* Corlib;
		extern const Image* System;
		extern const Image* UnityCoreModule;
		extern const Image* UnityPhysicsModule;
		extern const Image* AssemblyCSharp;
	}

	namespace CommonCShrap
	{
		void INIT();
	}
}
