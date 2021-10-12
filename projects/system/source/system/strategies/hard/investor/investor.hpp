#ifndef SOLUTION_SYSTEM_STRATEGIES_HARD_INVESTOR_HPP
#define SOLUTION_SYSTEM_STRATEGIES_HARD_INVESTOR_HPP

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
				class Investor : public Strategy
				{
				public:

					explicit Investor(const Config & config) noexcept :
						Strategy(config)
					{}

					virtual ~Investor() noexcept = default;

				public:

					virtual std::string type() const noexcept override final
					{
						return name;
					}

					virtual State handle(const inputs_container_t & inputs, 
						const State & input_state, double transaction) const override final;

				public:

					static inline const std::string name = "Investor";
				};

			} // namespace hard

		} // namespace strategies

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_STRATEGIES_HARD_INVESTOR_HPP