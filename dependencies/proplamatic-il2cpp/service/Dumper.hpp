#pragma once
#include "../Main.hpp"

namespace IL2CPP
{
	namespace Dumper
	{
		// TODO: yeah yk
		struct DumperSettings
		{
			bool generatePattern = true;
			bool generateFullnameTyping = true;
			bool mergeDumpFiles = false;
		};

		std::string DumpImage(Image* imageToDump, bool generatePattern = true);

		void StartDumper(const std::string& dumpDirectoryPath, bool generatePattern = true);
	}
}