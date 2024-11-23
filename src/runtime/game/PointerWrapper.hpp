#pragma once
#include "../Unity/Unity.hpp"
#include "ClassFinder.hpp"
#include <optional>
#include <vector>

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

	PointerWrapper(const char* className, unsigned int index)
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

	PointerWrapper(const char* className, const SignaturePattern sigPattern, int indexOffset = 0)
	{
		this->className = className;
		this->sigPattern = sigPattern;
		this->indexOffset = indexOffset;
	}

	MethodPtr GetPointer() const
	{
		return ptr;
	}

	std::optional<const char*> GetKlassName() const
	{
		if (this->className != nullptr)
		{
			return this->className;
		}

		return std::nullopt;
	}

	std::optional<const char*> GetMethodName() const
	{
		if (this->methodName != nullptr)
		{
			return this->methodName;
		}

		return std::nullopt;
	}

	std::optional<uintptr_t> GetOffset() const
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

	bool Define();
};

namespace PointerWrapperManager
{
	extern std::vector<PointerWrapper*> PointerWrapperList;
	void INIT();
}

template <typename T>
class Pointer;

template <typename R, typename... Args>
class Pointer<R(Args...)>
{
	private:
	PointerWrapper* declaration;

	public:
	Pointer(uintptr_t offset)
	{
		PointerWrapper* def = new PointerWrapper(offset);
		PointerWrapperManager::PointerWrapperList.push_back(def);
		declaration = def;
	}

	Pointer(const char* klass, int index)
	{
		PointerWrapper* def = new PointerWrapper(klass, index);
		PointerWrapperManager::PointerWrapperList.push_back(def);
		declaration = def;
	}

	Pointer(const char* klass, const char* methodName, int indexOffset = 0)
	{
		PointerWrapper* def = new PointerWrapper(klass, methodName, indexOffset);
		PointerWrapperManager::PointerWrapperList.push_back(def);
		declaration = def;
	}

	Pointer(const char* klass, const SignaturePattern pattern, int indexOffset = 0)
	{
		PointerWrapper* def = new PointerWrapper(klass, pattern, indexOffset);
		PointerWrapperManager::PointerWrapperList.push_back(def);
		declaration = def;
	}

	inline bool IsDeclarationNull()
	{
		return declaration == nullptr;
	}

	inline bool IsNull()
	{
		return IsDeclarationNull() && declaration->GetPointer() == nullptr;
	}

	inline MethodPtr GetPointer()
	{
		return IsNull() ? nullptr : declaration->GetPointer();
	}

	inline R operator()(Args... args)
	{
		if (IsNull())
		{
			LOG_ERROR("Trying to call a nullptr.");
		}

		return ((R(*)(Args...)) declaration->GetPointer()) (std::forward<Args>(args)...);
	}
};