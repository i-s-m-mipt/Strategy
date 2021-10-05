#ifndef SOLUTION_SYSTEM_STRATEGIES_HARD_HPP
#define SOLUTION_SYSTEM_STRATEGIES_HARD_HPP

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
				class MACD_MFI : public Strategy_Base
				{
				public:

					virtual ~MACD_MFI() noexcept = default;

				public:

					virtual State handle(const inputs_container_t & inputs, double transaction) const override final;
				};

			} // namespace hard

		} // namespace strategies

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_STRATEGIES_HARD_HPP