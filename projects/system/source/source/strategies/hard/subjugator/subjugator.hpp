#ifndef SOLUTION_SYSTEM_SOURCE_STRATEGIES_HARD_SUBJUGATOR_HPP
#define SOLUTION_SYSTEM_SOURCE_STRATEGIES_HARD_SUBJUGATOR_HPP

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
					class Subjugator final : public Strategy
					{
					public:

						explicit Subjugator(const Config & config) noexcept : Strategy(config)
						{}

						virtual ~Subjugator() noexcept = default;

					public:

						virtual std::string name() const noexcept override
						{
							return Subjugator::m_name;
						}

						virtual State run(const inputs_container_t & inputs, State current_state) const override;

					public:

						static inline const std::string m_name = "Subjugator";
					};

				} // namespace hard

			} // namespace strategies

		} // namespace source

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_SOURCE_STRATEGIES_HARD_SUBJUGATOR_HPP