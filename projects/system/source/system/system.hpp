#ifndef SOLUTION_SYSTEM_SYSTEM_HPP
#define SOLUTION_SYSTEM_SYSTEM_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <ctime>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/asio.hpp>

#include <nlohmann/json.hpp>

#include "../config/config.hpp"
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

			using sources_container_t = std::vector < std::shared_ptr < Source > > ;

			using thread_pool_t = boost::asio::thread_pool;

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
					static inline const path_t assets_data = "assets.data";
					static inline const path_t config_json = "config.json";
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
					};
				};

			public:

				static void load_source_config(const path_t & path, Config & config);

				static void load_system_config(const path_t & path, Config & config);

				static void load_assets(assets_container_t & assets);

			public:

				static void save_config(const path_t & path, const Config & config);

			private:

				static void load(const path_t & path, json_t & object);

				static void save(const path_t & path, const json_t & object);
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

			void save() const;

		private:

			void load_config();

			void load_assets();

			void load_sources();

		private:

			void save_sources() const;

		public:

			void run();

		private:



		public:

			const auto & assets() const noexcept
			{
				return m_assets;
			}

		private:

			Config m_config;

			assets_container_t m_assets;

			sources_container_t m_sources;

			thread_pool_t m_thread_pool;
		};

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_SYSTEM_HPP