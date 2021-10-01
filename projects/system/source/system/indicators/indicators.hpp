#ifndef SOLUTION_SYSTEM_INDICATORS_HPP
#define SOLUTION_SYSTEM_INDICATORS_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <algorithm>
#include <climits>
#include <cmath>
#include <exception>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../detail/inputs/inputs.hpp"

#include "logger/logger.hpp"

namespace solution
{
	namespace system
	{
		namespace indicators
		{
			class indicator_exception : public std::exception
			{
			public:

				explicit indicator_exception(const std::string & message) noexcept :
					std::exception(message.c_str())
				{}

				explicit indicator_exception(const char * const message) noexcept :
					std::exception(message)
				{}

				~indicator_exception() noexcept = default;
			};

			using inputs_container_t = detail::inputs_container_t;

		} // namespace indicators
		
	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_INDICATORS_HPP