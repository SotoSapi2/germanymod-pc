#pragma once
#include <IL2CPP.hpp>
#include <optional>
#include <Logger.hpp>

extern int gTotalFailedPointerDef;

enum class DefinatorType : int8_t
{
	INDEX,
	NAME,
	PATTERN
};

class PointerBase
{
private:
	DefinatorType definatorType;
	void* ptr = nullptr;
	int index = 0;
	const char* className = nullptr;
	const char* methodName = nullptr;
	IL2CPP::SignaturePattern pattern{ "", nullptr, nullptr, {} };

	protected:
	PointerBase(const char* className, int index) :
		className(className),
		index(index),
		definatorType(DefinatorType::INDEX)
	{}

	PointerBase(const char* className, const char* methodName, int indexOffset = 0) :
		className(className),
		methodName(methodName),
		index(indexOffset),
		definatorType(DefinatorType::NAME)
	{}

	PointerBase(const char* className, const IL2CPP::SignaturePattern& pattern, int indexOffset = 0) :
		className(className),
		pattern(pattern),
		index(indexOffset),
		definatorType(DefinatorType::PATTERN)
	{}

	public:
	inline void* GetPointer()
	{
		return this->ptr;
	}

	inline bool IsNull()
	{
		return this->ptr == nullptr;
	}

	explicit operator bool() const
	{
		return this->ptr != nullptr;
	}

	virtual bool Define();
};

template <typename T>
class Pointer;

template <typename R, typename... Args>
class Pointer<R(Args...)> : public PointerBase
{
	public:
	Pointer(const char* className, int index) :
		PointerBase(className, index)
	{}

	Pointer(const char* className, const char* methodName, int indexOffset = 0) :
		PointerBase(className, methodName, indexOffset)
	{}

	Pointer(const char* className, const IL2CPP::SignaturePattern& pattern, int indexOffset = 0) :
		PointerBase(className, pattern, indexOffset)
	{}

	R operator()(Args... args)
	{
		if (IsNull())
		{
			throw IL2CPP::Exception::UnexpectedError();
		}

		return ((R(*)(Args...)) GetPointer()) (std::forward<Args>(args)...);
	}
};

