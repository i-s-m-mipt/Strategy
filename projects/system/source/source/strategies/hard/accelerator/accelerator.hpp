#ifndef SOLUTION_SYSTEM_SOURCE_STRATEGIES_HARD_ACCELERATOR_HPP
#define SOLUTION_SYSTEM_SOURCE_STRATEGIES_HARD_ACCELERATOR_HPP

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
					class Accelerator final : public Strategy
					{
					public:

						explicit Accelerator(const Config & config) noexcept : Strategy(config)
						{}

						virtual ~Accelerator() noexcept = default;

					public:

						virtual std::string name() const noexcept override
						{
							return Accelerator::m_name;
						}

						virtual State run(frame_t frame, State current_state) const override;

					public:

						static inline const std::string m_name = "Accelerator";

					private:

						mutable Date_Time m_last_rrr_date_time;
						mutable Date_Time m_last_ggg_date_time;

						mutable double m_last_position_price;
					};

				} // namespace hard

			} // namespace strategies

		} // namespace source

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_SOURCE_STRATEGIES_HARD_ACCELERATOR_HPP