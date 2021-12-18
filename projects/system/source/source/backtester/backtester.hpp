#ifndef SOLUTION_SYSTEM_SOURCE_BACKTESTER_HPP
#define SOLUTION_SYSTEM_SOURCE_BACKTESTER_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <climits>
#include <cmath>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../strategies/strategy.hpp"

#include "../../config/config.hpp"
#include "../../detail/detail.hpp"
#include "../../detail/inputs/inputs.hpp"

#include "logger/logger.hpp"

namespace solution
{
	namespace system
	{
		namespace source
		{
			class backtester_exception : public std::exception
			{
			public:

				explicit backtester_exception(const std::string & message) noexcept :
					std::exception(message.c_str())
				{}

				explicit backtester_exception(const char * const message) noexcept :
					std::exception(message)
				{}

				~backtester_exception() noexcept = default;
			};

			class Backtester
			{
			public:

				using inputs_container_t = detail::inputs_container_t;

				using frame_t = Strategy::frame_t;

				using State = Strategy::State;

			public:

				struct Result
				{
				public:

					struct Trade
					{
						Date_Time begin;
						Date_Time end;

						State state;

						double reward;
					};

				public:

					std::vector < std::pair < Date_Time, double > > rewards;

					std::vector < Trade > trades;
				};

			public:

				explicit Backtester(const Config & config, 
					const inputs_container_t & inputs_backtest,
					const inputs_container_t & inputs_strategy,
					std::shared_ptr < Strategy > strategy) : m_config(config), 
						m_inputs_backtest(inputs_backtest),
						m_inputs_strategy(inputs_strategy), m_strategy(strategy)
				{
					initialize();
				}

				~Backtester() noexcept
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

			public:

				Result run() const;

			private:

				Result handle_investor() const;

				Result handle_manager() const;

			private:

				std::size_t make_strategy_begin() const;

				std::size_t make_backtest_begin() const;

				void update_transaction(double & transaction, double total_reward) const noexcept;

				frame_t make_prehistory(std::size_t index) const;

				void verify(const Result & result) const;

			private:

				const Config m_config;

			private:

				inputs_container_t m_inputs_backtest;
				inputs_container_t m_inputs_strategy;

				std::shared_ptr < Strategy > m_strategy;
			};

		} // namespace source

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_SOURCE_BACKTESTER_HPP