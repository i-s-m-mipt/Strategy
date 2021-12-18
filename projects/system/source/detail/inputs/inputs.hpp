#ifndef SOLUTION_SYSTEM_DETAIL_INPUTS_HPP
#define SOLUTION_SYSTEM_DETAIL_INPUTS_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include "../detail.hpp"

namespace solution
{
	namespace system
	{
		namespace detail
		{
			struct Input
			{
			public:

				using indicators_container_t = std::map < std::string, 
					std::map < std::string, double > > ;

			public:

				Date_Time date_time_open;
				Date_Time date_time_close;

				unsigned int day;
				unsigned int hour;

				double price_open;
				double price_high;
				double price_low;
				double price_close;

				double volume_base;
				double volume_quote;

				double volume_buy_base;
				double volume_buy_quote;

				double volume_sell_base;
				double volume_sell_quote;

			public:

				indicators_container_t indicators;
			};

			using inputs_container_t = std::vector < Input > ;

			using database_t = std::vector < inputs_container_t > ;

		} // namespace detail

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_DETAIL_INPUTS_HPP