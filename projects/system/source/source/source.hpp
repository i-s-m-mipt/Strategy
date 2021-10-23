#ifndef SOLUTION_SYSTEM_SOURCE_HPP
#define SOLUTION_SYSTEM_SOURCE_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <algorithm>
#include <climits>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/asio.hpp>

#include "backtester/backtester.hpp"

#include "indicators/indicator.hpp"
#include "indicators/awvb/awvb.hpp"

#include "strategies/strategy.hpp"
#include "strategies/hard/assimilator/assimilator.hpp"

#include "../config/config.hpp"

#include "../detail/detail.hpp"

#include "../detail/inputs/inputs.hpp"
#include "../detail/klines/klines.hpp"
#include "../detail/orders/orders.hpp"
#include "../detail/trades/trades.hpp"

#include "logger/logger.hpp"
#include "python/python.hpp"

namespace solution
{
	namespace system
	{
		class source_exception : public std::exception
		{
		public:

			explicit source_exception(const std::string & message) noexcept :
				std::exception(message.c_str())
			{}

			explicit source_exception(const char * const message) noexcept :
				std::exception(message)
			{}

			~source_exception() noexcept = default;
		};

		class Source
		{
		private:

			using path_t = std::filesystem::path;

			using Input = detail::Input;

			using inputs_container_t = detail::inputs_container_t;

			using Indicator = source::Indicator ;

			using indicators_container_t = std::unordered_map <
				std::string, std::shared_ptr < Indicator > > ;

			using Strategy = source::Strategy;

			using strategies_container_t = std::unordered_map <
				std::string, std::shared_ptr < Strategy > > ;

			using Kline = detail::Kline;
			using Order = detail::Order;
			using Trade = detail::Trade;

			using klines_container_t = std::vector < Kline > ;
			using orders_container_t = std::vector < Order > ;
			using trades_container_t = std::vector < Trade > ;

			using Backtester = source::Backtester;

			using Result = Backtester::Result;

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
			public:

				struct Directory
				{
					static inline const path_t config = "system/config";
					static inline const path_t inputs = "system/inputs";
					static inline const path_t klines = "system/klines";
					static inline const path_t orders = "system/orders";
					static inline const path_t result = "system/result";
					static inline const path_t trades = "system/trades";
				};

			public:

				struct File
				{
					static inline const path_t config_json = "config.json";
					static inline const path_t inputs_data = "inputs.data";
					static inline const path_t reward_data = "reward.data";
					static inline const path_t trades_data = "trades.data";

					static inline const path_t reward_HS_data = "reward_HS.data";
					static inline const path_t reward_SS_data = "reward_SS.data";
					static inline const path_t reward_BH_data = "reward_BH.data";

					static inline const path_t trades_HS_data = "trades_HS.data";
					static inline const path_t trades_SS_data = "trades_SS.data";
					static inline const path_t trades_BH_data = "trades_BH.data";

					static inline const path_t research_volumes_data = "research_volumes.data";

					static inline const path_t research_volatility_data = "research_volatility.data";
				};

			public:

				explicit Data(const Config & config) noexcept : m_config(config)
				{}

				~Data() noexcept = default;

			public:

				void save_inputs(const inputs_container_t & inputs) const;

				void save_result(const Result & result) const;

			private:

				void save_reward(const Result & result) const;

				void save_trades(const Result & result) const;

			private:

				const Config & m_config;
			};

		public:

			explicit Source(const Config & config) : m_config(config), 
				m_data(m_config), m_asset(m_config.inputs_asset), 
				m_thread_pool(2U * std::thread::hardware_concurrency())
			{
				initialize();
			}

			~Source() noexcept
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

			void load_indicators();

			void load_strategies();

		private:

			void reload_indicators();

			void reload_strategies();

		private:

			void handle_inputs() const;

			void handle_backtest();

			void handle_backtest_fit();

			void handle_research() const;

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

			void make_report() const;

		private:

			void research_volumes(const inputs_container_t & inputs) const;

			void research_volatility(const inputs_container_t & inputs) const;

		public:

			const auto & config() const noexcept
			{
				return m_config;
			}

			const auto & asset() const noexcept
			{
				return m_asset;
			}

		private:

			Config m_config;

			Data m_data;

			std::string m_asset;

			indicators_container_t m_indicators;

			strategies_container_t m_strategies;

			Result m_result_BH;
			Result m_result_HS;
			Result m_result_SS;

			thread_pool_t m_thread_pool;
		};

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_SOURCE_HPP