#pragma once
#include "../Main.hpp"

namespace IL2CPP
{
	struct Domain : private IMPORT::Il2CppDomain
	{
		/**
		* Attempts to open and retrieve the image of the specified assembly. 
		* 
		* @param assemblyName: The name of the assembly.
		* @throw NoSuchImage: Thrown when the function failed to find assembly with the specified name.
		* 
		* @returns A pointer to the Image object associated with the specified assembly.
		*/
		Image* OpenAssembly(const std::string& assemblyName);

		std::vector<Image*> OpenEveryAssembly();
	};
}