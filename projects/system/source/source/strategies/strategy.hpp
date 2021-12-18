#ifndef SOLUTION_SYSTEM_SOURCE_STRATEGY_HPP
#define SOLUTION_SYSTEM_SOURCE_STRATEGY_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <algorithm>
#include <climits>
#include <cmath>
#include <exception>
#include <fstream>
#include <functional>
#include <future>
#include <iterator>
#include <memory>
#include <mutex>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <boost/asio.hpp>

#include "../indicators/apwvb/apwvb.hpp"
#include "../indicators/rsi/rsi.hpp"

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
			class strategy_exception : public std::exception
			{
			public:

				explicit strategy_exception(const std::string & message) noexcept :
					std::exception(message.c_str())
				{}

				explicit strategy_exception(const char * const message) noexcept :
					std::exception(message)
				{}

				~strategy_exception() noexcept = default;
			};

			class Strategy
			{
			public:

				using Input = detail::Input;

				using inputs_container_t = detail::inputs_container_t;

				using iterator_t = inputs_container_t::const_iterator;

				using frame_t = std::pair < iterator_t, iterator_t > ;

			public:

				enum class State
				{
					N,
					L,
					S
				};

			public:

				explicit Strategy(const Config & config) noexcept : m_config(config)
				{}

				virtual ~Strategy() noexcept = default;

			public:

				virtual std::string name() const noexcept
				{
					return Strategy::m_name;
				}

				virtual State run(frame_t frame, State current_state) const
				{
					return State::L;
				}

			public:

				static inline const std::string m_name = "Strategy";

			protected:

				const Config m_config;
			};

		} // namespace source

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_SOURCE_STRATEGY_HPP