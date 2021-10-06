#ifndef SOLUTION_SYSTEM_BACKTESTER_HPP
#define SOLUTION_SYSTEM_BACKTESTER_HPP

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
#include <vector>

#include "../strategies/strategies.hpp"

#include "../../config/config.hpp"
#include "../../detail/detail.hpp"
#include "../../detail/inputs/inputs.hpp"

#include "logger/logger.hpp"

namespace solution
{
	namespace system
	{
		class backtester_exception : public std::exception
		{
		public:

			explicit backtester_exception(const std::string& message) noexcept :
				std::exception(message.c_str())
			{}

			explicit backtester_exception(const char* const message) noexcept :
				std::exception(message)
			{}

			~backtester_exception() noexcept = default;
		};

		class Backtester
		{
		public:

			using Date_Time = detail::Date_Time;

			using inputs_container_t = detail::inputs_container_t;

			using Strategy = strategies::Strategy;

		public:

			struct Result
			{
				std::vector < std::pair < Date_Time, double > > rewards;
			};

		public:

			Backtester(const Config & config, 
				const inputs_container_t & inputs,
				std::shared_ptr < Strategy > strategy) :
				m_config(config), m_inputs(inputs), m_strategy(strategy)
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

			Config m_config;

			inputs_container_t m_inputs;

			std::shared_ptr < Strategy > m_strategy;
		};

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_BACKTESTER_HPP