#ifndef SOLUTION_SYSTEM_SOURCE_INDICATOR_HPP
#define SOLUTION_SYSTEM_SOURCE_INDICATOR_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <algorithm>
#include <climits>
#include <cmath>
#include <exception>
#include <fstream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../config/config.hpp"

#include "../../detail/inputs/inputs.hpp"

#include "logger/logger.hpp"

namespace solution
{
	namespace system
	{
		namespace source
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

			class Indicator
			{
			public:

				using Input = detail::Input;

				using inputs_container_t = detail::inputs_container_t;

			public:

				explicit Indicator(const Config & config) noexcept : m_config(config)
				{}

				virtual ~Indicator() noexcept = default;

			public:

				virtual void run(inputs_container_t & inputs) const
				{}

			public:

				static inline const std::string name = "Indicator";

			protected:

				const Config m_config;
			};

		} // namespace source
		
	} // namespace system

} // namespace solution

#endif // #ifndef #define SOLUTION_SYSTEM_SOURCE_INDICATOR_HPP