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

				config.test                   = raw_config[Key::Config::test                  ].get < bool > ();
				config.required_run           = raw_config[Key::Config::required_run          ].get < bool > ();
				config.interval               = raw_config[Key::Config::interval              ].get < std::time_t > ();
				config.max_drawdown           = raw_config[Key::Config::max_drawdown          ].get < double > ();
				config.benchmark              = raw_config[Key::Config::benchmark             ].get < std::string > ();
				config.server_start_hour      = raw_config[Key::Config::server_start_hour     ].get < std::time_t > ();
				config.server_start_minute    = raw_config[Key::Config::server_start_minute   ].get < std::time_t > ();
				config.server_start_iteration = raw_config[Key::Config::server_start_iteration].get < std::size_t > ();
				config.required_correlation   = raw_config[Key::Config::required_correlation  ].get < bool > ();

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
				config.required_markup               = raw_config[Key::Config::required_markup              ].get < bool > ();
				config.ema_timesteps                 = raw_config[Key::Config::ema_timesteps                ].get < std::size_t > ();
				config.rsi_timesteps                 = raw_config[Key::Config::rsi_timesteps                ].get < std::size_t > ();
				config.rsi_threshold_long            = raw_config[Key::Config::rsi_threshold_long           ].get < double > ();
				config.rsi_threshold_short           = raw_config[Key::Config::rsi_threshold_short          ].get < double > ();
				config.stop_loss_relaxation          = raw_config[Key::Config::stop_loss_relaxation         ].get < std::size_t > ();
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
				raw_config[Key::Config::required_markup              ] = config.required_markup;
				raw_config[Key::Config::ema_timesteps                ] = config.ema_timesteps;
				raw_config[Key::Config::rsi_timesteps                ] = config.rsi_timesteps;
				raw_config[Key::Config::rsi_threshold_long           ] = config.rsi_threshold_long;
				raw_config[Key::Config::rsi_threshold_short          ] = config.rsi_threshold_short;
				raw_config[Key::Config::stop_loss_relaxation         ] = config.stop_loss_relaxation;

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

				if ((config_1.inputs_timeframe      != config_2.inputs_timeframe     ) ||
					(config_1.inputs_timeframe_type != config_2.inputs_timeframe_type))
				{
					throw system_exception("invalid timeframes pair");
				}

				const auto & klines_1 = m_sources.at(asset_1)->load_klines();
				const auto & klines_2 = m_sources.at(asset_2)->load_klines();

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

							wait_until_day_end();

							if (!m_is_interrupted.load())
							{
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

		void System::wait_until_day_end() const
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
				static auto counter = m_config.server_start_iteration;

				boost::posix_time::seconds interval(m_config.interval);

				if (counter == m_config.server_start_iteration)
				{
					m_timer.expires_from_now(interval);
				}
				else
				{
					m_timer.expires_at(m_timer.expires_at() + interval);
				}
				
				m_timer.async_wait(boost::bind(&System::handle, this));

				auto n_iterations = static_cast < std::size_t > (
					seconds_in_day / m_config.interval);

				if (counter++ % n_iterations == 0)
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

				for (const auto & client : m_clients)
				{
					try
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
					catch (const boost::python::error_already_set &)
					{
						LOGGER_WRITE_ERROR(logger, shared::Python::exception());
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