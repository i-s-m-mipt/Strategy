#ifndef SOLUTION_SYSTEM_DETAIL_HPP
#define SOLUTION_SYSTEM_DETAIL_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <cstdlib>
#include <ctime>
#include <exception>
#include <iomanip>
#include <ostream>
#include <stdexcept>
#include <string>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>

#include "logger/logger.hpp"

namespace solution
{
	namespace system
	{
		namespace detail
		{
			class detail_exception : public std::exception
			{
			public:

				explicit detail_exception(const std::string & message) noexcept :
					std::exception(message.c_str())
				{}

				explicit detail_exception(const char * const message) noexcept :
					std::exception(message)
				{}

				~detail_exception() noexcept = default;
			};

			struct Date_Time
			{
			public:

				std::time_t to_time_t() const noexcept;

				void from_time_t(std::time_t time, bool has_milliseconds = false) noexcept;

			public:

				unsigned int year   = 0U;
				unsigned int month  = 0U;
				unsigned int day    = 0U;

				unsigned int hour   = 0U;
				unsigned int minute = 0U;
				unsigned int second = 0U;
			};

			bool operator== (const Date_Time & lhs, const Date_Time & rhs);
			bool operator!= (const Date_Time & lhs, const Date_Time & rhs);
			bool operator<  (const Date_Time & lhs, const Date_Time & rhs);
			bool operator<= (const Date_Time & lhs, const Date_Time & rhs);
			bool operator>  (const Date_Time & lhs, const Date_Time & rhs);
			bool operator>= (const Date_Time & lhs, const Date_Time & rhs);

			std::ostream & operator<< (std::ostream & stream, const Date_Time & date_time);

			std::time_t duration(const Date_Time & date_time_1, const Date_Time & date_time_2);

		} // namespace detail

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_DETAIL_HPP