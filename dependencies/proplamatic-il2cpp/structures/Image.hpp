#pragma once
#include "../Main.hpp"
#include <functional>

namespace IL2CPP
{
	struct Image : private IMPORT::Il2CppImage
	{
		/**
		* Retrieves the name of the image.
		* 
		* @returns The name of the image.
		*/
		std::string GetName() const;

		/**
		* Retrieves the total number of classes in the image.
		* 
		* @returns The number of classes in the image.
		*/
		size_t ClassCount() const;

		/**
		* Iterates over all classes in the image and applies a callback function to each.
		* 
		* @param callback: A function to be applied to each class.
		*/
		void IterateClasses(const std::function<void(Class*)>& callback) const;

		/**
		* Retrieves the class at the specified index.
		* 
		* @param index: The index of the desired class.
		* 
		* @throw IndexOutOfBounds: Thrown when the specified index is out of range.
		* @returns A pointer to the Class object at the given index.
		*/
		Class* GetClass(size_t index) const;

		/**
		* Retrieves the class by its full name.
		* 
		* @param fullname: The full name of the class.
		* 
		* @throw NoSuchClass: Thrown when no class with the specified name is found in the image.
		* @returns A pointer to the Class object with the specified full name.
		*/
		Class* GetClass(const std::string& fullname) const;
	};
}