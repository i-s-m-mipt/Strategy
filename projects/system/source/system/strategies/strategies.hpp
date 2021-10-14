#ifndef SOLUTION_SYSTEM_STRATEGIES_HPP
#define SOLUTION_SYSTEM_STRATEGIES_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <cmath>
#include <exception>
#include <fstream>
#include <stdexcept>
#include <string>

#include "../indicators/adx/adx.hpp"
#include "../indicators/awvb/awvb.hpp"
#include "../indicators/ema/ema.hpp"
#include "../indicators/macd/macd.hpp"
#include "../indicators/mfi/mfi.hpp"
#include "../indicators/rsi/rsi.hpp"

#include "../../config/config.hpp"
#include "../../detail/inputs/inputs.hpp"

#include "logger/logger.hpp"

namespace solution
{
	namespace system
	{
		namespace strategies
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

				using inputs_container_t = detail::inputs_container_t;

			public:

				struct State
				{
				public:

					auto is_long() const noexcept
					{
						return (position > 0.0);
					}

					auto is_short() const noexcept
					{
						return (position < 0.0);
					}

					auto is_null() const noexcept
					{
						return (position == 0.0);
					}

				public:

					double position = 0.0;
				};

			public:

				explicit Strategy(const Config & config) noexcept : 
					m_config(config)
				{}

				virtual ~Strategy() noexcept = default;

			public:

				virtual std::string type() const noexcept = 0;

				virtual State handle(const inputs_container_t & inputs, 
					const State & input_state, double transaction) const = 0;

			protected:

				const Config & m_config;
			};

		} // namespace strategies

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_STRATEGIES_HPP