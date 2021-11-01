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

				config.required_run = raw_config[Key::Config::required_run].get < bool > ();
				config.interval     = raw_config[Key::Config::interval    ].get < std::time_t > ();
				config.max_drawdown = raw_config[Key::Config::max_drawdown].get < double > ();
				config.benchmark    = raw_config[Key::Config::benchmark   ].get < std::string > ();

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

				config.test                          = raw_config[Key::Config::test                         ].get < bool > ();
				config.required_inputs               = raw_config[Key::Config::required_inputs              ].get < bool > ();
				config.inputs_year_begin             = raw_config[Key::Config::inputs_year_begin            ].get < std::time_t > ();
				config.inputs_year_end               = raw_config[Key::Config::inputs_year_end              ].get < std::time_t > ();
				config.inputs_timeframe              = raw_config[Key::Config::inputs_timeframe             ].get < std::size_t > ();
				config.inputs_timeframe_type         = raw_config[Key::Config::inputs_timeframe_type        ].get < std::string > ();
				config.inputs_asset                  = raw_config[Key::Config::inputs_asset                 ].get < std::string > ();
				config.price_aggregated_trade_step   = raw_config[Key::Config::price_aggregated_trade_step  ].get < double > ();
				config.price_aggregated_trades_depth = raw_config[Key::Config::price_aggregated_trades_depth].get < std::size_t > ();
				config.timesteps_prehistory          = raw_config[Key::Config::timesteps_prehistory         ].get < std::size_t > ();
				config.timesteps_prediction          = raw_config[Key::Config::timesteps_prediction         ].get < std::size_t > ();
				config.skipped_timesteps             = raw_config[Key::Config::skipped_timesteps            ].get < std::size_t > ();
				config.min_movement                  = raw_config[Key::Config::min_movement                 ].get < double > ();
				config.transaction                   = raw_config[Key::Config::transaction                  ].get < double > ();
				config.commission                    = raw_config[Key::Config::commission                   ].get < double > ();
				config.stop_loss                     = raw_config[Key::Config::stop_loss                    ].get < double > ();
				config.test_hard_strategy            = raw_config[Key::Config::test_hard_strategy           ].get < std::string > ();
				config.test_soft_strategy            = raw_config[Key::Config::test_soft_strategy           ].get < std::string > ();
				config.required_backtest             = raw_config[Key::Config::required_backtest            ].get < bool > ();
				config.awvb_timesteps_wvb            = raw_config[Key::Config::awvb_timesteps_wvb           ].get < std::size_t > ();
				config.awvb_timesteps_sma            = raw_config[Key::Config::awvb_timesteps_sma           ].get < std::size_t > ();
				config.required_research             = raw_config[Key::Config::required_research            ].get < bool > ();
				config.assimilator_min_deviation     = raw_config[Key::Config::assimilator_min_deviation    ].get < double > ();
				config.required_backtest_fit         = raw_config[Key::Config::required_backtest_fit        ].get < bool > ();
				config.has_reinvestment              = raw_config[Key::Config::has_reinvestment             ].get < bool > ();
				config.main_strategy                 = raw_config[Key::Config::main_strategy                ].get < std::string > ();
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

				raw_config[Key::Config::test                         ] = config.test;
				raw_config[Key::Config::required_inputs              ] = config.required_inputs;
				raw_config[Key::Config::inputs_year_begin            ] = config.inputs_year_begin;
				raw_config[Key::Config::inputs_year_end              ] = config.inputs_year_end;
				raw_config[Key::Config::inputs_timeframe             ] = config.inputs_timeframe;
				raw_config[Key::Config::inputs_timeframe_type        ] = config.inputs_timeframe_type;
				raw_config[Key::Config::inputs_asset                 ] = config.inputs_asset;
				raw_config[Key::Config::price_aggregated_trade_step  ] = config.price_aggregated_trade_step;
				raw_config[Key::Config::price_aggregated_trades_depth] = config.price_aggregated_trades_depth;
				raw_config[Key::Config::timesteps_prehistory         ] = config.timesteps_prehistory;
				raw_config[Key::Config::timesteps_prediction         ] = config.timesteps_prediction;
				raw_config[Key::Config::skipped_timesteps            ] = config.skipped_timesteps;
				raw_config[Key::Config::min_movement                 ] = config.min_movement;
				raw_config[Key::Config::transaction                  ] = config.transaction;
				raw_config[Key::Config::commission                   ] = config.commission;
				raw_config[Key::Config::stop_loss                    ] = config.stop_loss;
				raw_config[Key::Config::test_hard_strategy           ] = config.test_hard_strategy;
				raw_config[Key::Config::test_soft_strategy           ] = config.test_soft_strategy;
				raw_config[Key::Config::required_backtest            ] = config.required_backtest;
				raw_config[Key::Config::awvb_timesteps_wvb           ] = config.awvb_timesteps_wvb;
				raw_config[Key::Config::awvb_timesteps_sma           ] = config.awvb_timesteps_sma;
				raw_config[Key::Config::required_research            ] = config.required_research;
				raw_config[Key::Config::assimilator_min_deviation    ] = config.assimilator_min_deviation;
				raw_config[Key::Config::required_backtest_fit        ] = config.required_backtest_fit;
				raw_config[Key::Config::has_reinvestment             ] = config.has_reinvestment;
				raw_config[Key::Config::main_strategy                ] = config.main_strategy;

				save(path, raw_config);
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
				load();
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

				for (const auto & asset : m_assets)
				{
					Config config;

					Data::load_source_config(Data::Directory::config / 
						asset / Data::File::config_json, config);

					m_sources[asset] = std::make_shared < Source > (std::move(config), m_python);
				}
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

							wait_until_day_end();

							if (!m_is_interrupted.load())
							{
								handle();
							}

							m_io_service.run();

							m_is_running.store(false);
						});

					std::string command;

					std::cin >> command;

					if (command == "exit")
					{
						m_is_interrupted.store(true);

						while (m_is_running.load())
						{
							m_io_service.stop();
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

		void System::wait_until_day_end() const
		{
			LOGGER(logger);

			try
			{
				while (!m_is_interrupted.load())
				{
					auto time = boost::posix_time::second_clock::universal_time();

					if (time.time_of_day().hours()   == 23 &&
						time.time_of_day().minutes() == 50)
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
				static std::size_t counter = 0;

				const auto interval = m_config.interval;

				if (counter == 0)
				{
					m_timer.expires_from_now(boost::posix_time::seconds(interval));
				}
				else
				{
					m_timer.expires_at(m_timer.expires_at() + 
						boost::posix_time::seconds(interval));
				}
				
				m_timer.async_wait(boost::bind(&System::handle, this));

				if (counter++ % static_cast < std::size_t > (seconds_in_day / interval) == 0)
				{
					for (const auto & asset : m_assets)
					{
						handle_implementation(asset);
					}
				}
				else
				{
					handle_implementation(m_config.benchmark);
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
				auto required_state = m_sources[asset]->handle();

				try
				{
					for (const auto & client : m_clients)
					{
						auto current_state = convert_state(boost::python::extract < std::string > (
							m_python.global()["get_current_state"](client.public_key.c_str(), asset.c_str())));

						if (current_state != required_state)
						{
							if (current_state != State::N)
							{
								m_python.global()["make_null_position"](client.public_key.c_str(), asset.c_str());
							}

							auto available_usdt = std::stod(boost::python::extract < std::string > (
								m_python.global()["get_available_usdt"](client.public_key.c_str())));

							auto position = std::min(client.initial_investments * (1.0 - m_config.max_drawdown) *
								(m_sources[asset]->config().transaction / 1000.0), available_usdt);

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