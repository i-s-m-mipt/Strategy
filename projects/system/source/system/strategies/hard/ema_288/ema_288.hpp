#ifndef SOLUTION_SYSTEM_STRATEGIES_HARD_EMA_288_HPP
#define SOLUTION_SYSTEM_STRATEGIES_HARD_EMA_288_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include "../../strategies.hpp"

namespace solution
{
	namespace system
	{
		namespace strategies
		{
			namespace hard
			{
				class EMA_288 : public Strategy
				{
				public:

					virtual ~EMA_288() noexcept = default;

				public:

					virtual State handle(const inputs_container_t & inputs, 
						double transaction) const override final;
				
				public:

					static inline const std::string type = "EMA_288";
				};

			} // namespace hard

		} // namespace strategies

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_STRATEGIES_HARD_EMA_288_HPP