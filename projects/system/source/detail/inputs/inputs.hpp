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
					double volume_buy_base;   // Trade, local normalization [0.0; 1.0]
					double volume_buy_quote;  // Trade, local normalization [0.0; 1.0]
					double volume_sell_base;  // Trade, local normalization [0.0; 1.0]
					double volume_sell_quote; // Trade, local normalization [0.0; 1.0]
				};

				struct Order
				{
					// TODO
				};

			public: // klines

				unsigned int day;  // Kline, OHE [7]
				unsigned int hour; // Kline, OHE [23]

				double price_open;  // Kline, local normalization [0.0; 1.0] (1)
				double price_high;  // Kline, local normalization [0.0; 1.0] (1)
				double price_low;   // Kline, local normalization [0.0; 1.0] (1)
				double price_close; // Kline, local normalization [0.0; 1.0] (1)

				double volume_base;  // Kline, local normalization [0.0; 1.0]
				double volume_quote; // Kline, local normalization [0.0; 1.0]

				double volume_taker_base;  // Kline, local normalization [0.0; 1.0]
				double volume_taker_quote; // Kline, local normalization [0.0; 1.0]

				double volume_maker_base;  // Kline, local normalization [0.0; 1.0]
				double volume_maker_quote; // Kline, local normalization [0.0; 1.0]

			public: // indicators

				indicators_container_t EMA; // Input, local normalization [0.0; 1.0] (1)

				indicators_container_t MACD; // Input, local normalization [-1.0; +1.0]

				indicators_container_t ADX; // Input, global normalization [0.0; 1.0]
				indicators_container_t MFI; // Input, global normalization [0.0; 1.0]
				indicators_container_t RSI; // Input, global normalization [0.0; 1.0]

			public: // trades

				std::vector < Aggregated_Trade > lower_price_aggregated_trades;
				std::vector < Aggregated_Trade > upper_price_aggregated_trades;
				
			public: // orders

				// TODO

			public:

				int movement_tag = 0;

				// TODO (regression_tags, classification_tag)
			};

			using inputs_container_t = std::vector < Input > ;

		} // namespace detail

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_DETAIL_INPUTS_HPP