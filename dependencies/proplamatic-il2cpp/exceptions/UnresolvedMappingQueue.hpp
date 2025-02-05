#pragma once
#include <format>
#include "ExceptionBase.hpp"

namespace IL2CPP
{
	namespace Exception
	{
		class UnresolvedMappingQueue : public ExceptionBase
		{
			public:
			size_t totalOfUnresolvedQueue;

			explicit UnresolvedMappingQueue(size_t totalOfUnresolvedQueue, std::string unresolvedQueueStr) : ExceptionBase()
			{
				this->totalOfUnresolvedQueue = totalOfUnresolvedQueue;

				InitializeException(
					IL2CPP_NAMEOF(UnresolvedMappingQueue), 
					std::format(
						"{0} class mapping queue(s) are failed to be resolved.\n"
						"Unresolved queue(s):"
						"{1}",
						totalOfUnresolvedQueue,
						unresolvedQueueStr
					)
				);
			}
		};
	}
}