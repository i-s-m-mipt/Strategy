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
					private:

						struct Level
						{
							iterator_t input;

							std::size_t locality;

							double price_low;
							double price_high;
						};

					private:

						using levels_container_t = std::vector < Level > ;

					public:

						explicit Subjugator(const Config & config) noexcept : Strategy(config)
						{}

						virtual ~Subjugator() noexcept = default;

					public:

						virtual std::string name() const noexcept override
						{
							return Subjugator::m_name;
						}

						virtual State run(frame_t frame, State current_state) const override;

					private:

						levels_container_t make_levels(frame_t frame) const;

						void make_level(levels_container_t & levels, 
							iterator_t extremum, std::size_t locality) const;

						bool is_violated(const Level & level, frame_t frame) const;

					public:

						static inline const std::string m_name = "Subjugator";
					};

				} // namespace hard

			} // namespace strategies

		} // namespace source

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_SOURCE_STRATEGIES_HARD_SUBJUGATOR_HPP