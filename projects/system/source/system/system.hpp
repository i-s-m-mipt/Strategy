#ifndef SOLUTION_SYSTEM_SYSTEM_HPP
#define SOLUTION_SYSTEM_SYSTEM_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/asio.hpp>

#include <nlohmann/json.hpp>

#include "indicators/adx/adx.hpp"
#include "indicators/ema/ema.hpp"
#include "indicators/macd/macd.hpp"
#include "indicators/mfi/mfi.hpp"
#include "indicators/rsi/rsi.hpp"

#include "strategies/strategies.hpp"

#include "strategies/hard/macd_mfi/macd_mfi.hpp"

#include "../config/config.hpp"

#include "../detail/detail.hpp"

#include "../detail/inputs/inputs.hpp"
#include "../detail/klines/klines.hpp"
#include "../detail/orders/orders.hpp"
#include "../detail/trades/trades.hpp"

#include "logger/logger.hpp"
//#include "python/python.hpp"

namespace solution 
{
	namespace system
	{
		class system_exception : public std::exception
		{
		public:

			explicit system_exception(const std::string & message) noexcept : 
				std::exception(message.c_str()) 
			{}

			explicit system_exception(const char * const message) noexcept :
				std::exception(message)
			{}

			~system_exception() noexcept = default;
		};

		class System
		{
		private:

			using path_t = std::filesystem::path;

			using Date_Time = detail::Date_Time;

			using assets_container_t = std::vector < std::string > ;

			using Input = detail::Input;

			using inputs_container_t = detail::inputs_container_t;

			using indicators_container_t = std::vector < 
				std::function < void(inputs_container_t &) > > ;

			using Kline = detail::Kline;
			using Order = detail::Order;
			using Trade = detail::Trade;

			using klines_container_t = std::vector < Kline > ;
			using orders_container_t = std::vector < Order > ;
			using trades_container_t = std::vector < Trade > ;

			using thread_pool_t = boost::asio::thread_pool;

		private:

			struct Extension
			{
				using extension_t = std::string;

				static inline const extension_t txt = ".txt";
				static inline const extension_t csv = ".csv";

				static inline const extension_t empty = "";
			};

		private:

			class Data
			{
			private:

				using json_t = nlohmann::json;

			private:

				struct File
				{
					static inline const path_t config_json = "system/config.json";
					static inline const path_t assets_data = "system/assets.data";

					static inline const path_t inputs_data = "system/inputs/inputs.data";
				};

			private:

				struct Key
				{
					struct Config
					{
						static inline const std::string test                          = "test";
						static inline const std::string required_inputs               = "required_inputs";
						static inline const std::string inputs_year_begin             = "inputs_year_begin";
						static inline const std::string inputs_year_end               = "inputs_year_end";
						static inline const std::string inputs_timeframe              = "inputs_timeframe";
						static inline const std::string inputs_asset                  = "inputs_asset";
						static inline const std::string price_aggregated_trade_step   = "price_aggregated_trade_step";
						static inline const std::string price_aggregated_trades_depth = "price_aggregated_trades_depth";
						static inline const std::string timesteps_prehistory          = "timesteps_prehistory";
						static inline const std::string timesteps_prediction          = "timesteps_prediction";
						static inline const std::string skipped_timesteps             = "skipped_timesteps";
						static inline const std::string min_movement                  = "min_movement";
					};
				};

			public:

				static void load_config(Config & config);

				static void load_assets(assets_container_t & assets);

			public:

				static void save_inputs(const inputs_container_t & inputs, const Config & config);

			private:

				static void load(const path_t & path, json_t & object);

				static void save(const path_t & path, const json_t & object);
			};

		private:

			struct State
			{
				static inline const std::string C = "C";
				static inline const std::string L = "L";
				static inline const std::string S = "S";
			};

		public:

			System() : m_thread_pool(2U * std::thread::hardware_concurrency())
			{
				initialize();
			}

			~System() noexcept
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

		private:

			void load();

		private:

			void load_config();

			void load_assets();

			void load_indicators();

		private:

			void handle_inputs() const;

		private:

			inputs_container_t load_inputs() const;

		private:

			klines_container_t load_klines() const;

			orders_container_t load_orders() const;

			trades_container_t load_trades() const;

		private:

			std::string make_klines_file_name(std::time_t year, std::time_t month) const;

			std::string make_orders_file_name(std::time_t year, std::time_t month) const;

			std::string make_trades_file_name(std::time_t year, std::time_t month) const;

		private:

			Kline parse_kline(const std::string & line) const;

			Order parse_order(const std::string & line) const;

			Trade parse_trade(const std::string & line) const;

		private:

			inputs_container_t make_inputs(
				const klines_container_t & klines,
				const orders_container_t & orders,
				const trades_container_t & trades) const;

		private:

			void update_indicators(inputs_container_t & inputs) const;

			void update_price_aggregated_trades(
				      inputs_container_t & inputs,
				const klines_container_t & klines,
				const trades_container_t & trades) const;

			void update_movement_tags(inputs_container_t & inputs) const;

		private:

			void save_inputs(const inputs_container_t & inputs) const;

		public:

			void run();

		public:

			const auto & assets() const noexcept
			{
				return m_assets;
			}

		private:

			static inline const path_t inputs_directory = "system/inputs";
			static inline const path_t klines_directory = "system/klines";
			static inline const path_t orders_directory = "system/orders";
			static inline const path_t trades_directory = "system/trades";
			
		private:

			static inline const std::time_t seconds_in_day = 86400LL;

		private:

			Config m_config;

			assets_container_t m_assets;

			indicators_container_t m_indicators;

			thread_pool_t m_thread_pool;
		};

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_SYSTEM_HPP