#include "system.hpp"

namespace solution
{
	namespace system
	{
		void System::Data::load_system_config(Config & config)
		{
			LOGGER(logger);

			try
			{
				auto path = Directory::config / File::config_json;

				json_t raw_config;

				load(path, raw_config);

				config.interval             = raw_config[Key::Config::interval            ].get < std::time_t > ();
				config.required_correlation = raw_config[Key::Config::required_correlation].get < bool > ();
				config.required_run         = raw_config[Key::Config::required_run        ].get < bool > ();
				config.reserve              = raw_config[Key::Config::reserve             ].get < double > ();
				config.server_start_hour    = raw_config[Key::Config::server_start_hour   ].get < std::time_t > ();
				config.server_start_minute  = raw_config[Key::Config::server_start_minute ].get < std::time_t > ();

			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::Data::load_source_config(const path_t & path, Config & config)
		{
			LOGGER(logger);

			try
			{
				json_t raw_config;

				load(path, raw_config);

				config.accelerator_flat_deviation  = raw_config[Key::Config::accelerator_flat_deviation ].get < double > ();
				config.accelerator_max_deviation_s = raw_config[Key::Config::accelerator_max_deviation_s].get < double > ();
				config.accelerator_min_deviation_s = raw_config[Key::Config::accelerator_min_deviation_s].get < double > ();
				config.accelerator_stop_loss       = raw_config[Key::Config::accelerator_stop_loss      ].get < double > ();
				config.apwvb_threshold_long        = raw_config[Key::Config::apwvb_threshold_long       ].get < double > ();
				config.apwvb_threshold_short       = raw_config[Key::Config::apwvb_threshold_short      ].get < double > ();
				config.apwvb_timesteps_pwvb_long   = raw_config[Key::Config::apwvb_timesteps_pwvb_long  ].get < std::size_t > ();
				config.apwvb_timesteps_pwvb_short  = raw_config[Key::Config::apwvb_timesteps_pwvb_short ].get < std::size_t > ();
				config.apwvb_timesteps_sma_long    = raw_config[Key::Config::apwvb_timesteps_sma_long   ].get < std::size_t > ();
				config.apwvb_timesteps_sma_short   = raw_config[Key::Config::apwvb_timesteps_sma_short  ].get < std::size_t > ();
				config.asset                       = raw_config[Key::Config::asset                      ].get < std::string > ();
				config.combinator_neighbors        = raw_config[Key::Config::combinator_neighbors       ].get < std::size_t > ();
				config.combinator_threshold_long   = raw_config[Key::Config::combinator_threshold_long  ].get < double > ();
				config.combinator_threshold_short  = raw_config[Key::Config::combinator_threshold_short ].get < double > ();
				config.commission                  = raw_config[Key::Config::commission                 ].get < double > ();
				config.ema_timesteps_fast          = raw_config[Key::Config::ema_timesteps_fast         ].get < std::size_t > ();
				config.ema_timesteps_slow          = raw_config[Key::Config::ema_timesteps_slow         ].get < std::size_t > ();
				config.has_reinvestment            = raw_config[Key::Config::has_reinvestment           ].get < bool > ();
				config.inputs_first_year           = raw_config[Key::Config::inputs_first_year          ].get < std::time_t > ();
				config.inputs_last_year            = raw_config[Key::Config::inputs_last_year           ].get < std::time_t > ();
				config.investment                  = raw_config[Key::Config::investment                 ].get < double > ();
				config.level_deviation             = raw_config[Key::Config::level_deviation            ].get < double > ();
				config.level_max_bias              = raw_config[Key::Config::level_max_bias             ].get < std::size_t > ();
				config.level_min_bias              = raw_config[Key::Config::level_min_bias             ].get < std::size_t > ();
				config.permitted_state             = raw_config[Key::Config::permitted_state            ].get < std::string > ();
				config.profit_rollback_long        = raw_config[Key::Config::profit_rollback_long       ].get < double > ();
				config.profit_rollback_short       = raw_config[Key::Config::profit_rollback_short      ].get < double > ();
				config.pulse_threshold             = raw_config[Key::Config::pulse_threshold            ].get < double > ();
				config.required_backtest           = raw_config[Key::Config::required_backtest          ].get < bool > ();
				config.required_backtest_fit       = raw_config[Key::Config::required_backtest_fit      ].get < bool > ();
				config.required_inputs             = raw_config[Key::Config::required_inputs            ].get < bool > ();
				config.rsi_threshold_long          = raw_config[Key::Config::rsi_threshold_long         ].get < double > ();
				config.rsi_threshold_short         = raw_config[Key::Config::rsi_threshold_short        ].get < double > ();
				config.rsi_timesteps_long          = raw_config[Key::Config::rsi_timesteps_long         ].get < std::size_t > ();
				config.rsi_timesteps_short         = raw_config[Key::Config::rsi_timesteps_short        ].get < std::size_t > ();
				config.skipped_timesteps           = raw_config[Key::Config::skipped_timesteps          ].get < std::size_t > ();
				config.stop_loss_long              = raw_config[Key::Config::stop_loss_long             ].get < double > ();
				config.stop_loss_short             = raw_config[Key::Config::stop_loss_short            ].get < double > ();
				config.strategy                    = raw_config[Key::Config::strategy                   ].get < std::string > ();
				config.take_profit_long            = raw_config[Key::Config::take_profit_long           ].get < double > ();
				config.take_profit_short           = raw_config[Key::Config::take_profit_short          ].get < double > ();
				config.timeframe_backtest          = raw_config[Key::Config::timeframe_backtest         ].get < std::string > ();
				config.timeframe_strategy          = raw_config[Key::Config::timeframe_strategy         ].get < std::string > ();
				config.timesteps_prediction        = raw_config[Key::Config::timesteps_prediction       ].get < std::size_t > ();
				config.timesteps_prehistory        = raw_config[Key::Config::timesteps_prehistory       ].get < std::size_t > ();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::Data::load_assets(assets_container_t & assets)
		{
			LOGGER(logger);

			try
			{
				auto path = Directory::config / File::assets_data;

				std::fstream fin(path.string(), std::ios::in);

				if (!fin)
				{
					throw system_exception("cannot open file " + path.string());
				}

				std::string asset;

				while (std::getline(fin, asset))
				{
					assets.push_back(asset);
				}

				assets.shrink_to_fit();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::Data::load_clients(clients_container_t & clients)
		{
			LOGGER(logger);

			try
			{
				auto path = Directory::config / File::clients_json;

				json_t array;

				load(path, array);

				for (const auto & element : array)
				{
					auto name                = element[Key::Client::name               ].get < std::string > ();
					auto public_key          = element[Key::Client::public_key         ].get < std::string > ();
					auto secret_key          = element[Key::Client::secret_key         ].get < std::string > ();
					auto initial_investments = element[Key::Client::initial_investments].get < double > ();

					clients.push_back({ name, public_key, secret_key, initial_investments });
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::Data::save_source_config(const path_t & path, const Config & config)
		{
			LOGGER(logger);

			try
			{
				json_t raw_config;

				raw_config[Key::Config::accelerator_flat_deviation ] = config.accelerator_flat_deviation;
				raw_config[Key::Config::accelerator_max_deviation_s] = config.accelerator_max_deviation_s;
				raw_config[Key::Config::accelerator_min_deviation_s] = config.accelerator_min_deviation_s;
				raw_config[Key::Config::accelerator_stop_loss      ] = config.accelerator_stop_loss;
				raw_config[Key::Config::apwvb_threshold_long       ] = config.apwvb_threshold_long;
				raw_config[Key::Config::apwvb_threshold_short      ] = config.apwvb_threshold_short;
				raw_config[Key::Config::apwvb_timesteps_pwvb_long  ] = config.apwvb_timesteps_pwvb_long;
				raw_config[Key::Config::apwvb_timesteps_pwvb_short ] = config.apwvb_timesteps_pwvb_short;
				raw_config[Key::Config::apwvb_timesteps_sma_long   ] = config.apwvb_timesteps_sma_long;
				raw_config[Key::Config::apwvb_timesteps_sma_short  ] = config.apwvb_timesteps_sma_short;
				raw_config[Key::Config::asset                      ] = config.asset;
				raw_config[Key::Config::combinator_neighbors       ] = config.combinator_neighbors;
				raw_config[Key::Config::combinator_threshold_long  ] = config.combinator_threshold_long;
				raw_config[Key::Config::combinator_threshold_short ] = config.combinator_threshold_short;
				raw_config[Key::Config::commission                 ] = config.commission;
				raw_config[Key::Config::ema_timesteps_fast         ] = config.ema_timesteps_fast;
				raw_config[Key::Config::ema_timesteps_slow         ] = config.ema_timesteps_slow;
				raw_config[Key::Config::has_reinvestment           ] = config.has_reinvestment;
				raw_config[Key::Config::inputs_first_year          ] = config.inputs_first_year;
				raw_config[Key::Config::inputs_last_year           ] = config.inputs_last_year;
				raw_config[Key::Config::investment                 ] = config.investment;
				raw_config[Key::Config::level_deviation            ] = config.level_deviation;
				raw_config[Key::Config::level_max_bias             ] = config.level_max_bias;
				raw_config[Key::Config::level_min_bias             ] = config.level_min_bias;
				raw_config[Key::Config::permitted_state            ] = config.permitted_state;
				raw_config[Key::Config::profit_rollback_long       ] = config.profit_rollback_long;
				raw_config[Key::Config::profit_rollback_short      ] = config.profit_rollback_short;
				raw_config[Key::Config::pulse_threshold            ] = config.pulse_threshold;
				raw_config[Key::Config::required_backtest          ] = config.required_backtest;
				raw_config[Key::Config::required_backtest_fit      ] = config.required_backtest_fit;
				raw_config[Key::Config::required_inputs            ] = config.required_inputs;
				raw_config[Key::Config::rsi_threshold_long         ] = config.rsi_threshold_long;
				raw_config[Key::Config::rsi_threshold_short        ] = config.rsi_threshold_short;
				raw_config[Key::Config::rsi_timesteps_long         ] = config.rsi_timesteps_long;
				raw_config[Key::Config::rsi_timesteps_short        ] = config.rsi_timesteps_short;
				raw_config[Key::Config::skipped_timesteps          ] = config.skipped_timesteps;
				raw_config[Key::Config::stop_loss_long             ] = config.stop_loss_long;
				raw_config[Key::Config::stop_loss_short            ] = config.stop_loss_short;
				raw_config[Key::Config::strategy                   ] = config.strategy;
				raw_config[Key::Config::take_profit_long           ] = config.take_profit_long;
				raw_config[Key::Config::take_profit_short          ] = config.take_profit_short;
				raw_config[Key::Config::timeframe_backtest         ] = config.timeframe_backtest;
				raw_config[Key::Config::timeframe_strategy         ] = config.timeframe_strategy;
				raw_config[Key::Config::timesteps_prediction       ] = config.timesteps_prediction;
				raw_config[Key::Config::timesteps_prehistory       ] = config.timesteps_prehistory;

				save(path, raw_config);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::Data::save_correlation(const correlation_matrix_t & correlation_matrix)
		{
			LOGGER(logger);

			try
			{
				std::filesystem::create_directory(Directory::output);

				auto path = Directory::output / File::correlation_data;

				std::fstream fout(path.string(), std::ios::out);

				if (!fout)
				{
					throw system_exception("cannot open file " + path.string());
				}

				std::ostringstream sout;

				auto size = correlation_matrix.size();

				for (auto i = 0ULL; i < size; ++i)
				{
					for (auto j = 0ULL; j < size; ++j)
					{
						sout << std::setw(2 + 1 + 3) << std::setfill(' ') << std::right << 
							std::setprecision(3) << std::fixed << std::showpos << correlation_matrix[i][j] << " ";
					}

					sout << "\n";
				}

				fout << sout.str();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::Data::load(const path_t & path, json_t & object)
		{
			LOGGER(logger);

			try
			{
				std::fstream fin(path.string(), std::ios::in);

				if (!fin)
				{
					throw system_exception("cannot open file " + path.string());
				}

				object = json_t::parse(fin);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::Data::save(const path_t & path, const json_t & object)
		{
			LOGGER(logger);

			try
			{
				std::fstream fout(path.string(), std::ios::out);

				if (!fout)
				{
					throw system_exception("cannot open file " + path.string());
				}

				fout << std::setw(4) << object;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::initialize()
		{
			LOGGER(logger);

			try
			{
				m_database = std::make_shared < database_t > ();

				load();

				if (m_config.required_correlation)
				{
					handle_correlation();
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::uninitialize()
		{
			LOGGER(logger);

			try
			{
				m_thread_pool.join();

				save();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::load()
		{
			LOGGER(logger);

			try
			{
				load_python();

				load_config();

				load_assets();

				load_sources();

				load_clients();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::save() const
		{
			LOGGER(logger);

			try
			{
				save_sources();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::load_python()
		{
			LOGGER(logger);

			try
			{
				boost::python::exec("from system import make_client",         m_python.global(), m_python.global());
				boost::python::exec("from system import get_current_state",   m_python.global(), m_python.global());
				boost::python::exec("from system import get_available_usdt",  m_python.global(), m_python.global());
				boost::python::exec("from system import make_null_position",  m_python.global(), m_python.global());
				boost::python::exec("from system import make_long_position",  m_python.global(), m_python.global());
				boost::python::exec("from system import make_short_position", m_python.global(), m_python.global());
			}
			catch (const boost::python::error_already_set &)
			{
				LOGGER_WRITE_ERROR(logger, shared::Python::exception());
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::load_config()
		{
			LOGGER(logger);

			try
			{
				Data::load_system_config(m_config);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::load_assets()
		{
			LOGGER(logger);

			try
			{
				Data::load_assets(m_assets);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::load_sources()
		{
			LOGGER(logger);

			try
			{
				m_sources.reserve(std::size(m_assets));

				m_database->reserve(std::size(m_assets));

				for (const auto & asset : m_assets)
				{
					Config config;

					Data::load_source_config(Data::Directory::config / 
						asset / Data::File::config_json, config);

					m_sources[asset] = std::make_shared < Source > (config, m_python);

					m_database->push_back(m_sources[asset]->load_inputs(config.timeframe_strategy, true));
				}

				update_sources();

				handle_sources();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::load_clients()
		{
			LOGGER(logger);

			try
			{
				Data::load_clients(m_clients);

				try
				{
					for (const auto & client : m_clients)
					{
						m_python.global()["make_client"](
							client.public_key.c_str(), 
							client.secret_key.c_str());
					}
				}
				catch (const boost::python::error_already_set &)
				{
					LOGGER_WRITE_ERROR(logger, shared::Python::exception());
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::update_sources()
		{
			LOGGER(logger);

			try
			{
				for (auto [asset, source] : m_sources)
				{
					source->update_database(m_database);
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::handle_sources()
		{
			LOGGER(logger);

			try
			{
				for (auto [asset, source] : m_sources)
				{
					source->handle();
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::save_sources() const
		{
			LOGGER(logger);

			try
			{
				for (const auto & [asset, source] : m_sources)
				{
					Data::save_source_config(Data::Directory::config /
						asset / Data::File::config_json, source->config());
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::handle_correlation()
		{
			LOGGER(logger);

			try
			{
				Data::save_correlation(make_correlation());
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		System::correlation_matrix_t System::make_correlation()
		{
			LOGGER(logger);

			try
			{
				const auto size = std::size(m_assets);

				std::vector < std::future < double > > futures;

				futures.reserve(size * (size - 1) / 2);

				for (auto i = 0ULL; i < size; ++i)
				{
					for (auto j = i + 1; j < size; ++j)
					{
						std::packaged_task < double() > task([this, i, j]()
							{ return make_correlation(m_assets[i], m_assets[j]); });

						futures.push_back(boost::asio::post(m_thread_pool, std::move(task)));
					}
				}

				auto index = 0ULL;

				correlation_matrix_t correlation_matrix(boost::extents[size][size]);

				for (auto i = 0ULL; i < size; ++i)
				{
					for (auto j = 0ULL; j < size; ++j)
					{
						if (i == j)
						{
							correlation_matrix[i][j] = 1.0;
						}
						else
						{
							if (i > j)
							{
								correlation_matrix[i][j] = correlation_matrix[j][i];
							}
							else
							{
								correlation_matrix[i][j] = futures[index++].get();
							}
						}
					}
				}
				
				return correlation_matrix;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		double System::make_correlation(const std::string & asset_1, const std::string & asset_2)
		{
			LOGGER(logger);

			try
			{
				const auto & config_1 = m_sources.at(asset_1)->config();
				const auto & config_2 = m_sources.at(asset_2)->config();

				if (config_1.timeframe_strategy != config_2.timeframe_strategy)
				{
					throw system_exception("invalid timeframes pair");
				}

				const auto & klines_1 = m_sources.at(asset_1)->load_klines(config_1.timeframe_strategy);
				const auto & klines_2 = m_sources.at(asset_2)->load_klines(config_2.timeframe_strategy);

				auto size = std::min(std::size(klines_1), std::size(klines_2));

				std::vector < int > signs_1(size, 0);
				std::vector < int > signs_2(size, 0);

				std::transform(std::crbegin(klines_1), std::next(std::crbegin(klines_1), size),
					std::begin(signs_1), [](const auto & kline) 
						{ return (kline.price_close > kline.price_open ? +1 : -1); });

				std::transform(std::crbegin(klines_2), std::next(std::crbegin(klines_2), size),
					std::begin(signs_2), [](const auto & kline)
						{ return (kline.price_close > kline.price_open ? +1 : -1); });

				return (std::transform_reduce(std::begin(signs_1), std::end(signs_1), std::begin(signs_2),
					0.0, std::plus(), [](auto lhs, auto rhs) { return (lhs == rhs ? +1.0 : 0.0); }) / size);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::run()
		{
			LOGGER(logger);

			try
			{
				if (m_config.required_run)
				{
					auto future = std::async(std::launch::async, [this]() 
						{
							m_is_running.store(true);

							//wait_until_hour_end(); // TODO

							if (!m_is_interrupted.load())
							{
								m_timer.expires_from_now(boost::posix_time::seconds(0));

								handle();
							}

							m_io_service.run();

							m_is_running.store(false);
						});

					std::string command;

					while (true)
					{
						std::cin >> command;

						if (command == "exit")
						{
							m_is_interrupted.store(true);

							while (m_is_running.load())
							{
								m_io_service.stop();
							}

							break;
						}
						else
						{
							std::cout << "command not found" << std::endl;
						}
					}

					future.wait();
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::wait_until_hour_end() const // TODO
		{
			LOGGER(logger);

			try
			{
				while (!m_is_interrupted.load())
				{
					auto time = boost::posix_time::second_clock::universal_time();

					if (time.time_of_day().hours()   == m_config.server_start_hour &&
						time.time_of_day().minutes() == m_config.server_start_minute)
					{
						break;
					}

					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::handle()
		{
			LOGGER(logger);

			try
			{
				m_timer.expires_at(m_timer.expires_at() + 
					boost::posix_time::seconds(m_config.interval));
				
				m_timer.async_wait(boost::bind(&System::handle, this));

				for (const auto & asset : m_assets)
				{
					handle_implementation(asset);
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::handle_implementation(const std::string & asset)
		{
			LOGGER(logger);

			try
			{
				auto current_state = convert_state(boost::python::extract < std::string > (
					m_python.global()["get_current_state"](m_clients.front().public_key.c_str(), asset.c_str())));

				auto required_state = m_sources[asset]->handle(current_state);

				char states[] = { 'N', 'L', 'S' }; // TODO

				std::cout << std::setw(8) << std::setfill(' ') << std::right << asset << " : " <<
					states[static_cast < int > (current_state )] << " -> " <<
					states[static_cast < int > (required_state)] << std::endl; // TODO

				if (false && current_state != required_state) // TODO
				{
					for (const auto & client : m_clients)
					{
						try
						{
							if (current_state != State::N)
							{
								m_python.global()["make_null_position"](client.public_key.c_str(), asset.c_str());
							}

							auto available_usdt = std::stod(boost::python::extract < std::string > (
								m_python.global()["get_available_usdt"](client.public_key.c_str())));

							auto position = std::min(client.initial_investments * (1.0 - m_config.reserve) *
								(m_sources[asset]->config().investment / 1000.0), available_usdt);

							auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

							std::cout << std::put_time(std::localtime(&time), "%y.%m.%d %H:%M:%S") << " : ";

							switch (required_state)
							{
							case State::L:
							{
								std::cout << std::setw(20) << std::setfill('.') << std::left <<
									client.name + " " << " : required L for " << asset << " on " <<
									std::setw(8) << std::setfill(' ') << std::right <<
									std::setprecision(2) << std::fixed << std::noshowpos <<
									position << " USDT" << std::endl;

								m_python.global()["make_long_position"](client.public_key.c_str(),
									asset.c_str(), std::to_string(position).c_str());

								break;
							}
							case State::S:
							{
								std::cout << std::setw(20) << std::setfill('.') << std::left <<
									client.name + " " << " : required S for " << asset << " on " <<
									std::setw(8) << std::setfill(' ') << std::right <<
									std::setprecision(2) << std::fixed << std::noshowpos <<
									position << " USDT" << std::endl;

								m_python.global()["make_short_position"](client.public_key.c_str(),
									asset.c_str(), std::to_string(position).c_str());

								break;
							}
							default:
							{
								std::cout << std::setw(20) << std::setfill('.') << std::left <<
									client.name + " " << " : required N for " << asset << std::endl;

								break;
							}
							}
						}
						catch (const boost::python::error_already_set &)
						{
							LOGGER_WRITE_ERROR(logger, shared::Python::exception());
						}
					}
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		System::State System::convert_state(const std::string & state) const
		{
			LOGGER(logger);

			try
			{
				auto c = state.front();

				if (c != 'L' && c != 'S' && c != 'N')
				{
					throw system_exception("invalid current state");
				}

				if (c == 'L')
				{
					return State::L;
				}
				else if (c == 'S')
				{
					return State::S;
				}
				else
				{
					return State::N;
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

	} // namespace system

} // namespace solution