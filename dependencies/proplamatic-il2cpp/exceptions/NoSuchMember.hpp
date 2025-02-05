#pragma once
#include <format>
#include "ExceptionBase.hpp"

namespace IL2CPP
{
	namespace Exception
	{
		enum class ExpectedMemberType : int8_t
		{
			FIELD,
			PROPERTY,
			METHOD
		};

		class NoSuchMember : public ExceptionBase
		{
			public:
			ExpectedMemberType expectedResult;
			std::string classname;
			std::string wantedName;

			explicit NoSuchMember(ExpectedMemberType expectedResult, const std::string& classname, const std::string& wantedName) : ExceptionBase()
			{
				this->expectedResult = expectedResult;
				this->classname = classname;
				this->wantedName = wantedName;

				std::string resultTypeName = "unknown";

				switch (expectedResult)
				{
					case ExpectedMemberType::FIELD:
						resultTypeName = "field";
						break;
					case ExpectedMemberType::PROPERTY:
						resultTypeName = "property";
						break;
					case ExpectedMemberType::METHOD:
						resultTypeName = "method";
						break;
					}

				InitializeException(
					IL2CPP_NAMEOF(NoSuchMember),
					std::format("No such {0} member named \"{1}\" found in {2}.",
						resultTypeName,
						wantedName,
						classname
					)
				);
			}
		};
	}
}

