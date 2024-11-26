#pragma once
#include "../../Unity/Unity.hpp"
#include "../data/ClassFinder.hpp"
#include <optional>
#include <vector>
#include <Logger.hpp>
#include <Windows.h>

struct PointerWrapper
{
	private:
	const char* className = nullptr;
	const char* methodName = nullptr;
	MethodPtr ptr = nullptr;
	SignaturePattern sigPattern = { "", nullptr, {} };
	int index = -1;
	int indexOffset = 0;
	uintptr_t offset = 0;

	public:
	PointerWrapper(uintptr_t offset)
	{
		this->offset = offset;
	}

	PointerWrapper(const char* className, int index)
	{
		this->className = className;
		this->index = index;
	}

	PointerWrapper(const char* className, const char* methodName, int indexOffset = 0)
	{
		this->className = className;
		this->methodName = methodName;
		this->indexOffset = indexOffset;
	}

	PointerWrapper(const char* className, const SignaturePattern& sigPattern, int indexOffset = 0)
	{
		this->className = className;
		this->sigPattern = sigPattern;
		this->indexOffset = indexOffset;
	}

	MethodPtr GetPointer() const
	{
		return ptr;
	}

	inline bool IsNull() const
	{
		return ptr == nullptr;
	}

	bool Define();

	private:
	inline std::optional<const char*> GetKlassName() const
	{
		if (this->className != nullptr)
		{
			return this->className;
		}

		return std::nullopt;
	}

	inline std::optional<const char*> GetMethodName() const
	{
		if (this->methodName != nullptr)
		{
			return this->methodName;
		}

		return std::nullopt;
	}

	inline std::optional<uintptr_t> GetOffset() const
	{
		if (this->offset > 0)
		{
			return this->offset;
		}

		return std::nullopt;
	}

	inline SignaturePattern GetPattern() const
	{
		return this->sigPattern;
	}

	inline int GetIndex() const
	{
		return this->index;
	}

	inline int GetIndexOffset() const
	{
		return this->indexOffset;
	}
};

template <typename T>
class Pointer;

template <typename R, typename... Args>
class Pointer<R(Args...)> : public PointerWrapper
{
	public:
	Pointer(uintptr_t offset) : PointerWrapper(offset)
	{}

	Pointer(const char* className, int index) : PointerWrapper(className, index)
	{}

	Pointer(const char* className, const char* methodName, int indexOffset = 0) : PointerWrapper(className, methodName, indexOffset)
	{}

	Pointer(const char* className, const SignaturePattern& sigPattern, int indexOffset = 0) : PointerWrapper(className, sigPattern, indexOffset)
	{}

	inline R operator()(Args... args)
	{
		if (IsNull())
		{
			LOG_ERROR("Trying to call a nullptr.");
		}

		return ((R(*)(Args...)) GetPointer()) (std::forward<Args>(args)...);
	}
};