#include "CommonCShrap.hpp"
#include "il2cpp/api-functions.hpp"

namespace CSharpType
{
	Il2CppClass* Type = nullptr;
	Il2CppClass* Boolean = nullptr;
	Il2CppClass* Byte = nullptr;
	Il2CppClass* Int16 = nullptr;
	Il2CppClass* Int32 = nullptr;
	Il2CppClass* Int64 = nullptr;
	Il2CppClass* Float = nullptr;
	Il2CppClass* Double = nullptr;
	Il2CppClass* Decimal = nullptr;
	Il2CppClass* Char = nullptr;
	Il2CppClass* String = nullptr;
	Il2CppClass* Object = nullptr;
	Il2CppClass* Array = nullptr;
}

namespace CommonLibImages
{
	const Il2CppImage* Corlib;
	const Il2CppImage* System;
	const Il2CppImage* UnityCoreModule;
	const Il2CppImage* UnityPhysicsModule;
	const Il2CppImage* AssemblyCSharp;
	const Il2CppImage* InputLegacyModule;
	const Il2CppImage* Photon3Unity3D;
	const Il2CppImage* SteamworksNet;
	const Il2CppImage* UserSessionManagement;
}

namespace CommonCShrap
{
	const Il2CppImage* ResolveImage(Il2CppDomain* _domain, const char* dll)
	{
		const Il2CppAssembly* assembly = nullptr;

		while (assembly == nullptr)
		{
			assembly = il2cpp_domain_assembly_open(_domain, dll);
		}

		auto img = il2cpp_assembly_get_image(assembly);
		return img;
	}

	void INIT()
	{
		Il2CppDomain* domain = il2cpp_domain_get();

		#define DEFAULTS_INIT(field, ns, n) CSharpType::field = il2cpp_class_from_name(il2cpp_get_corlib(), ns, n);

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

		CommonLibImages::Corlib = il2cpp_get_corlib();
		CommonLibImages::System = ResolveImage(domain, "System.dll");
		CommonLibImages::AssemblyCSharp = ResolveImage(domain, "Assembly-CSharp.dll");
		CommonLibImages::UnityCoreModule = ResolveImage(domain, "UnityEngine.CoreModule.dll");
		CommonLibImages::UnityPhysicsModule = ResolveImage(domain, "UnityEngine.PhysicsModule.dll");
		CommonLibImages::InputLegacyModule = ResolveImage(domain, "UnityEngine.InputLegacyModule.dll");
		CommonLibImages::Photon3Unity3D = ResolveImage(domain, "Photon3Unity3D.dll");
		CommonLibImages::SteamworksNet = ResolveImage(domain, "com.rlabrecque.steamworks.net.dll");
		CommonLibImages::UserSessionManagement = ResolveImage(domain, "UserSessionManagement.dll");
	}
}