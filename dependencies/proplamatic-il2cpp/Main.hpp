#pragma once
#define IL2CPP_NAMEOF(x) #x

#include <windows.h>
#include "backend/vm-functions.hpp"
#include "backend/api-function.hpp"

#include "exceptions/ExceptionBase.hpp"
#include "exceptions/NoSuchClass.hpp"
#include "exceptions/NoSuchImage.hpp"
#include "exceptions/NoSuchMember.hpp"
#include "exceptions/NoSuchResult.hpp"
#include "exceptions/IndexOutOfBound.hpp"
#include "exceptions/NullArgument.hpp"
#include "exceptions/PatternScanFailed.hpp"
#include "exceptions/UnexpectedError.hpp"
#include "exceptions/UnresolvedMappingQueue.hpp"

#include "structures/misc/PatternStructs.hpp"

#include "structures/Typedefs.hpp"
#include "structures/Type.hpp"
#include "structures/MethodInfo.hpp"
#include "structures/FieldInfo.hpp"
#include "structures/Class.hpp"
#include "structures/Image.hpp"
#include "structures/Object.hpp"
#include "structures/Domain.hpp"

#include "service/CommonCShrap.hpp"

namespace IL2CPP 
{
	extern HMODULE GameAssembly_handle;

	/*
	* Retrieves the current game domain.
	* 
	* @returns A pointer to the current game Domain object.
	*/
	Domain* CurrentDomain();

	/*
	* Attaches the current thread to the il2cpp process. 
	* This allows the current thread to safely call game methods or API functions without causing the game to crash.
	*/
	void AttachCurrentThread();

	/*
	* Initialize the library.
	*/
	void INIT();
}