#include "CommonCShrap.hpp"

namespace IL2CPP
{
	namespace DefaultTypeClass
	{
		Class* Void;
		Class* Type;
		Class* Boolean;
		Class* Byte;
		Class* Int16;
		Class* Int32;
		Class* Int64;
		Class* Float;
		Class* Double;
		Class* Decimal;
		Class* Char;
		Class* String;
		Class* Object;
		Class* Array;
	}

	namespace DefaultImage
	{
		const Image* Corlib;
		const Image* System;
		const Image* UnityCoreModule;
		const Image* UnityPhysicsModule;
		const Image* AssemblyCSharp;
	}

	namespace CommonCShrap
	{
		void INIT()
		{
			Domain* domain = CurrentDomain();

			DefaultImage::Corlib = (Image*)IMPORT::il2cpp_get_corlib();
			DefaultImage::System = domain->OpenAssembly("System.dll");
			DefaultImage::UnityCoreModule = domain->OpenAssembly("UnityEngine.CoreModule.dll");
			DefaultImage::UnityPhysicsModule = domain->OpenAssembly("UnityEngine.PhysicsModule.dll");
			DefaultImage::AssemblyCSharp = domain->OpenAssembly("Assembly-CSharp.dll");

			#define DEFAULTS_INIT(field, ns, n) DefaultTypeClass::field = (Class*)IMPORT::il2cpp_class_from_name(IMPORT::il2cpp_get_corlib(), ns, n)
			DEFAULTS_INIT(Void, "System", "Void");
			DEFAULTS_INIT(Type, "System", "Type");
			DEFAULTS_INIT(Boolean, "System", "Boolean");
			DEFAULTS_INIT(Byte, "System", "Byte");
			DEFAULTS_INIT(Int16, "System", "Int16");
			DEFAULTS_INIT(Int32, "System", "Int32");
			DEFAULTS_INIT(Int64, "System", "Int64");
			DEFAULTS_INIT(Float, "System", "Single");
			DEFAULTS_INIT(Double, "System", "Double");
			DEFAULTS_INIT(Decimal, "System", "Decimal");
			DEFAULTS_INIT(Char, "System", "Char");
			DEFAULTS_INIT(String, "System", "String");
			DEFAULTS_INIT(Object, "System", "Object");
			DEFAULTS_INIT(Array, "System", "Array");

			#undef DEFAULTS_INIT
		}
	}
}