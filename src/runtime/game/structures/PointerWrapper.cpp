#include "PointerWrapper.hpp"

int gTotalFailedPointerDef = 0;

bool PointerBase::Define()
{
	try
	{
		if (definatorType == DefinatorType::INDEX)
		{
			this->ptr = IL2CPP::ClassMapping::GetClass(className)->GetMethod(index)->GetPointer();
		}
		else if (definatorType == DefinatorType::NAME)
		{
			this->ptr = IL2CPP::ClassMapping::GetClass(className)->GetMethod(methodName)->GetPointer();
		}
		else if (definatorType == DefinatorType::PATTERN)
		{
			this->ptr = IL2CPP::ClassMapping::GetClass(className)->GetMethodByPattern(pattern, index)->GetPointer();
		}
	}
	catch (const IL2CPP::Exception::ExceptionBase& exception)
	{
		gTotalFailedPointerDef++;
		return false;
	}

	return true;
}