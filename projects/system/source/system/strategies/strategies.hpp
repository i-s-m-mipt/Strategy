#ifndef SOLUTION_SYSTEM_STRATEGIES_HPP
#define SOLUTION_SYSTEM_STRATEGIES_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <exception>
#include <stdexcept>
#include <string>

#include "../indicators/adx/adx.hpp"
#include "../indicators/ema/ema.hpp"
#include "../indicators/macd/macd.hpp"
#include "../indicators/mfi/mfi.hpp"
#include "../indicators/rsi/rsi.hpp"

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

					enum class Type
					{
						L, // Long
						S, // Short
						N  // Null
					};

				public:

					bool is_long() const noexcept
					{
						return (/*type == Type::L || */position > 0.0);
					}

					bool is_short() const noexcept
					{
						return (/*type == Type::S || */position < 0.0);
					}

					bool is_null() const noexcept
					{
						return (/*type == Type::N || */position == 0.0);
					}

				public:

					Type type = Type::N;

					double position = 0.0;
				};

			public:

				virtual ~Strategy() noexcept = default;

			public:

				virtual State handle(const inputs_container_t & inputs, 
					double transaction) const;

				virtual State handle(const inputs_container_t & inputs, 
					double transaction, const State & input_state) const;
			};

		} // namespace strategies

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_STRATEGIES_HPP