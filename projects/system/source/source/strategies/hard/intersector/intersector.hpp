#ifndef SOLUTION_SYSTEM_SOURCE_STRATEGIES_HARD_INTERSECTOR_HPP
#define SOLUTION_SYSTEM_SOURCE_STRATEGIES_HARD_INTERSECTOR_HPP

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
					class Intersector final : public Strategy
					{
					public:

						explicit Intersector(const Config & config) noexcept : Strategy(config)
						{}

						virtual ~Intersector() noexcept = default;

					public:

						virtual std::string name() const noexcept override
						{
							return Intersector::m_name;
						}

						virtual State run(frame_t frame, State current_state) const override;

					public:

						static inline const std::string m_name = "Intersector";
					};

				} // namespace hard

			} // namespace strategies

		} // namespace source

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_SOURCE_STRATEGIES_HARD_INTERSECTOR_HPP