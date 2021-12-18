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

						explicit Assimilator(const Config & config) noexcept : 
							Strategy(config)/*, m_previous_volume_delta(+1.0)*/
						{}

						virtual ~Assimilator() noexcept = default;

					public:

						virtual std::string name() const noexcept override
						{
							return Assimilator::m_name;
						}

						virtual State run(frame_t frame, State current_state) const override;

					public:

						static inline const std::string m_name = "Assimilator";

					private:

						/*mutable double m_previous_volume_delta;*/
					};

				} // namespace hard

			} // namespace strategies

		} // namespace source

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_SOURCE_STRATEGIES_HARD_ASSIMILATOR_HPP