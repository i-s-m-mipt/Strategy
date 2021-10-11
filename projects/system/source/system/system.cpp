#include "system.hpp"

namespace solution
{
	namespace system
	{
		void System::Data::load_config(Config & config)
		{
			LOGGER(logger);

			try
			{
				json_t raw_config;

				load(File::config_json, raw_config);

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
				config.test_strategy                 = raw_config[Key::Config::test_strategy                ].get < std::string > ();
				config.required_backtest             = raw_config[Key::Config::required_backtest            ].get < bool > ();
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
				auto path = File::assets_data;

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

		void System::Data::save_inputs(const inputs_container_t & inputs, const Config & config)
		{
			LOGGER(logger);

			try
			{
				auto path = File::inputs_data;

				std::fstream fout(path.string(), std::ios::out);

				if (!fout)
				{
					throw system_exception("cannot open file " + path.string());
				}

				const auto delimeter = ',';

				std::stringstream sout;

				for (auto i = config.skipped_timesteps; i < std::size(inputs); ++i)
				{
					const auto & input = inputs[i];

					for (auto j = 0U; j < 7U; ++j)
					{
						sout << std::noshowpos << (j == input.day ? 1 : 0) << delimeter;
					}

					for (auto j = 0U; j < 24U; ++j)
					{
						sout << std::noshowpos << (j == input.hour ? 1 : 0) << delimeter;
					}

					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.price_open  << delimeter;
					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.price_high  << delimeter;
					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.price_low   << delimeter;
					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.price_close << delimeter;

					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.volume_base  << delimeter;
					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.volume_quote << delimeter;

					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.volume_buy_base  << delimeter;
					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.volume_buy_quote << delimeter;

					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.volume_sell_base  << delimeter;
					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.volume_sell_quote << delimeter;

					for (const auto & [type, variants] : input.indicators)
					{
						for (const auto & [name, value] : variants)
						{
							sout << std::setprecision(6) << std::fixed << // ?
								std::showpos << value << delimeter;
						}
					}

					// TODO (save price_aggregated_trades)

					sout << std::showpos << input.movement_tag << '\n';
				}

				fout << sout.str();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::Data::save_result(const Result & result)
		{
			LOGGER(logger);

			try
			{
				save_reward(result);
				save_trades(result);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::Data::save_reward(const Result & result)
		{
			LOGGER(logger);

			try
			{
				auto path = File::reward_data;

				std::fstream fout(path.string(), std::ios::out);

				if (!fout)
				{
					throw system_exception("cannot open file " + path.string());
				}

				const auto delimeter = ',';

				for (const auto & [date_time, reward] : result.rewards)
				{
					fout << std::noshowpos << date_time << delimeter;

					fout << std::noshowpos << detail::to_time_t(date_time) << delimeter;

					fout << std::setprecision(3) << std::fixed << std::showpos << reward << '\n';
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::Data::save_trades(const Result & result)
		{
			LOGGER(logger);

			try
			{
				auto path = File::trades_data;

				std::fstream fout(path.string(), std::ios::out);

				if (!fout)
				{
					throw system_exception("cannot open file " + path.string());
				}

				const auto delimeter = ',';

				for (const auto & trade : result.trades)
				{
					fout << std::noshowpos << trade.begin << delimeter;
					fout << std::noshowpos << trade.end   << delimeter;

					fout << std::noshowpos << detail::to_time_t(trade.begin) << delimeter;
					fout << std::noshowpos << detail::to_time_t(trade.end)   << delimeter;

					switch (trade.type) 
					{
					case Result::Trade::Type::L:
					{
						fout << "long" << delimeter;

						break;
					}
					case Result::Trade::Type::S:
					{
						fout << "short" << delimeter;

						break;
					}
					default:
					{
						fout << "unknown" << delimeter;

						break;
					}
					}

					fout << std::setprecision(3) << std::fixed << std::showpos << trade.reward << '\n';
				}
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
				std::filesystem::create_directory(inputs_directory);
				std::filesystem::create_directory(result_directory);

				load();

				if (m_config.required_inputs)
				{
					handle_inputs();
				}

				if (m_config.required_backtest)
				{
					handle_backtest();
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
				load_config();

				load_assets();

				load_indicators();

				load_strategies();
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
				Data::load_config(m_config);
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

		void System::load_indicators()
		{
			LOGGER(logger);

			try
			{
				m_indicators.push_back(indicators::ADX ("default",  14          ));
				m_indicators.push_back(indicators::EMA ("6",         6          ));
				m_indicators.push_back(indicators::EMA ("12",       12          ));
				m_indicators.push_back(indicators::EMA ("48",       48          ));
				m_indicators.push_back(indicators::EMA ("120",     120          ));
				m_indicators.push_back(indicators::EMA ("240",     240          ));
				m_indicators.push_back(indicators::EMA ("288",     288          ));
				m_indicators.push_back(indicators::MACD("default",  12,  26,   9));
				m_indicators.push_back(indicators::MACD("slow",     60, 130,  45));
				m_indicators.push_back(indicators::MFI ("default",  14          ));
				m_indicators.push_back(indicators::RSI ("default",  14          ));
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::load_strategies()
		{
			LOGGER(logger);

			try
			{
				m_strategies[strategies::hard::BUY_HOLD::type] =
					std::make_shared < strategies::hard::BUY_HOLD > ();
				m_strategies[strategies::hard::MACD_MFI::type] =
					std::make_shared < strategies::hard::MACD_MFI > ();
				m_strategies[strategies::hard::EMA_288::type] =
					std::make_shared < strategies::hard::EMA_288 > ();
				m_strategies[strategies::hard::EMA_ADX::type] =
					std::make_shared < strategies::hard::EMA_ADX > ();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::handle_inputs() const
		{
			LOGGER(logger);

			try
			{
				auto inputs = load_inputs();

				save_inputs(inputs);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::handle_backtest() const
		{
			LOGGER(logger);

			try
			{
				{
					Backtester backtester(m_config, load_inputs(),
						m_strategies.at(strategies::hard::BUY_HOLD::type));

					save_result(backtester.run());
				}

				std::filesystem::rename(Data::File::reward_data, Data::File::reward_BH_data);
				std::filesystem::rename(Data::File::trades_data, Data::File::trades_BH_data);

				{
					Backtester backtester(m_config, load_inputs(),
						m_strategies.at(m_config.test_strategy));

					save_result(backtester.run());
				}

				make_report();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		System::inputs_container_t System::load_inputs() const
		{
			LOGGER(logger);

			try
			{
				auto klines = load_klines();
				auto orders = load_orders();
				auto trades = load_trades();

				return make_inputs(klines, orders, trades);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		System::klines_container_t System::load_klines() const
		{
			LOGGER(logger);

			try
			{
				const auto epsilon = std::numeric_limits < double > ::epsilon();

				klines_container_t klines;

				for (auto year = m_config.inputs_year_begin; year <= m_config.inputs_year_end; ++year)
				{
					for (std::time_t month = 1LL; month <= 12LL; ++month)
					{
						auto path = klines_directory; path /= make_klines_file_name(year, month);

						if (!std::filesystem::exists(path))
						{
							LOGGER_WRITE_ERROR(logger, "file " + path.string() + " doesn't exist");

							continue;
						}

						std::fstream fin(path.string(), std::ios::in);

						if (!fin)
						{
							throw system_exception("cannot open file " + path.string());
						}

						std::string line;

						while (std::getline(fin, line))
						{
							auto kline = parse_kline(line);

							if (kline.price_open < epsilon)
							{
								throw std::domain_error("required: (price_open > 0.0)");
							}

							if (kline.price_high < epsilon)
							{
								throw std::domain_error("required: (price_high > 0.0)");
							}

							if (kline.price_low < epsilon)
							{
								throw std::domain_error("required: (price_low > 0.0)");
							}

							if (kline.price_close < epsilon)
							{
								throw std::domain_error("required: (price_close > 0.0)");
							}

							klines.push_back(std::move(kline));
						}
					}
				}

				std::sort(std::begin(klines), std::end(klines), 
					[](const auto & lhs, const auto & rhs)
						{ return (lhs.time_open < rhs.time_open); });

				return klines;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		System::orders_container_t System::load_orders() const
		{
			LOGGER(logger);

			try
			{
				return {}; // TODO
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		System::trades_container_t System::load_trades() const
		{
			LOGGER(logger);

			try
			{
				trades_container_t trades;

				for (auto year = m_config.inputs_year_begin; year <= m_config.inputs_year_end; ++year)
				{
					for (std::time_t month = 1LL; month <= 12LL; ++month)
					{
						auto path = trades_directory; path /= make_trades_file_name(year, month);

						if (!std::filesystem::exists(path))
						{
							LOGGER_WRITE_ERROR(logger, "file " + path.string() + " doesn't exist");

							continue;
						}

						std::fstream fin(path.string(), std::ios::in);

						if (!fin)
						{
							throw system_exception("cannot open file " + path.string());
						}

						std::string line;

						while (std::getline(fin, line))
						{
							trades.push_back(parse_trade(line));
						}
					}
				}

				std::sort(std::begin(trades), std::end(trades),
					[](const auto & lhs, const auto & rhs)
						{ return (lhs.time < rhs.time); });

				return trades;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		std::string System::make_klines_file_name(std::time_t year, std::time_t month) const
		{
			LOGGER(logger, false);

			try
			{
				const auto delimeter = '-';

				std::stringstream sout;

				sout << 
					m_config.inputs_asset << delimeter << m_config.inputs_timeframe << 
					m_config.inputs_timeframe_type << delimeter << year << delimeter << 
					std::setw(2) << std::setfill('0') << std::right << month << Extension::csv;

				return sout.str();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		std::string System::make_orders_file_name(std::time_t year, std::time_t month) const
		{
			LOGGER(logger, false);

			try
			{
				const auto delimeter = '-';

				std::stringstream sout;

				sout << m_config.inputs_asset << delimeter << "orders" << delimeter <<
					year << delimeter << std::setw(2) << std::setfill('0') << std::right << month << Extension::csv;

				return sout.str();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		std::string System::make_trades_file_name(std::time_t year, std::time_t month) const
		{
			LOGGER(logger, false);

			try
			{
				const auto delimeter = '-';

				std::stringstream sout;

				sout << m_config.inputs_asset << delimeter << "trades" << delimeter <<
					year << delimeter << std::setw(2) << std::setfill('0') << std::right << month << Extension::csv;

				return sout.str();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		System::Kline System::parse_kline(const std::string & line) const
		{
			LOGGER(logger, false);

			try
			{
				detail::Kline_Parser < std::string::const_iterator > parser;

				auto first = std::begin(line);
				auto last  = std::end(line);

				Kline kline;

				auto result = boost::spirit::qi::phrase_parse(
					first, last, parser, boost::spirit::qi::blank, kline);

				if (result)
				{
					return kline;
				}
				else
				{
					throw system_exception("cannot parse line " + line);
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		System::Order System::parse_order(const std::string & line) const
		{
			LOGGER(logger, false);

			try
			{
				Order order;

				return order;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		System::Trade System::parse_trade(const std::string & line) const
		{
			LOGGER(logger, false);

			try
			{
				detail::Trade_Parser < std::string::const_iterator > parser;

				auto first = std::begin(line);
				auto last  = std::end(line);

				Trade trade;

				auto result = boost::spirit::qi::phrase_parse(
					first, last, parser, boost::spirit::qi::blank, trade);

				if (result)
				{
					return trade;
				}
				else
				{
					throw system_exception("cannot parse line " + line);
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		System::inputs_container_t System::make_inputs(
			const klines_container_t & klines,
			const orders_container_t & orders,
			const trades_container_t & trades) const
		{
			LOGGER(logger);

			try
			{
				inputs_container_t inputs(std::size(klines));

				for (auto i = 0ULL; i < std::size(klines); ++i)
				{
					auto date_time = detail::from_time_t(klines[i].time_open, true);

					inputs[i].date_time = date_time;

					inputs[i].day  = detail::day_of_week(date_time);
					inputs[i].hour = date_time.hour;

					inputs[i].price_open  = klines[i].price_open;
					inputs[i].price_high  = klines[i].price_high;
					inputs[i].price_low   = klines[i].price_low;
					inputs[i].price_close = klines[i].price_close;

					inputs[i].volume_base  = klines[i].volume_base;
					inputs[i].volume_quote = klines[i].volume_quote;

					inputs[i].volume_buy_base  = klines[i].volume_buy_base;
					inputs[i].volume_buy_quote = klines[i].volume_buy_quote;

					inputs[i].volume_sell_base  = klines[i].volume_base  - klines[i].volume_buy_base;
					inputs[i].volume_sell_quote = klines[i].volume_quote - klines[i].volume_buy_quote;
				}

				update_indicators(inputs);

				update_price_aggregated_trades(inputs, klines, trades);

				update_movement_tags(inputs);

				return inputs;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::update_indicators(inputs_container_t & inputs) const
		{
			LOGGER(logger);

			try
			{
				for (const auto & indicator : m_indicators)
				{
					indicator(inputs);
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::update_price_aggregated_trades(
				  inputs_container_t & inputs,
			const klines_container_t & klines,
			const trades_container_t & trades) const
		{
			LOGGER(logger);

			try
			{
				if (std::size(klines) != std::size(inputs))
				{
					throw std::domain_error("invalid klines/inputs size");
				}

				const auto epsilon = std::numeric_limits < double > ::epsilon();

				for (auto i = m_config.timesteps_prehistory - 1; i < std::size(klines); ++i)
				{
					auto begin = std::lower_bound(std::begin(trades), std::end(trades), 
						klines[i + 1 - m_config.timesteps_prehistory].time_open,
							[](const auto & trade, auto time) { return (trade.time < time); });

					auto end = std::upper_bound(std::begin(trades), std::end(trades), klines[i].time_close,
						[](auto time, const auto & trade) { return (time < trade.time); });

					inputs[i].lower_price_aggregated_trades.resize(
						m_config.price_aggregated_trades_depth, { 0.0, 0.0, 0.0, 0.0 });

					inputs[i].upper_price_aggregated_trades.resize(
						m_config.price_aggregated_trades_depth, { 0.0, 0.0, 0.0, 0.0 });

					for (auto iterator = begin; iterator != end; ++iterator)
					{
						if (iterator->time < klines[i + 1 - m_config.timesteps_prehistory].time_open ||
							iterator->time > klines[i].time_close)
						{
							LOGGER_WRITE_FATAL(logger, std::string("trade time: ") + std::to_string(iterator->time));
							LOGGER_WRITE_FATAL(logger, std::string("open  time: ") + std::to_string(klines[i].time_open));
							LOGGER_WRITE_FATAL(logger, std::string("close time: ") + std::to_string(klines[i].time_close));

							throw std::domain_error("invalid trade time");
						}

						if (auto index = static_cast < std::size_t > (std::floor(
								std::abs(iterator->price - inputs[i].price_close) /
									std::max(inputs[i].price_close, epsilon) / 
										m_config.price_aggregated_trade_step)); 
							index < m_config.price_aggregated_trades_depth)
						{
							if (iterator->price < inputs[i].price_close)
							{
								if (iterator->is_buyer_maker == "True")
								{
									inputs[i].lower_price_aggregated_trades[index].volume_buy_base  += iterator->volume_base;
									inputs[i].lower_price_aggregated_trades[index].volume_buy_quote += iterator->volume_quote;
								}
								else
								{
									inputs[i].lower_price_aggregated_trades[index].volume_sell_base  += iterator->volume_base;
									inputs[i].lower_price_aggregated_trades[index].volume_sell_quote += iterator->volume_quote;
								}
							}
							else
							{
								if (iterator->is_buyer_maker == "True")
								{
									inputs[i].upper_price_aggregated_trades[index].volume_buy_base  += iterator->volume_base;
									inputs[i].upper_price_aggregated_trades[index].volume_buy_quote += iterator->volume_quote;
								}
								else
								{
									inputs[i].upper_price_aggregated_trades[index].volume_sell_base  += iterator->volume_base;
									inputs[i].upper_price_aggregated_trades[index].volume_sell_quote += iterator->volume_quote;
								}
							}
						}
					}
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::update_movement_tags(inputs_container_t & inputs) const
		{
			LOGGER(logger);

			try
			{
				const auto prediction_range = m_config.timesteps_prediction;

				for (auto i = 0ULL; i < std::size(inputs) - prediction_range; ++i)
				{
					auto max_price = inputs[i + prediction_range].price_close;
					auto min_price = inputs[i + prediction_range].price_close;

					for (auto j = 1ULL; j < prediction_range; ++j)
					{
						max_price = std::max(max_price, inputs[i + j].price_close);
						min_price = std::min(min_price, inputs[i + j].price_close);
					}

					auto current_price_close = inputs[i].price_close;

					auto deviation_L = std::abs((max_price - current_price_close) / current_price_close);
					auto deviation_S = std::abs((min_price - current_price_close) / current_price_close);

					if ((deviation_L > m_config.min_movement) && 
						(deviation_S > m_config.min_movement) ||
						(deviation_L < m_config.min_movement) &&
						(deviation_S < m_config.min_movement))
					{
						inputs[i].movement_tag = 0;
					}
					else
					{
						if (deviation_L > deviation_S)
						{
							inputs[i].movement_tag = +1;
						}
						else
						{
							inputs[i].movement_tag = -1;
						}
					}
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::save_inputs(const inputs_container_t & inputs) const
		{
			LOGGER(logger);

			try
			{
				Data::save_inputs(inputs, m_config);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::save_result(const Result & result) const
		{
			LOGGER(logger);

			try
			{
				Data::save_result(result);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::make_report() const
		{
			LOGGER(logger);

			shared::Python python;

			try
			{
				boost::python::exec("from system import make_report", python.global(), python.global());

				python.global()["make_report"](
					Data::File::reward_data.   string().c_str(),
					Data::File::trades_data.   string().c_str(),
					Data::File::reward_BH_data.string().c_str(),
					Data::File::config_json.   string().c_str());
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

		void System::run()
		{
			LOGGER(logger);

			try
			{
				// TODO
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

	} // namespace system

} // namespace solution