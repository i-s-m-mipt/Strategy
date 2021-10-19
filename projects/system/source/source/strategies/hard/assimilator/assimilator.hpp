#ifndef SOLUTION_SYSTEM_SOURCE_STRATEGIES_HARD_ASSIMILATOR_HPP
#define SOLUTION_SYSTEM_SOURCE_STRATEGIES_HARD_ASSIMILATOR_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include "../../strategy.hpp"

namespace solution
{
	namespace system
	{
		namespace source
		{
			namespace strategies
			{
				namespace hard
				{
					class Assimilator final : public Strategy
					{
					public:

						explicit Assimilator(const Config & config) noexcept : Strategy(config)
						{}

						virtual ~Assimilator() noexcept = default;

					public:

						virtual std::string name() const noexcept override
						{
							return Assimilator::m_name;
						}

						virtual State run(const inputs_container_t & inputs, State current_state) const override;

					public:

						static inline const std::string m_name = "Assimilator";
					};

				} // namespace hard

			} // namespace strategies

		} // namespace source

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_STRATEGIES_HARD_ASSIMILATOR_HPP