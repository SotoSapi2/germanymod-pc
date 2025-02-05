#include "Image.hpp"

namespace IL2CPP
{
	std::string Image::GetName() const
	{
		return this->name;
	}

	size_t Image::ClassCount() const
	{
		return this->typeCount;
	}

	void Image::IterateClasses(const std::function<void(Class*)>& callback) const
	{
		for (size_t i = 0; i < ClassCount(); i++)
		{
			Class* klass = (Class*)IMPORT::il2cpp_image_get_class(this, i);
			callback(klass);
		}
	}

	Class* Image::GetClass(size_t index) const
	{
		if (index >= ClassCount())
		{
			throw Exception::IndexOutOfBounds(index, ClassCount());
		}

		return (Class*)IMPORT::il2cpp_image_get_class(this, index);
	}

	Class* Image::GetClass(const std::string& fullname) const
	{
		for (size_t i = 0; i < ClassCount(); i++)
		{
			Class* klass = (Class*)IMPORT::il2cpp_image_get_class(this, i);

			if (klass->GetFullName() == fullname)
			{
				return klass;
			}
		}

		throw Exception::NoSuchClass(
			this->name,
			fullname
		);

		return nullptr;
	}
}