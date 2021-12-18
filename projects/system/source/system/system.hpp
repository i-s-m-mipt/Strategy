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
#include <boost/multi_array.hpp>

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

			using correlation_matrix_t = boost::multi_array < double, 2 > ;

			using database_t = detail::database_t;

		private:

			class Data
			{
			private:

				using json_t = nlohmann::json;

			public:

				struct Directory
				{
					static inline const path_t config = "system/config";
					static inline const path_t output = "system/output";
				};

			public:

				struct File
				{
					static inline const path_t assets_data      = "assets.data";
					static inline const path_t clients_json     = "clients.json";
					static inline const path_t config_json      = "config.json";
					static inline const path_t correlation_data = "correlation.data";
				};

			private:

				struct Key
				{
					struct Config
					{
						static inline const std::string accelerator_flat_deviation  = "accelerator_flat_deviation";
						static inline const std::string accelerator_max_deviation_s = "accelerator_max_deviation_s";
						static inline const std::string accelerator_min_deviation_s = "accelerator_min_deviation_s";
						static inline const std::string accelerator_stop_loss       = "accelerator_stop_loss";
						static inline const std::string apwvb_threshold_long        = "apwvb_threshold_long";
						static inline const std::string apwvb_threshold_short       = "apwvb_threshold_short";
						static inline const std::string apwvb_timesteps_pwvb_long   = "apwvb_timesteps_pwvb_long";
						static inline const std::string apwvb_timesteps_pwvb_short  = "apwvb_timesteps_pwvb_short";
						static inline const std::string apwvb_timesteps_sma_long    = "apwvb_timesteps_sma_long";
						static inline const std::string apwvb_timesteps_sma_short   = "apwvb_timesteps_sma_short";
						static inline const std::string asset                       = "asset";
						static inline const std::string combinator_neighbors        = "combinator_neighbors";
						static inline const std::string combinator_threshold_long   = "combinator_threshold_long";
						static inline const std::string combinator_threshold_short  = "combinator_threshold_short";
						static inline const std::string commission                  = "commission";
						static inline const std::string ema_timesteps_fast          = "ema_timesteps_fast";
						static inline const std::string ema_timesteps_slow          = "ema_timesteps_slow";
						static inline const std::string has_reinvestment            = "has_reinvestment";
						static inline const std::string inputs_first_year           = "inputs_first_year";
						static inline const std::string inputs_last_year            = "inputs_last_year";
						static inline const std::string investment                  = "investment";
						static inline const std::string level_deviation             = "level_deviation";
						static inline const std::string level_max_bias              = "level_max_bias";
						static inline const std::string level_min_bias              = "level_min_bias";
						static inline const std::string permitted_state             = "permitted_state";
						static inline const std::string profit_rollback_long        = "profit_rollback_long";
						static inline const std::string profit_rollback_short       = "profit_rollback_short";
						static inline const std::string pulse_threshold             = "pulse_threshold";
						static inline const std::string required_backtest           = "required_backtest";
						static inline const std::string required_backtest_fit       = "required_backtest_fit";
						static inline const std::string required_inputs             = "required_inputs";
						static inline const std::string rsi_threshold_long          = "rsi_threshold_long";
						static inline const std::string rsi_threshold_short         = "rsi_threshold_short";
						static inline const std::string rsi_timesteps_long          = "rsi_timesteps_long";
						static inline const std::string rsi_timesteps_short         = "rsi_timesteps_short";
						static inline const std::string skipped_timesteps           = "skipped_timesteps";
						static inline const std::string stop_loss_long              = "stop_loss_long";
						static inline const std::string stop_loss_short             = "stop_loss_short";
						static inline const std::string strategy                    = "strategy";
						static inline const std::string take_profit_long            = "take_profit_long";
						static inline const std::string take_profit_short           = "take_profit_short";
						static inline const std::string timeframe_backtest          = "timeframe_backtest";
						static inline const std::string timeframe_strategy          = "timeframe_strategy";
						static inline const std::string timesteps_prediction        = "timesteps_prediction";
						static inline const std::string timesteps_prehistory        = "timesteps_prehistory";
						static inline const std::string interval                    = "interval";
						static inline const std::string required_correlation        = "required_correlation";
						static inline const std::string required_run                = "required_run";
						static inline const std::string reserve                     = "reserve";
						static inline const std::string server_start_hour           = "server_start_hour";
						static inline const std::string server_start_minute         = "server_start_minute";
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

				static void save_correlation(const correlation_matrix_t & correlation_matrix);

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

			void update_sources();

			void handle_sources();

		private:

			void save_sources() const;

		private:

			void handle_correlation();

		private:

			correlation_matrix_t make_correlation();

		private:

			double make_correlation(const std::string & asset_1, const std::string & asset_2);

		public:

			void run();

		private:

			void wait_until_hour_end() const;

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

			std::shared_ptr < database_t > m_database;
		};

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_SYSTEM_HPP