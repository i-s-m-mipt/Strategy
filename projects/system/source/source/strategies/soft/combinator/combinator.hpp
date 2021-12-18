#ifndef SOLUTION_SYSTEM_SOURCE_STRATEGIES_SOFT_COMBINATOR_HPP
#define SOLUTION_SYSTEM_SOURCE_STRATEGIES_SOFT_COMBINATOR_HPP

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
				namespace soft
				{
					class Combinator final : public Strategy
					{
					private:

						using database_t = detail::database_t;

						using pattern_t = std::vector < double > ;

						using iterator_t = inputs_container_t::const_iterator;

						using frame_t = std::pair < iterator_t, iterator_t > ;

						using extractor_t = std::function < pattern_t(frame_t) > ;

						using extractors_container_t = std::vector < extractor_t > ;

						using thread_pool_t = boost::asio::thread_pool;

					private:

						struct Level
						{
						public:

							using date_time_t = Date_Time;

						public:

							date_time_t begin;

							double price;
						};

					private:

						using levels_container_t = std::vector < Level > ;

					public:

						explicit Combinator(const Config & config, 
							std::shared_ptr < database_t > database) noexcept : 
								Strategy(config), m_database(database),
									m_thread_pool(2U * std::thread::hardware_concurrency())
						{
							initialize();
						}

						~Combinator() noexcept
						{
							try
							{
								uninitialize();
							}
							catch (...)
							{
								// std::abort();
							}
						}

					private:

						void initialize();

						void uninitialize();

					private:

						void load_extractors();

					private:

						levels_container_t make_levels(frame_t frame) const;

					public:

						virtual std::string name() const noexcept override
						{
							return Combinator::m_name;
						}

						virtual State run(frame_t frame, State current_state) const override;

					private:

						frame_t make_frame(const inputs_container_t & inputs) const;

						frame_t make_frame(const inputs_container_t & inputs, std::size_t index) const;

						double compare_patterns(const pattern_t & alpha, const pattern_t & omega) const;

					public:

						static inline const std::string m_name = "Combinator";

					private:

						std::shared_ptr < database_t > m_database;

						extractors_container_t m_extractors;

					private:

						mutable thread_pool_t m_thread_pool;
					};

				} // namespace hard

			} // namespace strategies

		} // namespace source

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_SOURCE_STRATEGIES_SOFT_COMBINATOR_HPP