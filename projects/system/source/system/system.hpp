#ifndef SOLUTION_SYSTEM_SYSTEM_HPP
#define SOLUTION_SYSTEM_SYSTEM_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <filesystem>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <boost/asio.hpp>
#include <boost/date_time.hpp>

#include <nlohmann/json.hpp>

#include "../config/config.hpp"
#include "../detail/detail.hpp"
#include "../detail/inputs/inputs.hpp"
#include "../source/source.hpp"

#include "logger/logger.hpp"
#include "python/python.hpp"

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

			using assets_container_t = std::vector < std::string > ;

			using sources_container_t = std::unordered_map < 
				std::string, std::shared_ptr < Source > > ;

		private:

			struct Client
			{
				std::string name;

				std::string public_key;
				std::string secret_key;

				double initial_investments;
			};

		private:

			using clients_container_t = std::vector < Client > ;

			using thread_pool_t = boost::asio::thread_pool;

			using io_service_t = boost::asio::io_service;

			using timer_t = boost::asio::deadline_timer;

			using inputs_container_t = detail::inputs_container_t;

			using State = Source::State;

		private:

			class Data
			{
			private:

				using json_t = nlohmann::json;

			public:

				struct Directory
				{
					static inline const path_t config = "system/config";
				};

			public:

				struct File
				{
					static inline const path_t config_json  = "config.json";
					static inline const path_t assets_data  = "assets.data";
					static inline const path_t clients_json = "clients.json";
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
						static inline const std::string inputs_timeframe_type         = "inputs_timeframe_type";
						static inline const std::string inputs_asset                  = "inputs_asset";
						static inline const std::string price_aggregated_trade_step   = "price_aggregated_trade_step";
						static inline const std::string price_aggregated_trades_depth = "price_aggregated_trades_depth";
						static inline const std::string timesteps_prehistory          = "timesteps_prehistory";
						static inline const std::string timesteps_prediction          = "timesteps_prediction";
						static inline const std::string skipped_timesteps             = "skipped_timesteps";
						static inline const std::string min_movement                  = "min_movement";
						static inline const std::string transaction                   = "transaction";
						static inline const std::string commission                    = "commission";
						static inline const std::string stop_loss                     = "stop_loss";
						static inline const std::string test_hard_strategy            = "test_hard_strategy";
						static inline const std::string test_soft_strategy            = "test_soft_strategy";
						static inline const std::string required_backtest             = "required_backtest";
						static inline const std::string awvb_timesteps_wvb            = "awvb_timesteps_wvb";
						static inline const std::string awvb_timesteps_sma            = "awvb_timesteps_sma";
						static inline const std::string required_research             = "required_research";
						static inline const std::string assimilator_min_deviation     = "assimilator_min_deviation";
						static inline const std::string required_backtest_fit         = "required_backtest_fit";
						static inline const std::string has_reinvestment              = "has_reinvestment";
						static inline const std::string required_run                  = "required_run";
						static inline const std::string interval                      = "interval";
						static inline const std::string max_drawdown                  = "max_drawdown";
						static inline const std::string benchmark                     = "benchmark";
						static inline const std::string main_strategy                 = "main_strategy";
					};

					struct Client
					{
						static inline const std::string name                = "name";
						static inline const std::string public_key          = "public_key";
						static inline const std::string secret_key          = "secret_key";
						static inline const std::string initial_investments = "initial_investments";
					};
				};

			public:

				static void load_system_config(Config & config);

				static void load_source_config(const path_t & path, Config & config);

				static void load_assets(assets_container_t & assets);

				static void load_clients(clients_container_t & clients);

			public:

				static void save_source_config(const path_t & path, const Config & config);

			private:

				static void load(const path_t & path, json_t & object);

				static void save(const path_t & path, const json_t & object);
			};

		public:

			System() : m_thread_pool(2 * std::thread::hardware_concurrency()), 
				m_timer(m_io_service), m_is_interrupted(false), m_is_running(false)
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

			void save() const;

		private:

			void load_python();

			void load_config();

			void load_assets();

			void load_sources();

			void load_clients();

		private:

			void save_sources() const;

		public:

			void run();

		private:

			void wait_until_day_end() const;

			void handle();

			void handle_implementation(const std::string & asset);

			State convert_state(const std::string & state) const;

		public:

			const auto & assets() const noexcept
			{
				return m_assets;
			}

		private:

			static inline const std::time_t seconds_in_day = 86400;

		private:

			Config m_config;

			assets_container_t m_assets;

			sources_container_t m_sources;

			clients_container_t m_clients;

			thread_pool_t m_thread_pool;

			io_service_t m_io_service;

			timer_t m_timer;

			std::atomic < bool > m_is_interrupted;
			std::atomic < bool > m_is_running;

			shared::Python m_python;
		};

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_SYSTEM_HPP