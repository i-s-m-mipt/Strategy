#ifndef SOLUTION_SYSTEM_STRATEGIES_HARD_ASSIMILATOR_HPP
#define SOLUTION_SYSTEM_STRATEGIES_HARD_ASSIMILATOR_HPP

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
				class Assimilator : public Strategy
				{
				public:

					explicit Assimilator(const Config & config) noexcept :
						Strategy(config)
					{}

					virtual ~Assimilator() noexcept = default;

				public:

					virtual std::string type() const noexcept override final
					{
						return "Assimilator";
					}

					virtual State handle(const inputs_container_t & inputs, 
						const State & input_state, double transaction) const override final;
				};

			} // namespace hard

		} // namespace strategies

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_STRATEGIES_HARD_ASSIMILATOR_HPP