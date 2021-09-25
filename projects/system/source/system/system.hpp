#ifndef SOLUTION_SYSTEM_SYSTEM_HPP
#define SOLUTION_SYSTEM_SYSTEM_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <array>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "../config/config.hpp"
#include "../market/market.hpp"

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

			class Data
			{
			private:

				using json_t = nlohmann::json;

			private:

				struct File
				{
					using path_t = std::filesystem::path;

					static inline const path_t config_json = "system/config.json";
				};

			private:

				using path_t = File::path_t;

			private:

				struct Key
				{
					struct Config
					{
						static inline const std::string test = "test";
					};
				};

			public:

				static void load_config(Config & config);

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

			System()
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

		public:

			void run();

		private:

			static inline const std::time_t seconds_in_day = 86400LL;

			static inline const std::time_t initialization_delay = 1LL;

		private:

			Config m_config;

			std::unique_ptr < Market > m_market;
		};

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_SYSTEM_HPP