#include "Domain.hpp"

namespace IL2CPP
{
	Image* Domain::OpenAssembly(const std::string& assemblyName)
	{
		const IMPORT::Il2CppAssembly* result = IMPORT::il2cpp_domain_assembly_open(
			this,
			assemblyName.c_str()
		);

		if (result == nullptr)
		{
			throw Exception::NoSuchImage(assemblyName);
		}

		return (Image*)result->image;
	}

	std::vector<Image*> Domain::OpenEveryAssembly()
	{
		std::vector<Image*> out;

		size_t size;
		const IMPORT::Il2CppAssembly** result = IMPORT::il2cpp_domain_get_assemblies(
			this,
			&size
		);

		for (size_t i = 0; i < size; i++)
		{
			out.push_back((Image*)result[i]->image);
		}

		return out;
	}
}