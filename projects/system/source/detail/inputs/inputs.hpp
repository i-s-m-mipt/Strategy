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

				using indicators_container_t = std::map < std::string, double > ;

			public:

				struct Aggregated_Trade
				{
					double volume_buy_base;
					double volume_buy_quote;
					double volume_sell_base;
					double volume_sell_quote;
				};

				struct Order
				{
					// TODO
				};

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

			public:

				std::vector < Aggregated_Trade > lower_price_aggregated_trades;
				std::vector < Aggregated_Trade > upper_price_aggregated_trades;
				
			public:

				// TODO (orders)

			public:

				int movement_tag = 0;

				// TODO (regression_tags, classification_tag)
			};

			using inputs_container_t = std::vector < Input > ;

		} // namespace detail

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_DETAIL_INPUTS_HPP