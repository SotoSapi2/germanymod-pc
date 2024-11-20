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

namespace CommonCShrap
{
	void INIT()
	{
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
	}
}